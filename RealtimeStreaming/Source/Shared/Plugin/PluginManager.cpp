// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "PluginManager.h"
#include "IUnityGraphicsD3D11.h"

using namespace winrt;
using namespace RealtimeStreaming::Plugin::implementation;
using namespace RealtimeStreaming::Media::implementation;
using namespace RealtimeStreaming::Network::implementation;

static com_ptr<IRealtimeMediaPlayer> s_spStreamingPlayer;

_Use_decl_annotations_
PluginManager::PluginManager()
{
    ENSURE_HR(MakeAndInitialize<ModuleManager>(&m_moduleManager));

    com_ptr<IThreadPoolStatics> threadPoolStatics;
    check_hresult(Windows::Foundation::GetActivationFactory(
        Wrappers::HStringReference(RuntimeClass_Windows_System_Threading_ThreadPool).get(),
        &threadPoolStatics));

    m_unityInterfaces = nullptr;
    m_unityGraphics = nullptr;
    m_unityCallback = nullptr;

    m_dxManager = nullptr;

    threadPoolStatics.as(&m_threadPoolStatics);
}

PluginManager::~PluginManager()
{
    Uninitialize();
}

_Use_decl_annotations_
void PluginManager::Uninitialize()
{
    Log(Log_Level_Info, L"PluginManagerImpl::Uninitialize()\n");

    auto lock = _lock.Lock();

    if (nullptr != m_dxManager)
    {
        m_dxManager->Uninitialize();
        LOG_RESULT(m_dxManager.Reset());
        m_dxManager = nullptr;
    }

    m_streamingAssetsPath.clear();

    assert(0 == m_eventTokens.size());
    m_eventTokens.clear();

    if (nullptr != m_moduleManager)
    {
        LOG_RESULT(m_moduleManager->Uninitialize());
        LOG_RESULT(m_moduleManager.Reset());
        m_moduleManager = nullptr;
    }
}

_Use_decl_annotations_
static PluginManager PluginManager::Instance()
{
    if (s_instance == nullptr)
    {
        s_instance = PluginManager();
        s_threadId = GetCurrentThreadId();
    }

    return s_instance;
}

_Use_decl_annotations_
ModuleManager PluginManager::ModuleManager()
{
    NULL_CHK_HR(m_moduleManager, E_NOT_SET);

    return m_moduleManager;
}

_Use_decl_annotations_
DirectXManager PluginManager::DirectXManager()
{
    NULL_CHK_HR(m_dxManager, E_NOT_SET);

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

    auto lock = _lock.Lock();

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

    auto lock = _lock.Lock();

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

    auto lock = _lock.Lock();

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
            m_dxManager->Lost();
        }
        break;

    case kUnityGfxDeviceEventAfterReset:
        Log(Log_Level_Info, L"- Reset\n");
        if (nullptr != m_dxManager)
        {
            m_dxManager->Reset();
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

    NULL_CHK(listenerHandle);
    NULL_CHK(callback);

    com_ptr<ListenerImpl> listener;
    check_hresult(MakeAndInitialize<ListenerImpl>(&listener, port));

    com_ptr<IConnectionCreatedOperation> connectedOp;
    check_hresult(listener->ListenAsync(&connectedOp));

    ModuleHandle handle = MODULE_HANDLE_INVALID;
    com_ptr<IModule> module;
    check_hresult(listener.As(&module));
    check_hresult(m_moduleManager->AddModule(module.get(), &handle));

    *listenerHandle = handle;

    // wait until we get a conneciton from the listner
    com_ptr<PluginManager> spThis(this);
    return StartAsyncThen(
        connectedOp.get(),
        [this, spThis, callback, pCallbackObject](_In_ HRESULT hr, _In_ IConnectionCreatedOperation *result, _In_ AsyncStatus asyncStatus) -> HRESULT
    {
        Log(Log_Level_Info, L"Manager::StartListener() - ListenAsync()\n");

        ModuleHandle handle = MODULE_HANDLE_INVALID;

        com_ptr<IConnection> connection;
        com_ptr<IModule> module;

        IFC(hr);

        IFC(result->GetResults(&connection));

        // QI for module
        IFC(connection.As(&module));

    done:
        {
            auto lock = _lock.Lock();

            if (nullptr == m_moduleManager)
            {
                return S_OK;
            }

            if (SUCCEEDED(hr))
            {
                hr = m_moduleManager->AddModule(module.get(), &handle);
                LOG_RESULT(hr);
            }
        }

        CompletePluginCallback(callback, pCallbackObject, handle, hr);

        return hr;
    });
}

