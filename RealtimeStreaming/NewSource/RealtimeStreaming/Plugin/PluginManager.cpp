// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "PluginManager.h"
#include "Network\Listener.h"
#include "Network\Connector.h"
#include "Network\DataBundle.h"
#include "Media\RealtimeServer.h"
#include "Media\RealtimeMediaPlayer.h"

using namespace winrt;
using namespace RealtimeStreaming::Plugin::implementation;
using namespace RealtimeStreaming::Common;
using namespace RealtimeStreaming::Network;

using namespace concurrency;
using namespace Windows::Foundation;
using namespace Windows::Storage::Streams;
using namespace Windows::Networking;
using namespace Windows::Media::MediaProperties;

_Use_decl_annotations_
PluginManager::PluginManager()
{
    m_unityInterfaces = nullptr;
    m_unityGraphics = nullptr;
    m_unityCallback = nullptr;

    m_dxManager = nullptr;
}

PluginManager::~PluginManager()
{
    Uninitialize();
}

void PluginManager::Uninitialize()
{
    Log(Log_Level_Info, L"PluginManagerImpl::Uninitialize()\n");

    slim_lock_guard guard(m_lock);

    if (nullptr != m_dxManager)
    {
        m_dxManager = nullptr;
    }

    m_streamingAssetsPath.clear();

    assert(0 == m_eventTokens.size());
    m_eventTokens.clear();

    if (nullptr != m_moduleManager)
    {
        m_moduleManager = nullptr;
    }
}

static RealtimeStreaming::Plugin::PluginManager s_instance{ nullptr };

/* static */
_Use_decl_annotations_
RealtimeStreaming::Plugin::PluginManager PluginManager::Instance()
{
    if (s_instance == nullptr)
    {
        s_instance = PluginManager();
        s_threadId = GetCurrentThreadId();
    }

    return s_instance;
}

_Use_decl_annotations_
RealtimeStreaming::Plugin::ModuleManager PluginManager::ModuleManager()
{
    return m_moduleManager;
}

_Use_decl_annotations_
RealtimeStreaming::Plugin::DirectXManager PluginManager::DirectXManager()
{
    return m_dxManager;
}

_Use_decl_annotations_
BOOL PluginManager::IsOnThread()
{
    return (s_threadId == GetCurrentThreadId());
}


_Use_decl_annotations_
void PluginManager::Load(IUnityInterfaces* unityInterfaces, IUnityGraphicsDeviceEventCallback callback)
{
    Log(Log_Level_Info, L"PluginManagerImpl::Load()\n");

    //slim_lock_guard guard(m_lock);
    slim_lock_guard guard(m_lock);

    // was already initialized, so need to reset
    if (nullptr != m_dxManager)
    {
        Uninitialize();
    }

    if (nullptr == unityInterfaces || nullptr == callback)
    {
        return;
    }

    IUnityGraphics* unityGraphics = unityInterfaces->Get<IUnityGraphics>();
    if (nullptr == unityGraphics)
    {
        return;
    }

    m_unityInterfaces = unityInterfaces;

    m_unityGraphics = unityGraphics;

    m_unityCallback = callback;
    m_unityGraphics->RegisterDeviceEventCallback(m_unityCallback);

    // Run OnGraphicsDeviceEvent(initialize) manually on plugin load
    OnDeviceEvent(kUnityGfxDeviceEventInitialize);
}

_Use_decl_annotations_
void PluginManager::UnLoad()
{
    Log(Log_Level_Info, L"PluginManagerImpl::UnLoad()\n");

    //slim_lock_guard guard(m_lock);
    slim_lock_guard guard(m_lock);

    if (nullptr == m_unityGraphics || nullptr == m_unityCallback)
    {
        return;
    }

    Uninitialize();

    m_unityGraphics->UnregisterDeviceEventCallback(m_unityCallback);

    m_unityCallback = nullptr;

    m_unityInterfaces = nullptr;

    m_unityGraphics = nullptr;
}

