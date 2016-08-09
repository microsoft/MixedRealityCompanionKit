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

#include "..\RemotingHostSampleShared\pch.h"
#include "RemotingHostSampleUWP.h"

#include "Common\DbgLog.h"


#include <Microsoft.Perception.Simulation.h>
#include <ppltasks.h>

#define STREAMER_WIDTH          1280
#define STREAMER_HEIGHT         720

#define INITIAL_WINDOW_WIDTH    STREAMER_WIDTH
#define INITIAL_WINDOW_HEIGHT   STREAMER_HEIGHT

using namespace RemotingHostSample;

using namespace concurrency;
using namespace Microsoft::Holographic;
using namespace Microsoft::WRL;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;

// The main function is only used to initialize our IFrameworkView class.
[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
   auto direct3DApplicationSource = ref new Direct3DApplicationSource();
   CoreApplication::Run(direct3DApplicationSource);
   return 0;
}

IFrameworkView^ Direct3DApplicationSource::CreateView()
{
   return ref new RemotingHostSampleUWP();
}

RemotingHostSampleUWP::RemotingHostSampleUWP()
{
}

// The first method called when the IFrameworkView is being created.
void RemotingHostSampleUWP::Initialize(CoreApplicationView^ applicationView)
{
   // Register event handlers for app lifecycle. This example includes Activated, so that we
   // can make the CoreWindow active and start rendering on the window.
   applicationView->Activated +=
      ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &RemotingHostSampleUWP::OnActivated);

   CoreApplication::Suspending +=
      ref new EventHandler<SuspendingEventArgs^>(this, &RemotingHostSampleUWP::OnSuspending);

   CoreApplication::Resuming +=
      ref new EventHandler<Platform::Object^>(this, &RemotingHostSampleUWP::OnResuming);


   m_appView = ref new RemotingHostSample::AppView();

   // At this point we have access to the device. 
   // We can create the device-dependent resources.
   m_deviceResources = std::make_shared<DX::DeviceResourcesWindowed>();

}

// Called when the CoreWindow object is created (or re-created).
void RemotingHostSampleUWP::SetWindow(CoreWindow^ window)
{
   window->SizeChanged += 
      ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &RemotingHostSampleUWP::OnWindowSizeChanged);

   window->VisibilityChanged +=
      ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &RemotingHostSampleUWP::OnVisibilityChanged);

   window->Closed += 
      ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &RemotingHostSampleUWP::OnWindowClosed);

   DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

   currentDisplayInformation->DpiChanged +=
      ref new TypedEventHandler<DisplayInformation^, Object^>(this, &RemotingHostSampleUWP::OnDpiChanged);

   currentDisplayInformation->OrientationChanged +=
      ref new TypedEventHandler<DisplayInformation^, Object^>(this, &RemotingHostSampleUWP::OnOrientationChanged);

   DisplayInformation::DisplayContentsInvalidated +=
      ref new TypedEventHandler<DisplayInformation^, Object^>(this, &RemotingHostSampleUWP::OnDisplayContentsInvalidated);

   m_deviceResources->SetWindow(window);
   m_deviceResources->SetLogicalSize(Size(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT));

   while (!ConnectToRemoteDevice());
}

// Initializes scene resources, or loads a previously saved app state.
void RemotingHostSampleUWP::Load(Platform::String^ entryPoint)
{

}

// This method is called after the window becomes active.
void RemotingHostSampleUWP::Run()
{
    while (!m_windowClosed)
    {
        if (m_windowVisible)
        {
            CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

            m_appView->Tick();
        }
        else
        {
            CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
        }
    }
}

// Required for IFrameworkView.
// Terminate events do not cause Uninitialize to be called. It will be called if your IFrameworkView
// class is torn down while the app is in the foreground.
void RemotingHostSampleUWP::Uninitialize()
{
}

// Application lifecycle event handlers.

void RemotingHostSampleUWP::OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
{
    // Run() won't start until the CoreWindow is activated.
    CoreWindow::GetForCurrentThread()->Activate();
}

void RemotingHostSampleUWP::OnSuspending(Platform::Object^ sender, SuspendingEventArgs^ args)
{
    // Save app state asynchronously after requesting a deferral. Holding a deferral
    // indicates that the application is busy performing suspending operations. Be
    // aware that a deferral may not be held indefinitely. After about five seconds,
    // the app will be forced to exit.
    SuspendingDeferral^ deferral = args->SuspendingOperation->GetDeferral();

    create_task([this, deferral]()
    {
        m_deviceResources->Trim();
        deferral->Complete();
    });
}

