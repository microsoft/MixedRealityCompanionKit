// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "stdafx.h"
#include "VideoEncoder.h"


VideoEncoder::VideoEncoder(UINT frameWidth, UINT frameHeight, UINT frameStride, UINT fps,
    UINT32 audioBufferSize, UINT32 audioSampleRate, UINT32 audioChannels, UINT32 audioBPS) :
    frameWidth(frameWidth),
    frameHeight(frameHeight),
    frameStride(frameStride),
    audioBufferSize(audioBufferSize),
    audioSampleRate(audioSampleRate),
    audioChannels(audioChannels),
    audioBPS(audioBPS),
    fps(fps),
    bitRate(frameWidth * frameHeight * fps * 4),
    videoEncodingFormat(MFVideoFormat_H264),
#if HARDWARE_ENCODE_VIDEO
    inputFormat(MFVideoFormat_NV12),
#else
    inputFormat(MFVideoFormat_RGB32),
#endif
    isRecording(false)
{

}

VideoEncoder::~VideoEncoder()
{
    MFShutdown();
}

bool VideoEncoder::Initialize(ID3D11Device* device)
{
    HRESULT hr = E_PENDING;
    hr = MFStartup(MF_VERSION);

    QueryPerformanceFrequency(&freq);

#if HARDWARE_ENCODE_VIDEO
    MFCreateDXGIDeviceManager(&resetToken, &deviceManager);

    if (deviceManager != nullptr)
    {
        OutputDebugString(L"Resetting device manager with graphics device.\n");
        deviceManager->ResetDevice(device, resetToken);
    }
#endif

    return SUCCEEDED(hr);
}

bool VideoEncoder::IsRecording()
{
    return isRecording;
}

void VideoEncoder::StartRecording(LPCWSTR videoPath, bool encodeAudio)
{
    std::unique_lock<std::shared_mutex> lock(videoStateLock);

    if (isRecording)
    {
        return;
    }

    // Reset previous times to get valid data for this recording.
    prevVideoTime = INVALID_TIMESTAMP;
    prevAudioTime = INVALID_TIMESTAMP;

    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);
    startTime = time.QuadPart;

    HRESULT hr = E_PENDING;

    sinkWriter = NULL;
    videoStreamIndex = NULL;
    audioStreamIndex = NULL;

    IMFMediaType*    pVideoTypeOut = NULL;
    IMFMediaType*    pVideoTypeIn = NULL;

#if ENCODE_AUDIO
    IMFMediaType*    pAudioTypeOut = NULL;
    IMFMediaType*    pAudioTypeIn = NULL;
#endif

    IMFAttributes *attr = nullptr;
    MFCreateAttributes(&attr, 3);

    if (SUCCEEDED(hr)) { hr = attr->SetUINT32(MF_SINK_WRITER_DISABLE_THROTTLING, TRUE); }

#if HARDWARE_ENCODE_VIDEO
    if (SUCCEEDED(hr)) { hr = attr->SetUINT32(MF_READWRITE_ENABLE_HARDWARE_TRANSFORMS, true); }
    if (SUCCEEDED(hr)) { hr = attr->SetUINT32(MF_READWRITE_DISABLE_CONVERTERS, false); }
