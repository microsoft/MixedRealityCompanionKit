//********************************************************* 
// 
// Copyright (c) Microsoft. All rights reserved. 
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY 
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR 
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT. 
// 
//********************************************************* 

#include "pch.h"
#include "DeviceResourcesWindowed.h"
#include "DirectXHelper.h"

#include <windows.graphics.directx.direct3d11.interop.h>
#include <windows.ui.xaml.media.dxinterop.h>

using namespace D2D1;
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::Graphics::DirectX::Direct3D11;
using namespace Windows::Graphics::Display;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml::Controls;
using namespace Platform;

namespace DisplayMetrics
{
   // High resolution displays can require a lot of GPU and battery power to render.
   // High resolution phones, for example, may suffer from poor battery life if
   // games attempt to render at 60 frames per second at full fidelity.
   // The decision to render at full fidelity across all platforms and form factors
   // should be deliberate.
   static const bool SupportHighResolutions = false;

   // The default thresholds that define a "high resolution" display. If the thresholds
   // are exceeded and SupportHighResolutions is false, the dimensions will be scaled
   // by 50%.
   static const float DpiThreshold = 192.0f;      // 200% of standard desktop display.
   static const float WidthThreshold = 1920.0f;   // 1080p width.
   static const float HeightThreshold = 1080.0f;   // 1080p height.
};

// Constants used to calculate screen rotations
namespace ScreenRotation
{
   // 0-degree Z-rotation
   static const XMFLOAT4X4 Rotation0(
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
      );

   // 90-degree Z-rotation
   static const XMFLOAT4X4 Rotation90(
      0.0f, 1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
      );

   // 180-degree Z-rotation
   static const XMFLOAT4X4 Rotation180(
      -1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, -1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
      );

   // 270-degree Z-rotation
   static const XMFLOAT4X4 Rotation270(
      0.0f, -1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
      );
};

// Constructor for DeviceResources.
DX::DeviceResourcesWindowed::DeviceResourcesWindowed() :
    m_d3dViewport(),
    m_d3dRenderTargetSize(),
    m_outputSize(),
    m_logicalSize(),
    m_nativeOrientation(DisplayOrientations::None),
    m_currentOrientation(DisplayOrientations::None),
    m_dpi(-1.0f),
    m_effectiveDpi(-1.0f),
    m_cameraPosition(0.f, 0.f, 0.f),
    m_cameraOrientation(0.f, 0.f, 0.f, 1.f)
{
    DeviceResources();
   CreateDeviceResources();
}

// Configures the Direct3D device, and stores handles to it and the device context.
void DX::DeviceResourcesWindowed::CreateDeviceResources()
{
    DeviceResources::CreateDeviceResources();

    // Acquire the DXGI interface for the Direct3D device.
    ComPtr<IDXGIDevice3> dxgiDevice;
    DX::ThrowIfFailed(
        m_d3dDevice.As(&dxgiDevice)
        );
    
    ComPtr<ID2D1Device> d2dDevice;
    DX::ThrowIfFailed(
      m_d2dFactory->CreateDevice(dxgiDevice.Get(), &d2dDevice)
      );
    d2dDevice.As(&m_d2dDevice);

   DX::ThrowIfFailed(
      m_d2dDevice->CreateDeviceContext(
         D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
         &m_d2dContext
         )
      );

    // Create a constant buffer to store view and projection matrices for the camera.
    CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
    DX::ThrowIfFailed(
        m_d3dDevice->CreateBuffer(
            &constantBufferDesc,
            nullptr,
            &m_viewProjectionConstantBuffer
            )
        );
}

