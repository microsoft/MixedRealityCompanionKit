// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

namespace MixedRemoteViewCompositor
{
    namespace Plugin
    {

        class DirectXManagerImpl
            : public RuntimeClass
            < RuntimeClassFlags<RuntimeClassType::WinRtClassicComMix>
            , IDirectXManager
            , FtmBase >
        {
            InspectableClass(L"MixedRemoteViewCompositor.Plugin.DirectXManager", BaseTrust)

        public:
            DirectXManagerImpl();
            ~DirectXManagerImpl();

            // RuntimeClass
            STDMETHODIMP RuntimeClassInitialize(_In_ ID3D11Device* pD3D11Device);

            STDMETHODIMP_(void) Uninitialize();
            STDMETHODIMP_(void) Lost();
            STDMETHODIMP_(void) Reset();

            STDMETHODIMP GetFrameStatistics(_In_ DXGI_FRAME_STATISTICS& pStats);

            STDMETHODIMP WaitForVBlank();

        public:
            STDMETHODIMP_(ID3D11Device*) GetDevice() const { return _spDevice.Get(); }
            STDMETHODIMP_(ID3D11DeviceContext*) GetDeviceContext() const { return _spContext.Get(); }

            STDMETHODIMP_(Plugin::DeviceType) GetDeviceType() { return _deviceType; }
            STDMETHODIMP_(ID3D11Device2*) GetDevice2() const { return _spDevice2.Get(); }
            STDMETHODIMP_(ID3D11DeviceContext2*) GetDeviceContext2() const { return _spContext2.Get(); }

        private:
            Wrappers::CriticalSection _lock;

            // seperate dx device for features not provided by unity device
            Plugin::DeviceType _deviceType;
            D3D_FEATURE_LEVEL _featureLevel;
            ComPtr<ID3D11Device2> _spDevice;
            ComPtr<ID3D11DeviceContext2> _spContext;
            ComPtr<ID3D11Device2> _spDevice2;
            ComPtr<ID3D11DeviceContext2> _spContext2;
            ComPtr<IDXGIOutput> _spDxgiOutput;
        };
    }
}