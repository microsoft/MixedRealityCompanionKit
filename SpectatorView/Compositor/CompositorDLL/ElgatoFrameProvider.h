// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "IFrameProvider.h"
#include "DirectXHelper.h"
#include "IVideoCaptureFilterTypes.h"
#include "IVideoCaptureFilter.h"
#include "ElgatoSampleCallback.h"

#include <dshow.h>
#include <initguid.h>
#include <wmcodecdsp.h>
#include <mmreg.h>
#include <dvdmedia.h>
#include <bdaiface.h>
#include "qedit.h"

#include <atlcomcli.h>

#pragma comment(lib, "wmcodecdspuuid")    
#pragma comment(lib, "dxguid")
#pragma comment(lib, "strmbase")

DEFINE_GUID(CLSID_ElgatoVideoCaptureFilter,
    0x39f50f4c, 0x99e1, 0x464a, 0xb6, 0xf9, 0xd6, 0x5, 0xb4, 0xfb, 0x59, 0x18);

DEFINE_GUID(IID_IElgatoVideoCaptureFilterEnumeration,
    0x13dd0ccf, 0xa773, 0x4cb7, 0x8c, 0x98, 0x8e, 0x31, 0xe6, 0x9f, 0x2, 0x52);

DEFINE_GUID(IID_IElgatoVideoCaptureFilter6,
    0x39f50f4c, 0x99e1, 0x464a, 0xb6, 0xf9, 0xd6, 0x05, 0xb4, 0xfb, 0x59, 0x25);

using namespace ElgatoGameCapture;

class ElgatoFrameProvider : public IFrameProvider
{
public:

    ElgatoFrameProvider(bool useCPU = false);
    ~ElgatoFrameProvider();

    virtual HRESULT Initialize(ID3D11ShaderResourceView* colorSRV, ID3D11Texture2D* outputTexture);
    virtual LONGLONG GetTimestamp(int frame);

    virtual LONGLONG GetDurationHNS();

    ProviderType GetProviderType() { return Elgato; }

    virtual void Update(int compositeFrameIndex);

    virtual bool IsEnabled();

    bool SupportsOutput()
    {
        return false;
    }

    virtual void Dispose();

    bool OutputYUV()
    {
        return true;
    }

    int GetCaptureFrameIndex()
    {
        if(frameCallback)
            return frameCallback->GetCaptureFrameIndex();

        return 0;
    }


private:
    ID3D11ShaderResourceView* _colorSRV;
    ID3D11Device* _device;

    bool _useCPU = false;
    HRESULT errorCode = S_OK;

    HRESULT InitGraph();
    HRESULT DestroyGraph();

    // From https://msdn.microsoft.com/en-us/library/windows/desktop/dd387915(v=vs.85).aspx
    HRESULT ConnectFilters(IGraphBuilder *pGraph, IBaseFilter *pSrc, IBaseFilter *pDest);
    HRESULT ConnectFilters(IGraphBuilder *pGraph, IBaseFilter *pSrc, IPin *pIn);
    HRESULT ConnectFilters(IGraphBuilder *pGraph, IPin *pOut, IBaseFilter *pDest);
    HRESULT FindUnconnectedPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin);
    // From: https://msdn.microsoft.com/en-us/library/windows/desktop/dd375792(v=vs.85).aspx
    HRESULT MatchPin(IPin *pPin, PIN_DIRECTION direction, BOOL bShouldBeConnected, BOOL *pResult);
    HRESULT IsPinConnected(IPin *pPin, BOOL *pResult);
    HRESULT IsPinDirection(IPin *pPin, PIN_DIRECTION dir, BOOL *pResult);

    HRESULT SetSampleGrabberParameters();

    IFilterGraph2 *pGraph = NULL;
    IMediaControl *pControl = NULL;
    IBaseFilter *pElgatoFilter = NULL;
    IBaseFilter *pGrabberF = NULL;
    ISampleGrabber *pGrabber = NULL;
    IEnumPins *pEnum = NULL;
    IPin *pPin = NULL;
    IBaseFilter *pNullF = NULL;
    ElgatoSampleCallback *frameCallback = NULL;
    IElgatoVideoCaptureFilter6 *filter = NULL;
};
