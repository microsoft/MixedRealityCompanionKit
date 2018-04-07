// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#if USE_OPENCV
#include "IFrameProvider.h"
#include <mutex>

//TODO: Update with the 3.x version of OpenCV you are using.
#pragma comment(lib, "opencv_world341")
#include "opencv2/opencv.hpp"

#include "DirectXHelper.h"

//TODO: Change this value to match the camera id you are using.
// If your PC has an integrated webcam, that will probably be id 0.
#define CAMERA_ID 0

class OpenCVFrameProvider : public IFrameProvider
{
public:
    OpenCVFrameProvider();
    ~OpenCVFrameProvider();

    // Inherited via IFrameProvider
    virtual HRESULT Initialize(ID3D11ShaderResourceView* srv) override;
    virtual bool IsEnabled() override;
    virtual void Update() override;
    virtual void Dispose() override;

    virtual bool OutputYUV()
    {
        return false;
    }

    virtual LONGLONG GetTimestamp()
    {
        return latestTimeStamp;
    }

    virtual LONGLONG GetDurationHNS() override;

    //TODO: Measure this on your setup.
    virtual LONGLONG GetFrameDelayMS()
    {
        return 0;
    }

private:
    std::mutex frameAccessLock;
    std::mutex videoLock;

    LONGLONG prevTimeStamp = 0;
    LONGLONG latestTimeStamp = 0;
    LONGLONG frameDuration = 0;

    LARGE_INTEGER freq;

    cv::VideoCapture* videoCapture = nullptr;
    ID3D11ShaderResourceView* _colorSRV;
    ID3D11Device* _device;

    bool isFrameDirty = false;

    BYTE* latestBuffer = new BYTE[FRAME_BUFSIZE];

    cv::Mat frame;
    cv::Mat rgbaFrame;
    int rgbaConversion[8];
};
#endif


