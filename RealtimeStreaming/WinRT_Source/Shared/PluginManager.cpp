// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "PluginManager.h"
#include "ModuleManager.h"

#include "Listener.h"
#include "Connector.h"
#include "DataBundle.h"

#include "RealtimeServer.h"
#include "RealtimeMediaPlayer.h"

#include "winrt/RealtimeStreaming.Common.h"

using namespace winrt;
using namespace RealtimeStreaming::Plugin::implementation;
using namespace RealtimeStreaming::Common;
using namespace RealtimeStreaming::Network;
using namespace RealtimeStreaming::Media;

using namespace Windows::Foundation;
using namespace Windows::Storage::Streams;
using namespace Windows::Networking;
using namespace Windows::Media::MediaProperties;

/* Static Variables */
static INSTANCE_HANDLE s_lastPluginHandleIndex = INSTANCE_HANDLE_INVALID;
static std::shared_ptr<IUnityDeviceResource> s_deviceResource;
static UnityGfxRenderer s_deviceType = kUnityGfxRendererNull;
static IUnityInterfaces* s_unityInterfaces = nullptr;
static IUnityGraphics* s_unityGraphics = nullptr;
static DWORD s_threadId;

static winrt::RealtimeStreaming::Media::RealtimeMediaPlayer s_spStreamingPlayer{ nullptr };

/*
_Use_decl_annotations_
PluginManager::PluginManager()
{
    m_unityInterfaces = nullptr;
    m_unityGraphics = nullptr;
    m_unityCallback = nullptr;
}
*/

PluginManager::~PluginManager()
{
    Uninitialize();
}

void PluginManager::Uninitialize()
{
    Log(Log_Level_Info, L"PluginManagerImpl::Uninitialize()\n");

    slim_lock_guard guard(m_lock);

    m_streamingAssetsPath.clear();

    assert(0 == m_eventTokens.size());
    m_eventTokens.clear();
    m_moduleManager = nullptr;
}

/* static */
_Use_decl_annotations_
RealtimeStreaming::Plugin::ModuleManager PluginManager::ModuleManager()
{
    if (m_moduleManager == nullptr)
    {
        m_moduleManager = winrt::make<Plugin::implementation::ModuleManager>();
    }

    return m_moduleManager;
}

_Use_decl_annotations_
bool PluginManager::IsOnThread()
{
    return (s_threadId == GetCurrentThreadId());
}


/* static */
_Use_decl_annotations_
void PluginManager::Load(IUnityInterfaces* unityInterfaces)
{
    Log(Log_Level_Info, L"PluginManagerImpl::Load()\n");

    //slim_lock_guard guard(m_lock);

    NULL_THROW(unityInterfaces);

    s_lastPluginHandleIndex = INSTANCE_HANDLE_START;
    //s_instances.clear();

    s_unityInterfaces = unityInterfaces;
    s_unityGraphics = s_unityInterfaces->Get<IUnityGraphics>();
    s_unityGraphics->RegisterDeviceEventCallback(OnDeviceEvent);

    // Run OnGraphicsDeviceEvent(initialize) manually on plugin load
    OnDeviceEvent(kUnityGfxDeviceEventInitialize);
}

/* static */
_Use_decl_annotations_
void PluginManager::UnLoad()
{
    Log(Log_Level_Info, L"PluginManagerImpl::UnLoad()\n");

    //slim_lock_guard guard(m_lock);

    if (nullptr == s_unityGraphics)
    {
        return;
    }

    s_unityGraphics->UnregisterDeviceEventCallback(OnDeviceEvent);

    s_unityInterfaces = nullptr;
    s_unityGraphics = nullptr;
    //s_instance = nullptr; //unload pluginmanager instance
}

