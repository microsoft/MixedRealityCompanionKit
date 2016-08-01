//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

//
// RemotingPage.xaml.h
// Declaration of the RemotingPage class.
//

#pragma once

#include "RemotingPage.g.h"

#include "Common\DeviceResourcesWindowed.h"
#include "AppMain.h"
#include "AppView.h"
#include <HolographicStreamerHelpers.h>

namespace RemotingHostSample
{
    /// <summary>
    /// A page that hosts a DirectX SwapChainPanel.
    /// </summary>
    public ref class RemotingPage sealed
    {
    public:
        RemotingPage();
        virtual ~RemotingPage();

        void SaveInternalState(Windows::Foundation::Collections::IPropertySet^ state);
        void LoadInternalState(Windows::Foundation::Collections::IPropertySet^ state);

    private:

        // Window event handlers.
        void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);

        // DisplayInformation event handlers.
        void OnDpiChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
        void OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
        void OnDisplayContentsInvalidated(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);

        // Other event handlers.
        void OnCompositionScaleChanged(Windows::UI::Xaml::Controls::SwapChainPanel^ sender, Object^ args);
        void OnSwapChainPanelSizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);
        void Key_Down(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e);
        void Toggle_Preview(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void Start_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void Stop_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void ipAddress_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);

        // Private methods for remoting.
        bool ConnectToRemoteDevice();
        void DisconnectFromRemoteDevice();
        void StartRenderLoop();
        void StopRenderLoop();

        // Resources used to render the remoting frame preview in the XAML page background.
        std::shared_ptr<DX::DeviceResourcesWindowed>        m_deviceResources;
        RemotingHostSample::AppView^                        m_appView;
        Platform::String^                                   m_ipAddress;
        Microsoft::Holographic::HolographicStreamerHelpers^ m_streamerHelpers;

        int  m_width                = 0;
        int  m_height               = 0;
        bool m_connectedState       = false;
        bool m_showPreview          = true;
        bool m_priorPreviewState    = true;
        bool m_windowClosed         = false;
        bool m_windowVisible        = true;

        // The holographic space the app will use for rendering.
        Windows::Graphics::Holographic::HolographicSpace^   m_holographicSpace = nullptr;

        // XAML render loop.
        Windows::Foundation::IAsyncAction^  m_renderLoopWorker;
        Concurrency::critical_section       m_criticalSection;
    };
}