#endif

    hr = MFCreateSinkWriterFromURL(videoPath, NULL, attr, &sinkWriter);

    // Set the output media types.
    if (SUCCEEDED(hr)) { hr = MFCreateMediaType(&pVideoTypeOut); }
    if (SUCCEEDED(hr)) { hr = pVideoTypeOut->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video); }
    if (SUCCEEDED(hr)) { hr = pVideoTypeOut->SetGUID(MF_MT_SUBTYPE, videoEncodingFormat); }
    if (SUCCEEDED(hr)) { hr = pVideoTypeOut->SetUINT32(MF_MT_AVG_BITRATE, bitRate); }
    if (SUCCEEDED(hr)) { hr = pVideoTypeOut->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive); }
    if (SUCCEEDED(hr)) { hr = MFSetAttributeSize(pVideoTypeOut, MF_MT_FRAME_SIZE, frameWidth, frameHeight); }
    if (SUCCEEDED(hr)) { hr = MFSetAttributeRatio(pVideoTypeOut, MF_MT_FRAME_RATE, fps, 1); }
    if (SUCCEEDED(hr)) { hr = MFSetAttributeRatio(pVideoTypeOut, MF_MT_PIXEL_ASPECT_RATIO, 1, 1); }
    if (SUCCEEDED(hr)) { hr = sinkWriter->AddStream(pVideoTypeOut, &videoStreamIndex); }

    if (encodeAudio)
    {
#if ENCODE_AUDIO
        if (SUCCEEDED(hr)) { hr = MFCreateMediaType(&pAudioTypeOut); }
        if (SUCCEEDED(hr)) { hr = pAudioTypeOut->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio); }
        if (SUCCEEDED(hr)) { hr = pAudioTypeOut->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_AAC); }
        if (SUCCEEDED(hr)) { hr = pAudioTypeOut->SetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, 16); }
        if (SUCCEEDED(hr)) { hr = pAudioTypeOut->SetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, audioSampleRate); }
        if (SUCCEEDED(hr)) { hr = pAudioTypeOut->SetUINT32(MF_MT_AUDIO_NUM_CHANNELS, audioChannels); }
        if (SUCCEEDED(hr)) { hr = pAudioTypeOut->SetUINT32(MF_MT_AUDIO_AVG_BYTES_PER_SECOND, audioBPS); }
        if (SUCCEEDED(hr)) { hr = pAudioTypeOut->SetUINT32(MF_MT_AUDIO_PREFER_WAVEFORMATEX, 1); }
        if (SUCCEEDED(hr)) { hr = pAudioTypeOut->SetUINT32(MF_MT_ALL_SAMPLES_INDEPENDENT, 1); }
        if (SUCCEEDED(hr)) { hr = pAudioTypeOut->SetUINT32(MF_MT_FIXED_SIZE_SAMPLES, 1); }
        if (SUCCEEDED(hr)) { hr = sinkWriter->AddStream(pAudioTypeOut, &audioStreamIndex); }
#endif
    }

    // Set the input media types.
    if (SUCCEEDED(hr)) { hr = MFCreateMediaType(&pVideoTypeIn); }
    if (SUCCEEDED(hr)) { hr = pVideoTypeIn->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video); }
    if (SUCCEEDED(hr)) { hr = pVideoTypeIn->SetGUID(MF_MT_SUBTYPE, inputFormat); }
    if (SUCCEEDED(hr)) { hr = pVideoTypeIn->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive); }
    if (SUCCEEDED(hr)) { hr = MFSetAttributeSize(pVideoTypeIn, MF_MT_FRAME_SIZE, frameWidth, frameHeight); }
    if (SUCCEEDED(hr)) { hr = MFSetAttributeRatio(pVideoTypeIn, MF_MT_FRAME_RATE, fps, 1); }
    if (SUCCEEDED(hr)) { hr = MFSetAttributeRatio(pVideoTypeIn, MF_MT_PIXEL_ASPECT_RATIO, 1, 1); }
    if (SUCCEEDED(hr)) { hr = sinkWriter->SetInputMediaType(videoStreamIndex, pVideoTypeIn, NULL); }

    if (encodeAudio)
    {
#if ENCODE_AUDIO
        if (SUCCEEDED(hr)) { hr = MFCreateMediaType(&pAudioTypeIn); }
        if (SUCCEEDED(hr)) { hr = pAudioTypeIn->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio); }
        if (SUCCEEDED(hr)) { hr = pAudioTypeIn->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM); }
        if (SUCCEEDED(hr)) { hr = pAudioTypeIn->SetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, 16); }
        if (SUCCEEDED(hr)) { hr = pAudioTypeIn->SetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, audioSampleRate); }
        if (SUCCEEDED(hr)) { hr = pAudioTypeIn->SetUINT32(MF_MT_AUDIO_NUM_CHANNELS, audioChannels); }
        if (SUCCEEDED(hr)) { hr = sinkWriter->SetInputMediaType(audioStreamIndex, pAudioTypeIn, NULL); }
