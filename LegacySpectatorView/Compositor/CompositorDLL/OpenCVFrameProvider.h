// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#if USE_OPENCV

#pragma comment(lib, "opencv_world320")
#include "opencv2/opencv.hpp"

#include "DirectXHelper.h"

#include "IFrameProvider.h"

//TODO: Change this value to match the camera id you are using.
// If your PC has an integrated webcam, that will probably be id 0.
#define CAMERA_ID 0

class OpenCVFrameProvider : public IFrameProvider
{
public:
    OpenCVFrameProvider(bool cacheFrames = true);
    ~OpenCVFrameProvider();

    virtual HRESULT Initialize(ID3D11ShaderResourceView* colorSRV, ID3D11Texture2D* outputTexture);
    virtual LONGLONG GetTimestamp();

    virtual LONGLONG GetDurationHNS();

    virtual void Update();

    virtual bool IsEnabled();

    bool SupportsOutput()
    {
        return false;
    }

    virtual void Dispose();

    bool OutputYUV()
    {
        return false;
    }

    virtual bool IsVideoFrameReady();

private:
    CRITICAL_SECTION lock;
    CRITICAL_SECTION frameAccessCriticalSection;

    bool _cacheFrames = true;

    LONGLONG latestTimeStamp = 0;
    LONGLONG secondTimeStamp = 0;
    LONGLONG thirdTimeStamp = 0;

    BYTE* tmpData = new BYTE[FRAME_BUFSIZE];
    BYTE* thirdCachedBuffer = new BYTE[FRAME_BUFSIZE];
    BYTE* secondCachedBuffer = new BYTE[FRAME_BUFSIZE];
    BYTE* latestBuffer = new BYTE[FRAME_BUFSIZE];

    cv::VideoCapture* videoCapture = nullptr;
    ID3D11ShaderResourceView* _colorSRV;
    ID3D11Device* _device;

    bool isVideoFrameReady = false;
    bool dirtyFrame = true;
};

#endif