// These resources need to be recreated every time the window size is changed.
void DX::DeviceResourcesWindowed::CreateWindowSizeDependentResources()
{
   // Clear the previous window size specific context.
   ID3D11RenderTargetView* nullViews[] = {nullptr};
   m_d3dContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
   m_d3dRenderTargetView = nullptr;
   m_d2dContext->SetTarget(nullptr);
   m_d2dTargetBitmap = nullptr;
   
   m_d3dContext->Flush1(D3D11_CONTEXT_TYPE_ALL, nullptr);

   UpdateRenderTargetSize();

   // The width and height of the swap chain must be based on the window's
   // natively-oriented width and height. If the window is not in the native
   // orientation, the dimensions must be reversed.
   DXGI_MODE_ROTATION displayRotation = ComputeDisplayRotation();

   bool swapDimensions = displayRotation == DXGI_MODE_ROTATION_ROTATE90 || displayRotation == DXGI_MODE_ROTATION_ROTATE270;
   //m_d3dRenderTargetSize.Width = swapDimensions ? m_outputSize.Height : m_outputSize.Width;
   //m_d3dRenderTargetSize.Height = swapDimensions ? m_outputSize.Width : m_outputSize.Height;
   m_d3dRenderTargetSize.Width  = 1280;
   m_d3dRenderTargetSize.Height = 720;

   if (m_swapChain != nullptr)
   {
      // If the swap chain already exists, resize it.
      HRESULT hr = m_swapChain->ResizeBuffers(
         2, // Double-buffered swap chain.
         lround(m_d3dRenderTargetSize.Width),
         lround(m_d3dRenderTargetSize.Height),
         DXGI_FORMAT_B8G8R8A8_UNORM,
         0
         );

      if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
      {
         // If the device was removed for any reason, a new device and swap chain will need to be created.
         HandleDeviceLost();

         // Everything is set up now. Do not continue execution of this method. HandleDeviceLost will reenter this method 
         // and correctly set up the new device.
         return;
      }
      else
      {
         DX::ThrowIfFailed(hr);
      }
   }
   else
   {
        // Otherwise, create a new one using the same adapter as the existing Direct3D device.
        DXGI_SCALING scaling = DXGI_SCALING_STRETCH;
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {0};

        swapChainDesc.Width = lround(m_d3dRenderTargetSize.Width);      // Match the size of the window.
        swapChainDesc.Height = lround(m_d3dRenderTargetSize.Height);
        swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;            // This is the most common swap chain format.
        swapChainDesc.Stereo = false;
        swapChainDesc.SampleDesc.Count = 1;                        // Don't use multi-sampling.
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = 2;                           // Use double-buffering to minimize latency.
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;   // All Windows Store apps must use this SwapEffect.
        swapChainDesc.Flags = 0;
        swapChainDesc.Scaling = scaling;
        swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

      // This sequence obtains the DXGI factory that was used to create the Direct3D device above.
      ComPtr<IDXGIDevice3> dxgiDevice;
      DX::ThrowIfFailed(
         m_d3dDevice.As(&dxgiDevice)
         );

      ComPtr<IDXGIAdapter> dxgiAdapter;
      DX::ThrowIfFailed(
         dxgiDevice->GetAdapter(&dxgiAdapter)
         );

      ComPtr<IDXGIFactory4> dxgiFactory;
      DX::ThrowIfFailed(
         dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory))
         );

      ComPtr<IDXGISwapChain1> swapChain;
      if (m_swapChainPanel == nullptr)
      {
          DX::ThrowIfFailed(
              dxgiFactory->CreateSwapChainForCoreWindow(
                  m_d3dDevice.Get(),
                  reinterpret_cast<IUnknown*>(m_window.Get()),
                  &swapChainDesc,
                  nullptr,
                  &swapChain
              )
          );
      }
      else
      {
          DX::ThrowIfFailed(
              dxgiFactory->CreateSwapChainForComposition(
                  m_d3dDevice.Get(),
                  &swapChainDesc,
                  nullptr,
                  &swapChain
              )
          );

          // Associate swap chain with SwapChainPanel.
          // UI changes will need to be dispatched back to the UI thread.
          m_swapChainPanel->Dispatcher->RunAsync(CoreDispatcherPriority::High, ref new DispatchedHandler([=]()
          {
              // Get backing native interface for SwapChainPanel
              ComPtr<ISwapChainPanelNative> panelNative;
              DX::ThrowIfFailed(
                  reinterpret_cast<IUnknown*>(m_swapChainPanel)->QueryInterface(IID_PPV_ARGS(&panelNative))
              );

              DX::ThrowIfFailed(
                  panelNative->SetSwapChain(m_swapChain.Get())
              );
          }, CallbackContext::Any));
      }
      DX::ThrowIfFailed(
         swapChain.As(&m_swapChain)
         );

      // Ensure that DXGI does not queue more than one frame at a time. This both reduces latency and
      // ensures that the application will only render after each VSync, minimizing power consumption.
      DX::ThrowIfFailed(
         dxgiDevice->SetMaximumFrameLatency(1)
         );
   }

   // Set the proper orientation for the swap chain, and generate 2D and
   // 3D matrix transformations for rendering to the rotated swap chain.
   // Note the rotation angle for the 2D and 3D transforms are different.
   // This is due to the difference in coordinate spaces.  Additionally,
   // the 3D matrix is specified explicitly to avoid rounding errors.

   switch (displayRotation)
   {
   case DXGI_MODE_ROTATION_IDENTITY:
      m_orientationTransform2D = Matrix3x2F::Identity();
      m_orientationTransform3D = ScreenRotation::Rotation0;
      break;

   case DXGI_MODE_ROTATION_ROTATE90:
      m_orientationTransform2D = 
         Matrix3x2F::Rotation(90.0f) *
         Matrix3x2F::Translation(m_logicalSize.Height, 0.0f);
      m_orientationTransform3D = ScreenRotation::Rotation270;
      break;

   case DXGI_MODE_ROTATION_ROTATE180:
      m_orientationTransform2D = 
         Matrix3x2F::Rotation(180.0f) *
         Matrix3x2F::Translation(m_logicalSize.Width, m_logicalSize.Height);
      m_orientationTransform3D = ScreenRotation::Rotation180;
      break;

   case DXGI_MODE_ROTATION_ROTATE270:
      m_orientationTransform2D = 
         Matrix3x2F::Rotation(270.0f) *
         Matrix3x2F::Translation(0.0f, m_logicalSize.Width);
      m_orientationTransform3D = ScreenRotation::Rotation90;
      break;

   default:
      throw ref new FailureException();
   }

   DX::ThrowIfFailed(
      m_swapChain->SetRotation(displayRotation)
      );

   // Create a render target view of the swap chain back buffer.
   ComPtr<ID3D11Texture2D1> backBuffer;
   DX::ThrowIfFailed(
      m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer))
      );

   DX::ThrowIfFailed(
      m_d3dDevice->CreateRenderTargetView1(
         backBuffer.Get(),
         nullptr,
         &m_d3dRenderTargetView
         )
      );

    // TODO: 16-bit depth buffers
   // Create a depth stencil view for use with 3D rendering if needed.
   CD3D11_TEXTURE2D_DESC1 depthStencilDesc(
        DXGI_FORMAT_R24G8_TYPELESS,
      lround(m_d3dRenderTargetSize.Width),
      lround(m_d3dRenderTargetSize.Height),
      1, // This depth stencil view has only one texture.
      1, // Use a single mipmap level.
      D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE
      );
    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D, DXGI_FORMAT_D24_UNORM_S8_UINT);
    CD3D11_SHADER_RESOURCE_VIEW_DESC depthStencilResourceViewDesc(D3D11_SRV_DIMENSION_TEXTURE2D, DXGI_FORMAT_R24_UNORM_X8_TYPELESS);

    // TODO: destruct these new resources

    // Create views for scene rendering.
    {
        ComPtr<ID3D11Texture2D1> depthStencil;
        DX::ThrowIfFailed(
            m_d3dDevice->CreateTexture2D1(
                &depthStencilDesc,
                nullptr,
                &depthStencil
                )
            );
        DX::ThrowIfFailed(
            m_d3dDevice->CreateDepthStencilView(
                depthStencil.Get(),
                &depthStencilViewDesc,
                &m_d3dDepthStencilView
                )
            );
    }

    // Create views for surface rendering
    {
        ComPtr<ID3D11Texture2D1> depthStencil;
        DX::ThrowIfFailed(
            m_d3dDevice->CreateTexture2D1(
                &depthStencilDesc,
                nullptr,
                &depthStencil
                )
            );
        DX::ThrowIfFailed(
            m_d3dDevice->CreateDepthStencilView(
                depthStencil.Get(),
                &depthStencilViewDesc,
                &m_d3dSurfaceRenderingDepthStencilView
                )
            );
    }
    {
        ComPtr<ID3D11Texture2D1> depthStencil;
        DX::ThrowIfFailed(
            m_d3dDevice->CreateTexture2D1(
                &depthStencilDesc,
                nullptr,
                &depthStencil
                )
            );
        DX::ThrowIfFailed(
            m_d3dDevice->CreateDepthStencilView(
                depthStencil.Get(),
                &depthStencilViewDesc,
                &m_d3dSurfaceOcclusionDepthStencilView
                )
            );
        DX::ThrowIfFailed(
            m_d3dDevice->CreateShaderResourceView(
                depthStencil.Get(),
                &depthStencilResourceViewDesc,
                &m_d3dSurfaceOcclusionShaderResourceView
                )
            );
    }

    // Create depth stencils for holographic content
    {
        ComPtr<ID3D11Texture2D1> depthStencilHologram;
        DX::ThrowIfFailed(
            m_d3dDevice->CreateTexture2D1(
                &depthStencilDesc,
                nullptr,
                &depthStencilHologram
                )
            );

        DX::ThrowIfFailed(
            m_d3dDevice->CreateDepthStencilView(
                depthStencilHologram.Get(),
                &depthStencilViewDesc,
                &m_d3dHoloRenderingDepthStencilView
                )
            );
    }
    {
        ComPtr<ID3D11Texture2D1> depthStencil;
        DX::ThrowIfFailed(
            m_d3dDevice->CreateTexture2D1(
                &depthStencilDesc,
                nullptr,
                &depthStencil
                )
            );
        DX::ThrowIfFailed(
            m_d3dDevice->CreateDepthStencilView(
                depthStencil.Get(),
                &depthStencilViewDesc,
                &m_d3dHoloOcclusionDepthStencilView
                )
            );
        DX::ThrowIfFailed(
            m_d3dDevice->CreateShaderResourceView(
                depthStencil.Get(),
                &depthStencilResourceViewDesc,
                &m_d3dHoloOcclusionShaderResourceView
                )
            );
    }

    {
        D3D11_SAMPLER_DESC samplerStateDesc;
        ZeroMemory(&samplerStateDesc, sizeof(D3D11_SAMPLER_DESC));

        samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        samplerStateDesc.MaxAnisotropy = 1;
        samplerStateDesc.MinLOD = 0;
        samplerStateDesc.MaxLOD = FLT_MAX;
        samplerStateDesc.BorderColor[0] = 0.f;
        samplerStateDesc.BorderColor[1] = 0.f;
        samplerStateDesc.BorderColor[2] = 0.f;
        samplerStateDesc.BorderColor[3] = 0.f;
        samplerStateDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        samplerStateDesc.MipLODBias = 0.f;

        DX::ThrowIfFailed(
            m_d3dDevice->CreateSamplerState(
                &samplerStateDesc,
                &m_depthTextureSamplerState
                )
            );
    }
   
   // Set the 3D rendering viewport to target the entire window.
    m_d3dViewport = CD3D11_VIEWPORT(
      0.0f,
      0.0f,
      m_d3dRenderTargetSize.Width,
      m_d3dRenderTargetSize.Height
      );

   // Create a Direct2D target bitmap associated with the
   // swap chain back buffer and set it as the current target.
   D2D1_BITMAP_PROPERTIES1 bitmapProperties = 
      D2D1::BitmapProperties1(
         D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
         D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
         m_dpi,
         m_dpi
         );

   ComPtr<IDXGISurface2> dxgiBackBuffer;
   DX::ThrowIfFailed(
      m_swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer))
      );

   DX::ThrowIfFailed(
      m_d2dContext->CreateBitmapFromDxgiSurface(
         dxgiBackBuffer.Get(),
         &bitmapProperties,
         &m_d2dTargetBitmap
         )
      );

   m_d2dContext->SetTarget(m_d2dTargetBitmap.Get());
   m_d2dContext->SetDpi(m_effectiveDpi, m_effectiveDpi);

   // Grayscale text anti-aliasing is recommended for all Windows Store apps.
   m_d2dContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
}