#endif
    }

    // Tell the sink writer to start accepting data.
    if (SUCCEEDED(hr)) { hr = sinkWriter->BeginWriting(); }

    if (FAILED(hr))
    {
        OutputDebugString(L"Error starting recording.\n");
    }

    isRecording = true;
    acceptQueuedFrames = true;

    SafeRelease(pVideoTypeOut);
    SafeRelease(pVideoTypeIn);

#if ENCODE_AUDIO
    SafeRelease(pAudioTypeOut);
    SafeRelease(pAudioTypeIn);
#endif
}

void VideoEncoder::WriteAudio(byte* buffer, LONGLONG timestamp)
{
    std::shared_lock<std::shared_mutex> lock(videoStateLock);

#if ENCODE_AUDIO
    if (!isRecording || startTime == INVALID_TIMESTAMP || timestamp < startTime)
    {
        return;
    }

    LONGLONG sampleTimeNow = timestamp;
    if (sampleTimeNow < 0) { sampleTimeNow *= -1; }
    LONGLONG sampleTimeStart = startTime;
    if (sampleTimeStart < 0) { sampleTimeStart *= -1; }

    LONGLONG sampleTime = sampleTimeNow - sampleTimeStart;
    if (sampleTime < 0) { sampleTime *= -1; }

    LONGLONG duration = AUDIO_POLLING_RATE_HNS;
    if (prevAudioTime != INVALID_TIMESTAMP)
    {
        duration = sampleTime - prevAudioTime;
        if (duration < 0) { duration *= -1; }

        duration *= QPC_MULTIPLIER;
        duration /= freq.QuadPart;
    }

    // Copy frame to a temporary buffer and process on a background thread.
    byte* tmpAudioBuffer = new byte[audioBufferSize];
    memcpy(tmpAudioBuffer, buffer, audioBufferSize);

    concurrency::create_task([=]()
    {
        std::shared_lock<std::shared_mutex> lock(videoStateLock);

        HRESULT hr = E_PENDING;
        if (sinkWriter == NULL || !isRecording)
        {
            OutputDebugString(L"Must start recording before writing audio frames.\n");
            delete[] tmpAudioBuffer;
            return;
        }

        IMFSample* pAudioSample = NULL;
        IMFMediaBuffer* pAudioBuffer = NULL;

        const DWORD cbAudioBuffer = audioBufferSize;

        BYTE* pData = NULL;

        hr = MFCreateMemoryBuffer(cbAudioBuffer, &pAudioBuffer);
        if (SUCCEEDED(hr)) { hr = pAudioBuffer->Lock(&pData, NULL, NULL); }
        memcpy(pData, tmpAudioBuffer, cbAudioBuffer);
        if (pAudioBuffer)
        {
            pAudioBuffer->Unlock();
        }

        if (SUCCEEDED(hr)) { hr = MFCreateSample(&pAudioSample); }
        LONGLONG t = sampleTime;
        t *= QPC_MULTIPLIER;
        t /= freq.QuadPart;
        if (SUCCEEDED(hr)) { hr = pAudioSample->SetSampleTime(t); }
        if (SUCCEEDED(hr)) { hr = pAudioSample->SetSampleDuration(duration); }
        if (SUCCEEDED(hr)) { hr = pAudioBuffer->SetCurrentLength(cbAudioBuffer); }
        if (SUCCEEDED(hr)) { hr = pAudioSample->AddBuffer(pAudioBuffer); }

        if (SUCCEEDED(hr)) { hr = sinkWriter->WriteSample(audioStreamIndex, pAudioSample); }

        SafeRelease(pAudioSample);
        SafeRelease(pAudioBuffer);

        if (FAILED(hr))
        {
            OutputDebugString(L"Error writing audio frame.\n");
        }

        delete[] tmpAudioBuffer;
    });

    prevAudioTime = sampleTime;
    if (prevAudioTime < 0) { prevAudioTime *= -1; }
#endif
}

