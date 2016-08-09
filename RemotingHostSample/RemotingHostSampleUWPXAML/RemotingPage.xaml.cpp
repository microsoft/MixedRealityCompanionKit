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
// RemotingPage.xaml.cpp
// Implementation of the RemotingPage class.
//

#include "pch.h"
#include "Common\DbgLog.h"
#include <ppltasks.h>

#include "RemotingPage.xaml.h"

#define STREAMER_WIDTH          1280
#define STREAMER_HEIGHT         720

#define INITIAL_WINDOW_WIDTH    STREAMER_WIDTH
#define INITIAL_WINDOW_HEIGHT   STREAMER_HEIGHT

static void ConsoleLog(Windows::UI::Xaml::Controls::TextBlock^ Console, _In_z_ LPCWSTR format, ...);

using namespace RemotingHostSample;

using namespace concurrency;
using namespace Microsoft::Holographic;
using namespace Microsoft::WRL;
using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;
using namespace Windows::System;
using namespace Windows::System::Threading;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

RemotingPage::RemotingPage()
{
    InitializeComponent();

    // Register event handlers for page lifecycle.
    CoreWindow^ window = Window::Current->CoreWindow;

    window->VisibilityChanged +=
        ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &RemotingPage::OnVisibilityChanged);

    DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

    currentDisplayInformation->DpiChanged +=
        ref new TypedEventHandler<DisplayInformation^, Object^>(this, &RemotingPage::OnDpiChanged);

    currentDisplayInformation->OrientationChanged +=
        ref new TypedEventHandler<DisplayInformation^, Object^>(this, &RemotingPage::OnOrientationChanged);

    DisplayInformation::DisplayContentsInvalidated +=
        ref new TypedEventHandler<DisplayInformation^, Object^>(this, &RemotingPage::OnDisplayContentsInvalidated);

    swapChainPanel->CompositionScaleChanged += 
        ref new TypedEventHandler<SwapChainPanel^, Object^>(this, &RemotingPage::OnCompositionScaleChanged);

    swapChainPanel->SizeChanged +=
        ref new SizeChangedEventHandler(this, &RemotingPage::OnSwapChainPanelSizeChanged);
    
    m_appView = ref new RemotingHostSample::AppView();

    // At this point we have access to the device. 
    // We can create the device-dependent resources.
    m_deviceResources = std::make_shared<DX::DeviceResourcesWindowed>();
    m_deviceResources->SetSwapChainPanel(swapChainPanel);
    
    Console->Text += "\n";
}

RemotingPage::~RemotingPage()
{
    critical_section::scoped_lock lock(m_criticalSection);

    // Stop rendering and processing events on destruction.
    StopRenderLoop();
}

// Saves the current state of the app for suspend and terminate events.
void RemotingPage::SaveInternalState(IPropertySet^ state)
{
    m_deviceResources->Trim();

    // Stop rendering when the app is suspended.
    StopRenderLoop();
}

// Loads the current state of the app for resume events.
void RemotingPage::LoadInternalState(IPropertySet^ state)
{

}

