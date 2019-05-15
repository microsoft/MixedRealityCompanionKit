// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"

#include "ModuleManager.h"

#include "Listener.h"
#include "Connector.h"
#include "DataBundle.h"

#include "RealtimeServer.h"
#include "RealtimeMediaPlayer.h"

using namespace winrt;
using namespace RealtimeStreaming::Media;
using namespace RealtimeStreaming::Common;
using namespace RealtimeStreaming::Network;
using namespace RealtimeStreaming::Plugin;

using namespace Windows::Foundation;
using namespace Windows::Storage::Streams;
using namespace Windows::Networking;
using namespace Windows::Media::MediaProperties;

// Callback Definitions
namespace winrt::RealtimeStreaming::Plugin
{
    typedef UINT32 ModuleHandle;

    // UNITY_INTERFACE_API instead of __stdcall
    extern "C" typedef void(__stdcall *PluginCallback)(
        _In_ ModuleHandle handle,
        _In_ void* pCallbackObject,
        _In_ HRESULT result);

    extern "C" typedef void(__stdcall *PlayerCreatedCallback)(
        _In_ void* pCallbackObject,
        _In_ HRESULT result,
        _In_ int width,
        _In_ int height);

    extern "C" typedef void(__stdcall *DataReceivedHandler)(
        _In_ ModuleHandle handle,
        _In_ void* pCallbackObject,
        _In_ UINT16 opertion,
        _In_ UINT32 bufferSize,
        _In_ const byte* buffer);
}

/* Static Variables */
static std::shared_ptr<IUnityDeviceResource> s_deviceResource;

static UnityGfxRenderer s_deviceType = kUnityGfxRendererNull;
static IUnityInterfaces* s_unityInterfaces = nullptr;
static IUnityGraphics* s_unityGraphics = nullptr;

static ModuleManager  s_moduleManager{ nullptr };

static std::vector<byte> s_bundleData;

template <class T>
static T GetModule(_In_ ModuleHandle handle)
{
    if (s_moduleManager != nullptr)
    {
        return s_moduleManager.GetModule(handle).as<T>();
    }

    return nullptr;
}

static void CompletePluginCallback(
    _In_ PluginCallback callback,
    _In_ void* pCallbackObject,
    _In_ ModuleHandle handle,
    _In_ HRESULT hr)
{
    LOG_RESULT(ExceptionBoundary([&]() -> HRESULT
    {
        callback(handle, pCallbackObject, hr);
        return hr;
    }));
}

static void UNITY_INTERFACE_API  OnDeviceEvent(UnityGfxDeviceEventType eventType)
{
    Log(Log_Level_Info, L"Plugin::OnDeviceEvent ");

    UnityGfxRenderer currentDeviceType = UnityGfxRenderer::kUnityGfxRendererNull;

    // Create graphics API implementation upon initialization
    if (eventType == kUnityGfxDeviceEventInitialize)
    {
        assert(s_deviceResource == nullptr);

        s_deviceType = s_unityGraphics->GetRenderer();

        s_deviceResource = CreateDeviceResource(s_deviceType);
    }

    // Let the implementation process the device related event
    if (s_deviceResource != nullptr)
    {
        s_deviceResource->ProcessDeviceEvent(eventType, s_unityInterfaces);
    }

    // Cleanup graphics API implementation upon shutdown
    if (eventType == kUnityGfxDeviceEventShutdown)
    {
        s_deviceResource.reset();
        s_deviceResource = nullptr;
        s_deviceType = kUnityGfxRendererNull;
    }
}

RTDLL_(void) UnityPluginLoad(_In_ IUnityInterfaces* unityInterfaces)
{
    Log(Log_Level_Info, L"Plugin::Load()\n");

    NULL_THROW(unityInterfaces);

    if (s_moduleManager == nullptr)
    {
        s_moduleManager = winrt::make<RealtimeStreaming::Plugin::implementation::ModuleManager>();
    }
    else
    {
        s_moduleManager.ClearModules();
    }

    s_unityInterfaces = unityInterfaces;
    s_unityGraphics = s_unityInterfaces->Get<IUnityGraphics>();
    s_unityGraphics->RegisterDeviceEventCallback(OnDeviceEvent);

    // Run OnGraphicsDeviceEvent(initialize) manually on plugin load
    OnDeviceEvent(kUnityGfxDeviceEventInitialize);
}