_Use_decl_annotations_
HRESULT PluginManager::ListenerStopAndClose(
    ModuleHandle handle)
{
    Log(Log_Level_Info, L"PluginManagerImpl::StopListener()\n");

    auto lock = _lock.Lock();

    if (nullptr == m_moduleManager)
    {
        return S_OK;
    }

    // release the handle
    com_ptr<IModule> module;
    return m_moduleManager->ReleaseModule(handle);
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
    std::wstring wsUri = address;

    // Get Factories
    com_ptr<ABI::Windows::Foundation::IUriRuntimeClassFactory> uriFactory;
    check_hresult(Windows::Foundation::GetActivationFactory(
        Wrappers::HStringReference(RuntimeClass_Windows_Foundation_Uri).get(),
        uriFactory.GetAddressOf()));

    com_ptr<ABI::Windows::Networking::IHostNameFactory> hostNameFactory;
    check_hresult(Windows::Foundation::GetActivationFactory(
        Wrappers::HStringReference(RuntimeClass_Windows_Networking_HostName).get(),
        &hostNameFactory));

    // convert to Uri
    Wrappers::HString uriHString;
    check_hresult(WindowsCreateString(wsUri.c_str(), wsUri.size(), uriHString.GetAddressOf()));

    com_ptr<ABI::Windows::Foundation::IUriRuntimeClass> uri;
    check_hresult(uriFactory->CreateUri(uriHString.get(), &uri));

    // generate hostname
    Wrappers::HString uriHostname;
    check_hresult(uri->get_Host(uriHostname.GetAddressOf()));

    INT32 uriPort = 0;
    check_hresult(uri->get_Port(&uriPort));

    com_ptr<ABI::Windows::Networking::IHostName> hostName;
    check_hresult(hostNameFactory->CreateHostName(uriHostname.get(), &hostName));

    com_ptr<ConnectorImpl> connector;
    check_hresult(MakeAndInitialize<ConnectorImpl>(&connector, hostName.get(), uriPort));

    com_ptr<IConnectionCreatedOperation> connectedOp;
    check_hresult(connector->ConnectAsync(&connectedOp));

    ModuleHandle handle = MODULE_HANDLE_INVALID;
    com_ptr<IModule> module;
    check_hresult(connector.As(&module));
    check_hresult(m_moduleManager->AddModule(module.get(), &handle));

    *connectorHandle = handle;

    // wait until we get a conneciton from the listner
    com_ptr<PluginManager> spThis(this);
    return StartAsyncThen(
        connectedOp.get(),
        [this, spThis, callback, pCallbackObject](_In_ HRESULT hr,
            _In_ IConnectionCreatedOperation* pResult, 
            _In_ AsyncStatus asyncStatus) -> HRESULT
    {
        //Log(Log_Level_Info, L"PluginManagerImpl::OpenConnection() - ConnectAsync()\n");
        Log(Log_Level_Info, L"PluginManagerImpl::ConnectorCreateAndStart() [ConnectAsync()] -Tid:%d \n", GetCurrentThreadId());

        ModuleHandle handle = MODULE_HANDLE_INVALID;

        com_ptr<IConnection> connection;
        com_ptr<IModule> module;

        IFC(hr);

        IFC(pResult->GetResults(&connection));

        IFC(connection.As(&module));

    done:
        {
            auto lock = _lock.Lock();

            if (nullptr == m_moduleManager)
            {
                return S_OK;
            }

            if (SUCCEEDED(hr))
            {
                hr = m_moduleManager->AddModule(module.get(), &handle);
                LOG_RESULT(hr);
            }
        }

        CompletePluginCallback(callback, pCallbackObject, handle, hr);

        return hr;
    });
}