// Determine the dimensions of the render target and whether it will be scaled down.
void DX::DeviceResourcesWindowed::UpdateRenderTargetSize()
{
   m_effectiveDpi = m_dpi;

   // To improve battery life on high resolution devices, render to a smaller render target
   // and allow the GPU to scale the output when it is presented.
   if (!DisplayMetrics::SupportHighResolutions && m_dpi > DisplayMetrics::DpiThreshold)
   {
      float width = DX::ConvertDipsToPixels(m_logicalSize.Width, m_dpi);
      float height = DX::ConvertDipsToPixels(m_logicalSize.Height, m_dpi);

      if (width > DisplayMetrics::WidthThreshold && height > DisplayMetrics::HeightThreshold)
      {
         // To scale the app we change the effective DPI. Logical size does not change.
         m_effectiveDpi /= 2.0f;
      }
   }

   // Calculate the necessary render target size in pixels.
   m_outputSize.Width = DX::ConvertDipsToPixels(m_logicalSize.Width, m_effectiveDpi);
   m_outputSize.Height = DX::ConvertDipsToPixels(m_logicalSize.Height, m_effectiveDpi);

   m_outputSize = m_logicalSize;

   // Prevent zero size DirectX content from being created.
   m_outputSize.Width = max(m_outputSize.Width, 1);
   m_outputSize.Height = max(m_outputSize.Height, 1);
}