bool RemotingPage::ConnectToRemoteDevice()
{
    if (!m_streamerHelpers)
    {
        ConsoleLog(Console, L"Connecting to %s", m_ipAddress->Data());

        m_streamerHelpers = ref new HolographicStreamerHelpers();
        m_appView->Initialize(m_streamerHelpers->HolographicSpace, m_streamerHelpers->RemoteSpeech);
        ComPtr<ID3D11Device4> spDevice = m_appView->GetDeviceResources()->GetD3DDevice();
        ComPtr<ID3D11DeviceContext3> spContext = m_appView->GetDeviceResources()->GetD3DDeviceContext();
        m_deviceResources->SetD3DDevice(spDevice.Get(), spContext.Get());

        m_streamerHelpers->CreateStreamer(m_appView->GetDeviceResources()->GetD3DDevice());
        m_streamerHelpers->OnSendFrame += ref new SendFrameEvent(
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

        m_streamerHelpers->OnConnected += ref new ConnectedEvent(
            [this]()
            {
                ConsoleLog(Console, L"Connected.");
            });

        m_streamerHelpers->OnDisconnected += ref new DisconnectedEvent(
            [this](_In_ HolographicStreamerConnectionFailureReason failureReason)
            {
                ConsoleLog(Console, L"Disconnected with reason %d", failureReason);

                {
                    critical_section::scoped_lock lock(m_criticalSection);
                    if (!m_connectedState)
                    {
                        ConsoleLog(Console, L"Aborting reconnect attempt.");
                        return;
                    }
                }

                // Reconnect if this is a transient failure.
                if (failureReason == HolographicStreamerConnectionFailureReason::Unreachable
                    || failureReason == HolographicStreamerConnectionFailureReason::ConnectionLost)
                {
                    ConsoleLog(Console, L"Reconnecting...");

                    try
                    {
                        m_streamerHelpers->Connect(m_ipAddress->Data(), 8001);
                    }
                    catch (Platform::Exception^ ex)
                    {
                        ConsoleLog(Console, L"Connect failed with hr = 0x%08X", ex->HResult);
                    }
                }
                else
                {
                    ConsoleLog(Console, L"Disconnected with unrecoverable error, not attempting to reconnect.");
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
            ConsoleLog(Console, L"Waiting for connection...");
            m_streamerHelpers->Connect(m_ipAddress->Data(), 8001);
            return true;
        }
        catch (Platform::Exception^ ex)
        {
            ConsoleLog(Console, L"Connect failed with hr = 0x%08X", ex->HResult);
        }
    }

    return false;
}

void RemotingPage::DisconnectFromRemoteDevice()
{
    m_streamerHelpers->Disconnect();
    ConsoleLog(Console, L"Disconnected.");
}

// Window event handlers.

void RemotingPage::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
    critical_section::scoped_lock lock(m_criticalSection);

    m_windowVisible = args->Visible;
    if (m_windowVisible)
    {
        m_showPreview = m_priorPreviewState;
    }
    else
    {
        m_priorPreviewState = m_showPreview;
        m_showPreview = false;
    }
}

// DisplayInformation event handlers.

void RemotingPage::OnDpiChanged(DisplayInformation^ sender, Object^ args)
{
    critical_section::scoped_lock lock(m_criticalSection);

    // Note: The value for LogicalDpi retrieved here may not match the effective DPI of the app
    // if it is being scaled for high resolution devices. Once the DPI is set on DeviceResources,
    // you should always retrieve it using the GetDpi method.
    // See DeviceResources.cpp for more details.
    m_deviceResources->SetDpi(sender->LogicalDpi);
}

void RemotingPage::OnOrientationChanged(DisplayInformation^ sender, Object^ args)
{
    critical_section::scoped_lock lock(m_criticalSection);

    m_deviceResources->SetCurrentOrientation(sender->CurrentOrientation);
}

void RemotingPage::OnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args)
{
    critical_section::scoped_lock lock(m_criticalSection);

    m_deviceResources->ValidateDevice();
}

void RemotingPage::OnCompositionScaleChanged(SwapChainPanel^ sender, Object^ args)
{
    critical_section::scoped_lock lock(m_criticalSection);

    m_deviceResources->SetCompositionScale(sender->CompositionScaleX, sender->CompositionScaleY);
}

void RemotingPage::OnSwapChainPanelSizeChanged(Object^ sender, SizeChangedEventArgs^ e)
{
    critical_section::scoped_lock lock(m_criticalSection);

    m_deviceResources->SetLogicalSize(e->NewSize);
}

void RemotingPage::Key_Down(Platform::Object ^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs ^ e)
{
    if (e->Key == VirtualKey::Enter)
    {
        Start_Click(sender, e);
    }
}

void RemotingPage::Toggle_Preview(Platform::Object ^ sender, Windows::UI::Xaml::RoutedEventArgs ^ e)
{
    m_showPreview = !m_showPreview;

    if (!m_showPreview)
    {
        ConsoleLog(Console, L"Preview paused.");
        TogglePreview->Content = L"Resume preview";
    }
    else
    {
        ConsoleLog(Console, L"Preview resumed.");
        TogglePreview->Content = L"Pause preview";
    }
}

void RemotingPage::Start_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    {
        critical_section::scoped_lock lock(m_criticalSection);

        if (m_connectedState == true)
        {
            // Already trying to connect - return "true" to break out of loop.
            return;
        }
        else
        {
            m_connectedState = true;
        }
    }

    if (!m_ipAddress)
    {
        ConsoleLog(Console, L"Error: Please set an IP address.");
    }
    else
    {
        Start->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
        ipAddress->IsEnabled = false;

        while (!ConnectToRemoteDevice());
        StartRenderLoop();
        
        Stop->Visibility = Windows::UI::Xaml::Visibility::Visible;
    }
}

void RemotingPage::Stop_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    {
        critical_section::scoped_lock lock(m_criticalSection);
        if (m_connectedState == false)
        {
            return;
        }
        else
        {
            m_connectedState = false;
        }
    }

    Stop->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
    
    DisconnectFromRemoteDevice();
    StopRenderLoop();
    
    Start->Visibility = Windows::UI::Xaml::Visibility::Visible;
    ipAddress->IsEnabled = true;
}

void RemotingPage::ipAddress_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
    m_ipAddress = ipAddress->Text;
}

void RemotingPage::StartRenderLoop()
{
    // If the animation render loop is already running then do not start another thread.
    if ((m_renderLoopWorker != nullptr) && (m_renderLoopWorker->Status == AsyncStatus::Started))
    {
        return;
    }

    // Create a task that will be run on a background thread.
    auto workItemHandler = ref new WorkItemHandler([this](IAsyncAction ^ action)
    {
        // Calculate the updated frame and render once per vertical blanking interval.
        while (action->Status == AsyncStatus::Started)
        {
            critical_section::scoped_lock lock(m_criticalSection);

            m_appView->Tick();
        }
    });

    // Run task on a dedicated high priority background thread.
    m_renderLoopWorker = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);
}

void RemotingPage::StopRenderLoop()
{
    if (m_renderLoopWorker != nullptr)
    {
        m_renderLoopWorker->Cancel();
    }
}

static void ConsoleLog(Windows::UI::Xaml::Controls::TextBlock^ Console, _In_z_ LPCWSTR format, ...)
{
    wchar_t buffer[1024];
    LPWSTR bufEnd = nullptr;

    va_list args;
    va_start(args, format);
    HRESULT hr = StringCchVPrintfExW(buffer, _countof(buffer), &bufEnd, nullptr, STRSAFE_FILL_BEHIND_NULL | STRSAFE_FILL_ON_FAILURE, format, args);

    if (SUCCEEDED(hr))
    {
        if (*bufEnd != L'\n')
        {
            StringCchCatW(buffer, _countof(buffer), L"\r\n");
        }

        OutputDebugStringW(buffer);
    }

    va_end(args);

    if (Console!= nullptr)
    {
        auto dispatcher = Console->Dispatcher;
        if (dispatcher != nullptr)
        {
            auto stringToSend = ref new Platform::String(buffer);
            dispatcher->RunAsync(
                Windows::UI::Core::CoreDispatcherPriority::Normal,
                ref new Windows::UI::Core::DispatchedHandler(
                    [Console, stringToSend]()
                    {
                        Console->Text += stringToSend;
                    }));
        }
    }
}