_Use_decl_annotations_
HRESULT PluginManager::ConnectorStopAndClose(
    ModuleHandle handle)
{
    Log(Log_Level_Info, L"PluginManagerImpl::CloseConnector()\n");

    auto lock = _lock.Lock();

    if (nullptr == m_moduleManager)
    {
        return S_OK;
    }

    // release the handle
    return m_moduleManager->ReleaseModule(handle);
}

_Use_decl_annotations_
HRESULT PluginManager::ConnectionAddDisconnected(
    ModuleHandle handle,
    PluginCallback callback,
    void* pCallbackObject,
    INT64* tokenValue)
{
    //Log(Log_Level_Info, L"PluginManagerImpl::ConnectionAddDisconnected\n");
    Log(Log_Level_Info, L"PluginManagerImpl::ConnectionAddDisconnected() -Tid:%d \n", GetCurrentThreadId());

    NULL_CHK(callback);
    NULL_CHK(tokenValue);

    auto lock = _lock.Lock();

    auto disconnectedCallback = Callback<IDisconnectedEventHandler>(
        [this, handle, callback, pCallbackObject](_In_ IConnection *sender) -> HRESULT
    {
        NULL_CHK(sender);

        callback(handle, pCallbackObject, S_OK); //L"");

        return S_OK;
    });

    // get connection
    com_ptr<IConnection> spConnection;
    check_hresult(GetConnection(handle, &spConnection));

    // register for callback
    EventRegistrationToken newToken;
    check_hresult(spConnection->add_Disconnected(disconnectedCallback.get(), &newToken));

    // set return
    *tokenValue = newToken.value;

    // track the token
    StoreToken(handle, newToken);

    return S_OK;
}

_Use_decl_annotations_
HRESULT PluginManager::ConnectionRemoveDisconnected(
    ModuleHandle handle,
    INT64 tokenValue)
{
    Log(Log_Level_Info, L"PluginManagerImpl::ConnectionRemoveReceived()\n");

    auto lock = _lock.Lock();

    // get connection
    com_ptr<IConnection> spConnection;
    check_hresult(GetConnection(handle, &spConnection));

    // get the token stored in the event list
    EventRegistrationToken removeToken;
    RemoveToken(handle, tokenValue, &removeToken);

    // unsubscribe from the event
    return spConnection->remove_Disconnected(removeToken);
}

_Use_decl_annotations_
HRESULT PluginManager::ConnectionAddReceived(
    ModuleHandle handle,
    DataReceivedHandler callback,
    void* pCallbackObject,
    INT64* tokenValue)
{
    //Log(Log_Level_Info, L"PluginManagerImpl::ConnectionAddReceived\n");
    Log(Log_Level_Info, L"PluginManagerImpl::ConnectionAddReceived() -Tid:%d \n", GetCurrentThreadId());

    NULL_CHK(callback);
    NULL_CHK(tokenValue);

    auto lock = _lock.Lock();

    // wrap callback
    auto bundleReceivedCallback = Callback<IBundleReceivedEventHandler>(
        [this, handle, callback, pCallbackObject](
            _In_ IConnection *sender,
            _In_ IBundleReceivedArgs *args) -> HRESULT
    {
        Log(Log_Level_Info, L"PluginManagerImpl::bundleReceivedCallback() -Tid:%d \n", GetCurrentThreadId());

        HRESULT hr = S_OK;

        PayloadType payloadType;
        IFC(args->get_PayloadType(&payloadType));

        switch (payloadType)
        {
        case PayloadType_State_Scene:
        case PayloadType_State_Input:
        {
            com_ptr<IDataBundle> dataBundle;
            IFC(args->get_DataBundle(&dataBundle));

            DataBundleImpl* rawDataBundle = static_cast<DataBundleImpl*>(dataBundle.get());
            IFC(rawDataBundle == nullptr ? E_INVALIDARG : S_OK);

            DWORD cbTotalLen = 0;
            hr = dataBundle->get_TotalSize(&cbTotalLen);
            if (SUCCEEDED(hr))
            {
                // TODO: need better locking mechanism? cicular lock
                // PluginManager -> Add_ConnectionReceived hold locks but then connection class is locked against this finishing
                //auto lock = _lock.Lock();

                // Copy the data structure
                DWORD copiedBytes = 0;
                if (cbTotalLen > _bundleData.size())
                {
                    _bundleData.resize(cbTotalLen);
                }

                hr = rawDataBundle->CopyTo(0, cbTotalLen, &_bundleData[0], &copiedBytes);
                if (SUCCEEDED(hr))
                {
                    callback(handle, pCallbackObject, (UINT16)payloadType, copiedBytes, _bundleData.data());
                }
                IFC(hr);
            }
        }
        break;

        default:
            //auto lock = _lock.Lock();
            callback(handle, pCallbackObject, static_cast<UINT16>(payloadType), 0, nullptr);
            break;
        };

    done:
        return S_OK;
    });

    // get connection
    com_ptr<IConnection> spConnection;
    check_hresult(GetConnection(handle, &spConnection));

    // register for callback
    EventRegistrationToken newToken;
    check_hresult(spConnection->add_Received(bundleReceivedCallback.get(), &newToken));

    // set return
    *tokenValue = newToken.value;

    // track the token
    StoreToken(handle, newToken);

    return S_OK;
}

