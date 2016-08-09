//********************************************************* 
// 
// Copyright (c) Microsoft. All rights reserved. 
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY 
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR 
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT. 
// 
//********************************************************* 

#pragma once

#include "DeviceResources.h"

namespace DX
{
    // Controls all the DirectX device resources.
    class DeviceResourcesWindowed : public DeviceResources
    {
    public:
        DeviceResourcesWindowed();
        void SetWindow(Windows::UI::Core::CoreWindow^ window);
        void SetSwapChainPanel(Windows::UI::Xaml::Controls::SwapChainPanel^ panel);
        void SetLogicalSize(Windows::Foundation::Size logicalSize);
        void SetCurrentOrientation(Windows::Graphics::Display::DisplayOrientations currentOrientation);
        void SetCompositionScale(float compositionScaleX, float compositionScaleY);
        void SetDpi(float dpi);
        void ValidateDevice();
        void HandleDeviceLost();
        void Present();

        void SetD3DDevice(ID3D11Device4* pDevice, ID3D11DeviceContext3* pDeviceContext);

        void UpdateViewProjectionBuffer();
        bool AttachViewProjectionBuffer();

        // The size of the render target, in pixels.
        Windows::Foundation::Size   GetOutputSize() const                           { return m_outputSize; }

        // The size of the render target, in dips.
        Windows::Foundation::Size   GetLogicalSize() const                          { return m_logicalSize; }
        float                       GetDpi() const                                  { return m_effectiveDpi; }

        // D3D Accessors.
        IDXGISwapChain3*            GetSwapChain() const                            { return m_swapChain.Get(); }
        ID3D11RenderTargetView1*    GetBackBufferRenderTargetView() const           { return m_d3dRenderTargetView.Get(); }
        ID3D11DepthStencilView*     GetSceneDepthStencilView() const                { return m_d3dDepthStencilView.Get(); }
        ID3D11DepthStencilView*     GetSurfaceDepthStencilView() const              { return m_d3dSurfaceRenderingDepthStencilView.Get(); }
        ID3D11DepthStencilView*     GetSurfaceOcclusionDepthStencilView() const     { return m_d3dSurfaceOcclusionDepthStencilView.Get(); }
        ID3D11ShaderResourceView*   GetSurfaceOcclusionShaderResourceView() const   { return m_d3dSurfaceOcclusionShaderResourceView.Get(); }
        ID3D11DepthStencilView*     GetHologramDepthStencilView() const             { return m_d3dHoloRenderingDepthStencilView.Get(); }
        ID3D11DepthStencilView*     GetHologramOcclusionDepthStencilView() const    { return m_d3dHoloOcclusionDepthStencilView.Get(); }
        ID3D11ShaderResourceView*   GetHologramOcclusionShaderResourceView() const  { return m_d3dHoloOcclusionShaderResourceView.Get(); }
        ID3D11SamplerState*         GetDepthTextureSamplerState() const             { return m_depthTextureSamplerState.Get(); }
        D3D11_VIEWPORT              GetScreenViewport() const                       { return m_d3dViewport; }
        DirectX::XMFLOAT4X4         GetOrientationTransform3D() const               { return m_orientationTransform3D; }

        // D2D Accessors.
        ID2D1Device2*               GetD2DDevice() const                            { return m_d2dDevice.Get(); }
        ID2D1DeviceContext2*        GetD2DDeviceContext() const                     { return m_d2dContext.Get(); }
        ID2D1Bitmap1*               GetD2DTargetBitmap() const                      { return m_d2dTargetBitmap.Get(); }
        D2D1::Matrix3x2F            GetOrientationTransform2D() const               { return m_orientationTransform2D; }

        Windows::Foundation::Numerics::float3 GetCameraPosition()                   { return m_cameraPosition; }
        void SetCameraPosition(Windows::Foundation::Numerics::float3 pos)           { m_cameraPosition = pos; }

        Windows::Foundation::Numerics::quaternion GetCameraOrientation()            { return m_cameraOrientation; }
        void SetCameraOrientation(Windows::Foundation::Numerics::quaternion rot)    { m_cameraOrientation = rot; }

        bool IsRenderingStereoscopic() const                                        { return m_isStereo; }

    private:
        void CreateViewProjection();
        void CreateDeviceResources();
        void CreateWindowSizeDependentResources();
        void UpdateRenderTargetSize();
        DXGI_MODE_ROTATION ComputeDisplayRotation();

        // Direct3D objects.
        Microsoft::WRL::ComPtr<IDXGISwapChain3>             m_swapChain;

        // Direct3D rendering objects. Required for 3D.
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView1>     m_d3dRenderTargetView;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView>      m_d3dDepthStencilView;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView>      m_d3dSurfaceRenderingDepthStencilView;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView>      m_d3dSurfaceOcclusionDepthStencilView;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    m_d3dSurfaceOcclusionShaderResourceView; // because it's read back during hologram rendering
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView>      m_d3dHoloRenderingDepthStencilView;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView>      m_d3dHoloOcclusionDepthStencilView;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    m_d3dHoloOcclusionShaderResourceView; // because it's read back during hologram rendering
        Microsoft::WRL::ComPtr<ID3D11SamplerState>          m_depthTextureSamplerState;
        D3D11_VIEWPORT                                      m_d3dViewport;

        // Direct2D drawing components.
        Microsoft::WRL::ComPtr<ID2D1Device2>                m_d2dDevice;
        Microsoft::WRL::ComPtr<ID2D1DeviceContext2>         m_d2dContext;
        Microsoft::WRL::ComPtr<ID2D1Bitmap1>                m_d2dTargetBitmap;

        // Cached reference to the Window.
        Platform::Agile<Windows::UI::Core::CoreWindow>      m_window;

        // Cached reference to the XAML panel.
        Windows::UI::Xaml::Controls::SwapChainPanel^        m_swapChainPanel;

        // Cached device properties.
        Windows::Foundation::Size                           m_d3dRenderTargetSize;
        Windows::Foundation::Size                           m_outputSize;
        Windows::Foundation::Size                           m_logicalSize;
        Windows::Graphics::Display::DisplayOrientations     m_nativeOrientation;
        Windows::Graphics::Display::DisplayOrientations     m_currentOrientation;
        float                                               m_dpi;
        float                                                m_compositionScaleX;
        float                                                m_compositionScaleY;

        // Constant buffer used to send the view-projection matrices to the shader pipeline.
        // Device resource to store view and projection matrices.
        Microsoft::WRL::ComPtr<ID3D11Buffer>                m_viewProjectionConstantBuffer;
        ViewProjectionConstantBuffer                        m_viewProjectionConstantBufferData;

        // This is the DPI that will be reported back to the app. It takes into account whether the app supports high resolution screens or not.
        float                                               m_effectiveDpi;

        // Transforms used for display orientation.
        D2D1::Matrix3x2F                                    m_orientationTransform2D;
        DirectX::XMFLOAT4X4                                 m_orientationTransform3D;

        // Indicates whether the camera supports stereoscopic rendering.
        bool                                                m_isStereo = false;

        // Indicates whether this camera has a pending frame.
        bool                                                m_framePending = false;

        Windows::Foundation::Numerics::float3               m_cameraPosition;
        Windows::Foundation::Numerics::quaternion           m_cameraOrientation;
    };
}