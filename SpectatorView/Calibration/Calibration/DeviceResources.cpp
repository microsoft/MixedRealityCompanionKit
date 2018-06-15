// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "stdafx.h"
#include "DeviceResources.h"
#include "FrameProviderStaticConfig.h"

using namespace DirectX;

using Microsoft::WRL::ComPtr;

namespace
{
#if defined(_DEBUG)
    // Check for SDK Layer support.
    inline bool SdkLayersAvailable()
    {
        HRESULT hr = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_NULL,       // There is no need to create a real hardware device.
            0,
            D3D11_CREATE_DEVICE_DEBUG,  // Check for the SDK layers.
            nullptr,                    // Any feature level will do.
            0,
            D3D11_SDK_VERSION,
            nullptr,                    // No need to keep the D3D device reference.
            nullptr,                    // No need to know the feature level.
            nullptr                     // No need to keep the D3D device context reference.
            );

        return SUCCEEDED(hr);
    }
#endif
};

// Constructor for DeviceResources.
DX::DeviceResources::DeviceResources(DXGI_FORMAT backBufferFormat, DXGI_FORMAT depthBufferFormat, UINT backBufferCount, D3D_FEATURE_LEVEL minFeatureLevel) :
    m_screenViewport{},
    m_backBufferFormat(backBufferFormat),
    m_depthBufferFormat(depthBufferFormat),
    m_backBufferCount(backBufferCount),
    m_d3dMinFeatureLevel(minFeatureLevel),
    m_window(0),
    m_d3dFeatureLevel(D3D_FEATURE_LEVEL_9_1),
    m_outputSize{0, 0, 1, 1},
    m_deviceNotify(nullptr)
{
}

// Configures the Direct3D device, and stores handles to it and the device context.
void DX::DeviceResources::CreateDeviceResources() 
{
    UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
    if (SdkLayersAvailable())
    {
        // If the project is in a debug build, enable debugging via SDK Layers with this flag.
        creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
    }
    else
    {
        OutputDebugStringA("WARNING: Direct3D Debug Device is not available\n");
    }
#endif

    // Determine DirectX hardware feature levels this app will support.
    static const D3D_FEATURE_LEVEL s_featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };

    UINT featLevelCount = 0;
    for (; featLevelCount < _countof(s_featureLevels); ++featLevelCount)
    {
        if (s_featureLevels[featLevelCount] < m_d3dMinFeatureLevel)
            break;
    }

    if (!featLevelCount)
    {
        throw std::out_of_range("minFeatureLevel too high");
    }

    ComPtr<IDXGIAdapter1> adapter;
    GetHardwareAdapter(adapter.GetAddressOf());

    // Create the Direct3D 11 API device object and a corresponding context.
    HRESULT hr = E_FAIL;
    if (adapter)
    {
        hr = D3D11CreateDevice(
            adapter.Get(),
            D3D_DRIVER_TYPE_UNKNOWN,
            0,
            creationFlags,
            s_featureLevels,
            featLevelCount,
            D3D11_SDK_VERSION,
            m_d3dDevice.ReleaseAndGetAddressOf(),   // Returns the Direct3D device created.
            &m_d3dFeatureLevel,                     // Returns feature level of device created.
            m_d3dContext.ReleaseAndGetAddressOf()   // Returns the device immediate context.
            );

        if (hr == E_INVALIDARG && featLevelCount > 1)
        {
            assert(s_featureLevels[0] == D3D_FEATURE_LEVEL_11_1);

            // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
            hr = D3D11CreateDevice(
                adapter.Get(),
                D3D_DRIVER_TYPE_UNKNOWN,
                nullptr,
                creationFlags,
                &s_featureLevels[1],
                featLevelCount - 1,
                D3D11_SDK_VERSION,
                m_d3dDevice.ReleaseAndGetAddressOf(),
                &m_d3dFeatureLevel,
                m_d3dContext.ReleaseAndGetAddressOf()
                );
        }
    }