RTDLL_(void) UnityPluginUnload()
{
    Log(Log_Level_Info, L"Plugin::UnLoad()\n");

    if (s_unityGraphics == nullptr)
    {
        return;
    }

    s_unityGraphics->UnregisterDeviceEventCallback(OnDeviceEvent);

    s_unityInterfaces = nullptr;
    s_unityGraphics = nullptr;

    s_moduleManager = nullptr;
    s_deviceResource = nullptr;
}

static void UNITY_INTERFACE_API OnRenderEvent(int32_t eventID) {}

RTDLL_(UnityRenderingEvent) GetPluginEventFunc()
{
    return OnRenderEvent;
}

//
//
// Network Connection Interop
//
//

void ListenAsync(Listener listener,
    _In_ PluginCallback callback,
    _In_ void* pManagedCallbackObject)
{
    auto listenAsync = listener.ListenAsync(true);

    listenAsync.Completed([=](auto const asyncOp, AsyncStatus const status)
    {
        Log(Log_Level_Info, L"Plugin::StartListener() - ListenAsync() Completed - Tid:%d \n", GetCurrentThreadId());

        auto connection = asyncOp.GetResults();

        if (s_moduleManager == nullptr || connection == nullptr)
        {
            Log(Log_Level_Info, L"Plugin::ListenerCreateAndStart() [ListenAsync()] FAILED connection = null \n");
            ModuleHandle invalid = MODULE_HANDLE_INVALID;
            CompletePluginCallback(callback, pManagedCallbackObject, invalid, E_INVALIDARG);
            return;
        }

        ModuleHandle handle = s_moduleManager.AddModule(connection);
        CompletePluginCallback(callback, pManagedCallbackObject, handle, S_OK);
    });
}

RTDLL ListenerCreateAndStart(
    _In_ UINT16 port,
    _Inout_ UINT32* listenerHandle,
    _In_ PluginCallback callback,
    _In_ void* pManagedCallbackObject)
{
    Log(Log_Level_Info, L"Plugin::StartListener() - Tid:%d \n", GetCurrentThreadId());

    *listenerHandle = MODULE_HANDLE_INVALID;
    NULL_CHK(pManagedCallbackObject);
    NULL_CHK(listenerHandle);
    NULL_CHK(callback);

    try
    {
        // Create a new listener
        Listener listener = winrt::make<winrt::RealtimeStreaming::Network::implementation::Listener>(port);

        // Save handle of newly created listener to output
        *listenerHandle = s_moduleManager.AddModule(listener);

        ListenAsync(listener, callback, pManagedCallbackObject);
    }
    catch (winrt::hresult_error const& ex)
    {
        return ex.to_abi();
    }

    return S_OK;
}

RTDLL ListenerStopAndClose(
    _In_ UINT32 handle)
{
    Log(Log_Level_Info, L"Plugin::StopListener()\n");

    NULL_CHK(s_moduleManager);

    s_moduleManager.ReleaseModule(handle);

    return S_OK;
}

void OnConnectAsyncCompleted(_In_ Connector connector,
    _In_ IAsyncOperation<Connection> connectAsyncOp,
    _In_ PluginCallback callback,
    _In_ void* pManagedCallbackObject)
{
    //auto connectAsync = connector.ConnectAsync(hostName, uri.Port());
    connectAsyncOp.Completed([=](auto const asyncOp, AsyncStatus const status)
    {
        Log(Log_Level_Info, L"Plugin::ConnectorCreateAndStart() - ConnectAsync() Completed -Tid:%d \n", GetCurrentThreadId());

        auto connection = asyncOp.GetResults();

        if (s_moduleManager == nullptr || connection == nullptr)
        {
            Log(Log_Level_Info, L"Plugin::ConnectorCreateAndStart() [ConnectAsync()] FAILED connection = null \n");
            CompletePluginCallback(callback, pManagedCallbackObject, MODULE_HANDLE_INVALID, E_INVALIDARG);
            return;
        }

        ModuleHandle handle = s_moduleManager.AddModule(connection);
        CompletePluginCallback(callback, pManagedCallbackObject, handle, S_OK);
    });
}

