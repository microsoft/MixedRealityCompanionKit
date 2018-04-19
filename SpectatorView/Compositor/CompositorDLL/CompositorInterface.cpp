// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "stdafx.h"
#include "CompositorInterface.h"


CompositorInterface::CompositorInterface()
{
    wchar_t myDocumentsPath[1024];
    SHGetFolderPathW(0, CSIDL_MYDOCUMENTS, 0, 0, myDocumentsPath);
    outputPath = std::wstring(myDocumentsPath) + L"\\HologramCapture\\";

    DirectoryHelper::CreateOutputDirectory(outputPath);

#if USE_DECKLINK || USE_DECKLINK_SHUTTLE
    frameProvider = new DeckLinkManager();
#endif
#if USE_ELGATO
    frameProvider = new ElgatoFrameProvider();
#endif
#if USE_OPENCV
    frameProvider = new OpenCVFrameProvider();
#endif
}

CompositorInterface::~CompositorInterface()
{
    delete[] photoBytes;
}

bool CompositorInterface::Initialize(ID3D11Device* device, ID3D11ShaderResourceView* colorSRV, ID3D11Texture2D* outputTexture)
{
    if (frameProvider == nullptr)
    {
        return false;
    }

    if (frameProvider->IsEnabled())
    {
        return true;
    }

    _device = device;

    HRESULT hr = frameProvider->Initialize(colorSRV);
    if (SUCCEEDED(hr))
    {
        frameProvider->SetOutputTexture(outputTexture);
        timeSynchronizer.Reset();
        poseCache.Reset();
        CurrentCompositeFrame = 0;

        AllocateVideoBuffers();
    }
    return SUCCEEDED(hr);
}

// Call from render thread.
void CompositorInterface::UpdateFrameProvider()
{
    if (frameProvider != nullptr)
    {
        frameProvider->Update(CurrentCompositeFrame);
    }
}

void CompositorInterface::Update()
{
    if (videoEncoder != nullptr)
    {
        videoEncoder->Update();
    }
}

void CompositorInterface::StopFrameProvider()
{
    if (frameProvider != nullptr)
    {
        frameProvider->Dispose();
    }

    poseCache.Reset();
    timeSynchronizer.Reset();
    CurrentCompositeFrame = 0;
    VideoTextureBuffer.ReleaseTextures();
    FreeVideoBuffers();
}

LONGLONG CompositorInterface::GetTimestamp(int frame)
{
    if (frameProvider != nullptr)
    {
        return frameProvider->GetTimestamp(frame);
    }

    return INVALID_TIMESTAMP;
}

LONGLONG CompositorInterface::GetColorDuration()
{
    if (frameProvider != nullptr)
    {
        return frameProvider->GetDurationHNS();
    }

    return (LONGLONG)((1.0f / 30.0f) * S2HNS);
}

void CompositorInterface::GetPose(XMFLOAT3& position, XMFLOAT4& rotation, float UnityTimeS, int frameOffset)
{
    int captureFrameIndex = GetCaptureFrameIndex();

    // Assume 60Hz camera.
    int minStep = 8;
    int maxStep = 16;

    if (frameProvider != nullptr &&
        frameProvider->GetDurationHNS() > 20 * MS2HNS)
    {
        // Compensate for 30Hz camera.
        minStep *= 2;
        maxStep *= 2;
    }

    // Set our current frame towards the latest captured frame. 
    // Don't get too close to it, and dont fall too far behind.
    int step = (captureFrameIndex - CurrentCompositeFrame);
    if (step < minStep) { step = 0; }
    else if (step > maxStep) { step -= maxStep; }
    else { step = 1; }

    CurrentCompositeFrame += step;

    // Update time synchronizer.
    float captureTime = GetTimeFromFrame(captureFrameIndex);

    PoseData* poseData = poseCache.GetLatestPose();
    if (poseData != nullptr)
    {
        timeSynchronizer.Update(GetCaptureFrameIndex(), captureTime, poseData->Index, poseData->TimeStamp, UnityTimeS);
    }

    // Set camera transform for the currently composited frame.
    float cameraTime = GetTimeFromFrame(CurrentCompositeFrame);

    float poseTime = timeSynchronizer.GetPoseTimeFromCameraTime(cameraTime);
    if (frameProvider != nullptr)
    {
        // Compensate for camera capture latency in seconds.
        poseTime -= (float)(0.0001f * (frameOffset * (float)frameProvider->GetDurationHNS()) / 1000.0f);
    }

    if (captureFrameIndex <= 0) // No frames captured yet, let's use the very latest camera transform.
    {
        poseTime = std::numeric_limits<float>::max();
    }

    poseCache.GetPose(position, rotation, poseTime);
}