// This method is called when the CoreWindow is created (or re-created).
void DX::DeviceResourcesWindowed::SetWindow(CoreWindow^ window)
{
   DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

   m_window = window;
   m_logicalSize = Windows::Foundation::Size(window->Bounds.Width, window->Bounds.Height);
   m_nativeOrientation = currentDisplayInformation->NativeOrientation;
   m_currentOrientation = currentDisplayInformation->CurrentOrientation;
   m_dpi = currentDisplayInformation->LogicalDpi;
   m_d2dContext->SetDpi(m_dpi, m_dpi);

   CreateWindowSizeDependentResources();
}

// This method is called when the XAML control is created (or re-created).
void DX::DeviceResourcesWindowed::SetSwapChainPanel(SwapChainPanel^ panel)
{
    DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

    m_swapChainPanel        = panel;
    m_logicalSize           = Windows::Foundation::Size(static_cast<float>(panel->ActualWidth), static_cast<float>(panel->ActualHeight));
    m_nativeOrientation     = currentDisplayInformation->NativeOrientation;
    m_currentOrientation    = currentDisplayInformation->CurrentOrientation;
    m_compositionScaleX     = panel->CompositionScaleX;
    m_compositionScaleY     = panel->CompositionScaleY;
    m_dpi                   = currentDisplayInformation->LogicalDpi;

    m_d2dContext->SetDpi(m_dpi, m_dpi);

    CreateWindowSizeDependentResources();
}