RTDLL ConnectorCreateAndStart(
    _In_ LPCWSTR address,
    _Inout_ UINT32* connectorHandle,
    _In_ PluginCallback callback,
    _In_ void* pManagedCallbackObject)
{
    Log(Log_Level_Info, L"Plugin::ConnectorCreateAndStart() -Tid:%d \n", GetCurrentThreadId());

    NULL_CHK(pManagedCallbackObject);
    NULL_CHK(connectorHandle);
    NULL_CHK(callback);

    *connectorHandle = MODULE_HANDLE_INVALID;

    try
    {
        Connector connector = winrt::make<RealtimeStreaming::Network::implementation::Connector>();
        *connectorHandle = s_moduleManager.AddModule(connector);

        std::wstring wsUri = address;
        Uri uri{ wsUri };
        HostName hostName = HostName(uri.Host());
        auto connectAsync = connector.ConnectAsync(hostName, uri.Port());
        OnConnectAsyncCompleted(connector, connectAsync, callback, pManagedCallbackObject);
    }
    catch (winrt::hresult_error const& ex)
    {
        return ex.to_abi();
    }

    return S_OK;
}

RTDLL ConnectorCreateAndDiscover(
    _Inout_ UINT32* connectorHandle,
    _In_ PluginCallback callback,
    _In_ void* pManagedCallbackObject)
{
    Log(Log_Level_Info, L"Plugin::ConnectorCreateAndDiscover() -Tid:%d \n", GetCurrentThreadId());

    NULL_CHK(pManagedCallbackObject);
    NULL_CHK(connectorHandle);
    NULL_CHK(callback);

    *connectorHandle = MODULE_HANDLE_INVALID;

    try
    {
        Connector connector = winrt::make<RealtimeStreaming::Network::implementation::Connector>();
        *connectorHandle = s_moduleManager.AddModule(connector);

        auto connectAsync = connector.DiscoverAsync();
        OnConnectAsyncCompleted(connector, connectAsync, callback, pManagedCallbackObject);
    }
    catch (winrt::hresult_error const& ex)
    {
        return ex.to_abi();
    }

    return S_OK;
}

RTDLL ConnectorStopAndClose(
    _In_ UINT32 handle)
{
    Log(Log_Level_Info, L"Plugin::CloseConnector()\n");

    NULL_CHK(s_moduleManager);

    s_moduleManager.ReleaseModule(handle);
}

RTDLL ConnectionAddDisconnected(
    _In_ UINT32 handle,
    _In_ PluginCallback callback,
    _In_ void* pManagedCallbackObject,
    _Out_ INT64* tokenValue)
{
    Log(Log_Level_Info, L"Plugin::ConnectionAddDisconnected() -Tid:%d \n", GetCurrentThreadId());

    NULL_CHK(pManagedCallbackObject);
    NULL_CHK(callback);
    NULL_CHK(tokenValue);

    Connection connection = GetModule<Connection>(handle);
    event_token disconnectedToken = connection.Disconnected([handle, callback, pManagedCallbackObject]()
    {
        callback(handle, pManagedCallbackObject, S_OK);
    });

    // set return
    *tokenValue = disconnectedToken.value;

    // track the token
    s_moduleManager.StoreToken(handle, disconnectedToken);

    return S_OK;
}