_Use_decl_annotations_
void PluginManager::OnDeviceEvent(UnityGfxDeviceEventType eventType)
{
    Log(Log_Level_Info, L"PluginManagerImpl::OnDeviceEvent ");

    HRESULT hr = S_OK;

    //slim_lock_guard guard(m_lock);
    slim_lock_guard guard(m_lock);

    UnityGfxRenderer currentDeviceType = UnityGfxRenderer::kUnityGfxRendererNull;

    if (nullptr == m_unityGraphics)
    {
        return;
    }

    currentDeviceType = m_unityGraphics->GetRenderer();
    switch (eventType)
    {
    case kUnityGfxDeviceEventInitialize:
        Log(Log_Level_Info, L"- Initialize\n");
#if SUPPORT_D3D11
        if (kUnityGfxRendererD3D11 == currentDeviceType)
        {
            auto dx11 = _unityInterfaces->Get<IUnityGraphicsD3D11>();

            IFC(MakeAndInitialize<DirectXManagerImpl>(&_dxManager, dx11->GetDevice()));
        }
#endif
        break;

    case kUnityGfxDeviceEventShutdown:
        Log(Log_Level_Info, L"- Shutdown\n");
        Uninitialize();
        break;

    case kUnityGfxDeviceEventBeforeReset:
        Log(Log_Level_Info, L"- Lost\n");
        if (nullptr != m_dxManager)
        {
            m_dxManager.Lost();
        }
        break;

    case kUnityGfxDeviceEventAfterReset:
        Log(Log_Level_Info, L"- Reset\n");
        if (nullptr != m_dxManager)
        {
            m_dxManager.Reset();
        }
        break;
    };

done:
    if (FAILED(hr))
    {
        Uninitialize();
    }
}

_Use_decl_annotations_
HRESULT PluginManager::ListenerCreateAndStart(
    UINT16 port,
    ModuleHandle *listenerHandle,
    PluginCallback callback,
    void* pCallbackObject)
{
    Log(Log_Level_Info, L"PluginManagerImpl::StartListener()\n");
    
    // Init with default
    *listenerHandle = MODULE_HANDLE_INVALID;

    NULL_CHK(listenerHandle);
    NULL_CHK(callback);

    // Create a new listener
    Network::Listener listener = Listener(port);

    // Save handle of newly created listener to output
    *listenerHandle = m_moduleManager.AddModule(listener);
    
    create_task(listener.ListenAsync()).then([this, callback, pCallbackObject](_In_ Connection connection)
    {
        Log(Log_Level_Info, L"Manager::StartListener() - ListenAsync()\n");

        //slim_lock_guard guard(m_lock);

        if (m_moduleManager == nullptr)
        {
            // TODO: look at better HResult to pass?
            CompletePluginCallback(callback, pCallbackObject, MODULE_HANDLE_INVALID, E_INVALIDARG);
        }

        ModuleHandle handle = m_moduleManager.AddModule(connection);
        CompletePluginCallback(callback, pCallbackObject, handle, S_OK);
    });
}

_Use_decl_annotations_
HRESULT PluginManager::ListenerStopAndClose(
    ModuleHandle handle)
{
    Log(Log_Level_Info, L"PluginManagerImpl::StopListener()\n");

    //slim_lock_guard guard(m_lock);
    slim_lock_guard guard(m_lock);

    if (nullptr == m_moduleManager)
    {
        return S_OK;
    }

    // release the handle
    m_moduleManager.ReleaseModule(handle);

    return S_OK;
}

