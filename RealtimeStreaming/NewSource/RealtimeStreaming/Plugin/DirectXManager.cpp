// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"

#include "DirectXManager.h"

// Check for SDK Layer support.
inline HRESULT SdkLayersAvailable()
{
    return ExceptionBoundary([&]() -> HRESULT
    {
        return D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_NULL,       // There is no need to create a real hardware device.
            0,
            D3D11_CREATE_DEVICE_DEBUG,  // Check for the SDK layers.
            nullptr,                    // Any feature level will do.
            0,
            D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Windows Store apps.
            nullptr,                    // No need to keep the D3D device reference.
            nullptr,                    // No need to know the feature level.
            nullptr                     // No need to keep the D3D device context reference.
        );
    });
}

_Use_decl_annotations_
DirectXManager::DirectXManager(ID3D11Device* pD3D11Device)
    : _deviceType(DeviceType_Null)
    , _spDevice(nullptr)
    , _spContext(nullptr)
    , _spDevice2(nullptr)
    , _spContext2(nullptr)
    , _spDxgiOutput(nullptr)
{
    Log(Log_Level_Info, L"DeviceResource::RuntimeClassInitialize()\n");

    HRESULT hr = S_OK;

    IFR_MSG(MFStartup(MF_VERSION), L"Not able to start Media Foundation.");

    auto lock = _lock.Lock();
    com_ptr<ID3D11Device> altDevice;
    com_ptr<ID3D11DeviceContext> altContext;
    D3D_FEATURE_LEVEL featureLevel;
    com_ptr<IDXGIFactory1> spFactory;
    com_ptr<IDXGIAdapter> spAdapter;

    // store local version of passed in device
    if (nullptr != pD3D11Device)
    {
        com_ptr<ID3D11Device> device;
        IFC(pD3D11Device->QueryInterface(IID_PPV_ARGS(&device)));

        com_ptr<ID3D11DeviceContext> ctx;
        device->GetImmediateContext(&ctx);
        if (nullptr == ctx)
        {
            IFC_MSG(E_UNEXPECTED, L"not able to get the ID3D11DeviceContext from passed in device.");
        }

        // set local version of the device/context
        IFC(device.as(&_spDevice));

        IFC(ctx.as(&_spContext));
    }

    // create d3d for media foundation device
    // Create the Direct3D 11 API device object and a corresponding context.

    // This flag adds support for surfaces with a different color channel ordering
    // than the API default. It is required for compatibility with Direct2D.
    UINT creationFlags = D3D11_CREATE_DEVICE_VIDEO_SUPPORT | D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
    if (SUCCEEDED(SdkLayersAvailable()))
    {
        // If the project is in a debug build, enable debugging via SDK Layers with this flag.
        creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
    }
#endif

    // This array defines the set of DirectX hardware feature levels this app will support.
    // Note the ordering should be preserved.
    // Don't forget to declare your application's minimum required feature level in its
    // description.  All applications are assumed to support 9.1 unless otherwise stated.
    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1
    };

    {
        hr = D3D11CreateDevice(
            nullptr,                    // Specify nullptr to use the default adapter.
            D3D_DRIVER_TYPE_HARDWARE,   // Create a device using the hardware graphics driver.
            0,                          // Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
            creationFlags,              // Set debug and Direct2D compatibility flags.
            featureLevels,              // List of feature levels this app can support.
            ARRAYSIZE(featureLevels),   // Size of the list above.
            D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Windows Store apps.
            &altDevice,                  // Returns the Direct3D device created.
            &featureLevel,              // Returns feature level of device created.
            &altContext                  // Returns the device immediate context.
        );

        if (FAILED(hr))
        {
            // If the initialization fails, fall back to the WARP device.
            // For more information on WARP, see: 
            // http://go.microsoft.com/fwlink/?LinkId=286690
            IFC(D3D11CreateDevice(
                nullptr,
                D3D_DRIVER_TYPE_WARP, // Create a WARP device instead of a hardware device.
                0,
                creationFlags,
                featureLevels,
                ARRAYSIZE(featureLevels),
                D3D11_SDK_VERSION,
                &altDevice,
                &featureLevel,
                &altContext));

            _deviceType = DeviceType_Warp;
        }
        else
        {
            // workaround for nvidia GPU's
            com_ptr<ID3D11VideoDevice> videoDevice;
            altDevice.As(&videoDevice);

            // MF support for shared textures
            com_ptr<ID3D10Multithread> spMultithread;
            if (SUCCEEDED(altDevice.As(&spMultithread)))
            {
                spMultithread->SetMultithreadProtected(TRUE);
            }
            else
            {
                Log(Log_Level_Warning, L"%s(%d): not able to QI for ID3D10Multithread.", __FILEW__, __LINE__);
            }

            _deviceType = DeviceType_DX11;
        }
    }


    // assumes primary adapter
    IFC(CreateDXGIFactory1(IID_PPV_ARGS(&spFactory)));

    IFC(spFactory->EnumAdapters(0, &spAdapter));

    IFC(spAdapter->EnumOutputs(0, &_spDxgiOutput));

    // Init complete, set member variables
    IFC(altDevice.As(&_spDevice2));
    IFC(altContext.As(&_spContext2));

    // store a copy of the created device
    // if one wasn't provided
    if (nullptr == pD3D11Device)
    {
        IFC(_spDevice2.As(&_spDevice));
        IFC(_spContext2.As(&_spContext));
    }

    _featureLevel = featureLevel;

done:
    if (FAILED(hr))
    {
        Uninitialize();
    }

    IFT(hr);
}

_Use_decl_annotations_
DirectXManager::~DirectXManager()
{
    Log(Log_Level_Info, L"DeviceResource::Uninitialize()\n");

    auto lock = _lock.Lock();

    _deviceType = DeviceType_Null;
    _spDevice = nullptr;
    _spContext = nullptr;
    _spDevice2 = nullptr;
    _spContext2 = nullptr;
    _spDxgiOutput = nullptr;
}


// Private
_Use_decl_annotations_
void DirectXManager::Lost()
{
    Log(Log_Level_Info, L"DeviceResource::Lost()\n");
}

_Use_decl_annotations_
void DirectXManager::Reset()
{
    Log(Log_Level_Info, L"DeviceResource::Reset()\n");
}

_Use_decl_annotations_
HRESULT DirectXManager::GetFrameStatistics(DXGI_FRAME_STATISTICS& stats)
{
    auto lock = _lock.Lock();

    HRESULT hr = E_UNEXPECTED;

    if (nullptr != _spDxgiOutput)
    {
        hr = _spDxgiOutput->GetFrameStatistics(&stats);
    }

    return S_OK;
}

_Use_decl_annotations_
HRESULT DirectXManager::WaitForVBlank()
{
    auto lock = _lock.Lock();

    HRESULT hr = E_UNEXPECTED;

    if (nullptr != _spDxgiOutput)
    {
        hr = _spDxgiOutput.get()->WaitForVBlank();
    }

    return hr;
}

