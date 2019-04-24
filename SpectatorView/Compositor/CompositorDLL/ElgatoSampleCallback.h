// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#include "qedit.h"
#include <dshow.h>

#include "DirectXHelper.h"

class ElgatoSampleCallback : public ISampleGrabberCB
{
public:
    ElgatoSampleCallback(ID3D11Device* device);
    ~ElgatoSampleCallback();

    STDMETHODIMP_(ULONG) AddRef() 
    {
        InterlockedIncrement(&m_cRef);
        return m_cRef;
    }
    STDMETHODIMP_(ULONG) Release() 
    {
        // Decrement the object's internal counter.
        ULONG ulRefCount = InterlockedDecrement(&m_cRef);
        if (0 == m_cRef)
        {
            delete this;
        }
        return ulRefCount;
    }

    STDMETHODIMP QueryInterface(REFIID riid, void **ppvObject)
    {
        if (NULL == ppvObject) return E_POINTER;
        if (riid == __uuidof(IUnknown))
        {
            *ppvObject = static_cast<IUnknown*>(this);
            return S_OK;
        }
        if (riid == __uuidof(ISampleGrabberCB))
        {
            *ppvObject = static_cast<ISampleGrabberCB*>(this);
            return S_OK;
        }
        return E_NOTIMPL;
    }

    STDMETHODIMP SampleCB(double time, IMediaSample *pSample)
    {
        return S_OK;
    }

    STDMETHODIMP BufferCB(double time, BYTE *pBuffer, long length);

    void UpdateSRV(ID3D11ShaderResourceView* srv, bool useCPU, int bufferIndex);
    
    LONGLONG GetTimestamp()
    {
        return latestTimeStamp;
    }

    bool IsEnabled()
    {
        return isEnabled;
    }

    int GetCaptureFrameIndex()
    {
        return captureFrameIndex;
    }

private:
    ULONG m_cRef = 0;

    ID3D11Device* _device;

    #define MAX_NUM_CACHED_BUFFERS 20
    BYTE* bufferCache[MAX_NUM_CACHED_BUFFERS];
    int captureFrameIndex;

    BYTE* stagingBytes;

    LONGLONG latestTimeStamp = 0;

    CRITICAL_SECTION frameAccessCriticalSection;
    bool isEnabled = false;
};