// connector
_Use_decl_annotations_
HRESULT PluginManager::ConnectorCreateAndStart(
    LPCWSTR address,
    ModuleHandle *connectorHandle,
    PluginCallback callback,
    void* pCallbackObject)
{
    Log(Log_Level_Info, L"PluginManagerImpl::ConnectorCreateAndStart() -Tid:%d \n", GetCurrentThreadId());

    NULL_CHK(connectorHandle);
    NULL_CHK(callback);

    *connectorHandle = MODULE_HANDLE_INVALID;
    
    std::wstring wsUri = address;
    Uri uri{ wsUri };
    HostName hostName = HostName(uri.Host);

    Connector connector = Connector(hostName, uri.Port);

    *connectorHandle = m_moduleManager.AddModule(connector);
    
    create_task(connector.ConnectAsync()).then([this, callback, pCallbackObject](_In_ Connection connection)
    {
        Log(Log_Level_Info, L"PluginManagerImpl::ConnectorCreateAndStart() [ConnectAsync()] -Tid:%d \n", GetCurrentThreadId());

        slim_lock_guard guard(m_lock);

        if (m_moduleManager == nullptr)
        {
            // TODO: look at better HResult to pass?
            CompletePluginCallback(callback, pCallbackObject, MODULE_HANDLE_INVALID, E_INVALIDARG);
        }

        ModuleHandle handle = m_moduleManager.AddModule(connection);
        CompletePluginCallback(callback, pCallbackObject, handle, S_OK);

    });
}

_Use_decl_annotations_
HRESULT PluginManager::ConnectorStopAndClose(
    ModuleHandle handle)
{
    Log(Log_Level_Info, L"PluginManagerImpl::CloseConnector()\n");

    slim_lock_guard guard(m_lock);

    if (nullptr == m_moduleManager)
    {
        return S_OK;
    }

    // release the handle
    m_moduleManager.ReleaseModule(handle);
}

_Use_decl_annotations_
HRESULT PluginManager::ConnectionAddDisconnected(
    ModuleHandle handle,
    PluginCallback callback,
    void* pCallbackObject,
    INT64* tokenValue)
{
    Log(Log_Level_Info, L"PluginManagerImpl::ConnectionAddDisconnected() -Tid:%d \n", GetCurrentThreadId());

    NULL_CHK(callback);
    NULL_CHK(tokenValue);

    slim_lock_guard guard(m_lock);

    Network::Connection connection = GetModule<Network::Connection>(handle);
    event_token disconnectedToken = connection.Disconnected([this, handle, callback, pCallbackObject](_In_ Connection sender)
    {
        NULL_CHK(sender);

        callback(handle, pCallbackObject, S_OK);
    });

    // set return
    *tokenValue = disconnectedToken.value;

    // track the token
    StoreToken(handle, disconnectedToken);

    return S_OK;
}

_Use_decl_annotations_
HRESULT PluginManager::ConnectionRemoveDisconnected(
    ModuleHandle handle,
    INT64 tokenValue)
{
    Log(Log_Level_Info, L"PluginManagerImpl::ConnectionRemoveReceived()\n");

    slim_lock_guard guard(m_lock);

    // get the token stored in the event list
    winrt::event_token removeToken = RemoveToken(handle, tokenValue);

    // unsubscribe from the event
    Network::Connection connection = GetModule<Network::Connection>(handle);
    connection.Disconnected(removeToken);

    return S_OK;
}

_Use_decl_annotations_
HRESULT PluginManager::ConnectionAddReceived(
    ModuleHandle handle,
    DataReceivedHandler callback,
    void* pCallbackObject,
    INT64* tokenValue)
{
    Log(Log_Level_Info, L"PluginManagerImpl::ConnectionAddReceived() -Tid:%d \n", GetCurrentThreadId());

    NULL_CHK(callback);
    NULL_CHK(tokenValue);

    slim_lock_guard guard(m_lock);

    // get connection
    Network::Connection connection = GetModule<Network::Connection>(handle);

    // register for callback
    winrt::event_token evtReceivedToken = 
        connection.Received([this, handle, callback, pCallbackObject](
            _In_ Network::Connection const& sender,
            _In_ Network::BundleReceivedArgs const& args)
    {
        Log(Log_Level_Info, L"PluginManagerImpl::bundleReceivedCallback() -Tid:%d \n", GetCurrentThreadId());

        HRESULT hr = S_OK;

        PayloadType payloadType = args.PayloadType;

        switch (args.PayloadType)
        {
        case PayloadType::State_Scene:
        case PayloadType::State_Input:
        {
            Network::implementation::DataBundle dataBundle = args.Bundle;

            DWORD cbTotalLen = dataBundle.TotalSize();

            // TODO: need better locking mechanism? cicular lock
            // PluginManager -> Add_ConnectionReceived hold locks but then connection class is locked against this finishing
            //slim_lock_guard guard(m_lock);

            // Copy the data structure
            UINT32 copiedBytes = 0;
            if (cbTotalLen > _bundleData.size())
            {
                _bundleData.resize(cbTotalLen);
            }

            hr = dataBundle.CopyTo(0, cbTotalLen, &_bundleData[0], &copiedBytes);
            if (SUCCEEDED(hr))
            {
                callback(handle, pCallbackObject, (UINT16)payloadType, copiedBytes, _bundleData.data());
            }
        }
        break;

        default:
            //slim_lock_guard guard(m_lock);
            callback(handle, pCallbackObject, static_cast<UINT16>(payloadType), 0, nullptr);
            break;
        };
    });
    
    // set return
    *tokenValue = evtReceivedToken.value;

    // track the token
    StoreToken(handle, evtReceivedToken);

    return S_OK;
}