// This method is called in the event handler for the SizeChanged event.
void DX::DeviceResourcesWindowed::SetLogicalSize(Windows::Foundation::Size logicalSize)
{
   if (m_logicalSize != logicalSize)
   {
      m_logicalSize = logicalSize;
      CreateWindowSizeDependentResources();
   }
}

// This method is called in the event handler for the DpiChanged event.
void DX::DeviceResourcesWindowed::SetDpi(float dpi)
{
   if (dpi != m_dpi)
   {
      m_dpi = dpi;

      // When the display DPI changes, the logical size of the window (measured in Dips) also changes and needs to be updated.
      m_logicalSize = Windows::Foundation::Size(m_window->Bounds.Width, m_window->Bounds.Height);

      m_d2dContext->SetDpi(m_dpi, m_dpi);
      CreateWindowSizeDependentResources();
   }
}

// This method is called in the event handler for the OrientationChanged event.
void DX::DeviceResourcesWindowed::SetCurrentOrientation(DisplayOrientations currentOrientation)
{
   if (m_currentOrientation != currentOrientation)
   {
      m_currentOrientation = currentOrientation;
      CreateWindowSizeDependentResources();
   }
}

// This method is called in the event handler for the CompositionScaleChanged event.
void DX::DeviceResourcesWindowed::SetCompositionScale(float compositionScaleX, float compositionScaleY)
{
    if (m_compositionScaleX != compositionScaleX ||
        m_compositionScaleY != compositionScaleY)
    {
        m_compositionScaleX = compositionScaleX;
        m_compositionScaleY = compositionScaleY;
        CreateWindowSizeDependentResources();
    }
}

