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

#include "RemotingHostSampleDesktop.h"
#include "Common\DbgLog.h"

#include <vector>
#include <Microsoft.Perception.Simulation.h>
#include <strsafe.h>

#define WINDOWCLASSNAME             L"RemotingHostSampleDesktopWindowClass"
#define WINDOWNAME                  L"RemotingHostSampleDesktop"
#define TITLE_SEPARATOR             L" | "
#define TITLE_CONNECT_TEXT          L"Press Space To Connect"
#define TITLE_DISCONNECT_TEXT       L"Press D to Disconnect"
#define TITLE_ENABLE_PREVIEW_TEXT   L"Preview Disabled (press P to enable)"
#define TITLE_DISABLE_PREVIEW_TEXT  L"Preview Enabled (press P to disable)"

using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
using namespace Microsoft::Holographic;
using namespace Windows::Graphics::Holographic;
using namespace std;

LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static DesktopWindow* s_desktopWindow;

    LRESULT result = 0;

    switch (msg)
    {
    case WM_CREATE:
    {
        CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        s_desktopWindow = reinterpret_cast<DesktopWindow*>(cs->lpCreateParams);

        RECT clientRect;
        GetClientRect(hWnd, &clientRect);

        s_desktopWindow->ResizeSwapChain(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
        result = 0;
    }
    break;
    case WM_WINDOWPOSCHANGED:
    {
        auto windowPos = reinterpret_cast<WINDOWPOS*>(lParam);
        if ((windowPos->flags & SWP_NOSIZE) == 0)
        {
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);

            s_desktopWindow->ResizeSwapChain(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
        }
        result = 0;
    }
    break;
    case WM_DESTROY:
    {
        s_desktopWindow = nullptr;
        result = 0;
        PostQuitMessage(0);
    }
    break;
    case WM_CLOSE:
    {
        DestroyWindow(hWnd);
        result = 0;
    }
    break;
    case WM_CHAR:
    {
        const int key = tolower(static_cast<int>(wParam));
        s_desktopWindow->ReceiveKeyPress(static_cast<char>(key));
    }
    break;
    default:
        result = DefWindowProc(hWnd, msg, wParam, lParam);
        break;
    }

    return result;
}

DesktopWindow::DesktopWindow(Platform::String^ ipAddress) :
    m_ipAddress(ipAddress)
{
    m_appView = ref new RemotingHostSample::AppView();
}

_Use_decl_annotations_
void DesktopWindow::InitializeHwnd(HWND hWnd)
{
    auto lock = m_deviceLock.Lock();

    m_hWnd = hWnd;

}

_Use_decl_annotations_
void DesktopWindow::CreateSwapChain(
    const ComPtr<ID3D11Device1>& device)
{
    auto lock = m_deviceLock.Lock();

    DXGI_SWAP_CHAIN_DESC1 desc = { 0 };
    desc.Width              = m_width;
    desc.Height             = m_height;
    desc.Format             = DXGI_FORMAT_B8G8R8A8_UNORM;
    desc.Stereo             = false;
    desc.SampleDesc.Count   = 1; // Don't use multi-sampling.
    desc.SampleDesc.Quality = 0;
    desc.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.BufferCount        = 2; // Double buffered
    desc.SwapEffect         = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    desc.Flags              = 0;
    desc.AlphaMode          = DXGI_ALPHA_MODE_IGNORE;
    desc.Scaling            = DXGI_SCALING_STRETCH;

    ComPtr<IDXGIDevice1> dxgiDevice;
    ThrowIfFailed(device.As(&dxgiDevice));

    ComPtr<IDXGIAdapter> dxgiAdapter;
    ThrowIfFailed(dxgiDevice->GetAdapter(&dxgiAdapter));

    ComPtr<IDXGIFactory2> dxgiFactory;
    ThrowIfFailed(dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), &dxgiFactory));

    ThrowIfFailed(dxgiFactory->MakeWindowAssociation(m_hWnd, DXGI_MWA_NO_ALT_ENTER));

    ThrowIfFailed(
        dxgiFactory->CreateSwapChainForHwnd(
            device.Get(),
            m_hWnd,
            &desc,
            nullptr,
            nullptr,
            &m_swapChain
        ));
}

void DesktopWindow::ResizeSwapChain(int width, int height)
{
    auto lock = m_deviceLock.Lock();

    m_width = width;
    m_height = height;

    if (m_swapChain)
    {
        ThrowIfFailed(m_swapChain->ResizeBuffers(2, m_width, m_height, DXGI_FORMAT_B8G8R8A8_UNORM, 0));
    }
}

