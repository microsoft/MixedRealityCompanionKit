// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#include "stdafx.h"
#include "VideoEncoder.h"
#include "DirectoryHelper.h"
#include "Shlobj.h" // To get MyDocuments path
#include "ScreenGrab.h"
#include "wincodec.h"

#define DLLEXPORT __declspec(dllexport)

class CompositorInterface
{
private:
    IFrameProvider* frameProvider;
    float alpha = 0.9f;

    VideoEncoder* videoEncoder = nullptr;
    int videoIndex = -1;
    LONGLONG videoRecordingStartTime;
    double audioRecordingStartTime;
    int photoIndex = -1;

    std::wstring outputPath, channelPath;

    ID3D11Device* _device;

    LONGLONG stubVideoTime = 0;

public:
    DLLEXPORT CompositorInterface();
    DLLEXPORT void SetFrameProvider(IFrameProvider::ProviderType type);

    DLLEXPORT bool Initialize(ID3D11Device* device, ID3D11ShaderResourceView* colorSRV, ID3D11Texture2D* outputTexture);

    DLLEXPORT void UpdateFrameProvider();
    DLLEXPORT void Update();
    DLLEXPORT void StopFrameProvider();

    DLLEXPORT LONGLONG GetTimestamp(int frame);

    DLLEXPORT LONGLONG GetColorDuration();
    DLLEXPORT int GetCaptureFrameIndex();
    DLLEXPORT int GetPixelChange(int frame);
    DLLEXPORT int GetNumQueuedOutputFrames();

    DLLEXPORT void SetCompositeFrameIndex(int index);

    DLLEXPORT void TakePicture(ID3D11Device* device, int width, int height, int bpp, BYTE* bytes);

    DLLEXPORT bool InitializeVideoEncoder(ID3D11Device* device);
    DLLEXPORT void StartRecording();
    DLLEXPORT void StopRecording();
    DLLEXPORT void RecordFrameAsync(BYTE* videoFrame, LONGLONG frameTime, int numFrames);
    DLLEXPORT void RecordAudioFrameAsync(BYTE* audioFrame, int audioSize, double audioTime);

    DLLEXPORT void SetAlpha(float newAlpha)
    {
        alpha = newAlpha;
    }

    DLLEXPORT float GetAlpha()
    {
        return alpha;
    }

    DLLEXPORT bool OutputYUV();

public:
    int compositeFrameIndex;
};