// This method is called in the event handler for the DisplayContentsInvalidated event.
void DX::DeviceResourcesWindowed::ValidateDevice()
{
   // The D3D Device is no longer valid if the default adapter changed since the device
   // was created or if the device has been removed.

   // First, get the information for the default adapter from when the device was created.

   ComPtr<IDXGIDevice3> dxgiDevice;
   DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

   ComPtr<IDXGIAdapter> deviceAdapter;
   DX::ThrowIfFailed(dxgiDevice->GetAdapter(&deviceAdapter));

   ComPtr<IDXGIFactory4> deviceFactory;
   DX::ThrowIfFailed(deviceAdapter->GetParent(IID_PPV_ARGS(&deviceFactory)));

   ComPtr<IDXGIAdapter1> previousDefaultAdapter;
   DX::ThrowIfFailed(deviceFactory->EnumAdapters1(0, &previousDefaultAdapter));

   DXGI_ADAPTER_DESC1 previousDesc;
   DX::ThrowIfFailed(previousDefaultAdapter->GetDesc1(&previousDesc));

   // Next, get the information for the current default adapter.

   ComPtr<IDXGIFactory4> currentFactory;
   DX::ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&currentFactory)));

   ComPtr<IDXGIAdapter1> currentDefaultAdapter;
   DX::ThrowIfFailed(currentFactory->EnumAdapters1(0, &currentDefaultAdapter));

   DXGI_ADAPTER_DESC1 currentDesc;
   DX::ThrowIfFailed(currentDefaultAdapter->GetDesc1(&currentDesc));

   // If the adapter LUIDs don't match, or if the device reports that it has been removed,
   // a new D3D device must be created.

   if (previousDesc.AdapterLuid.LowPart != currentDesc.AdapterLuid.LowPart ||
      previousDesc.AdapterLuid.HighPart != currentDesc.AdapterLuid.HighPart ||
      FAILED(m_d3dDevice->GetDeviceRemovedReason()))
   {
      // Release references to resources related to the old device.
      dxgiDevice = nullptr;
      deviceAdapter = nullptr;
      deviceFactory = nullptr;
      previousDefaultAdapter = nullptr;

      // Create a new device and swap chain.
      HandleDeviceLost();
   }
}

// Recreate all device resources and set them back to the current state.
void DX::DeviceResourcesWindowed::HandleDeviceLost()
{
   m_swapChain = nullptr;

   if (m_deviceNotify != nullptr)
   {
      m_deviceNotify->OnDeviceLost();
   }

   CreateDeviceResources();
   m_d2dContext->SetDpi(m_dpi, m_dpi);
   CreateWindowSizeDependentResources();

   if (m_deviceNotify != nullptr)
   {
      m_deviceNotify->OnDeviceRestored();
   }
}

// Present the contents of the swap chain to the screen.
void DX::DeviceResourcesWindowed::Present()
{
   // The first argument instructs DXGI to block until VSync, putting the application
   // to sleep until the next VSync. This ensures we don't waste any cycles rendering
   // frames that will never be displayed to the screen.
   DXGI_PRESENT_PARAMETERS parameters = { 0 };
   HRESULT hr = m_swapChain->Present1(1, 0, &parameters);

   // Discard the contents of the render target.
   // This is a valid operation only when the existing contents will be entirely
   // overwritten. If dirty or scroll rects are used, this call should be removed.
   m_d3dContext->DiscardView1(m_d3dRenderTargetView.Get(), nullptr, 0);

   // Discard the contents of the depth stencil.
   m_d3dContext->DiscardView1(m_d3dSurfaceOcclusionDepthStencilView.Get(), nullptr, 0);
    m_d3dContext->DiscardView1(m_d3dSurfaceRenderingDepthStencilView.Get(), nullptr, 0);
   m_d3dContext->DiscardView1(m_d3dHoloOcclusionDepthStencilView.Get(), nullptr, 0);
    m_d3dContext->DiscardView1(m_d3dHoloRenderingDepthStencilView.Get(), nullptr, 0);

   // If the device was removed either by a disconnection or a driver upgrade, we 
   // must recreate all device resources.
   if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
   {
      HandleDeviceLost();
   }
   else
   {
      DX::ThrowIfFailed(hr);
   }
}

