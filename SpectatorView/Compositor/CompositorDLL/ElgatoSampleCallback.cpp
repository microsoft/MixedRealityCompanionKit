// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "stdafx.h"
#include "ElgatoSampleCallback.h"


ElgatoSampleCallback::ElgatoSampleCallback(ID3D11Device* device) :
    _device(device)
{
    QueryPerformanceFrequency(&freq);

    for (int i = 0; i < MAX_NUM_CACHED_BUFFERS; i++)
    {
        bufferCache[i].buffer = new BYTE[FrameProviderStaticConfig::getFrameBuffSize()];
        bufferCache[i].timeStamp = 0;
    }

    captureFrameIndex = 0;
}

ElgatoSampleCallback::~ElgatoSampleCallback()
{
    isEnabled = false;

    for (int i = 0; i < MAX_NUM_CACHED_BUFFERS; i++)
    {
        delete[] bufferCache[i].buffer;
    }
}

STDMETHODIMP ElgatoSampleCallback::BufferCB(double time, BYTE *pBuffer, long length)
{
    isEnabled = true;

    // Get frame time.
    prevTimeStamp = latestTimeStamp;
    LARGE_INTEGER t;
    QueryPerformanceCounter(&t);
    latestTimeStamp = t.QuadPart;

    if (prevTimeStamp != 0 && latestTimeStamp != 0)
    {
        frameDuration = ((latestTimeStamp - prevTimeStamp) * S2HNS) / freq.QuadPart;
    }

    int copyLength = length;
    if (copyLength > FrameProviderStaticConfig::getFrameBuffSize())
    {
        // This might happen if the camera is outputting 4K but the system is expecting 1080.
        copyLength = static_cast<int>(FrameProviderStaticConfig::getFrameBuffSize());
    }

    captureFrameIndex++;
    BYTE* buffer = bufferCache[captureFrameIndex % MAX_NUM_CACHED_BUFFERS].buffer;
    memcpy(buffer, pBuffer, copyLength);

    bufferCache[captureFrameIndex % MAX_NUM_CACHED_BUFFERS].timeStamp = (latestTimeStamp * S2HNS) / freq.QuadPart;

    return S_OK;
}

// Call this from the Render thread.
void ElgatoSampleCallback::UpdateSRV(ID3D11ShaderResourceView* srv, int compositeFrameIndex)
{
    const BufferCache& buffer = bufferCache[compositeFrameIndex % MAX_NUM_CACHED_BUFFERS];
    if (buffer.buffer != nullptr)
    {
        DirectXHelper::UpdateSRV(_device, srv, buffer.buffer, FrameProviderStaticConfig::width * FRAME_BPP);
    }
}