RTDLL ConnectionRemoveDisconnected(
    _In_ UINT32 handle,
    _In_ INT64 tokenValue)
{
    Log(Log_Level_Info, L"Plugin::ConnectionRemoveReceived()\n");

    // get the token stored in the event list
    winrt::event_token removeToken = s_moduleManager.RemoveToken(handle, tokenValue);

    // unsubscribe from the event
    Connection connection = GetModule<Connection>(handle);
    connection.Disconnected(removeToken);

    return S_OK;
}

RTDLL ConnectionAddReceived(
    _In_ UINT32 handle,
    _In_ DataReceivedHandler callback,
    _In_ void* pManagedCallbackObject,
    _Out_ INT64* tokenValue)
{
    Log(Log_Level_Info, L"Plugin::ConnectionAddReceived() -Tid:%d \n", GetCurrentThreadId());
    NULL_CHK(pManagedCallbackObject);
    NULL_CHK(callback);
    NULL_CHK(tokenValue);

    // get connection
    Connection connection = GetModule<Connection>(handle);

    // register for callback
    winrt::event_token evtReceivedToken =
        connection.Received([handle, callback, pManagedCallbackObject](
            _In_ IInspectable const& /* sender */,
            _In_ DataBundleArgs const& args)
    {
        Log(Log_Level_Info, L"Plugin::bundleReceivedCallback() -Tid:%d \n", GetCurrentThreadId());

        HRESULT hr = S_OK;

        PayloadType payloadType = args.PayloadType();

        switch (payloadType)
        {
        case PayloadType::State_Scene:
        case PayloadType::State_Input:
        {
            auto dataBundle = args.Bundle().try_as<RealtimeStreaming::Network::implementation::DataBundle>();
            NULL_CHK(dataBundle);

            DWORD cbTotalLen = dataBundle->TotalSize();

            // Copy the data structure
            UINT32 copiedBytes = 0;
            if (cbTotalLen > s_bundleData.size())
            {
                s_bundleData.resize(cbTotalLen);
            }

            hr = dataBundle->CopyTo(0, cbTotalLen, &s_bundleData[0], &copiedBytes);
            if (SUCCEEDED(hr))
            {
                callback(handle, pManagedCallbackObject, (UINT16)payloadType, copiedBytes, s_bundleData.data());
            }
        }
        break;

        default:
            //slim_lock_guard guard(m_lock);
            callback(handle, pManagedCallbackObject, static_cast<UINT16>(payloadType), 0, nullptr);
            break;
        };
    });

    // set return
    *tokenValue = evtReceivedToken.value;

    // track the token
    s_moduleManager.StoreToken(handle, evtReceivedToken);

    return S_OK;
}

RTDLL ConnectionRemoveReceived(
    _In_ UINT32 handle,
    _In_ INT64 tokenValue)
{
    Log(Log_Level_Info, L"Plugin::ConnectionRemoveReceived()\n");

    // get the token stored in the event list
    winrt::event_token removeToken = s_moduleManager.RemoveToken(handle, tokenValue);

    // unsubscribe from the event
    Connection connection = GetModule<Connection>(handle);
    connection.Received(removeToken);

    return S_OK;
}

RTDLL ConnectionClose(
    _In_ UINT32 connectionHandle)
{
    Log(Log_Level_Info, L"Plugin::ConnectionClose()\n");

    try
    {
        Connection connection = GetModule<Connection>(connectionHandle);

        auto moduleManagerImpl = s_moduleManager.as<RealtimeStreaming::Plugin::implementation::ModuleManager>();

        // find any tokens register to this handle
        auto eventTokens = moduleManagerImpl->RemoveTokens(connectionHandle);
        for each (auto token in eventTokens)
        {
            connection.Disconnected(token);
            connection.Received(token);
        }

        // unregister from the connection
        s_moduleManager.ReleaseModule(connectionHandle);
    }
    catch (winrt::hresult_error const& ex)
    {
        return ex.to_abi();
    }

    return S_OK;
}

//
//
// Realtime Streaming Provider Interop
//
//