void DX::DeviceResourcesWindowed::SetD3DDevice(ID3D11Device4 * pDevice, ID3D11DeviceContext3* pDeviceContext)
{
    if (m_deviceNotify != nullptr)
    {
        m_deviceNotify->OnDeviceLost();
    }

    m_swapChain.Reset();
    m_d2dDevice.Reset();

    m_d3dDevice = pDevice;
    m_d3dContext = pDeviceContext;

    // Acquire the DXGI interface for the Direct3D device.
    ComPtr<IDXGIDevice3> dxgiDevice;
    DX::ThrowIfFailed(
        m_d3dDevice.As(&dxgiDevice)
    );

    // Wrap the native device using a WinRT interop object.
    m_d3dInteropDevice = CreateDirect3DDevice(dxgiDevice.Get());

    // Cache the DXGI adapter.
    // This is for the case of no preferred DXGI adapter, or fallback to WARP.
    ComPtr<IDXGIAdapter> dxgiAdapter;
    DX::ThrowIfFailed(
        dxgiDevice->GetAdapter(&dxgiAdapter)
    );
    DX::ThrowIfFailed(
        dxgiAdapter.As(&m_dxgiAdapter)
    );

    // Check for device support for the optional feature that allows setting the render target array index from the vertex shader stage.
    D3D11_FEATURE_DATA_D3D11_OPTIONS3 options;
    m_d3dDevice->CheckFeatureSupport(D3D11_FEATURE_D3D11_OPTIONS3, &options, sizeof(options));
    if (options.VPAndRTArrayIndexFromAnyShaderFeedingRasterizer)
    {
        m_supportsVprt = true;
    }


    ComPtr<ID2D1Device> d2dDevice;
    DX::ThrowIfFailed(
        m_d2dFactory->CreateDevice(dxgiDevice.Get(), &d2dDevice)
    );
    d2dDevice.As(&m_d2dDevice);

    DX::ThrowIfFailed(
        m_d2dDevice->CreateDeviceContext(
            D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
            &m_d2dContext
        )
    );

    // Create a constant buffer to store view and projection matrices for the camera.
    CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
    DX::ThrowIfFailed(
        m_d3dDevice->CreateBuffer(
            &constantBufferDesc,
            nullptr,
            &m_viewProjectionConstantBuffer
        )
    );

    m_d2dContext->SetDpi(m_dpi, m_dpi);
    CreateWindowSizeDependentResources();



    if (m_deviceNotify != nullptr)
    {
        m_deviceNotify->OnDeviceRestored();
    }
}

// This method determines the rotation between the display device's native orientation and the
// current display orientation.
DXGI_MODE_ROTATION DX::DeviceResourcesWindowed::ComputeDisplayRotation()
{
   DXGI_MODE_ROTATION rotation = DXGI_MODE_ROTATION_UNSPECIFIED;

   // Note: NativeOrientation can only be Landscape or Portrait even though
   // the DisplayOrientations enum has other values.
   switch (m_nativeOrientation)
   {
   case DisplayOrientations::Landscape:
      switch (m_currentOrientation)
      {
      case DisplayOrientations::Landscape:
         rotation = DXGI_MODE_ROTATION_IDENTITY;
         break;

      case DisplayOrientations::Portrait:
         rotation = DXGI_MODE_ROTATION_ROTATE270;
         break;

      case DisplayOrientations::LandscapeFlipped:
         rotation = DXGI_MODE_ROTATION_ROTATE180;
         break;

      case DisplayOrientations::PortraitFlipped:
         rotation = DXGI_MODE_ROTATION_ROTATE90;
         break;
      }
      break;

   case DisplayOrientations::Portrait:
      switch (m_currentOrientation)
      {
      case DisplayOrientations::Landscape:
         rotation = DXGI_MODE_ROTATION_ROTATE90;
         break;

      case DisplayOrientations::Portrait:
         rotation = DXGI_MODE_ROTATION_IDENTITY;
         break;

      case DisplayOrientations::LandscapeFlipped:
         rotation = DXGI_MODE_ROTATION_ROTATE270;
         break;

      case DisplayOrientations::PortraitFlipped:
         rotation = DXGI_MODE_ROTATION_ROTATE180;
         break;
      }
      break;
   }
   return rotation;
}

