// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#include "stdafx.h"
#include "VideoEncoder.h"
#include "DirectoryHelper.h"
#include "Shlobj.h" // To get MyDocuments path
#include "ScreenGrab.h"
#include "wincodec.h"
#include "HologramQueue.h"

#if USE_CANON_SDK
#include "CanonSDKManager.h"
#include "WICTextureLoader.h"
#endif

#define DLLEXPORT __declspec(dllexport)

class CompositorInterface
{
private:
    IFrameProvider* frameProvider;
    float alpha = 0.9f;

    VideoEncoder* videoEncoder = nullptr;
    int videoIndex = -1;
    int photoIndex = -1;
#if USE_CANON_SDK
    int canonPhotoIndex = -1;
    bool canonPictureDownloaded = false;
    bool takingCanonPicture = false;
    std::wstring canonPhotoPath = L"";
    BYTE* cachedHiResHoloBytes = new BYTE[HOLOGRAM_BUFSIZE_HIRES];
    BYTE* canonColorBytes = new BYTE[HOLOGRAM_BUFSIZE_HIRES];

    CRITICAL_SECTION canonLock;
#endif

    std::wstring outputPath, outputPathCanon, channelPath;

    LONGLONG prevAudioTime = INVALID_TIMESTAMP;
    ID3D11Device* _device;

    HologramQueue* hologramQueue;
    LONGLONG stubVideoTime = 0;

#if USE_CANON_SDK
    CanonSDKManager* canonManager;
#endif

public:
    DLLEXPORT CompositorInterface();

    DLLEXPORT bool Initialize(ID3D11Device* device, ID3D11ShaderResourceView* colorSRV, ID3D11Texture2D* outputTexture);

    DLLEXPORT void UpdateFrameProvider();
    DLLEXPORT void Update();
    DLLEXPORT void StopFrameProvider();

    DLLEXPORT LONGLONG GetTimestamp();

    DLLEXPORT LONGLONG GetColorDuration();

    DLLEXPORT void TakePicture(ID3D11Device* device, int width, int height, int bpp, 
        BYTE* bytes, BYTE* colorBytes, BYTE* holoBytes);
    DLLEXPORT void TakeCanonPicture(ID3D11Device* device, BYTE* hiResHoloBytes);

    DLLEXPORT bool InitializeVideoEncoder(ID3D11Device* device);
    DLLEXPORT void StartRecording();
    DLLEXPORT void StopRecording();
    DLLEXPORT bool IsVideoFrameReady();
    DLLEXPORT void RecordFrameAsync(BYTE* videoFrame, LONGLONG frameTime);
    DLLEXPORT void RecordAudioFrameAsync(BYTE* audioFrame, LONGLONG frameTime);

    DLLEXPORT void SetAlpha(float newAlpha)
    {
        alpha = newAlpha;
    }

    DLLEXPORT float GetAlpha()
    {
        return alpha;
    }

    DLLEXPORT bool OutputYUV();

    DLLEXPORT FrameMessage* GetNextHologramFrame(LONGLONG timeStamp);
    DLLEXPORT FrameMessage* FindClosestHologramFrame(LONGLONG timeStamp, LONGLONG frameOffset);
};