_Use_decl_annotations_
HRESULT PluginManager::ConnectionRemoveReceived(
    ModuleHandle handle,
    INT64 tokenValue)
{
    Log(Log_Level_Info, L"PluginManagerImpl::ConnectionRemoveReceived()\n");

    auto lock = _lock.Lock();

    // get connection
    com_ptr<IConnection> spConnection;
    check_hresult(GetConnection(handle, &spConnection));

    // get the token stored in the event list
    EventRegistrationToken removeToken;
    RemoveToken(handle, tokenValue, &removeToken);

    // unsubscribe from the event
    return spConnection->remove_Received(removeToken);
}

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

    if (type != PayloadType_State_Scene
        &&
        type != PayloadType_State_Input)
    {
        return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
    }

    auto lock = _lock.Lock();

    // get connection
    com_ptr<IConnection> spConnection;
    check_hresult(GetConnection(handle, &spConnection));

    const DWORD c_cbPayloadSize = static_cast<DWORD>(bufferSize);
    const DWORD c_cbBufferSize = sizeof(PayloadHeader) + c_cbPayloadSize;

    // Create send buffer
    com_ptr<IDataBuffer> spDataBuffer;
    check_hresult(MakeAndInitialize<DataBufferImpl>(&spDataBuffer, c_cbBufferSize));

    com_ptr<IBuffer> spBuffer;
    check_hresult(spDataBuffer.As(&spBuffer));

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
    check_hresult(spDataBuffer->put_CurrentLength(c_cbBufferSize));

    com_ptr<IDataBundle> spBundle;
    check_hresult(MakeAndInitialize<DataBundleImpl>(&spBundle));

    check_hresult(spBundle->AddBuffer(spDataBuffer.get()));

    com_ptr<IAsyncAction> spSendAction;
    check_hresult(spConnection->SendBundleAsync(spBundle.get(), &spSendAction));

    com_ptr<PluginManager> spThis(this);
    return StartAsyncThen(
        spSendAction.get(),
        [this, spThis](_In_ HRESULT hr, _In_ IAsyncAction* pResult, _In_ AsyncStatus asyncStatus) -> HRESULT
    {
        LOG_RESULT(hr);

        return S_OK;
    });
}

_Use_decl_annotations_
HRESULT PluginManager::ConnectionClose(
    ModuleHandle handle)
{
    Log(Log_Level_Info, L"PluginManagerImpl::ConnectionClose()\n");

    auto lock = _lock.Lock();

    // get connection
    com_ptr<IConnection> spConnection;
    check_hresult(GetConnection(handle, &spConnection));

    // find any tokens register to this handle
    auto iter = m_eventTokens.find(handle);
    if (iter != m_eventTokens.end())
    {
        for each (auto token in (*iter).second)
        {
            spConnection->remove_Disconnected(token);
            spConnection->remove_Received(token);
        }
        (*iter).second.clear();

        m_eventTokens.erase(iter);
    }

    // unregister from the connection
    return m_moduleManager->ReleaseModule(handle);
}


