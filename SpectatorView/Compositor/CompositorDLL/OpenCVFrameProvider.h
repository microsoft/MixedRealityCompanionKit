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
private:
    std::mutex frameAccessLock;
    std::mutex videoLock;

    LONGLONG latestTimeStamp = 0;

    LARGE_INTEGER freq;

    cv::VideoCapture* videoCapture = nullptr;
    ID3D11ShaderResourceView* _colorSRV;
    ID3D11Device* _device;

    cv::Mat frame;
    cv::Mat rgbaFrame;
    int rgbaConversion[8];

    class BufferCache
    {
    public:
        BYTE * buffer;
        LONGLONG timeStamp;
    };

    BufferCache bufferCache[MAX_NUM_CACHED_BUFFERS];
    int captureFrameIndex = 0;

    BufferCache& GetOldestBuffer();

public:
    OpenCVFrameProvider();
    ~OpenCVFrameProvider();

    // Inherited via IFrameProvider
    virtual HRESULT Initialize(ID3D11ShaderResourceView* srv) override;
    virtual bool IsEnabled() override;
    virtual void Update(int compositeFrameIndex) override;
    virtual void Dispose() override;

    virtual bool OutputYUV()
    {
        return false;
    }

    virtual LONGLONG GetTimestamp(int frame)
    {
        return bufferCache[frame % MAX_NUM_CACHED_BUFFERS].timeStamp;
    }

    virtual LONGLONG GetDurationHNS()
    {
        return (LONGLONG)((1.0f / 60.0f) * S2HNS);
    }

    virtual int GetCaptureFrameIndex()
    {
        return captureFrameIndex;
    }
};
#endif