void DesktopWindow::Tick()
{
    // Determine whether or not to copy to the preview buffer.
    bool copyPreview = false;
    {
        // Hold the lock only long enough to evaluate the state of m_streamerHelpers and the connection without grabbing any other locks.
        auto exclusiveLock = m_connectionStateLock.LockExclusive();
        copyPreview = (m_streamerHelpers && m_streamerHelpers->GetConnectionState() != HolographicStreamerConnectionState::Connected) ||
            (!m_streamerHelpers);
    }

    auto lock = m_deviceLock.Lock();
    m_appView->Tick();

    if (copyPreview && m_appView->GetIsInitialized())
    {
        ComPtr<ID3D11Device1> spDevice = m_appView->GetDeviceResources()->GetD3DDevice();
        ComPtr<ID3D11DeviceContext> spContext = m_appView->GetDeviceResources()->GetD3DDeviceContext();

        ComPtr<ID3D11Texture2D> spBackBuffer;
        ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&spBackBuffer)));

        // Create a render target view of the back buffer.
        // Creating this resource is inexpensive, and is better than keeping track of
        // the back buffers in order to pre-allocate render target views for each one.
        ComPtr<ID3D11RenderTargetView> spRenderTargetView;
        ThrowIfFailed(spDevice->CreateRenderTargetView(spBackBuffer.Get(), nullptr, &spRenderTargetView));

        spContext->ClearRenderTargetView(spRenderTargetView.Get(), DirectX::Colors::CornflowerBlue);

        DXGI_PRESENT_PARAMETERS parameters = { 0 };
        ThrowIfFailed(m_swapChain->Present1(1, 0, &parameters));
    }
}

void DesktopWindow::ReceiveKeyPress(char key)
{
    if (key == ' ')
    {
        ConnectToRemoteDevice();
    }
    else if (key == 'd')
    {
        DisconnectFromRemoteDevice();
    }
    else if (key == 'p')
    {
        m_showPreview = !m_showPreview;
    }

    UpdateWindowTitle();
}

void DesktopWindow::ConnectToRemoteDevice()
{
    // Connecting to the remote device can change the connection state.
    auto exclusiveLock = m_connectionStateLock.LockExclusive();

    if (!m_streamerHelpers)
    {
        m_streamerHelpers = ref new HolographicStreamerHelpers();

        m_appView->Initialize(m_streamerHelpers->HolographicSpace, m_streamerHelpers->RemoteSpeech);

        CreateSwapChain(m_appView->GetDeviceResources()->GetD3DDevice());

        m_streamerHelpers->CreateStreamer(m_appView->GetDeviceResources()->GetD3DDevice());

        m_streamerHelpers->OnConnected += ref new ConnectedEvent(
            [this]()
            {
                UpdateWindowTitle();
            });

        Platform::WeakReference streamerHelpersWeakRef = Platform::WeakReference(m_streamerHelpers);
        m_streamerHelpers->OnDisconnected += ref new DisconnectedEvent(
            [this, streamerHelpersWeakRef](_In_ HolographicStreamerConnectionFailureReason failureReason)
            {
                DebugLog(L"Disconnected with reason %d", failureReason);
                UpdateWindowTitle();

                // Reconnect if this is a transient failure.
                if (failureReason == HolographicStreamerConnectionFailureReason::Unreachable ||
                    failureReason == HolographicStreamerConnectionFailureReason::ConnectionLost)
                {
                    DebugLog(L"Reconnecting...");

                    try
                    {
                        auto helpersResolved = streamerHelpersWeakRef.Resolve<HolographicStreamerHelpers>();
                        if (helpersResolved)
                        {
                            helpersResolved->Connect(m_ipAddress->Data(), 8001);
                        }
                        else
                        {
                            DebugLog(L"Failed to reconnect because a disconnect has already occurred.\n");
                        }
                    }
                    catch (Platform::Exception^ ex)
                    {
                        DebugLog(L"Connect failed with hr = 0x%08X", ex->HResult);
                    }
                }
                else
                {
                    DebugLog(L"Disconnected with unrecoverable error, not attempting to reconnect.");
                }
            });

        m_streamerHelpers->OnSendFrame += ref new SendFrameEvent(
            [this](_In_ const ComPtr<ID3D11Texture2D>& spTexture, _In_ FrameMetadata metadata)
            {
                if (m_showPreview)
                {
                    ComPtr<ID3D11Device1> spDevice = m_appView->GetDeviceResources()->GetD3DDevice();
                    ComPtr<ID3D11DeviceContext> spContext = m_appView->GetDeviceResources()->GetD3DDeviceContext();

                    ComPtr<ID3D11Texture2D> spBackBuffer;
                    ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&spBackBuffer)));

                    spContext->CopySubresourceRegion(
                        spBackBuffer.Get(), // dest
                        0,                  // dest subresource
                        0, 0, 0,            // dest x, y, z
                        spTexture.Get(),    // source
                        0,                  // source subresource
                        nullptr);           // source box, null means the entire resource

                    DXGI_PRESENT_PARAMETERS parameters = { 0 };
                    ThrowIfFailed(m_swapChain->Present1(1, 0, &parameters));
                }
            });

        // We currently need to stream at 720p because that's the resolution of our remote display.
        // There is a check in the holographic streamer that makes sure the remote and local 
        // resolutions match. The default streamer resolution is 1080p.
        m_streamerHelpers->SetVideoFrameSize(STREAMER_WIDTH, STREAMER_HEIGHT);
        try
        {
            m_streamerHelpers->Connect(m_ipAddress->Data(), 8001);
        }
        catch (Platform::Exception^ ex)
        {
            DebugLog(L"Connect failed with hr = 0x%08X", ex->HResult);
        }
    }
}

