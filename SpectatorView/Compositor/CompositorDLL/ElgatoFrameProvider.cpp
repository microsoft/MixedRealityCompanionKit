// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "stdafx.h"
#include "ElgatoFrameProvider.h"

#if USE_ELGATO

ElgatoFrameProvider::ElgatoFrameProvider()
{
}

ElgatoFrameProvider::~ElgatoFrameProvider()
{
    DestroyGraph();

    isEnabled = false;

    if (frameCallback != NULL)
    {
        delete frameCallback;
        frameCallback = NULL;
    }
}

HRESULT ElgatoFrameProvider::Initialize(ID3D11ShaderResourceView* colorSRV)
{
    if (IsEnabled())
    {
        return S_OK;
    }

    _colorSRV = colorSRV;
    if (colorSRV != nullptr)
    {
        colorSRV->GetDevice(&_device);
    }

    HRESULT hr = E_PENDING;

    frameCallback = new ElgatoSampleCallback(_device);

    hr = InitGraph();
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed on InitGraph.\n");
    }

    if (IsEnabled())
    {
        isEnabled = true;
        hr = S_OK;
    }

    return hr;
}

void ElgatoFrameProvider::Update(int compositeFrameIndex)
{
    if (!IsEnabled() ||
        _colorSRV == nullptr ||
        _device == nullptr ||
        frameCallback == nullptr)
    {
        return;
    }
    
    frameCallback->UpdateSRV(_colorSRV, compositeFrameIndex);
}

bool ElgatoFrameProvider::IsEnabled()
{
    if (frameCallback == nullptr)
    {
        return false;
    }

    return frameCallback->IsEnabled();
}

void ElgatoFrameProvider::Dispose()
{
    DestroyGraph();

    isEnabled = false;

    if (frameCallback != NULL)
    {
        delete frameCallback;
        frameCallback = NULL;
    }
}

HRESULT ElgatoFrameProvider::InitGraph()
{
    HRESULT hr = S_OK;

    // Create the filter graph.
    hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IFilterGraph2, (void **)&pGraph);
    _ASSERT(SUCCEEDED(hr));

    hr = pGraph->QueryInterface(IID_IMediaControl, (void**)&pControl);
    _ASSERT(SUCCEEDED(hr));


    // Add "Elgato Game Capture HD" filter which was registered when installing the elgato capture software.
    hr = CoCreateInstance(CLSID_ElgatoVideoCaptureFilter, NULL, CLSCTX_INPROC, IID_IBaseFilter, (void **)&pElgatoFilter);
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed creating elgato cap filter.\n");
    }

    hr = pElgatoFilter->QueryInterface(IID_IElgatoVideoCaptureFilter6, (void**)&filter);
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed creating elgato filter 6.\n");
    }

    // Set default resolution.
    VIDEO_CAPTURE_FILTER_SETTINGS_EX settings;
    hr = filter->GetSettingsEx(&settings);
    _ASSERT(SUCCEEDED(hr));

    if (FRAME_HEIGHT == 1080)
    {
        settings.Settings.profile = VIDEO_CAPTURE_FILTER_VID_ENC_PROFILE_1080;
    }
    else if (FRAME_HEIGHT == 720)
    {
        settings.Settings.profile = VIDEO_CAPTURE_FILTER_VID_ENC_PROFILE_720;
    }
    else if (FRAME_HEIGHT == 480)
    {
        settings.Settings.profile = VIDEO_CAPTURE_FILTER_VID_ENC_PROFILE_480;
    }
    else if (FRAME_HEIGHT == 360)
    {
        settings.Settings.profile = VIDEO_CAPTURE_FILTER_VID_ENC_PROFILE_360;
    }
    else if (FRAME_HEIGHT == 240)
    {
        settings.Settings.profile = VIDEO_CAPTURE_FILTER_VID_ENC_PROFILE_240;
    }
    else
    {
        // Resolution does not fit filter - default to 1080 and see what happens.
        settings.Settings.profile = VIDEO_CAPTURE_FILTER_VID_ENC_PROFILE_1080;
    }
    filter->SetSettingsEx(&settings);
    SafeRelease(&filter);

    hr = pGraph->AddFilter(pElgatoFilter, L"Elgato Game Capture HD");
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed adding elgato filter.\n");
    }

    // Create the Sample Grabber filter.
    hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&pGrabberF));
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed creating sample grabber.\n");
    }

    hr = pGraph->AddFilter(pGrabberF, L"Sample Grabber");
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed adding grabber.\n");
    }

    hr = pGrabberF->QueryInterface(IID_PPV_ARGS(&pGrabber));
    _ASSERT(SUCCEEDED(hr));
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed creating grabber 2.\n");
    }

    // Set parameters to default resolution and UYVY frame format.
    SetSampleGrabberParameters();

    hr = pElgatoFilter->EnumPins(&pEnum);
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed enumerating pins.\n");
    }

    while (S_OK == pEnum->Next(1, &pPin, NULL))
    {
        hr = ConnectFilters(pGraph, pPin, pGrabberF);
        SafeRelease(&pPin);
        if (SUCCEEDED(hr))
        {
            break;
        }
    }

    if (FAILED(hr))
    {
        OutputDebugString(L"Failed connecting filters.\n");
    }

    // Connect sample grabber to a null renderer.
    hr = CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&pNullF));
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed creating null renderer.\n");
    }

    hr = pGraph->AddFilter(pNullF, L"Null Filter");
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed adding null renderer.\n");
    }

    // Call frame buffer callback on the sample grabber.
    hr = pGrabber->SetCallback(frameCallback, 1);
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed creating grabber.\n");
    }

    hr = ConnectFilters(pGraph, pGrabberF, pNullF);
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed connecting grabber to null renderer.\n");
    }

    // Start playback.
    hr = pControl->Run();
    if (FAILED(hr))
    {
        OutputDebugString(L"Failed starting.\n");
    }

    return hr;
}