#pragma region
void CompositorInterface::TakePicture(ID3D11Texture2D* outputTexture)
{
    if (outputTexture == nullptr || _device == nullptr)
    {
        return;
    }

    ID3D11DeviceContext* context;
    _device->GetImmediateContext(&context);

    // Get bytes from texture because screengrab does not support texture format provided by Unity.
    DirectXHelper::GetBytesFromTexture(_device, outputTexture, FRAME_BPP, photoBytes);
    ID3D11Texture2D* tex = DirectXHelper::CreateTexture(_device, photoBytes, FRAME_WIDTH, FRAME_HEIGHT, FRAME_BPP, DXGI_FORMAT_B8G8R8A8_UNORM_SRGB);

    photoIndex++;
    std::wstring photoPath = DirectoryHelper::FindUniqueFileName(outputPath, L"Photo", L".png", photoIndex);

    //TODO: Save each channel?
    DirectX::SaveWICTextureToFile(context, tex, GUID_ContainerFormatPng, photoPath.c_str());
}

bool CompositorInterface::InitializeVideoEncoder(ID3D11Device* device)
{
    videoEncoder = new VideoEncoder(FRAME_WIDTH, FRAME_HEIGHT, FRAME_WIDTH * FRAME_BPP, VIDEO_FPS,
        AUDIO_BUFSIZE, AUDIO_SAMPLE_RATE, AUDIO_CHANNELS, AUDIO_BPS);

    return videoEncoder->Initialize(device);
}

void CompositorInterface::StartRecording()
{
    if (videoEncoder == nullptr)
    {
        return;
    }

    videoIndex++;
    std::wstring videoPath = DirectoryHelper::FindUniqueFileName(outputPath, L"Video", L".mp4", videoIndex);
    videoEncoder->StartRecording(videoPath.c_str(), ENCODE_AUDIO);
}

void CompositorInterface::StopRecording()
{
    if (videoEncoder == nullptr)
    {
        return;
    }

    videoEncoder->StopRecording();
    VideoTextureBuffer.Reset();
    queuedVideoFrameCount = 0;
    lastRecordedVideoFrame = -1;
    lastVideoFrame = -1;
}

void CompositorInterface::UpdateVideoRecordingFrame(ID3D11Texture2D* videoTexture)
{
    // We have an old frame, lets get the data and queue it now.
    if (VideoTextureBuffer.IsDataAvailable())
    {
        videoBufferIndex = (videoBufferIndex + 1) % NUM_VIDEO_BUFFERS;
        float bpp = 1.5f;

        VideoTextureBuffer.FetchTextureData(_device, videoBytes[videoBufferIndex], bpp);
        RecordFrameAsync(videoBytes[videoBufferIndex], queuedVideoFrameTime, queuedVideoFrameCount);
    }

    if (lastVideoFrame >= 0 && lastRecordedVideoFrame != lastVideoFrame)
    {
        queuedVideoFrameCount = GetCurrentCompositeFrame() - lastVideoFrame;
        if (queuedVideoFrameCount <= 0)
            queuedVideoFrameCount = lastVideoFrame - lastRecordedVideoFrame;
        if (queuedVideoFrameCount <= 0)
            queuedVideoFrameCount = 1;

        lastRecordedVideoFrame = lastVideoFrame;
        queuedVideoFrameTime = lastVideoFrame * GetColorDuration();
        VideoTextureBuffer.PrepareTextureFetch(_device, videoTexture);
    }

    lastVideoFrame = GetCurrentCompositeFrame();
}

void CompositorInterface::AllocateVideoBuffers()
{
    if (videoBytes != nullptr)
    {
        return;
    }

    videoBytes = new byte*[NUM_VIDEO_BUFFERS];

    for (int i = 0; i < NUM_VIDEO_BUFFERS; i++)
    {
        videoBytes[i] = new byte[(int)(1.5f * FRAME_WIDTH * FRAME_HEIGHT)];
    }
}

void CompositorInterface::FreeVideoBuffers()
{
    if (videoBytes == nullptr)
    {
        return;
    }

    for (int i = 0; i < NUM_VIDEO_BUFFERS; i++)
    {
        delete[] videoBytes[i];
    }
    delete[] videoBytes;
    videoBytes = nullptr;
}

void CompositorInterface::RecordFrameAsync(byte* bytes, LONGLONG frameTime, int numFrames)
{
    if (frameProvider == nullptr || videoEncoder == nullptr || _device == nullptr)
    {
        return;
    }

    if (numFrames < 1)
    {
        numFrames = 1;
    }
    else if (numFrames > 5)
    {
        numFrames = 5;
    }

    videoEncoder->QueueVideoFrame(bytes, frameTime, numFrames * frameProvider->GetDurationHNS());
}

void CompositorInterface::RecordAudioFrameAsync(BYTE* audioFrame, LONGLONG frameTime)
{
    if (videoEncoder == nullptr)
    {
        return;
    }

    videoEncoder->QueueAudioFrame(audioFrame, frameTime);
}
#pragma endregion Recording