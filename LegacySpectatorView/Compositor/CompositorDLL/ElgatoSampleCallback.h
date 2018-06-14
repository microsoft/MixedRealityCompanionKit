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

    void UpdateSRV(ID3D11ShaderResourceView* srv, bool useCPU);
    
    LONGLONG GetTimestamp()
    {
        return thirdTimeStamp;
    }

    bool IsVideoFrameReady();
    bool IsEnabled()
    {
        return isEnabled;
    }

private:
    ULONG m_cRef = 0;

    ID3D11Device* _device;
    BYTE* thirdCachedBuffer = new BYTE[FRAME_BUFSIZE];
    BYTE* secondCachedBuffer = new BYTE[FRAME_BUFSIZE];
    BYTE* latestBuffer = new BYTE[FRAME_BUFSIZE];

    LONGLONG latestTimeStamp = 0;
    LONGLONG secondTimeStamp = 0;
    LONGLONG thirdTimeStamp = 0;

    CRITICAL_SECTION frameAccessCriticalSection;
    bool isVideoFrameReady = false;
    bool isEnabled = false;
};

