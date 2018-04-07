// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "stdafx.h"
#include "OpenCVFrameProvider.h"

#if USE_OPENCV

OpenCVFrameProvider::OpenCVFrameProvider()
{
    QueryPerformanceFrequency(&freq);
    rgbaFrame = cv::Mat(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC4);

    for (int i = 0; i < 4; i++)
    {
        rgbaConversion[i * 2] = i;
        rgbaConversion[(i * 2) + 1] = i;
    }
}

OpenCVFrameProvider::~OpenCVFrameProvider()
{
}

HRESULT OpenCVFrameProvider::Initialize(ID3D11ShaderResourceView* srv)
{
    if (IsEnabled())
    {
        return S_OK;
    }

    _colorSRV = srv;
    if (_colorSRV != nullptr)
    {
        _colorSRV->GetDevice(&_device);
    }

    HRESULT hr = E_PENDING;
    videoCapture = new cv::VideoCapture(CAMERA_ID);

    videoCapture->open(CAMERA_ID);

    // Attempt to update camera resolution to desired resolution.
    // Note: This may fail, and your capture will resume at the camera's native resolution.
    // In this case, the Update loop will print an error with the expected frame resolution.
    videoCapture->set(cv::CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
    videoCapture->set(cv::CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);

    if (IsEnabled())
    {
        hr = S_OK;
    }

    return hr;
}

bool OpenCVFrameProvider::IsEnabled()
{
    if (videoCapture != nullptr)
    {
        return videoCapture->isOpened();
    }

    return false;
}

void OpenCVFrameProvider::Update()
{
    if (!IsEnabled() ||
        _colorSRV == nullptr ||
        _device == nullptr)
    {
        return;
    }

    concurrency::create_task([=]
    {
        if (videoCapture->grab())
        {
            // Get frame time and duration.
            LARGE_INTEGER time;
            QueryPerformanceCounter(&time);

            prevTimeStamp = latestTimeStamp;
            latestTimeStamp = time.QuadPart;

            if (prevTimeStamp != 0 && latestTimeStamp != 0)
            {
                frameDuration = ((latestTimeStamp - prevTimeStamp) * S2HNS) / freq.QuadPart;
            }

            videoCapture->retrieve(frame);

            double width = videoCapture->get(cv::CAP_PROP_FRAME_WIDTH);
            double height = videoCapture->get(cv::CAP_PROP_FRAME_HEIGHT);

            if (width != FRAME_WIDTH)
            {
                OutputDebugString(L"ERROR: captured width does not equal FRAME_WIDTH.  Expecting: ");
                OutputDebugString(std::to_wstring(width).c_str());
                OutputDebugString(L"\n");
            }

            if (height != FRAME_HEIGHT)
            {
                OutputDebugString(L"ERROR: captured height does not equal FRAME_HEIGHT.  Expecting: ");
                OutputDebugString(std::to_wstring(height).c_str());
                OutputDebugString(L"\n");
            }

            // Convert to rgba
            std::lock_guard<std::mutex> lock(frameAccessLock);
            mixChannels(&frame, 2, &rgbaFrame, 1, rgbaConversion, 4);
            memcpy(latestBuffer, rgbaFrame.data, FRAME_BUFSIZE);
            isFrameDirty = true;
        }
    });

    if (isFrameDirty)
    {
        std::lock_guard<std::mutex> lock(frameAccessLock);
        DirectXHelper::UpdateSRV(_device, _colorSRV, latestBuffer, FRAME_WIDTH * FRAME_BPP);
        isFrameDirty = false;
    }
}

LONGLONG OpenCVFrameProvider::GetDurationHNS()
{
    if (frameDuration != 0)
    {
        return frameDuration;
    }

    return (LONGLONG)((1.0f / 30.0f) * S2HNS);
}

void OpenCVFrameProvider::Dispose()
{
    if (videoCapture != nullptr)
    {
        videoCapture->release();
        videoCapture = nullptr;
    }
}
#endif