_Use_decl_annotations_
HRESULT PluginManager::ConnectionRemoveReceived(
    ModuleHandle handle,
    INT64 tokenValue)
{
    Log(Log_Level_Info, L"PluginManagerImpl::ConnectionRemoveReceived()\n");

    slim_lock_guard guard(m_lock);

    // get the token stored in the event list
    winrt::event_token removeToken = RemoveToken(handle, tokenValue);

    // unsubscribe from the event
    Network::Connection connection = GetModule<Network::Connection>(handle);
    connection.Received(removeToken);

    return S_OK;
}

/*
_Use_decl_annotations_
HRESULT PluginManager::ConnectionSendRawData(
    ModuleHandle handle,
    PayloadType payloadType,
    byte* pBuffer,
    UINT32 bufferSize)
{
    Log(Log_Level_Info, L"PluginManagerImpl::ConnectionSendRawData\n");

    HRESULT hr = S_OK;

    NULL_CHK(pBuffer);

    PayloadType type = static_cast<PayloadType>(payloadType);

    if (type != PayloadType::State_Scene &&
        type != PayloadType::State_Input)
    {
        return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
    }

    slim_lock_guard guard(m_lock);

    // get connection
    Network::Connection connection = GetModule<Network::Connection>(handle);

    const DWORD c_cbPayloadSize = static_cast<DWORD>(bufferSize);
    const DWORD c_cbBufferSize = sizeof(PayloadHeader) + c_cbPayloadSize;

    // Create send buffer
    DataBuffer spDataBuffer = DataBuffer(c_cbBufferSize);
    com_ptr<IBuffer> spBuffer;
    spBuffer.attach(spDataBuffer.as<IBuffer>());

    // Prepare operation header
    PayloadHeader* pHeader = GetDataType<PayloadHeader*>(spBuffer.get());
    NULL_CHK_HR(pHeader, E_POINTER);
    pHeader->ePayloadType = type;
    pHeader->cbPayloadSize = c_cbPayloadSize;

    // Prepare location
    BYTE* pBlobStart = (reinterpret_cast<BYTE*>(pHeader) + sizeof(PayloadHeader));
    NULL_CHK_HR(pBlobStart, E_POINTER);
    memcpy_s(pBlobStart, c_cbPayloadSize, pBuffer, c_cbPayloadSize);

    // Set length of the packet
    spDataBuffer.CurrentLength(c_cbBufferSize);

    DataBundle bundle = make<Network::implementation::DataBundle>(spDataBuffer);
    //bundle.AddBuffer(spDataBuffer);

    // TODO: co-await?
    connection.SendBundleAsync(bundle);

    return S_OK;
}*/

_Use_decl_annotations_
HRESULT PluginManager::ConnectionClose(
    ModuleHandle handle)
{
    Log(Log_Level_Info, L"PluginManagerImpl::ConnectionClose()\n");

    slim_lock_guard guard(m_lock);

    // get connection
    Network::Connection connection = GetModule<Network::Connection>(handle);

    // find any tokens register to this handle
    auto iter = m_eventTokens.find(handle);
    if (iter != m_eventTokens.end())
    {
        for each (auto token in (*iter).second)
        {
            connection.Disconnected(token);
            connection.Received(token);
        }
        (*iter).second.clear();

        m_eventTokens.erase(iter);
    }

    // unregister from the connection
    m_moduleManager.ReleaseModule(handle);
}