_Use_decl_annotations_
HRESULT PluginManager::RTServerCreate(
    ModuleHandle connectionHandle,
    ModuleHandle* serverHandle)
{
    NULL_CHK(serverHandle);

    Log(Log_Level_Info, L"PluginManagerImpl::RTServerCreate()\n");

    com_ptr<IConnection> spConnection;
    check_hresult(GetConnection(connectionHandle, &spConnection));

    // Default encoding activation
    com_ptr<IMediaEncodingProfileStatics3> spEncodingProfileStatics;
    check_hresult(Windows::Foundation::GetActivationFactory(
        Wrappers::HStringReference(RuntimeClass_Windows_Media_MediaProperties_MediaEncodingProfile).get(),
        &spEncodingProfileStatics));

    com_ptr<IMediaEncodingProfile> spMediaEncodingProfile;
    check_hresult(spEncodingProfileStatics->CreateHevc(
        ABI::Windows::Media::MediaProperties::VideoEncodingQuality_HD720p,
        &spMediaEncodingProfile));

    com_ptr<RealtimeServer> spRTServer;
    check_hresult(MakeAndInitialize<RealtimeServer>(&spRTServer,
        spConnection.get(),
        MFVideoFormat_RGB32,
        spMediaEncodingProfile.get()));

    auto lock = _lock.Lock();

    ModuleHandle handle = MODULE_HANDLE_INVALID;
    com_ptr<IModule> module;

    if (nullptr == m_moduleManager)
    {
        check_hresult(E_NOT_SET);
    }

    check_hresult(spRTServer.As(&module));

    check_hresult(m_moduleManager->AddModule(module.get(), &handle));

    *serverHandle = handle;

    return S_OK;
}

_Use_decl_annotations_
HRESULT PluginManager::RTServerShutdown(
    ModuleHandle serverHandle)
{
    Log(Log_Level_Info, L"PluginManagerImpl::RTServerShutdown()\n");

    auto lock = _lock.Lock();

    // get capture engine
    com_ptr<IRealtimeServer> spRTServer;
    check_hresult(GetRealtimeServer(serverHandle, &spRTServer));

    check_hresult(spRTServer->Shutdown());

    // release the handle
    return m_moduleManager->ReleaseModule(serverHandle);
}

_Use_decl_annotations_
HRESULT PluginManager::RTServerWriteFrame(
    ModuleHandle serverHandle,
    BYTE* pBuffer,
    UINT32 bufferSize)
{
    NULL_CHK(pBuffer);

    Log(Log_Level_Info, L"PluginManagerImpl::RTServerWriteFrame()\n");

    auto lock = _lock.Lock();

    // get realtime server
    com_ptr<IRealtimeServer> spRTServer;
    check_hresult(GetRealtimeServer(serverHandle, &spRTServer));

	check_hresult(spRTServer->WriteFrame(bufferSize, pBuffer));

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

    auto lock = _lock.Lock();

    // get connection
    com_ptr<IConnection> spConnection;
    check_hresult(GetConnection(handle, &spConnection));

    UnityGfxRenderer unityGraphics = m_unityGraphics->GetRenderer();

    com_ptr<IRealtimeMediaPlayer> spPlayer;
    check_hresult(RealtimeMediaPlayer::Create(unityGraphics,
        m_unityInterfaces,
        spConnection.get(),
        //fnCallback,
        &spPlayer));

    // Copy out the streaming player
    check_hresult(spPlayer.CopyTo(&s_spStreamingPlayer));

    com_ptr<IAsyncAction> spInitAction;
    check_hresult(s_spStreamingPlayer.As(&spInitAction));

    com_ptr<PluginManager> spThis(this);
    return StartAsyncThen(
        spInitAction.get(),
        [this, spThis, spPlayer, callback, pCallbackObject](_In_ HRESULT hr, 
            _In_ IAsyncAction* asyncAction, 
            _In_ AsyncStatus asyncStatus)
    {
        Log(Log_Level_Info, L"PluginManagerImpl::RTPlayerCreate() [InitAsync()] -Tid:%d \n", GetCurrentThreadId());
        auto lock = _lock.Lock();

        UINT32 width, height;
        IFC(spPlayer->GetCurrentResolution(&width, &height));

        Log(Log_Level_Info, L"PluginManagerImpl::RTPlayerCreate() w:%d - h:%d \n", width, height);
    done:
        callback(pCallbackObject,
            hr,
            width,
            height);

        return hr;
    });
}