// Set up a view/projection matrix.
void DX::DeviceResourcesWindowed::CreateViewProjection()
{
    float aspectRatio = m_outputSize.Width / m_outputSize.Height;

    // Use the same vertical FOV as the HoloLens
    float fovAngleY = 17 * XM_PI / 180.0f;
    
    // Note that the OrientationTransform3D matrix is post-multiplied here
    // in order to correctly orient the scene to match the display orientation.
    // This post-multiplication step is required for any draw calls that are
    // made to the swap chain render target. For draw calls to other targets,
    // this transform should not be applied.

    // This sample makes use of a right-handed coordinate system using row-major matrices.
    XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovRH(
        fovAngleY,
        aspectRatio,
        0.01f,
        100.0f
        );
    XMMATRIX orientationMatrix = XMLoadFloat4x4(&m_orientationTransform3D);
    XMMATRIX projection = perspectiveMatrix * orientationMatrix;

    // TODO: Implement a way to control the camera

    // Eye starts at same position and direction as HoloLens coordinate system
    const XMVECTOR    eye     = XMLoadFloat3(&m_cameraPosition);
    static const XMVECTORF32 zNormal = { 0.0f, 0.0f, -1.0f, 0.0f };
    const XMVECTOR    at      = eye + XMVector3Rotate(zNormal, XMLoadQuaternion(&m_cameraOrientation));
    static const XMVECTORF32 up      = { 0.0f, 1.0f, 0.0f, 0.0f };
    XMMATRIX view = XMMatrixLookAtRH(eye, at, up);

    DirectX::XMStoreFloat4x4(&m_viewProjectionConstantBufferData.viewProjection[0], XMMatrixTranspose(view * projection));

    // just in case we have a stereo desktop view
    DirectX::XMStoreFloat4x4(&m_viewProjectionConstantBufferData.viewProjection[1], XMMatrixTranspose(view * projection));
}

// Updates the view/projection constant buffer for a holographic camera.
void DX::DeviceResourcesWindowed::UpdateViewProjectionBuffer()
{
    CreateViewProjection();

    // Loading is asynchronous. Resources must be created before they can be updated.
    if (m_d3dContext == nullptr || m_viewProjectionConstantBuffer == nullptr)
    {
        m_framePending = false;
    }
    else
    {
        // Update the view and projection matrices.
        m_d3dContext->UpdateSubresource(
            m_viewProjectionConstantBuffer.Get(),
            0,
            nullptr,
            &m_viewProjectionConstantBufferData,
            0,
            0
            );

        m_framePending = true;
    }
}

// Gets the view-projection constant buffer for the HolographicCamera and attaches it
// to the shader pipeline.
bool DX::DeviceResourcesWindowed::AttachViewProjectionBuffer()
{
    // Loading is asynchronous. Resources must be created before they can be updated.
    // Cameras can also be added asynchronously, in which case they must be initialized
    // before they can be used.
    if (m_d3dContext == nullptr || m_viewProjectionConstantBuffer == nullptr || m_framePending == false)
    {
        return false;
    }

    // Set the viewport for this camera.
    m_d3dContext->RSSetViewports(1, &m_d3dViewport);

    // Send the constant buffer to the vertex shader.
    m_d3dContext->VSSetConstantBuffers(
        1,
        1,
        m_viewProjectionConstantBuffer.GetAddressOf()
        );

    // The template includes a pass-through geometry shader that is used by
    // default on systems that don't support the D3D11_FEATURE_D3D11_OPTIONS3::
    // VPAndRTArrayIndexFromAnyShaderFeedingRasterizer extension. The shader 
    // will be enabled at run-time on systems that require it.
    // If your app will also use the geometry shader for other tasks and those
    // tasks require the view/projection matrix, uncomment the following line 
    // of code to send the constant buffer to the geometry shader as well.
    /*context->GSSetConstantBuffers(
    1,
    1,
    m_viewProjectionConstantBuffer.GetAddressOf()
    );*/

    // Send the constant buffer to the pixel shader.
    m_d3dContext->PSSetConstantBuffers(
        1,
        1,
        m_viewProjectionConstantBuffer.GetAddressOf()
        );

    m_framePending = false;

    return true;
}
