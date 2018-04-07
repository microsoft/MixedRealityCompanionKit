// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#include "stdafx.h"

#if USE_ELGATO
// Disable warnings in downloaded Elgato Filter code.
#pragma warning (disable : 4091 )
#endif

#define DLLEXPORT __declspec(dllexport)

#include "IFrameProvider.h"
#include "DeckLinkManager.h"
#include "ElgatoFrameProvider.h"
#include "OpenCVFrameProvider.h"

#include "DirectXHelper.h"

#include "DirectoryHelper.h"
#include "Shlobj.h" // To get MyDocuments path

#include "ScreenGrab.h"
#include <wincodec.h>

#include "VideoEncoder.h"

#include "BufferedTextureFetch.h"
#include "PoseCache.h"
#include "TimeSynchronizer.h"

class CompositorInterface
{
#define NUM_VIDEO_BUFFERS 10
public:
    DLLEXPORT CompositorInterface();
    ~CompositorInterface();

    DLLEXPORT bool Initialize(ID3D11Device* device, ID3D11ShaderResourceView* colorSRV, ID3D11Texture2D* outputTexture);
    DLLEXPORT void UpdateFrameProvider();
    DLLEXPORT void Update();
    DLLEXPORT void StopFrameProvider();

    DLLEXPORT LONGLONG GetTimestamp(int frame);
	DLLEXPORT LONGLONG GetColorDuration();

    DLLEXPORT bool OutputYUV()
    {
        if (frameProvider == nullptr)
        {
            return true;
        }

        return frameProvider->OutputYUV();
    }

    // Recording
    DLLEXPORT void TakePicture(ID3D11Texture2D* outputTexture);
    DLLEXPORT bool InitializeVideoEncoder(ID3D11Device* device);
    DLLEXPORT void StartRecording();
    DLLEXPORT void StopRecording();
    DLLEXPORT void RecordFrameAsync(byte* bytes, LONGLONG frameTime, int numFrames);
    DLLEXPORT void RecordAudioFrameAsync(BYTE* audioFrame, LONGLONG frameTime);
    DLLEXPORT void UpdateVideoRecordingFrame(ID3D11Texture2D* videoTexture);

    // Poses
    DLLEXPORT void GetPose(XMFLOAT3& position, XMFLOAT4& rotation, float UnityTimeS, int frameOffset, float timeOffsetS);
    DLLEXPORT void AddPoseToPoseCache(XMFLOAT3 position, XMFLOAT4 rotation, float time)
    {
        poseCache.AddPose(position, rotation, time);
    }

    DLLEXPORT int GetCaptureFrameIndex()
    {
        if (frameProvider != nullptr)
        {
            return frameProvider->GetCaptureFrameIndex();
        }

        return 0;
    }

    DLLEXPORT int GetCurrentCompositeFrame()
    {
        return CurrentCompositeFrame;
    }

    DLLEXPORT int LastPoseSelectedIndex()
    {
        return poseCache.LastSelectedIndex;
    }

    DLLEXPORT void SetNetworkLatency(LONGLONG RTT)
    {
        networkLatencyS = (float)(0.0001f * RTT / 1000.0f);
    }

private:
    IFrameProvider* frameProvider;
    std::wstring outputPath;
    ID3D11Device* _device;

    // Recording
    LONGLONG stubVideoTime = 0;
    int photoIndex = -1;
    int videoIndex = -1;
    LONGLONG queuedVideoFrameTime;
    int queuedVideoFrameCount = 0;
    int lastRecordedVideoFrame = -1;
    int lastVideoFrame = -1;
    BufferedTextureFetch VideoTextureBuffer;
    VideoEncoder* videoEncoder = nullptr;
    byte* photoBytes = new byte[FRAME_BUFSIZE];
    byte** videoBytes = nullptr;
    int videoBufferIndex = 0;

    void AllocateVideoBuffers();
    void FreeVideoBuffers();

    // Pose
    PoseCache poseCache;
    TimeSynchronizer timeSynchronizer;

    float networkLatencyS = 0;
    int CurrentCompositeFrame = 0;

    // Abstracts time in seconds from frame index based on known duration.
    float GetTimeFromFrame(int frame)
    {
        if (frameProvider != nullptr)
        {
            //return (float)(0.0001f * frameProvider->GetDurationHNS() / 1000.0f) * frame;
            return (float)(0.0001f * frameProvider->GetTimestamp(frame) / 1000.0f);
        }

        return (1.0f / 30) * frame;
    }
};