_Use_decl_annotations_
HRESULT PluginManager::RTPlayerRelease()
{
    Log(Log_Level_Info, L"PluginManagerImpl::RTPlayerRelease() -Tid:%d \n", GetCurrentThreadId());

    if (s_spStreamingPlayer != nullptr)
    {
        s_spStreamingPlayer.Reset();
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

    return s_spStreamingPlayer->CreateStreamingTexture(width, height, ppvTexture);
}

_Use_decl_annotations_
HRESULT  PluginManager::RTPlayerStart()
{
    Log(Log_Level_Info, L"PluginManagerImpl::RTPlayerStart()\n");

    NULL_CHK(s_spStreamingPlayer);

    return s_spStreamingPlayer->Play();
}

_Use_decl_annotations_
HRESULT  PluginManager::RTPlayerPause()
{
    Log(Log_Level_Info, L"PluginManagerImpl::RTPlayerPause()\n");

    NULL_CHK(s_spStreamingPlayer);

    return s_spStreamingPlayer->Pause();
}

_Use_decl_annotations_
HRESULT PluginManager::RTPlayerStop()
{
    Log(Log_Level_Info, L"PluginManagerImpl::RTPlayerStop()\n");

    NULL_CHK(s_spStreamingPlayer);

    return s_spStreamingPlayer->Stop();
}


// internal
_Use_decl_annotations_
void PluginManager::CompletePluginCallback(
    _In_ PluginCallback callback,
    _In_ void* pCallbackObject,
    _In_ ModuleHandle handle,
    _In_ HRESULT hr)
{
    LOG_RESULT(ExceptionBoundary([&]() -> HRESULT
    {
        if (FAILED(hr))
        {
            callback(handle, pCallbackObject, hr);//, ErrorMessage(hr));
        }
        else
        {
            callback(handle, pCallbackObject, S_OK);// , L"");
        }

        return S_OK;
    }));
}

// TODO: Change this into template
_Use_decl_annotations_
Connection PluginManager::GetConnection(
    _In_ ModuleHandle handle)
{
    if (m_moduleManager != nullptr)
    {

        return m_moduleManager.GetModule(handle);
    }

    return nullptr;
}

_Use_decl_annotations_
RealtimeServer PluginManager::GetRealtimeServer(
    _In_ ModuleHandle handle)
{
    if (m_moduleManager != nullptr)
    {

        return m_moduleManager.GetModule(handle);
    }

    return nullptr;
}

_Use_decl_annotations_
void PluginManager::StoreToken(
    ModuleHandle handle,
    EventRegistrationToken &newToken)
{
    // get the list of tokens for connection
    auto iter = m_eventTokens.find(handle);
    if (iter == m_eventTokens.end())
    {
        // if none exist, create then list
        m_eventTokens.insert(std::move(std::make_pair(handle, std::vector<EventRegistrationToken>())));

        // it should be in the list now
        iter = m_eventTokens.find(handle);
    }

    // set eventList
    std::vector<EventRegistrationToken> &eventList = iter->second;

    // register callback and track token
    eventList.emplace_back(std::move(newToken));
}

_Use_decl_annotations_
void PluginManager::RemoveToken(
    ModuleHandle handle,
    INT64 tokenValue,
    EventRegistrationToken* token)
{
    // get the list of tokens for connection
    auto mapIt = m_eventTokens.find(handle);
    if (mapIt != m_eventTokens.end())
    {
        // iterate tokens to match value
        std::vector<EventRegistrationToken> &eventList = mapIt->second;
        auto vectorIt = std::find_if(
            eventList.begin(),
            eventList.end(),
            [&tv = tokenValue](const EventRegistrationToken& t) -> bool
        {
            return (tv == t.value);
        });

        if (vectorIt != eventList.end())
        {
            *token = (*vectorIt);

            eventList.erase(vectorIt);
        }
    }
}