void VideoEncoder::WriteVideo(byte* buffer, LONGLONG timestamp, LONGLONG duration)
{
    std::shared_lock<std::shared_mutex> lock(videoStateLock);

    if (!isRecording || startTime == INVALID_TIMESTAMP || timestamp < startTime)
    {
        return;
    }

    LONGLONG sampleTimeNow = timestamp;
    if (sampleTimeNow < 0) { sampleTimeNow *= -1; }
    LONGLONG sampleTimeStart = startTime;
    if (sampleTimeStart < 0) { sampleTimeStart *= -1; }

    LONGLONG sampleTime = sampleTimeNow - sampleTimeStart;
    if (sampleTime < 0) { sampleTime *= -1; }

    if (prevVideoTime != INVALID_TIMESTAMP)
    {
        duration = sampleTime - prevVideoTime;
        if (duration < 0) { duration *= -1; }

        duration *= QPC_MULTIPLIER;
        duration /= freq.QuadPart;
    }

    // Copy frame to a temporary buffer and process on a background thread.
#if HARDWARE_ENCODE_VIDEO
    BYTE* tmpVideoBuffer = new BYTE[(int)(1.5f * frameHeight * frameWidth)];
    memcpy(tmpVideoBuffer, buffer, (int)(1.5f * frameHeight * frameWidth));
#else
    BYTE* tmpVideoBuffer = new BYTE[frameHeight * frameStride];
    memcpy(tmpVideoBuffer, buffer, frameHeight * frameStride);
#endif

    concurrency::create_task([=]()
    {
        std::shared_lock<std::shared_mutex> lock(videoStateLock);

        HRESULT hr = E_PENDING;
        if (sinkWriter == NULL || !isRecording)
        {
            OutputDebugString(L"Must start recording before writing video frames.\n");
            delete[] tmpVideoBuffer;
            return;
        }

        LONG cbWidth = frameStride;
        DWORD cbBuffer = cbWidth * frameHeight;
        DWORD imageHeight = frameHeight;

#if HARDWARE_ENCODE_VIDEO
        cbWidth = frameWidth;
        cbBuffer = (int)(1.5f * frameWidth * frameHeight);
        imageHeight = (int)(1.5f * frameHeight);
#endif

        IMFSample* pVideoSample = NULL;
        IMFMediaBuffer* pVideoBuffer = NULL;
        BYTE* pData = NULL;

        // Create a new memory buffer.
        hr = MFCreateMemoryBuffer(cbBuffer, &pVideoBuffer);

        // Lock the buffer and copy the video frame to the buffer.
        if (SUCCEEDED(hr)) { hr = pVideoBuffer->Lock(&pData, NULL, NULL); }

        if (SUCCEEDED(hr))
        {
            //TODO: Can pVideoBuffer be created from an ID3D11Texture2D*?
            hr = MFCopyImage(
                pData,                      // Destination buffer.
                cbWidth,                    // Destination stride.
                tmpVideoBuffer,
                cbWidth,                    // Source stride.
                cbWidth,                    // Image width in bytes.
                imageHeight                 // Image height in pixels.
            );
        }

        if (pVideoBuffer)
        {
            pVideoBuffer->Unlock();
        }

        // Set the data length of the buffer.
        if (SUCCEEDED(hr)) { hr = pVideoBuffer->SetCurrentLength(cbBuffer); }

        // Create a media sample and add the buffer to the sample.
        if (SUCCEEDED(hr)) { hr = MFCreateSample(&pVideoSample); }
        if (SUCCEEDED(hr)) { hr = pVideoSample->AddBuffer(pVideoBuffer); }

        // Set the frame timestamp.
        LONGLONG t = sampleTime;
        t *= QPC_MULTIPLIER;
        t /= freq.QuadPart;
        if (SUCCEEDED(hr)) { hr = pVideoSample->SetSampleTime(t); }
        if (SUCCEEDED(hr)) { hr = pVideoSample->SetSampleDuration(duration); }

        // Send the sample to the Sink Writer.
        if (SUCCEEDED(hr)) { hr = sinkWriter->WriteSample(videoStreamIndex, pVideoSample); }

        SafeRelease(pVideoSample);
        SafeRelease(pVideoBuffer);
        delete[] tmpVideoBuffer;

        if (FAILED(hr))
        {
            OutputDebugString(L"Error writing video frame.\n");
        }
    });

    prevVideoTime = sampleTime;
    if (prevVideoTime < 0) { prevVideoTime *= -1; }
}