/* static */
_Use_decl_annotations_
void PluginManager::OnDeviceEvent(UnityGfxDeviceEventType eventType)
{
    Log(Log_Level_Info, L"PluginManagerImpl::OnDeviceEvent ");

    //slim_lock_guard guard(m_lock);

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

_Use_decl_annotations_
HRESULT PluginManager::ListenerCreateAndStart(
    UINT16 port,
    RealtimeStreaming::Plugin::ModuleHandle *listenerHandle,
    RealtimeStreaming::Plugin::PluginCallback callback,
    void* pCallbackObject)
{
    Log(Log_Level_Info, L"PluginManager::StartListener() - Tid:%d \n", GetCurrentThreadId());
    
    // Init with default
    *listenerHandle = MODULE_HANDLE_INVALID;

    NULL_CHK(listenerHandle);
    NULL_CHK(callback);

    // Create a new listener
    Network::Listener listener = winrt::make<Network::implementation::Listener>(port);

    // Save handle of newly created listener to output
    *listenerHandle = ModuleManager().AddModule(listener);

    auto listenAsync = listener.ListenAsync(); // TODO: need to save reference outside of function call?
    listenAsync.Completed([=](auto const asyncOp, AsyncStatus const status)
    {
        Log(Log_Level_Info, L"PluginManager::StartListener() - ListenAsync() Completed - Tid:%d \n", GetCurrentThreadId());

        //slim_lock_guard guard(m_lock);

        auto connection = asyncOp.GetResults();

        if (m_moduleManager == nullptr)
        {
            // TODO: look at better HResult to pass?
            CompletePluginCallback(callback, pCallbackObject, MODULE_HANDLE_INVALID, E_INVALIDARG);
        }

        ModuleHandle handle = ModuleManager().AddModule(connection);
        CompletePluginCallback(callback, pCallbackObject, handle, S_OK);
    });

    return S_OK;
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
    ModuleManager().ReleaseModule(handle);

    return S_OK;
}

// connector
_Use_decl_annotations_
HRESULT PluginManager::ConnectorCreateAndStart(
    LPCWSTR address,
    RealtimeStreaming::Plugin::ModuleHandle *connectorHandle,
    RealtimeStreaming::Plugin::PluginCallback callback,
    void* pCallbackObject)
{
    Log(Log_Level_Info, L"PluginManagerImpl::ConnectorCreateAndStart() -Tid:%d \n", GetCurrentThreadId());

    NULL_CHK(connectorHandle);
    NULL_CHK(callback);

    *connectorHandle = MODULE_HANDLE_INVALID;
    
    std::wstring wsUri = address;
    Uri uri{ wsUri };
    HostName hostName = HostName(uri.Host());

    Connector connector = winrt::make<Network::implementation::Connector>(hostName, uri.Port());

    *connectorHandle = ModuleManager().AddModule(connector);

    auto connectAsync = connector.ConnectAsync(); // TODO: need to save reference outside of function call?
    connectAsync.Completed([=](auto const asyncOp, AsyncStatus const status)
    {
        Log(Log_Level_Info, L"PluginManagerImpl::ConnectorCreateAndStart() - ConnectAsync() Completed -Tid:%d \n", GetCurrentThreadId());

        //slim_lock_guard guard(m_lock);

        auto connection = asyncOp.GetResults();

        if (m_moduleManager == nullptr)
        {
            // TODO: look at better HResult to pass?
            CompletePluginCallback(callback, pCallbackObject, MODULE_HANDLE_INVALID, E_INVALIDARG);
        }

        ModuleHandle handle = ModuleManager().AddModule(connection);
        CompletePluginCallback(callback, pCallbackObject, handle, S_OK);
    });

    return S_OK;
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
    ModuleManager().ReleaseModule(handle);
}

_Use_decl_annotations_
HRESULT PluginManager::ConnectionAddDisconnected(
    ModuleHandle handle,
    RealtimeStreaming::Plugin::PluginCallback callback,
    void* pCallbackObject,
    INT64* tokenValue)
{
    Log(Log_Level_Info, L"PluginManagerImpl::ConnectionAddDisconnected() -Tid:%d \n", GetCurrentThreadId());

    NULL_CHK(callback);
    NULL_CHK(tokenValue);

    slim_lock_guard guard(m_lock);

    Network::Connection connection = GetModule<Network::Connection>(handle);
    event_token disconnectedToken = connection.Disconnected([this, handle, callback, pCallbackObject]()
    {
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
    RealtimeStreaming::Plugin::DataReceivedHandler callback,
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
            _In_ IInspectable const& /* sender */,
            _In_ Network::DataBundleArgs const& args)
    {
        Log(Log_Level_Info, L"PluginManagerImpl::bundleReceivedCallback() -Tid:%d \n", GetCurrentThreadId());

        HRESULT hr = S_OK;

        PayloadType payloadType = args.PayloadType();

        switch (payloadType)
        {
        case PayloadType::State_Scene:
        case PayloadType::State_Input:
        {
            auto dataBundle = args.Bundle().as<Network::implementation::DataBundle>();

            NULL_CHK(dataBundle);

            DWORD cbTotalLen = dataBundle->TotalSize();

            // TODO: need better locking mechanism? cicular lock
            // PluginManager -> Add_ConnectionReceived hold locks but then connection class is locked against this finishing
            //slim_lock_guard guard(m_lock);

            // Copy the data structure
            UINT32 copiedBytes = 0;
            if (cbTotalLen > _bundleData.size())
            {
                _bundleData.resize(cbTotalLen);
            }

            hr = dataBundle->CopyTo(0, cbTotalLen, &_bundleData[0], &copiedBytes);
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
    DataBuffer spDataBuffer = winrt::make<Network::implementation::DataBuffer>(c_cbBufferSize);
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
    ModuleManager().ReleaseModule(handle);
    return S_OK;
}

_Use_decl_annotations_
HRESULT PluginManager::RTServerCreate(
    ModuleHandle connectionHandle,
    ModuleHandle* serverHandle)
{
    NULL_CHK(serverHandle);

    Log(Log_Level_Info, L"PluginManagerImpl::RTServerCreate() -Tid:%d \n", GetCurrentThreadId());

    Network::Connection connection = GetModule<Network::Connection>(connectionHandle);

    // Default encoding activation
    auto mediaProfile = MediaEncodingProfile::CreateHevc(VideoEncodingQuality::HD720p);

    Media::RealtimeServer rtServer = winrt::make<Media::implementation::RealtimeServer>(
        connection, 
        MFVideoFormat_RGB32, 
        mediaProfile);

    slim_lock_guard guard(m_lock);

    ModuleHandle handle = MODULE_HANDLE_INVALID;

    if (nullptr == m_moduleManager)
    {
        IFT(E_NOT_SET);
    }

    handle = ModuleManager().AddModule(rtServer);
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
    ModuleManager().ReleaseModule(serverHandle);

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
    RealtimeStreaming::Plugin::PlayerCreatedCallback callback,
    void* pCallbackObject)
{
    Log(Log_Level_Info, L"PluginManagerImpl::RTPlayerCreate() -Tid:%d \n", GetCurrentThreadId());

    NULL_CHK(callback);
    NULL_CHK(pCallbackObject);

    slim_lock_guard guard(m_lock);

    // get connection
    auto connection = GetModule<Connection>(handle);
    //UnityGfxRenderer unityGraphics = m_unityGraphics->GetRenderer();

    RealtimeMediaPlayer rtPlayer = winrt::make<Media::implementation::RealtimeMediaPlayer>();

    //*connectorHandle = m_moduleManager.AddModule(rtPlayer);

    auto rtPlayerImpl = rtPlayer.as<Media::implementation::RealtimeMediaPlayer>();

    rtPlayerImpl->Initialize(s_deviceResource);

    s_spStreamingPlayer = nullptr; // make sure we unseat if previously assigned
    s_spStreamingPlayer = rtPlayer;

    auto initAsync = rtPlayer.InitAsync(connection); // TODO: need to save reference outside of function call?
    initAsync.Completed([=](auto const asyncOp, AsyncStatus const status)
    {
        Log(Log_Level_Info, L"PluginManagerImpl::RTPlayerCreate() [InitAsync()] -Tid:%d \n", GetCurrentThreadId());

        //slim_lock_guard guard(m_lock);
        
        auto videoProps = asyncOp.GetResults();
        UINT32 width = videoProps.Width(), height = videoProps.Height();

        Log(Log_Level_Info, L"PluginManagerImpl::RTPlayerCreate() [InitAsync()] w:%d - h:%d \n", width, height);
        
        callback(pCallbackObject,
            S_OK,
            width,
            height);
    });

    return S_OK;
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

    auto rtPlayerImpl = s_spStreamingPlayer.as<RealtimeStreaming::Media::implementation::RealtimeMediaPlayer>();

    NULL_CHK(rtPlayerImpl);

    return rtPlayerImpl->CreateStreamingTexture(width, height, ppvTexture);
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
    _In_ RealtimeStreaming::Plugin::PluginCallback callback,
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

template <class T>
T PluginManager::GetModule(_In_ ModuleHandle handle)
{
    if (m_moduleManager != nullptr)
    {
        return ModuleManager().GetModule(handle).as<T>();
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
            returnToken = *vectorIt;

            eventList.erase(vectorIt);
        }
    }

    return returnToken;
}

