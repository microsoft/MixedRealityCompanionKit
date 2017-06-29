// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

// IMFSinkWriter:
// https://msdn.microsoft.com/en-us/library/windows/desktop/ff819477(v=vs.85).aspx

#pragma once

#include <Windows.h>
#include <mfapi.h>
#include <mfidl.h>
#include <Mfreadwrite.h>
#include <mferror.h>
#include <shared_mutex>

#include "DirectXHelper.h"

#include <queue>

#pragma comment(lib, "mf")
#pragma comment(lib, "mfreadwrite")
#pragma comment(lib, "mfplat")
#pragma comment(lib, "mfuuid")

#define INVALID_TIMESTAMP -1

class VideoEncoder
{
public:
    VideoEncoder(UINT frameWidth, UINT frameHeight, UINT frameStride, UINT fps,
        UINT32 audioBufferSize, UINT32 audioSampleRate, UINT32 audioChannels, UINT32 audioBPS);
    ~VideoEncoder();

    bool Initialize(ID3D11Device* device);

    void StartRecording(LPCWSTR videoPath, bool encodeAudio = false);
    bool IsRecording();
    void StopRecording();

    // Used for recording video from a background thread.
    void QueueVideoFrame(byte* buffer, LONGLONG timestamp, LONGLONG duration);
    void QueueAudioFrame(byte* buffer, LONGLONG timestamp);

    // Do not call this from a background thread.
    void Update();

private:
    void WriteVideo(byte* buffer, LONGLONG timestamp, LONGLONG duration);
    void WriteAudio(byte* buffer, LONGLONG timestamp);

    LARGE_INTEGER freq;

    class VideoInput
    {
    public:
#if HARDWARE_ENCODE_VIDEO
        byte* buffer = new byte[(int)(1.5f * FRAME_WIDTH * FRAME_HEIGHT)];
#else
        byte* buffer = new byte[FRAME_BUFSIZE];
#endif

        LONGLONG timestamp;
        LONGLONG duration;

        VideoInput(byte* buffer, LONGLONG timestamp, LONGLONG duration)
        {
#if HARDWARE_ENCODE_VIDEO
            memcpy(this->buffer, buffer, (int)(1.5f * FRAME_WIDTH * FRAME_HEIGHT));
#else
            memcpy(this->buffer, buffer, FRAME_BUFSIZE);
#endif
            this->timestamp = timestamp;
            this->duration = duration;
        }
    };

    class AudioInput
    {
    public:
        byte* buffer = new byte[AUDIO_BUFSIZE];
        LONGLONG timestamp;

        AudioInput(byte* buffer, LONGLONG timestamp)
        {
            memcpy(this->buffer, buffer, AUDIO_BUFSIZE);
            this->timestamp = timestamp;
        }
    };

    IMFSinkWriter* sinkWriter;
    DWORD videoStreamIndex;
    DWORD audioStreamIndex;

    byte* nullAudioBuffer = new byte[AUDIO_BUFSIZE];

    bool isRecording = false;
    bool acceptQueuedFrames = false;

    // Video Parameters.
    UINT frameWidth;
    UINT frameHeight;
    UINT frameStride;
    UINT32 fps;
    UINT32 bitRate;
    GUID videoEncodingFormat;
    GUID inputFormat;
    LONGLONG prevVideoTime = INVALID_TIMESTAMP;

    // Audio Parameters.
    UINT32 audioBufferSize;
    UINT32 audioSampleRate;
    UINT32 audioChannels;
    UINT32 audioBPS;
    LONGLONG prevAudioTime = INVALID_TIMESTAMP;

    LONGLONG startTime = INVALID_TIMESTAMP;

    std::queue<VideoInput> videoQueue;
    std::queue<AudioInput> audioQueue;

    std::shared_mutex videoStateLock;

#if HARDWARE_ENCODE_VIDEO
    IMFDXGIDeviceManager* deviceManager = NULL;
    UINT resetToken = 0;
#endif
};

