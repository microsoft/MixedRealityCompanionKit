// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "stdafx.h"
#include "ElgatoSampleCallback.h"


ElgatoSampleCallback::ElgatoSampleCallback(ID3D11Device* device) :
    _device(device)
{
    InitializeCriticalSection(&frameAccessCriticalSection);
}

ElgatoSampleCallback::~ElgatoSampleCallback()
{
    isEnabled = false;
}

STDMETHODIMP ElgatoSampleCallback::BufferCB(double time, BYTE *pBuffer, long length)
{
    isEnabled = true;

    // Get frame time.
    LARGE_INTEGER t;
    QueryPerformanceCounter(&t);
    thirdTimeStamp = secondTimeStamp;
    secondTimeStamp = latestTimeStamp;
    latestTimeStamp = t.QuadPart;

    int copyLength = length;
    if (copyLength > FRAME_BUFSIZE)
    {
        // This might happen if the camera is outputting 4K but the system is expecting 1080.
        copyLength = FRAME_BUFSIZE;
    }

    memcpy(thirdCachedBuffer, secondCachedBuffer, FRAME_BUFSIZE);
    memcpy(secondCachedBuffer, latestBuffer, FRAME_BUFSIZE);
    memcpy(latestBuffer, pBuffer, copyLength);

    EnterCriticalSection(&frameAccessCriticalSection);
    isVideoFrameReady = true;
    LeaveCriticalSection(&frameAccessCriticalSection);
    return S_OK;
}

// Call this from the Render thread.
void ElgatoSampleCallback::UpdateSRV(ID3D11ShaderResourceView* srv, bool useCPU)
{
    if (useCPU)
    {
        BYTE* stagingBytes = new BYTE[FRAME_BUFSIZE];
        // Do not cache when using the CPU
        DirectXHelper::ConvertYUVtoBGRA(latestBuffer, stagingBytes, FRAME_WIDTH, FRAME_HEIGHT, true);
        DirectXHelper::UpdateSRV(_device, srv, stagingBytes, FRAME_WIDTH * FRAME_BPP);
        delete[] stagingBytes;
    }
    else
    {
        DirectXHelper::UpdateSRV(_device, srv, thirdCachedBuffer, FRAME_WIDTH * FRAME_BPP);
    }
}

bool ElgatoSampleCallback::IsVideoFrameReady()
{
    EnterCriticalSection(&frameAccessCriticalSection);
    bool ret = isVideoFrameReady;
    if (isVideoFrameReady)
    {
        isVideoFrameReady = false;
    }
    LeaveCriticalSection(&frameAccessCriticalSection);

    return ret;
}