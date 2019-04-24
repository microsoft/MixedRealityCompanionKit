// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "stdafx.h"
#include "ElgatoSampleCallback.h"


ElgatoSampleCallback::ElgatoSampleCallback(ID3D11Device* device) :
    _device(device)
{
    InitializeCriticalSection(&frameAccessCriticalSection);

    for(int i =0; i < MAX_NUM_CACHED_BUFFERS; i++)
        bufferCache[i] = new BYTE[FRAME_BUFSIZE];

    stagingBytes = new BYTE[FRAME_BUFSIZE];

    captureFrameIndex = 0;
}

ElgatoSampleCallback::~ElgatoSampleCallback()
{
    isEnabled = false;
    for (int i = 0; i < MAX_NUM_CACHED_BUFFERS; i++)
        delete [] bufferCache[i];

    delete[] stagingBytes;
}

STDMETHODIMP ElgatoSampleCallback::BufferCB(double time, BYTE *pBuffer, long length)
{
    isEnabled = true;

    // Get frame time.
    LARGE_INTEGER t;
    QueryPerformanceCounter(&t);
    latestTimeStamp = t.QuadPart;

    int copyLength = length;
    if (copyLength > FRAME_BUFSIZE)
    {
        // This might happen if the camera is outputting 4K but the system is expecting 1080.
        copyLength = FRAME_BUFSIZE;
    }

    captureFrameIndex++;
    memcpy(bufferCache[captureFrameIndex%MAX_NUM_CACHED_BUFFERS], pBuffer, copyLength);
    return S_OK;
}

// Call this from the Render thread.
void ElgatoSampleCallback::UpdateSRV(ID3D11ShaderResourceView* srv, bool useCPU, int bufferIndex)
{
    BYTE* srcBuffer = bufferCache[bufferIndex%MAX_NUM_CACHED_BUFFERS];
    if (useCPU)
    {
        // Do not cache when using the CPU
        DirectXHelper::ConvertYUVtoBGRA(srcBuffer, stagingBytes, FRAME_WIDTH, FRAME_HEIGHT, true);
        DirectXHelper::UpdateSRV(_device, srv, stagingBytes, FRAME_WIDTH * FRAME_BPP);
    }
    else
    {
        DirectXHelper::UpdateSRV(_device, srv, srcBuffer, FRAME_WIDTH * FRAME_BPP);
    }
}