_Use_decl_annotations_
HRESULT PluginManager::RTServerCreate(
    ModuleHandle connectionHandle,
    ModuleHandle* serverHandle)
{
    NULL_CHK(serverHandle);

    Log(Log_Level_Info, L"PluginManagerImpl::RTServerCreate()\n");

    Network::Connection connection = GetModule<Network::Connection>(*serverHandle);

    // Default encoding activation
    auto mediaProfile = MediaEncodingProfile::CreateHevc(VideoEncodingQuality::HD720p);

    Media::RealtimeServer rtServer = RealtimeStreaming::Media::RealtimeServer(connection, 
        MFVideoFormat_RGB32, 
        mediaProfile);

    slim_lock_guard guard(m_lock);

    ModuleHandle handle = MODULE_HANDLE_INVALID;

    if (nullptr == m_moduleManager)
    {
        IFT(E_NOT_SET);
    }

    handle = m_moduleManager.AddModule(rtServer);
    *serverHandle = handle;

    return S_OK;
}

_Use_decl_annotations_
HRESULT PluginManager::RTServerShutdown(
    ModuleHandle serverHandle)
{
    Log(Log_Level_Info, L"PluginManagerImpl::RTServerShutdown()\n");

    slim_lock_guard guard(m_lock);

    auto rtServer = GetModule<Media::RealtimeServer>(serverHandle);
    rtServer.Shutdown();

    // release the handle
    m_moduleManager.ReleaseModule(serverHandle);

    return S_OK;
}

_Use_decl_annotations_
HRESULT PluginManager::RTServerWriteFrame(
    ModuleHandle serverHandle,
    BYTE const* pBuffer,
    UINT32 bufferSize)
{
    NULL_CHK(pBuffer);

    Log(Log_Level_Info, L"PluginManagerImpl::RTServerWriteFrame()\n");

    slim_lock_guard guard(m_lock);

    array_view<byte const> bufferArrayView{ pBuffer, pBuffer + bufferSize };

    auto rtServer = GetModule<Media::RealtimeServer>(serverHandle);
    rtServer.WriteFrame(bufferSize, bufferArrayView);

    return S_OK;
}

_Use_decl_annotations_
HRESULT PluginManager::RTPlayerCreate(
    ModuleHandle handle,
    //StateChangedCallback fnCallback,
    PlayerCreatedCallback callback,
    void* pCallbackObject)
{
    Log(Log_Level_Info, L"PluginManagerImpl::RTPlayerCreate() -Tid:%d \n", GetCurrentThreadId());

    NULL_CHK(callback);
    NULL_CHK(pCallbackObject);

    slim_lock_guard guard(m_lock);

    // get connection
    auto connection = GetModule<Connection>(handle);
    UnityGfxRenderer unityGraphics = m_unityGraphics->GetRenderer();

    // TODO: update realtimemediaplayer with connection & realtimemediasource
    s_spStreamingPlayer = nullptr; // make sure we unseat if previously assigned
    //s_spStreamingPlayer = Media::implementation::RealtimeMediaPlayer::Create()
    auto spPlayer = Media::RealtimeMediaPlayer();
    /*
    RealtimeMediaPlayer::Create(unityGraphics,
        m_unityInterfaces,
        spConnection.get(),
        //fnCallback,
        &spPlayer));
    */
    co_await spPlayer.InitAsync(connection)

    Log(Log_Level_Info, L"PluginManagerImpl::RTPlayerCreate() [InitAsync()] -Tid:%d \n", GetCurrentThreadId());
        //slim_lock_guard guard(m_lock);

    //spPlayer.MediaEncodingProfile
    Log(Log_Level_Info, L"PluginManagerImpl::RTPlayerCreate() w:%d - h:%d \n", width, height);
    callback(pCallbackObject,
        hr,
        width,
        height);

    return hr;
}