#if defined(NDEBUG)
    else
    {
        throw std::exception("No Direct3D hardware device found");
    }
#else
    if (FAILED(hr))
    {
        // If the initialization fails, fall back to the WARP device.
        // For more information on WARP, see: 
        // http://go.microsoft.com/fwlink/?LinkId=286690
        hr = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_WARP, // Create a WARP device instead of a hardware device.
            0,
            creationFlags,
            s_featureLevels,
            featLevelCount,
            D3D11_SDK_VERSION,
            m_d3dDevice.ReleaseAndGetAddressOf(),
            &m_d3dFeatureLevel,
            m_d3dContext.ReleaseAndGetAddressOf()
            );

        if (hr == E_INVALIDARG && featLevelCount > 1)
        {
            assert(s_featureLevels[0] == D3D_FEATURE_LEVEL_11_1);

            // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
            hr = D3D11CreateDevice(
                nullptr,
                D3D_DRIVER_TYPE_WARP,
                nullptr,
                creationFlags,
                &s_featureLevels[1],
                featLevelCount - 1,
                D3D11_SDK_VERSION,
                m_d3dDevice.ReleaseAndGetAddressOf(),
                &m_d3dFeatureLevel,
                m_d3dContext.ReleaseAndGetAddressOf()
                );
        }

        if (SUCCEEDED(hr))
        {
            OutputDebugStringA("Direct3D Adapter - WARP\n");
        }
    }
#endif

    Calibration::DX::ThrowIfFailed(hr);

#ifndef NDEBUG
    ComPtr<ID3D11Debug> d3dDebug;
    if (SUCCEEDED(m_d3dDevice.As(&d3dDebug)))
    {
        ComPtr<ID3D11InfoQueue> d3dInfoQueue;
        if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
        {
#ifdef _DEBUG
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
            D3D11_MESSAGE_ID hide [] =
            {
                D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
            };
            D3D11_INFO_QUEUE_FILTER filter;
            memset(&filter, 0, sizeof(filter));
            filter.DenyList.NumIDs = _countof(hide);
            filter.DenyList.pIDList = hide;
            d3dInfoQueue->AddStorageFilterEntries(&filter);
        }
    }
#endif

    // Obtain Direct3D 11.1 interfaces (if available)
    if (SUCCEEDED(m_d3dDevice.As(&m_d3dDevice1)))
    {
        (void) m_d3dContext.As(&m_d3dContext1);
    }
}