void DesktopWindow::DisconnectFromRemoteDevice()
{
    // Disconnecting from the remote device can change the connection state.
    auto exclusiveLock = m_connectionStateLock.LockExclusive();

    if (m_streamerHelpers != nullptr)
    {
        m_streamerHelpers->Disconnect();

        // Reset state
        m_streamerHelpers = nullptr;
    }
}

static Platform::String^ StreamerConnectionStateToString(HolographicStreamerConnectionState state)
{
    Platform::String^ connectState;

    switch (state)
    {
    case HolographicStreamerConnectionState::Disconnected:
        connectState = "Disconnected";
        break;
    case HolographicStreamerConnectionState::Connecting:
        connectState = "Connecting";
        break;
    case HolographicStreamerConnectionState::Connected:
        connectState = "Connected";
        break;
    default:
        connectState = "Unknown";
    }

    return connectState;
}

void DesktopWindow::UpdateWindowTitle()
{
    Platform::String^ title = ref new Platform::String(WINDOWNAME);
    Platform::String^ separator = ref new Platform::String(TITLE_SEPARATOR);

    // Title | {ip} | {State} [| Press Space to Connect] [| Preview Disabled (p toggles)]
    title += separator + m_ipAddress;
    {
        // The window title update depends on connection state; that should not change
        // while we are doing this update.
        auto sharedLock = m_connectionStateLock.LockShared();

        if (m_streamerHelpers)
        {
            title += separator + (m_hWnd ? StreamerConnectionStateToString(m_streamerHelpers->GetConnectionState()) : L"Initializing");
            title += separator + ((m_streamerHelpers->GetConnectionState() == HolographicStreamerConnectionState::Disconnected) ? TITLE_CONNECT_TEXT : TITLE_DISCONNECT_TEXT);
        }
        else
        {
            title += separator + TITLE_CONNECT_TEXT;
        }
        title += separator + (m_showPreview ? TITLE_DISABLE_PREVIEW_TEXT : TITLE_ENABLE_PREVIEW_TEXT);
    }

    if (m_hWnd)
    {
        if (!SetWindowText(m_hWnd, title->Data()))
        {
            ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
        }
    }
}

int main(Platform::Array<Platform::String^>^ args)
{
    RoInitializeWrapper roinit(RO_INIT_MULTITHREADED);
    ThrowIfFailed(HRESULT(roinit));

    Platform::String^ ipAddress = L"127.0.0.1";
    if (args->Length > 1)
    {
        ipAddress = args[1];
    }

    shared_ptr<DesktopWindow> desktopWindow = make_shared<DesktopWindow>(ipAddress);

    WNDCLASSEX wcex     = {};
    wcex.cbSize         = sizeof(wcex);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = wndProc;
    wcex.hInstance      = 0;
    wcex.hIcon          = LoadIcon(0, IDI_APPLICATION);
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = static_cast<HBRUSH>(GetStockObject(NULL_BRUSH));
    wcex.lpszClassName  = WINDOWCLASSNAME;
    RegisterClassEx(&wcex);

    RECT rc = { 0, 0, INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT };
    AdjustWindowRectEx(&rc, WS_OVERLAPPEDWINDOW, FALSE, 0);

    Platform::String^ windowName = ref new Platform::String(WINDOWNAME);

    HWND hWnd = CreateWindow(
        WINDOWCLASSNAME, 
        windowName->Data(), 
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 
        CW_USEDEFAULT, 
        rc.right - rc.left, 
        rc.bottom - rc.top, 
        nullptr, 
        nullptr, 
        0, 
        desktopWindow.get());

    RECT clientRect;
    GetClientRect(hWnd, &clientRect);

    desktopWindow->InitializeHwnd(hWnd);
    desktopWindow->UpdateWindowTitle();

    ShowWindow(hWnd, SW_SHOWNORMAL);
    bool quit = false;
    while (!quit)
    {
        MSG msg = { 0 };
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                quit = true;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            desktopWindow->Tick();
        }
    }

    return 0;
}