_Use_decl_annotations_
HRESULT PluginManager::RTPlayerRelease()
{
    Log(Log_Level_Info, L"PluginManagerImpl::RTPlayerRelease() -Tid:%d \n", GetCurrentThreadId());

    if (s_spStreamingPlayer != nullptr)
    {
        s_spStreamingPlayer = nullptr;
    }

    return S_OK;
}

_Use_decl_annotations_
HRESULT PluginManager::RTPlayerCreateTexture(_In_ UINT32 width, 
    _In_ UINT32 height,
    _COM_Outptr_ void** ppvTexture)
{
    Log(Log_Level_Info, L"PluginManagerImpl::RTPlayerCreateTexture(%d, %d) -Tid:%d \n", width, height, GetCurrentThreadId());

    NULL_CHK(ppvTexture);
    NULL_CHK(s_spStreamingPlayer);

    return s_spStreamingPlayer.CreateStreamingTexture(width, height, ppvTexture);
}

_Use_decl_annotations_
HRESULT  PluginManager::RTPlayerStart()
{
    Log(Log_Level_Info, L"PluginManagerImpl::RTPlayerStart()\n");

    NULL_CHK(s_spStreamingPlayer);

    s_spStreamingPlayer.Play();
    return S_OK;
}

_Use_decl_annotations_
HRESULT  PluginManager::RTPlayerPause()
{
    Log(Log_Level_Info, L"PluginManagerImpl::RTPlayerPause()\n");

    NULL_CHK(s_spStreamingPlayer);

    s_spStreamingPlayer.Pause();
    return S_OK;
}

_Use_decl_annotations_
HRESULT PluginManager::RTPlayerStop()
{
    Log(Log_Level_Info, L"PluginManagerImpl::RTPlayerStop()\n");

    NULL_CHK(s_spStreamingPlayer);

    s_spStreamingPlayer.Stop();
    return S_OK;
}


// internal
_Use_decl_annotations_
void PluginManager::CompletePluginCallback(
    _In_ PluginCallback callback,
    _In_ void* pCallbackObject,
    _In_ ModuleHandle handle,
    _In_ HRESULT hr)
{
    LOG_RESULT(ExceptionBoundary([&]()
    {
        callback(handle, pCallbackObject, hr);
    }));
}

template <class T>
T PluginManager::GetModule(_In_ ModuleHandle handle)
{
    if (m_moduleManager != nullptr)
    {
        return m_moduleManager.GetModule(handle).as<T>();
    }

    return nullptr;
}

_Use_decl_annotations_
void PluginManager::StoreToken(
    ModuleHandle handle,
    winrt::event_token const& newToken)
{
    // get the list of tokens for connection
    auto iter = m_eventTokens.find(handle);
    if (iter == m_eventTokens.end())
    {
        // if none exist, create then list
        m_eventTokens.insert(std::move(std::make_pair(handle, std::vector<winrt::event_token>())));

        // it should be in the list now
        iter = m_eventTokens.find(handle);
    }

    // set eventList
    std::vector<winrt::event_token> &eventList = iter->second;

    // register callback and track token
    eventList.emplace_back(std::move(newToken));
}

_Use_decl_annotations_
winrt::event_token PluginManager::RemoveToken(
    ModuleHandle handle,
    INT64 tokenValue)
{
    winrt::event_token returnToken;

    // get the list of tokens for connection
    auto mapIt = m_eventTokens.find(handle);
    if (mapIt != m_eventTokens.end())
    {
        // iterate tokens to match value
        std::vector<winrt::event_token> &eventList = mapIt->second;
        auto vectorIt = std::find_if(
            eventList.begin(),
            eventList.end(),
            [&tv = tokenValue](const winrt::event_token& t) -> bool
        {
            return (tv == t.value);
        });

        if (vectorIt != eventList.end())
        {
            eventList.erase(vectorIt);

            returnToken = *vectorIt;
        }
    }

    return returnToken;
}

