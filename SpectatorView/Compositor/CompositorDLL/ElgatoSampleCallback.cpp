// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "stdafx.h"
#include "ElgatoSampleCallback.h"


ElgatoSampleCallback::ElgatoSampleCallback(ID3D11Device* device) :
    _device(device)
{
    InitializeCriticalSection(&frameAccessCriticalSection);
    QueryPerformanceFrequency(&freq);
}

ElgatoSampleCallback::~ElgatoSampleCallback()
{
    isEnabled = false;
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
    if (copyLength > FRAME_BUFSIZE)
    {
        // This might happen if the camera is outputting 4K but the system is expecting 1080.
        copyLength = FRAME_BUFSIZE;
    }

    memcpy(latestBuffer, pBuffer, copyLength);

    return S_OK;
}

// Call this from the Render thread.
void ElgatoSampleCallback::UpdateSRV(ID3D11ShaderResourceView* srv)
{
    DirectXHelper::UpdateSRV(_device, srv, latestBuffer, FRAME_WIDTH * FRAME_BPP);
}