void VideoEncoder::StopRecording()
{
    std::unique_lock<std::shared_mutex> lock(videoStateLock);

    if (sinkWriter == NULL || !isRecording)
    {
        OutputDebugString(L"Must start recording before it can be stopped.\n");
        return;
    }

    // Clear any async frames.
    acceptQueuedFrames = false;
    isRecording = false;

    std::mutex completion_mutex;

    bool doneCleaningVideoTasks = false;
    bool doneCleaningAudioTasks = false;

    std::unique_lock<std::mutex> completion_lock(completion_mutex);
    std::condition_variable completion_lock_check;

    concurrency::create_task([&]
    {
        while (!videoQueue.empty())
        {
            videoQueue.pop();
        }

        {
            std::lock_guard<std::mutex> lk(completion_mutex);
            doneCleaningVideoTasks = true;
        }
        completion_lock_check.notify_one();
    });

    concurrency::create_task([&]
    {
        while (!audioQueue.empty())
        {
            audioQueue.pop();
        }

        {
            std::lock_guard<std::mutex> lk(completion_mutex);
            doneCleaningAudioTasks = true;
        }
        completion_lock_check.notify_one();
    });

    completion_lock_check.wait(completion_lock, [&] {return doneCleaningVideoTasks && doneCleaningAudioTasks; });

    if (videoStreamIndex != NULL)
    {
        sinkWriter->Flush(videoStreamIndex);
    }
    if (audioStreamIndex != NULL)
    {
        sinkWriter->Flush(audioStreamIndex);
    }

    sinkWriter->Finalize();
    SafeRelease(sinkWriter);
}

void VideoEncoder::QueueVideoFrame(byte* buffer, LONGLONG timestamp, LONGLONG duration)
{
    std::shared_lock<std::shared_mutex> lock(videoStateLock);

    if (acceptQueuedFrames)
    {
        videoQueue.push(VideoInput(buffer, timestamp, duration));
    }
}

void VideoEncoder::QueueAudioFrame(byte* buffer, LONGLONG timestamp)
{
    std::shared_lock<std::shared_mutex> lock(videoStateLock);

    if (acceptQueuedFrames)
    {
        audioQueue.push(AudioInput(buffer, timestamp));
    }
}

void VideoEncoder::Update()
{
    std::shared_lock<std::shared_mutex> lock(videoStateLock);
    if (!isRecording)
    {
        return;
    }

    while (!videoQueue.empty())
    {
        if (isRecording)
        {
            VideoInput input = videoQueue.front();
            WriteVideo(input.buffer, input.timestamp, input.duration);
            delete[] input.buffer;
            videoQueue.pop();
        }
    }

    while (!audioQueue.empty())
    {
        if (isRecording)
        {
            AudioInput input = audioQueue.front();
            WriteAudio(input.buffer, input.timestamp);
            delete[] input.buffer;
            audioQueue.pop();
        }
    }
}