HRESULT ElgatoFrameProvider::SetSampleGrabberParameters()
{
    AM_MEDIA_TYPE amt;
    ZeroMemory(&amt, sizeof(AM_MEDIA_TYPE));
    amt.majortype = MEDIATYPE_Video;
    amt.subtype = MEDIASUBTYPE_UYVY;
    amt.formattype = FORMAT_VideoInfo;
    amt.bFixedSizeSamples = TRUE;
    amt.lSampleSize = FRAME_WIDTH * FRAME_HEIGHT * FRAME_BPP_RAW;
    amt.bTemporalCompression = FALSE;
    
    VIDEOINFOHEADER vih;
    ZeroMemory(&vih, sizeof(VIDEOINFOHEADER));
    vih.rcTarget.right = FRAME_WIDTH;
    vih.rcTarget.bottom = FRAME_HEIGHT;
    vih.AvgTimePerFrame = (REFERENCE_TIME)((1.0f / 30.0f) * S2HNS);
    vih.bmiHeader.biWidth = FRAME_WIDTH;
    vih.bmiHeader.biHeight = FRAME_HEIGHT;
    vih.bmiHeader.biSizeImage = FRAME_WIDTH * FRAME_HEIGHT * FRAME_BPP_RAW;
        
    amt.pbFormat = (BYTE*)&vih;
    
    return pGrabber->SetMediaType(&amt);
}

HRESULT ElgatoFrameProvider::DestroyGraph()
{
    HRESULT hr = S_FALSE;

    if (pGrabberF != nullptr)
    {
        hr = pGrabberF->Stop();
        _ASSERT(SUCCEEDED(hr));
    }

    if (pGraph != nullptr)
    {
        hr = pGraph->RemoveFilter(pGrabberF);
        _ASSERT(SUCCEEDED(hr));
    }

    if (pElgatoFilter != nullptr)
    {
        hr = pElgatoFilter->Stop();
        _ASSERT(SUCCEEDED(hr));
    }

    if (pGraph != nullptr)
    {
        hr = pGraph->RemoveFilter(pElgatoFilter);
        _ASSERT(SUCCEEDED(hr));
    }

    if (pControl != nullptr)
    {
        hr = pControl->Stop();
        _ASSERT(SUCCEEDED(hr));
    }

    SafeRelease(&pPin);
    SafeRelease(&pEnum);
    SafeRelease(&pNullF);
    SafeRelease(&pGrabber);
    SafeRelease(&pGrabberF);
    SafeRelease(&pControl);
    SafeRelease(&pElgatoFilter);
    SafeRelease(&filter);
    SafeRelease(&pGraph);
    SafeRelease(&frameCallback);

    return hr;
}

