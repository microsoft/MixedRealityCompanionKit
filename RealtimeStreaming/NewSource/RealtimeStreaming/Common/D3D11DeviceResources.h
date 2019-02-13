// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include <d3d11_1.h>
#include <DirectXMath.h>
#include "PlatformBase.h"
#include "UnityDeviceResource.h"

struct __declspec(uuid("cdaad344-9a90-4668-ba3b-8a5ddba72492")) ID3D11DeviceResource : ::IUnknown
{
    virtual winrt::com_ptr<ID3D11Device> __stdcall GetDevice() = 0;
};

struct D3D11DeviceResources
    : winrt::implements<D3D11DeviceResources, ID3D11DeviceResource, IUnityDeviceResource>
{
    D3D11DeviceResources();
    virtual ~D3D11DeviceResources();

    // IUnityDeviceResource
    STDOVERRIDEMETHODIMP_(void) ProcessDeviceEvent(
        _In_ UnityGfxDeviceEventType type,
        _In_ IUnityInterfaces* interfaces);
    STDOVERRIDEMETHODIMP_(bool) GetUsesReverseZ();

    // ID3D11DeviceResource
    STDOVERRIDEMETHODIMP_(winrt::com_ptr<ID3D11Device>) GetDevice() 
    {
        auto guard = winrt::slim_shared_lock_guard(m_mutex);
        return m_unityDevice;
    }

private:
    HRESULT InitializeResources(ID3D11Device* d3dDevice);
    void ReleaseResources();

private:
    winrt::slim_mutex m_mutex;

    winrt::com_ptr<ID3D11Device> m_unityDevice;

    winrt::event<winrt::Windows::Foundation::EventHandler<IInspectable>> m_initializedEvent;
    winrt::event<winrt::Windows::Foundation::EventHandler<IInspectable>> m_shutdownEvent;
    winrt::event<winrt::Windows::Foundation::EventHandler<IInspectable>> m_resetStartedEvent;
    winrt::event<winrt::Windows::Foundation::EventHandler<IInspectable>> m_resetCompletedEvent;
};
