// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#include "qedit.h"
#include <dshow.h>

#include "DirectXHelper.h"

class ElgatoSampleCallback : public ISampleGrabberCB
{
private:
    class BufferCache
    {
    public:
        BYTE * buffer;
        LONGLONG timeStamp;
    };

    BufferCache bufferCache[MAX_NUM_CACHED_BUFFERS];
    int captureFrameIndex = 0;

    ULONG m_cRef = 0;

    ID3D11Device* _device;

    LONGLONG prevTimeStamp = 0;
    LONGLONG latestTimeStamp = 0;

    LARGE_INTEGER freq;
    LONGLONG frameDuration = 0;

    bool isEnabled = false;

public:
    ElgatoSampleCallback(ID3D11Device* device);
    ~ElgatoSampleCallback();

    LONGLONG GetTimestamp(int frame)
    {
        return bufferCache[frame % MAX_NUM_CACHED_BUFFERS].timeStamp;
    }

    int GetCaptureFrameIndex()
    {
        return captureFrameIndex;
    }

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

    void UpdateSRV(ID3D11ShaderResourceView* srv, int compositeFrameIndex);
    
    LONGLONG GetTimestamp()
    {
        return latestTimeStamp;
    }

    LONGLONG GetDurationHNS()
    {
        return frameDuration;
    }

    bool IsEnabled()
    {
        return isEnabled;
    }
};