#pragma region DirectShow Filter Logic
HRESULT ElgatoFrameProvider::ConnectFilters(IGraphBuilder *pGraph, IBaseFilter *pSrc, IBaseFilter *pDest)
{
    IPin *pOut = NULL;

    // Find an output pin on the first filter.
    HRESULT hr = FindUnconnectedPin(pSrc, PINDIR_OUTPUT, &pOut);
    if (SUCCEEDED(hr))
    {
        hr = ConnectFilters(pGraph, pOut, pDest);
        pOut->Release();
    }
    return hr;
}

HRESULT ElgatoFrameProvider::ConnectFilters(IGraphBuilder *pGraph, IPin *pOut, IBaseFilter *pDest)
{
    IPin *pIn = NULL;

    // Find an input pin on the downstream filter.
    HRESULT hr = FindUnconnectedPin(pDest, PINDIR_INPUT, &pIn);
    if (SUCCEEDED(hr))
    {
        // Try to connect them.
        hr = pGraph->Connect(pOut, pIn);
        pIn->Release();
    }
    return hr;
}

HRESULT ElgatoFrameProvider::ConnectFilters(IGraphBuilder *pGraph, IBaseFilter *pSrc, IPin *pIn)
{
    IPin *pOut = NULL;

    // Find an output pin on the upstream filter.
    HRESULT hr = FindUnconnectedPin(pSrc, PINDIR_OUTPUT, &pOut);
    if (SUCCEEDED(hr))
    {
        // Try to connect them.
        hr = pGraph->Connect(pOut, pIn);
        pOut->Release();
    }
    return hr;
}

HRESULT ElgatoFrameProvider::MatchPin(IPin *pPin, PIN_DIRECTION direction, BOOL bShouldBeConnected, BOOL *pResult)
{
    _ASSERT(pResult != NULL);

    BOOL bMatch = FALSE;
    BOOL bIsConnected = FALSE;

    HRESULT hr = IsPinConnected(pPin, &bIsConnected);
    if (SUCCEEDED(hr))
    {
        if (bIsConnected == bShouldBeConnected)
        {
            hr = IsPinDirection(pPin, direction, &bMatch);
        }
    }

    if (SUCCEEDED(hr))
    {
        *pResult = bMatch;
    }
    return hr;
}

HRESULT ElgatoFrameProvider::FindUnconnectedPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin)
{
    IEnumPins *pEnum = NULL;
    IPin *pPin = NULL;
    BOOL bFound = FALSE;

    HRESULT hr = pFilter->EnumPins(&pEnum);
    if (FAILED(hr))
    {
        goto done;
    }

    while (S_OK == pEnum->Next(1, &pPin, NULL))
    {
        hr = MatchPin(pPin, PinDir, FALSE, &bFound);
        if (FAILED(hr))
        {
            goto done;
        }
        if (bFound)
        {
            *ppPin = pPin;
            (*ppPin)->AddRef();
            break;
        }
        if (pPin != nullptr)
        {
            pPin->Release();
            pPin = nullptr;
        }
    }

    if (!bFound)
    {
        hr = VFW_E_NOT_FOUND;
    }

done:
    if (pPin != nullptr)
    {
        pPin->Release();
        pPin = nullptr;
    }

    if (pEnum != nullptr)
    {
        pEnum->Release();
        pEnum = nullptr;
    }
    return hr;
}

HRESULT ElgatoFrameProvider::IsPinConnected(IPin *pPin, BOOL *pResult)
{
    IPin *pTmp = NULL;
    HRESULT hr = pPin->ConnectedTo(&pTmp);
    if (SUCCEEDED(hr))
    {
        *pResult = TRUE;
    }
    else if (hr == VFW_E_NOT_CONNECTED)
    {
        // The pin is not connected. This is not an error for our purposes.
        *pResult = FALSE;
        hr = S_OK;
    }

    if (pTmp != nullptr)
    {
        pTmp->Release();
        pTmp = nullptr;
    }
    return hr;
}

HRESULT ElgatoFrameProvider::IsPinDirection(IPin *pPin, PIN_DIRECTION dir, BOOL *pResult)
{
    PIN_DIRECTION pinDir;
    HRESULT hr = pPin->QueryDirection(&pinDir);
    if (SUCCEEDED(hr))
    {
        *pResult = (pinDir == dir);
    }
    return hr;
}
#pragma endregion

#endif