RTDLL CreateRealtimeStreamingServer(
    _In_ UINT32 connectionHandle,
    _In_ bool useHEVC,
    _In_ UINT32 width,
    _In_ UINT32 height,
    _Inout_ UINT32* serverHandle)
{
    NULL_CHK(serverHandle);

    Log(Log_Level_Info, L"Plugin::CreateRealtimeStreamingServer() -Tid:%d \n", GetCurrentThreadId());

    Connection connection = GetModule<Connection>(connectionHandle);

    // Default encoding activation
    auto mediaProfile = useHEVC ? MediaEncodingProfile::CreateHevc(VideoEncodingQuality::HD720p) : MediaEncodingProfile::CreateMp4(VideoEncodingQuality::HD720p);
    mediaProfile.Video().Width(width);
    mediaProfile.Video().Height(height);
    GUID videoFormat = MFVideoFormat_RGB32;

    RealtimeServer rtServer = winrt::make<RealtimeStreaming::Media::implementation::RealtimeServer>(
        connection,
        videoFormat,
        mediaProfile);

    ModuleHandle handle = MODULE_HANDLE_INVALID;
    NULL_CHK(s_moduleManager);
    handle = s_moduleManager.AddModule(rtServer);
    *serverHandle = handle;

    return S_OK;
}

RTDLL RealtimeStreamingWrite(
    _In_ UINT32 serverHandle,
    __in_ecount(bufferSize) BYTE* pBuffer,
    _In_ UINT32 bufferSize)
{
    NULL_CHK(pBuffer);

    Log(Log_Level_Info, L"Plugin::RealtimeStreamingWrite()\n");

    array_view<byte const> bufferArrayView{ pBuffer, pBuffer + bufferSize };

    auto rtServer = GetModule<RealtimeServer>(serverHandle);
    rtServer.WriteFrame(bufferSize, bufferArrayView);

    return S_OK;
}

RTDLL RealtimeStreamingShutdown(
    _In_ UINT32 serverHandle)
{
    Log(Log_Level_Info, L"Plugin::RTServerShutdown()\n");

    auto rtServer = GetModule<RealtimeServer>(serverHandle);
    rtServer.Shutdown();

    s_moduleManager.ReleaseModule(serverHandle);

    return S_OK;
}

//
//
// Realtime Streaming Playback Interop
//
//

// Lambdas cannot be in extern C method
void InitPlayerAsync(RealtimeMediaPlayer rtPlayer,
    Connection connection,
    _In_ PlayerCreatedCallback callback,
    _In_ void* pManagedCallbackObject)
{
    auto rtPlayerImpl = rtPlayer.as<RealtimeStreaming::Media::implementation::RealtimeMediaPlayer>();
    rtPlayerImpl->Initialize(s_deviceResource);

    auto initAsync = rtPlayer.InitAsync(connection);
    initAsync.Completed([=](auto const asyncOp, AsyncStatus const status)
    {
        auto videoProps = asyncOp.GetResults();

        if (videoProps == nullptr)
        {
            Log(Log_Level_Info, L"Plugin::RTPlayerCreate() [InitAsync()] FAILED videoProps = null \n");

            callback(pManagedCallbackObject, E_INVALIDARG, 0, 0);
            return;
        }

        UINT32 width = videoProps.Width(), height = videoProps.Height();

        Log(Log_Level_Info, L"Plugin::RTPlayerCreate() [InitAsync()] w:%d - h:%d \n", width, height);

        callback(pManagedCallbackObject, S_OK, width, height);
    });
}

