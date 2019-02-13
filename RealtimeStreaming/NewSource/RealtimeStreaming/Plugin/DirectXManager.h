// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

namespace winrt::RealtimeStreaming::Plugin::implementation
{
    struct DirectXManager : DirectXManagerT<DirectXManager, Module>
    {
        public:
            DirectXManager(_In_ ID3D11Device* pD3D11Device);
            ~DirectXManager();

            // RuntimeClass
            void Lost();
            void Reset();

            STDMETHODIMP GetFrameStatistics(_In_ DXGI_FRAME_STATISTICS& pStats);

            STDMETHODIMP WaitForVBlank();

        public:
            STDMETHODIMP_(ID3D11Device*) GetDevice() const { return _spDevice.get(); }
            STDMETHODIMP_(ID3D11DeviceContext*) GetDeviceContext() const { return _spContext.get(); }

            STDMETHODIMP_(Plugin::DeviceType) GetDeviceType() { return _deviceType; }
            STDMETHODIMP_(ID3D11Device2*) GetDevice2() const { return _spDevice2.get(); }
            STDMETHODIMP_(ID3D11DeviceContext2*) GetDeviceContext2() const { return _spContext2.get(); }

        private:
            Wrappers::CriticalSection _lock;

            // seperate dx device for features not provided by unity device
            Plugin::DeviceType _deviceType;
            D3D_FEATURE_LEVEL _featureLevel;
            com_ptr<ID3D11Device2> _spDevice;
            com_ptr<ID3D11DeviceContext2> _spContext;
            com_ptr<ID3D11Device2> _spDevice2;
            com_ptr<ID3D11DeviceContext2> _spContext2;
            com_ptr<IDXGIOutput> _spDxgiOutput;
        };
    }
}