void RemotingHostSampleUWP::OnResuming(Platform::Object^ sender, Platform::Object^ args)
{
    // Restore any data or state that was unloaded on suspend. By default, data
    // and state are persisted when resuming from suspend. Note that this event
    // does not occur if the app was previously terminated.

    // Insert your code here.
    m_deviceResources->SetLogicalSize(Size(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT));
}

// Window event handlers.

void RemotingHostSampleUWP::OnWindowSizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ args)
{
    m_deviceResources->SetLogicalSize(Size(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT));
}

void RemotingHostSampleUWP::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
    m_windowVisible = args->Visible;
}

void RemotingHostSampleUWP::OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
{
    m_windowClosed = true;
}

// DisplayInformation event handlers.

void RemotingHostSampleUWP::OnDpiChanged(DisplayInformation^ sender, Object^ args)
{
    // Note: The value for LogicalDpi retrieved here may not match the effective DPI of the app
    // if it is being scaled for high resolution devices. Once the DPI is set on DeviceResources,
    // you should always retrieve it using the GetDpi method.
    // See DeviceResources.cpp for more details.
    m_deviceResources->SetDpi(sender->LogicalDpi);
    //m_main->CreateWindowSizeDependentResources();
}

void RemotingHostSampleUWP::OnOrientationChanged(DisplayInformation^ sender, Object^ args)
{
    m_deviceResources->SetCurrentOrientation(sender->CurrentOrientation);
}

void RemotingHostSampleUWP::OnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args)
{
    m_deviceResources->ValidateDevice();
}

bool RemotingHostSampleUWP::ConnectToRemoteDevice()
{
    if (!m_streamerHelpers)
    {
        m_streamerHelpers = ref new HolographicStreamerHelpers();
        m_appView->Initialize(m_streamerHelpers->HolographicSpace, m_streamerHelpers->RemoteSpeech);
        ComPtr<ID3D11Device4> spDevice = m_appView->GetDeviceResources()->GetD3DDevice();
        ComPtr<ID3D11DeviceContext3> spContext = m_appView->GetDeviceResources()->GetD3DDeviceContext();
        m_deviceResources->SetD3DDevice(spDevice.Get(), spContext.Get());
        m_ipAddress = "192.168.0.1";

        m_streamerHelpers->CreateStreamer(m_appView->GetDeviceResources()->GetD3DDevice());
        m_streamerHelpers->OnSendFrame += ref new Microsoft::Holographic::SendFrameEvent(
            [this](_In_ const ComPtr<ID3D11Texture2D>& spTexture, _In_ FrameMetadata metadata)
        {
            if (m_showPreview)
            {
                ComPtr<ID3D11Device4> spDevice = m_appView->GetDeviceResources()->GetD3DDevice();
                ComPtr<ID3D11DeviceContext3> spContext = m_appView->GetDeviceResources()->GetD3DDeviceContext();
                
                ComPtr<ID3D11Texture2D> spBackBuffer;
                ThrowIfFailed(m_deviceResources->GetSwapChain()->GetBuffer(0, IID_PPV_ARGS(&spBackBuffer)));

                DXGI_PRESENT_PARAMETERS parameters = { 0 };
                ThrowIfFailed(m_deviceResources->GetSwapChain()->Present1(1, 0, &parameters));

                spContext->CopySubresourceRegion(
                    spBackBuffer.Get(), // dest
                    0,                  // dest subresource
                    0, 0, 0,            // dest x, y, z
                    spTexture.Get(),    // source
                    0,                  // source subresource
                    nullptr);           // source box, null means the entire resource
            }
        });

        // We currently need to stream at 720p because that's the resolution of our remote display.
        // There is a check in the holographic streamer that makes sure the remote and local 
        // resolutions match. The default streamer resolution is 1080p.
        m_streamerHelpers->SetVideoFrameSize(STREAMER_WIDTH, STREAMER_HEIGHT);
    }

    if (m_streamerHelpers)
    {
        try
        {
            m_streamerHelpers->Connect(m_ipAddress->Data(), 8001);
            return true;
        }
        catch (Platform::Exception^ ex)
        {
            DebugLog(L"Connect failed with hr = 0x%08X", ex->HResult);
        }
    }

    return false;
}