// These resources need to be recreated every time the window size is changed.
void DX::DeviceResources::CreateWindowSizeDependentResources() 
{
    if (!m_window)
    {
        throw std::exception("Call SetWindow with a valid Win32 window handle");
    }

    // Clear the previous window size specific context.
    ID3D11RenderTargetView* nullViews[] = {nullptr};
    m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
    m_d3dRenderTargetView.Reset();
    m_d3dDepthStencilView.Reset();
    m_d3dContext->Flush();

    // Determine the render target size in pixels.
    // Force backbuffer to captured image dimensions.
    UINT backBufferWidth = FrameProviderStaticConfig::width;// std::max<UINT>(m_outputSize.right - m_outputSize.left, 1);
    UINT backBufferHeight = FrameProviderStaticConfig::height;// std::max<UINT>(m_outputSize.bottom - m_outputSize.top, 1);

    if (m_swapChain)
    {
        // If the swap chain already exists, resize it.
        HRESULT hr = m_swapChain->ResizeBuffers(
            m_backBufferCount,
            backBufferWidth,
            backBufferHeight,
            m_backBufferFormat,
            0
            );

        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
        {
#ifdef _DEBUG
            char buff[64] = {};
            sprintf_s(buff, "Device Lost on ResizeBuffers: Reason code 0x%08X\n", (hr == DXGI_ERROR_DEVICE_REMOVED) ? m_d3dDevice->GetDeviceRemovedReason() : hr);
            OutputDebugStringA(buff);
#endif
            // If the device was removed for any reason, a new device and swap chain will need to be created.
            HandleDeviceLost();

            // Everything is set up now. Do not continue execution of this method. HandleDeviceLost will reenter this method 
            // and correctly set up the new device.
            return;
        }
        else
        {
            Calibration::DX::ThrowIfFailed(hr);
        }
    }
    else
    {
        // Otherwise, create a new one using the same adapter as the existing Direct3D device.

        // This sequence obtains the DXGI factory that was used to create the Direct3D device above.
        ComPtr<IDXGIDevice1> dxgiDevice;
        Calibration::DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

        ComPtr<IDXGIAdapter> dxgiAdapter;
        Calibration::DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

        ComPtr<IDXGIFactory1> dxgiFactory;
        Calibration::DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

        ComPtr<IDXGIFactory2> dxgiFactory2;
        if (SUCCEEDED(dxgiFactory.As(&dxgiFactory2)))
        {
            // DirectX 11.1 or later
            DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
            swapChainDesc.Width = backBufferWidth;
            swapChainDesc.Height = backBufferHeight;
            swapChainDesc.Format = m_backBufferFormat;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.BufferCount = m_backBufferCount;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
            swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
            swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

            DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = { 0 };
            fsSwapChainDesc.Windowed = TRUE;

            // Create a SwapChain from a Win32 window.
            Calibration::DX::ThrowIfFailed(dxgiFactory2->CreateSwapChainForHwnd(
                m_d3dDevice.Get(),
                m_window,
                &swapChainDesc,
                &fsSwapChainDesc,
                nullptr, m_swapChain1.ReleaseAndGetAddressOf()
                ));

            Calibration::DX::ThrowIfFailed(m_swapChain1.As(&m_swapChain));
        }
        else
        {
            // DirectX 11.0
            DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
            swapChainDesc.BufferDesc.Width = backBufferWidth;
            swapChainDesc.BufferDesc.Height = backBufferHeight;
            swapChainDesc.BufferDesc.Format = m_backBufferFormat;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.BufferCount = m_backBufferCount;
            swapChainDesc.OutputWindow = m_window;
            swapChainDesc.Windowed = TRUE;
            swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

            Calibration::DX::ThrowIfFailed(dxgiFactory->CreateSwapChain(
                m_d3dDevice.Get(),
                &swapChainDesc,
                m_swapChain.ReleaseAndGetAddressOf()
                ));
        }

        // This class does not support exclusive full-screen mode and prevents DXGI from responding to the ALT+ENTER shortcut
        Calibration::DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
    }

    // Create a render target view of the swap chain back buffer.
    ComPtr<ID3D11Texture2D> backBuffer;
    Calibration::DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

    Calibration::DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(
        backBuffer.Get(),
        nullptr,
        m_d3dRenderTargetView.ReleaseAndGetAddressOf()
        ));

    // Create a depth stencil view for use with 3D rendering if needed.
    CD3D11_TEXTURE2D_DESC depthStencilDesc(
        m_depthBufferFormat, 
        backBufferWidth,
        backBufferHeight,
        1, // This depth stencil view has only one texture.
        1, // Use a single mipmap level.
        D3D11_BIND_DEPTH_STENCIL
        );

    ComPtr<ID3D11Texture2D> depthStencil;
    Calibration::DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(
        &depthStencilDesc,
        nullptr,
        depthStencil.GetAddressOf()
        ));

    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    Calibration::DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(
        depthStencil.Get(),
        &depthStencilViewDesc,
        m_d3dDepthStencilView.ReleaseAndGetAddressOf()
        ));
    
    // Set the 3D rendering viewport to target the entire window.
    m_screenViewport = CD3D11_VIEWPORT(
        0.0f,
        0.0f,
        static_cast<float>(backBufferWidth),
        static_cast<float>(backBufferHeight)
        );
}

