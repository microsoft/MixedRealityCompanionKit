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

#include "Common\DeviceResources.h"
#include "Common\DeviceResourcesWindowed.h"
#include "AppMain.h"
#include <HolographicStreamerHelpers.h>

namespace RemotingHostSample
{
    // IFrameworkView class. Connects the app with the Windows shell and handles application lifecycle events.
    ref class AppView sealed
    {
    public:
        AppView();

        void Initialize(
            Windows::Graphics::Holographic::HolographicSpace^ holographicSpace, 
            Microsoft::Holographic::RemoteSpeech^ remoteSpeech);
        void Tick();

    internal:
        const std::shared_ptr<DX::DeviceResources>& GetDeviceResources()
        {
            return m_deviceResources;
        }

        bool const& GetIsInitialized()
        {
            return m_isInitialized;
        }

    private:
        std::unique_ptr<AppMain> m_main;

        std::shared_ptr<DX::DeviceResources>                m_deviceResources;

        // The holographic space the app will use for rendering.
        Windows::Graphics::Holographic::HolographicSpace^   m_holographicSpace = nullptr;

        // Debug view infrastructure.
        Platform::Agile<Windows::UI::Core::CoreWindow>      m_window;

        bool m_isInitialized = false;
    };
}

