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

#pragma once

#include "Common\DeviceResourcesWindowed.h"
#include "AppMain.h"
#include "AppView.h"

namespace RemotingHostSample
{
    // Main entry point for our app. Connects the app with the Windows shell and handles application lifecycle events.
    ref class RemotingHostSampleUWP sealed : public Windows::ApplicationModel::Core::IFrameworkView
    {
    public:
        RemotingHostSampleUWP();
    
        // IFrameworkView Methods.
        virtual void Initialize(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView);
        virtual void SetWindow(Windows::UI::Core::CoreWindow^ window);
        virtual void Load(Platform::String^ entryPoint);
        virtual void Run();
        virtual void Uninitialize();
    
    protected:
        // Application lifecycle event handlers.
        void OnActivated(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView, Windows::ApplicationModel::Activation::IActivatedEventArgs^ args);
        void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ args);
        void OnResuming(Platform::Object^ sender, Platform::Object^ args);
    
        // Window event handlers.
        void OnWindowSizeChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ args);
        void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);
        void OnWindowClosed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::CoreWindowEventArgs^ args);
    
        // DisplayInformation event handlers.
        void OnDpiChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
        void OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
        void OnDisplayContentsInvalidated(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
    
    private:
        bool ConnectToRemoteDevice();
    
        std::shared_ptr<DX::DeviceResourcesWindowed>        m_deviceResources;
        RemotingHostSample::AppView^                        m_appView;
        Platform::String^                                   m_ipAddress;
        Microsoft::Holographic::HolographicStreamerHelpers^ m_streamerHelpers;

        int  m_width            = 0;
        int  m_height           = 0;
        bool m_showPreview      = true;
        bool m_windowClosed     = false;
        bool m_windowVisible    = true;
    };
}

ref class Direct3DApplicationSource sealed : Windows::ApplicationModel::Core::IFrameworkViewSource
{
public:
   virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView();
};