// This method is called when the Win32 window is created (or re-created).
void DX::DeviceResources::SetWindow(HWND window, int width, int height)
{
    m_window = window;

    m_outputSize.left = m_outputSize.top = 0;
    m_outputSize.right = width;
    m_outputSize.bottom = height;
}

// This method is called when the Win32 window changes size
bool DX::DeviceResources::WindowSizeChanged(int width, int height)
{
    RECT newRc;
    newRc.left = newRc.top = 0;
    newRc.right = width;
    newRc.bottom = height;
    if (newRc == m_outputSize)
    {
        return false;
    }

    m_outputSize = newRc;
    CreateWindowSizeDependentResources();
    return true;
}

// Recreate all device resources and set them back to the current state.
void DX::DeviceResources::HandleDeviceLost()
{
    if (m_deviceNotify)
    {
        m_deviceNotify->OnDeviceLost();
    }

    m_d3dDepthStencilView.Reset();
    m_d3dRenderTargetView.Reset();
    m_swapChain.Reset();
    m_swapChain1.Reset();
    m_d3dContext.Reset();
    m_d3dContext1.Reset();
    m_d3dDevice1.Reset();

#ifdef _DEBUG
    {
        ComPtr<ID3D11Debug> d3dDebug;
        if (SUCCEEDED(m_d3dDevice.As(&d3dDebug)))
        {
            d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY);
        }
    }
#endif

    m_d3dDevice.Reset();

    CreateDeviceResources();
    CreateWindowSizeDependentResources();

    if (m_deviceNotify)
    {
        m_deviceNotify->OnDeviceRestored();
    }
}

// Present the contents of the swap chain to the screen.
void DX::DeviceResources::Present() 
{
    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
    HRESULT hr = m_swapChain->Present(1, 0);

    if (m_d3dContext1)
    {
        // Discard the contents of the render target.
        // This is a valid operation only when the existing contents will be entirely
        // overwritten. If dirty or scroll rects are used, this call should be removed.
        m_d3dContext1->DiscardView(m_d3dRenderTargetView.Get());

        // Discard the contents of the depth stencil.
        m_d3dContext1->DiscardView(m_d3dDepthStencilView.Get());
    }

    // If the device was removed either by a disconnection or a driver upgrade, we 
    // must recreate all device resources.
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
#ifdef _DEBUG
        char buff[64] = {};
        sprintf_s(buff, "Device Lost on Present: Reason code 0x%08X\n", (hr == DXGI_ERROR_DEVICE_REMOVED) ? m_d3dDevice->GetDeviceRemovedReason() : hr);
        OutputDebugStringA(buff);
#endif
        HandleDeviceLost();
    }
    else
    {
        Calibration::DX::ThrowIfFailed(hr);
    }
}

// This method acquires the first available hardware adapter.
// If no such adapter can be found, *ppAdapter will be set to nullptr.
void DX::DeviceResources::GetHardwareAdapter(IDXGIAdapter1** ppAdapter)
{
    *ppAdapter = nullptr;

    ComPtr<IDXGIFactory1> dxgiFactory;
    Calibration::DX::ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

    ComPtr<IDXGIAdapter1> adapter;
    for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != dxgiFactory->EnumAdapters1(adapterIndex, adapter.ReleaseAndGetAddressOf()); adapterIndex++)
    {
        DXGI_ADAPTER_DESC1 desc;
        adapter->GetDesc1(&desc);

        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
        {
            // Don't select the Basic Render Driver adapter.
            continue;
        }

#ifdef _DEBUG
        WCHAR buff[256] = {};
        swprintf_s(buff, L"Direct3D Adapter (%u): VID:%04X, PID:%04X - %ls\n", adapterIndex, desc.VendorId, desc.DeviceId, desc.Description);
        OutputDebugStringW(buff);
#endif

        break;
    }

    *ppAdapter = adapter.Detach();
}