RTDLL CreateRealtimePlayer(
    _In_ ModuleHandle connectionHandle,
    _In_ StateChangedCallback stateCallback,
    _In_ PlayerCreatedCallback createdCallback,
    _In_ void* pManagedCallbackObject,
    _Inout_ UINT32* serverHandle)
{
    if (pManagedCallbackObject == nullptr)
    {
        return E_INVALIDARG;
    }

    Log(Log_Level_Info, L"Plugin::RTPlayerCreate() -Tid:%d \n", GetCurrentThreadId());

    NULL_CHK(stateCallback);
    NULL_CHK(createdCallback);
    NULL_CHK(pManagedCallbackObject);

    auto connection = GetModule<Connection>(connectionHandle);

    RealtimeMediaPlayer rtPlayer = winrt::make<RealtimeStreaming::Media::implementation::RealtimeMediaPlayer>();
    auto rtPlayerImpl = rtPlayer.as<RealtimeStreaming::Media::implementation::RealtimeMediaPlayer>();
    NULL_CHK(rtPlayerImpl);

    rtPlayerImpl->SetStateChangeCallback(stateCallback, pManagedCallbackObject);

    ModuleHandle handle = MODULE_HANDLE_INVALID;
    NULL_CHK(s_moduleManager);

    handle = s_moduleManager.AddModule(rtPlayer);
    *serverHandle = handle;

    InitPlayerAsync(rtPlayer, connection, createdCallback, pManagedCallbackObject);

    return S_OK;
}

RTDLL ReleaseRealtimePlayer(
    _In_ UINT32 playerHandle)
{
    Log(Log_Level_Info, L"Plugin::ReleaseRealtimePlayer() -Tid:%d \n", GetCurrentThreadId());

    auto rtPlayer = GetModule<RealtimeMediaPlayer>(playerHandle);
    rtPlayer.Shutdown();

    s_moduleManager.ReleaseModule(playerHandle);

    return S_OK;
}

RTDLL GetPlayerStreamResolution(
    _In_ UINT32 playerHandle,
    _Inout_ UINT32* pWidth,
    _Inout_ UINT32* pHeight)
{
    NULL_CHK(pWidth);
    NULL_CHK(pHeight);

    auto rtPlayer = GetModule<RealtimeMediaPlayer>(playerHandle);
    VideoEncodingProperties videoProps = rtPlayer.GetVideoProperties();

    NULL_CHK(videoProps);

    *pWidth = videoProps.Width();
    *pHeight = videoProps.Height();

    return S_OK;
}

RTDLL CreateRealtimePlayerTexture(
    _In_ UINT32 playerHandle,
    _In_ UINT32 width,
    _In_ UINT32 height,
    _COM_Outptr_ void** ppvTexture_L,
    _COM_Outptr_ void** ppvTexture_UV)
{
    Log(Log_Level_Info, L"Plugin::CreateRealtimePlayerTexture(%d, %d) -Tid:%d \n", width, height, GetCurrentThreadId());

    NULL_CHK(ppvTexture_L);
    NULL_CHK(ppvTexture_UV);

    auto rtPlayer = GetModule<RealtimeMediaPlayer>(playerHandle);
    auto rtPlayerImpl = rtPlayer.as<RealtimeStreaming::Media::implementation::RealtimeMediaPlayer>();
    NULL_CHK(rtPlayerImpl);

    return rtPlayerImpl->CreateStreamingTexture(width, height, ppvTexture_L, ppvTexture_UV);
}

RTDLL RealtimePlayerPlay(
    _In_ UINT32 playerHandle)
{
    Log(Log_Level_Info, L"Plugin::RTPlayerStart()\n");

    auto rtPlayer = GetModule<RealtimeMediaPlayer>(playerHandle);
    NULL_CHK(rtPlayer);

    rtPlayer.Play();

    return S_OK;
}

RTDLL RealtimePlayerPause(
    _In_ UINT32 playerHandle)
{
    Log(Log_Level_Info, L"Plugin::RTPlayerPause()\n");

    auto rtPlayer = GetModule<RealtimeMediaPlayer>(playerHandle);
    NULL_CHK(rtPlayer);

    rtPlayer.Pause();

    return S_OK;
}

RTDLL RealtimePlayerStop(
    _In_ UINT32 playerHandle)
{
    Log(Log_Level_Info, L"Plugin::RTPlayerStop()\n");

    auto rtPlayer = GetModule<RealtimeMediaPlayer>(playerHandle);
    NULL_CHK(rtPlayer);

    rtPlayer.Stop();

    return S_OK;
}