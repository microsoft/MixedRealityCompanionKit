// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "PluginManager.h"
#include "IUnityGraphicsD3D11.h"

static ComPtr<IStreamingMediaPlayer> s_spStreamingPlayer;

PluginManagerImpl::~PluginManagerImpl()
{
    Uninitialize();
}

_Use_decl_annotations_
HRESULT PluginManagerImpl::RuntimeClassInitialize()
{
    ENSURE_HR(MakeAndInitialize<ModuleManagerImpl>(&_moduleManager));

    ComPtr<IThreadPoolStatics> threadPoolStatics;
    IFR(Windows::Foundation::GetActivationFactory(
        Wrappers::HStringReference(RuntimeClass_Windows_System_Threading_ThreadPool).Get(),
        &threadPoolStatics));

    _unityInterfaces = nullptr;
    _unityGraphics = nullptr;
    _unityCallback = nullptr;
    _timeElasped = 0.0f;
    _deltaTime = 0.0f;

    _dxManager = nullptr;

    _updateQueue.clear();
    _renderQueue.clear();

    return threadPoolStatics.As(&_threadPoolStatics);
}

_Use_decl_annotations_
void PluginManagerImpl::Uninitialize()
{
    Log(Log_Level_Info, L"PluginManagerImpl::Uninitialize()\n");

    auto lock = _lock.Lock();

    if (nullptr != _dxManager)
    {
        _dxManager->Uninitialize();
        LOG_RESULT(_dxManager.Reset());
        _dxManager = nullptr;
    }

    _timeElasped = _deltaTime = 0.0f;

    _streamingAssetsPath.clear();

    _updateQueue.clear();
    _renderQueue.clear();

    assert(0 == _eventTokens.size());
    _eventTokens.clear();

    if (nullptr != _moduleManager)
    {
        LOG_RESULT(_moduleManager->Uninitialize());
        LOG_RESULT(_moduleManager.Reset());
        _moduleManager = nullptr;
    }
}


_Use_decl_annotations_
HRESULT PluginManagerImpl::get_ModuleManager(
    IModuleManager** ppModuleManager)
{
    NULL_CHK_HR(_moduleManager, E_NOT_SET);

    return _moduleManager.CopyTo(ppModuleManager);
}

_Use_decl_annotations_
HRESULT PluginManagerImpl::get_ThreadPool(
    IThreadPoolStatics** ppThreadPoolStatics)
{
    NULL_CHK_HR(_threadPoolStatics, E_NOT_SET);

    return _threadPoolStatics.CopyTo(ppThreadPoolStatics);
}

_Use_decl_annotations_
HRESULT PluginManagerImpl::get_DirectXManager(
    IDirectXManager** ppDXManager)
{
    NULL_CHK(ppDXManager);

    return _dxManager.CopyTo(ppDXManager);
}

_Use_decl_annotations_
void PluginManagerImpl::Load(IUnityInterfaces* unityInterfaces, IUnityGraphicsDeviceEventCallback callback)
{
    Log(Log_Level_Info, L"PluginManagerImpl::Load()\n");

    auto lock = _lock.Lock();

    // was already initialized, so need to reset
    if (nullptr != _dxManager)
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

    _unityInterfaces = unityInterfaces;

    _unityGraphics = unityGraphics;

    _unityCallback = callback;
    _unityGraphics->RegisterDeviceEventCallback(_unityCallback);

    // Run OnGraphicsDeviceEvent(initialize) manually on plugin load
    OnDeviceEvent(kUnityGfxDeviceEventInitialize);
}

_Use_decl_annotations_
void PluginManagerImpl::UnLoad()
{
    Log(Log_Level_Info, L"PluginManagerImpl::UnLoad()\n");

    auto lock = _lock.Lock();

    if (nullptr == _unityGraphics || nullptr == _unityCallback)
    {
        return;
    }

    Uninitialize();

    _unityGraphics->UnregisterDeviceEventCallback(_unityCallback);

    _unityCallback = nullptr;

    _unityInterfaces = nullptr;

    _unityGraphics = nullptr;
}

_Use_decl_annotations_
void PluginManagerImpl::OnDeviceEvent(UnityGfxDeviceEventType eventType)
{
    Log(Log_Level_Info, L"PluginManagerImpl::OnDeviceEvent ");

    HRESULT hr = S_OK;

    auto lock = _lock.Lock();

    UnityGfxRenderer currentDeviceType = UnityGfxRenderer::kUnityGfxRendererNull;

    if (nullptr == _unityGraphics)
    {
        return;
    }

    currentDeviceType = _unityGraphics->GetRenderer();
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
        if (nullptr != _dxManager)
        {
            _dxManager->Lost();
        }
        break;

    case kUnityGfxDeviceEventAfterReset:
        Log(Log_Level_Info, L"- Reset\n");
        if (nullptr != _dxManager)
        {
            _dxManager->Reset();
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
void PluginManagerImpl::SetTime(float t)
{
    // TODO: uncomment these
    //Log(Log_Level_Info, L"PluginManagerImpl::SetTime()\n");

    auto lock = _lock.Lock();

    if (_deltaTime == 0 && _timeElasped == 0)
    {
        // first timed called since last reset
        _deltaTime = 0.0f;
    }
    else
    {
        _deltaTime = t - _timeElasped;
    }

    _timeElasped = t;
}

_Use_decl_annotations_
void PluginManagerImpl::OnPlugInEvent(PluginEventType event)
{
    // TODO: uncomment these
    //Log(Log_Level_Info, L"PluginManagerImpl::OnRenderEvent()\n");

    // we have no control that the callback is still valid
    LOG_RESULT(
        ExceptionBoundary([this, event]()->HRESULT
    {
        auto lock = _lock.Lock();

        switch (event)
        {
        case PluginEventType_Update:
            if (!_updateQueue.empty())
            {
                UpdateAction action;
                while (_updateQueue.try_pop(action))
                {
                    action(_deltaTime, _timeElasped);
                }
            }
            break;

        case PluginEventType_Render:
            if (!_renderQueue.empty())
            {
                RenderAction action;
                while (_renderQueue.try_pop(action))
                {
                    action();
                }
            }
            break;

        case PluginEventType_Flush:
            _updateQueue.clear();
            _renderQueue.clear();
            break;
        }

        return S_OK;
    }));
}

_Use_decl_annotations_
HRESULT PluginManagerImpl::ListenerCreateAndStart(
    UINT16 port,
    ModuleHandle *listenerHandle,
    PluginCallback callback,
    void* pCallbackObject)
{
    Log(Log_Level_Info, L"PluginManagerImpl::StartListener()\n");

    NULL_CHK(listenerHandle);
    NULL_CHK(callback);

    ComPtr<ListenerImpl> listener;
    IFR(MakeAndInitialize<ListenerImpl>(&listener, port));

    ComPtr<IConnectionCreatedOperation> connectedOp;
    IFR(listener->ListenAsync(&connectedOp));

    ModuleHandle handle = MODULE_HANDLE_INVALID;
    ComPtr<IModule> module;
    IFR(listener.As(&module));
    IFR(_moduleManager->AddModule(module.Get(), &handle));

    *listenerHandle = handle;

    // wait until we get a conneciton from the listner
    return StartAsyncThen(
        connectedOp.Get(),
        [this, callback, pCallbackObject](_In_ HRESULT hr, _In_ IConnectionCreatedOperation *result, _In_ AsyncStatus asyncStatus) -> HRESULT
    {
        Log(Log_Level_Info, L"Manager::StartListener() - ListenAsync()\n");

        ModuleHandle handle = MODULE_HANDLE_INVALID;

        ComPtr<IConnection> connection;
        ComPtr<IModule> module;

        IFC(hr);

        IFC(result->GetResults(&connection));

        // QI for module
        IFC(connection.As(&module));

    done:
        auto lock = _lock.Lock();

        if (nullptr == _moduleManager)
        {
            return S_OK;
        }

        if (SUCCEEDED(hr))
        {
            hr = _moduleManager->AddModule(module.Get(), &handle);
            LOG_RESULT(hr);
        }

        CompletePluginCallback(callback, pCallbackObject, handle, hr);

        return hr;
    });
}

_Use_decl_annotations_
HRESULT PluginManagerImpl::ListenerStopAndClose(
    ModuleHandle handle)
{
    Log(Log_Level_Info, L"PluginManagerImpl::StopListener()\n");

    auto lock = _lock.Lock();

    if (nullptr == _moduleManager)
    {
        return S_OK;
    }

    // release the handle
    ComPtr<IModule> module;
    return _moduleManager->ReleaseModule(handle);
}

// connector
_Use_decl_annotations_
HRESULT PluginManagerImpl::ConnectorCreateAndStart(
    LPCWSTR address,
    ModuleHandle *connectorHandle,
    PluginCallback callback,
    void* pCallbackObject)
{
    Log(Log_Level_Info, L"PluginManagerImpl::OpenConnection()\n");

    NULL_CHK(connectorHandle);
    NULL_CHK(callback);
    std::wstring wsUri = address;

    // Get Factories
    ComPtr<ABI::Windows::Foundation::IUriRuntimeClassFactory> uriFactory;
    IFR(Windows::Foundation::GetActivationFactory(
        Wrappers::HStringReference(RuntimeClass_Windows_Foundation_Uri).Get(),
        uriFactory.GetAddressOf()));

    ComPtr<ABI::Windows::Networking::IHostNameFactory> hostNameFactory;
    IFR(Windows::Foundation::GetActivationFactory(
        Wrappers::HStringReference(RuntimeClass_Windows_Networking_HostName).Get(),
        &hostNameFactory));

    // convert to Uri
    Microsoft::WRL::Wrappers::HString uriHString;
    IFR(WindowsCreateString(wsUri.c_str(), wsUri.size(), uriHString.GetAddressOf()));

    ComPtr<ABI::Windows::Foundation::IUriRuntimeClass> uri;
    IFR(uriFactory->CreateUri(uriHString.Get(), &uri));

    // generate hostname
    Microsoft::WRL::Wrappers::HString uriHostname;
    IFR(uri->get_Host(uriHostname.GetAddressOf()));

    INT32 uriPort = 0;
    IFR(uri->get_Port(&uriPort));

    ComPtr<ABI::Windows::Networking::IHostName> hostName;
    IFR(hostNameFactory->CreateHostName(uriHostname.Get(), &hostName));

    ComPtr<ConnectorImpl> connector;
    IFR(MakeAndInitialize<ConnectorImpl>(&connector, hostName.Get(), uriPort));

    ComPtr<IConnectionCreatedOperation> connectedOp;
    IFR(connector->ConnectAsync(&connectedOp));

    ModuleHandle handle = MODULE_HANDLE_INVALID;
    ComPtr<IModule> module;
    IFR(connector.As(&module));
    IFR(_moduleManager->AddModule(module.Get(), &handle));

    *connectorHandle = handle;

    // wait until we get a conneciton from the listner
    return StartAsyncThen(
        connectedOp.Get(),
        [this, callback, pCallbackObject](_In_ HRESULT hr, _In_ IConnectionCreatedOperation* pResult, _In_ AsyncStatus asyncStatus) -> HRESULT
    {
        Log(Log_Level_Info, L"PluginManagerImpl::OpenConnection() - ConnectAsync()\n");

        ModuleHandle handle = MODULE_HANDLE_INVALID;

        ComPtr<IConnection> connection;
        ComPtr<IModule> module;

        IFC(hr);

        IFC(pResult->GetResults(&connection));

        IFC(connection.As(&module));

    done:
        auto lock = _lock.Lock();

        if (nullptr == _moduleManager)
        {
            return S_OK;
        }

        if (SUCCEEDED(hr))
        {
            hr = _moduleManager->AddModule(module.Get(), &handle);
            LOG_RESULT(hr);
        }

        CompletePluginCallback(callback, pCallbackObject, handle, hr);

        return hr;
    });
}

_Use_decl_annotations_
HRESULT PluginManagerImpl::ConnectorStopAndClose(
    ModuleHandle handle)
{
    Log(Log_Level_Info, L"PluginManagerImpl::CloseConnector()\n");

    auto lock = _lock.Lock();

    if (nullptr == _moduleManager)
    {
        return S_OK;
    }

    // release the handle
    return _moduleManager->ReleaseModule(handle);
}

_Use_decl_annotations_
HRESULT PluginManagerImpl::ConnectionAddDisconnected(
    ModuleHandle handle,
    PluginCallback callback,
    void* pCallbackObject,
    INT64* tokenValue)
{
    Log(Log_Level_Info, L"PluginManagerImpl::ConnectionAddReceived\n");

    NULL_CHK(callback);
    NULL_CHK(tokenValue);

    auto lock = _lock.Lock();

    auto disconnectedCallback = Callback<IDisconnectedEventHandler>(
        [this, handle, callback, pCallbackObject](_In_ IConnection *sender) -> HRESULT
    {
        NULL_CHK(sender);

        callback(handle, pCallbackObject, S_OK, L"");

        return S_OK;
    });

    // get connection
    ComPtr<IConnection> spConnection;
    IFR(GetConnection(handle, &spConnection));

    // register for callback
    EventRegistrationToken newToken;
    IFR(spConnection->add_Disconnected(disconnectedCallback.Get(), &newToken));

    // set return
    *tokenValue = newToken.value;

    // track the token
    StoreToken(handle, newToken);

    return S_OK;
}

_Use_decl_annotations_
HRESULT PluginManagerImpl::ConnectionRemoveDisconnected(
    ModuleHandle handle,
    INT64 tokenValue)
{
    Log(Log_Level_Info, L"PluginManagerImpl::ConnectionRemoveReceived()\n");

    auto lock = _lock.Lock();

    // get connection
    ComPtr<IConnection> spConnection;
    IFR(GetConnection(handle, &spConnection));

    // get the token stored in the event list
    EventRegistrationToken removeToken;
    RemoveToken(handle, tokenValue, &removeToken);

    // unsubscribe from the event
    return spConnection->remove_Disconnected(removeToken);
}

_Use_decl_annotations_
HRESULT PluginManagerImpl::ConnectionAddReceived(
    ModuleHandle handle,
    DataReceivedHandler callback,
    void* pCallbackObject,
    INT64* tokenValue)
{
    Log(Log_Level_Info, L"PluginManagerImpl::ConnectionAddReceived\n");

    NULL_CHK(callback);
    NULL_CHK(tokenValue);

    auto lock = _lock.Lock();

    // wrap callback
    auto bundleReceivedCallback = Callback<IBundleReceivedEventHandler>(
        [this, handle, callback, pCallbackObject](
            _In_ IConnection *sender,
            _In_ IBundleReceivedArgs *args) -> HRESULT
    {
        HRESULT hr = S_OK;

        PayloadType payloadType;
        IFC(args->get_PayloadType(&payloadType));

        switch (payloadType)
        {
        case PayloadType_State_Scene:
        case PayloadType_State_Input:
        {
            ComPtr<IDataBundle> dataBundle;
            IFC(args->get_DataBundle(&dataBundle));

            DataBundleImpl* rawDataBundle = static_cast<DataBundleImpl*>(dataBundle.Get());
            IFC(rawDataBundle == nullptr ? E_INVALIDARG : S_OK);

            DWORD cbTotalLen = 0;
            hr = dataBundle->get_TotalSize(&cbTotalLen);
            if (SUCCEEDED(hr))
            {
                auto lock = _lock.Lock();

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
            auto lock = _lock.Lock();
            callback(handle, pCallbackObject, static_cast<UINT16>(payloadType), 0, nullptr);
            break;
        };

    done:
        return S_OK;
    });

    // get connection
    ComPtr<IConnection> spConnection;
    IFR(GetConnection(handle, &spConnection));

    // register for callback
    EventRegistrationToken newToken;
    IFR(spConnection->add_Received(bundleReceivedCallback.Get(), &newToken));

    // set return
    *tokenValue = newToken.value;

    // track the token
    StoreToken(handle, newToken);

    return S_OK;
}

_Use_decl_annotations_
HRESULT PluginManagerImpl::ConnectionRemoveReceived(
    ModuleHandle handle,
    INT64 tokenValue)
{
    Log(Log_Level_Info, L"PluginManagerImpl::ConnectionRemoveReceived()\n");

    auto lock = _lock.Lock();

    // get connection
    ComPtr<IConnection> spConnection;
    IFR(GetConnection(handle, &spConnection));

    // get the token stored in the event list
    EventRegistrationToken removeToken;
    RemoveToken(handle, tokenValue, &removeToken);

    // unsubscribe from the event
    return spConnection->remove_Received(removeToken);
}

_Use_decl_annotations_
HRESULT PluginManagerImpl::ConnectionSendRawData(
    ModuleHandle handle,
    PayloadType payloadType,
    byte* pBuffer,
    UINT32 bufferSize)
{
    Log(Log_Level_Info, L"PluginManagerImpl::ConnectionAddReceived\n");

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
    ComPtr<IConnection> spConnection;
    IFR(GetConnection(handle, &spConnection));

    const DWORD c_cbPayloadSize = static_cast<DWORD>(bufferSize);
    const DWORD c_cbBufferSize = sizeof(PayloadHeader) + c_cbPayloadSize;

    // Create send buffer
    ComPtr<IDataBuffer> spDataBuffer;
    IFR(MakeAndInitialize<DataBufferImpl>(&spDataBuffer, c_cbBufferSize));

    ComPtr<IBuffer> spBuffer;
    IFR(spDataBuffer.As(&spBuffer));

    // Prepare operation header
    PayloadHeader* pHeader = GetDataType<PayloadHeader*>(spBuffer.Get());
    NULL_CHK_HR(pHeader, E_POINTER);
    pHeader->ePayloadType = type;
    pHeader->cbPayloadSize = c_cbPayloadSize;

    // Prepare location
    BYTE* pBlobStart = (reinterpret_cast<BYTE*>(pHeader) + sizeof(PayloadHeader));
    NULL_CHK_HR(pBlobStart, E_POINTER);
    memcpy_s(pBlobStart, c_cbPayloadSize, pBuffer, c_cbPayloadSize);

    // Set length of the packet
    IFR(spDataBuffer->put_CurrentLength(c_cbBufferSize));

    ComPtr<IDataBundle> spBundle;
    IFR(MakeAndInitialize<DataBundleImpl>(&spBundle));

    IFR(spBundle->AddBuffer(spDataBuffer.Get()));

    ComPtr<IAsyncAction> spSendAction;
    IFR(spConnection->SendBundleAsync(spBundle.Get(), &spSendAction));

    ComPtr<PluginManagerImpl> spThis(this);
    return StartAsyncThen(
        spSendAction.Get(),
        [this, spThis](_In_ HRESULT hr, _In_ IAsyncAction* pResult, _In_ AsyncStatus asyncStatus) -> HRESULT
    {
        LOG_RESULT(hr);

        return S_OK;
    });
}

_Use_decl_annotations_
HRESULT PluginManagerImpl::ConnectionClose(
    ModuleHandle handle)
{
    Log(Log_Level_Info, L"PluginManagerImpl::ConnectionClose()\n");

    auto lock = _lock.Lock();

    // get connection
    ComPtr<IConnection> spConnection;
    IFR(GetConnection(handle, &spConnection));

    // find any tokens register to this handle
    auto iter = _eventTokens.find(handle);
    if (iter != _eventTokens.end())
    {
        for each (auto token in (*iter).second)
        {
            spConnection->remove_Disconnected(token);
            spConnection->remove_Received(token);
        }
        (*iter).second.clear();

        _eventTokens.erase(iter);
    }

    // unregister from the connection
    return _moduleManager->ReleaseModule(handle);
}


_Use_decl_annotations_
HRESULT PluginManagerImpl::RTServerCreate(
    ModuleHandle connectionHandle,
    ModuleHandle* serverHandle)
{
    NULL_CHK(serverHandle);

    Log(Log_Level_Info, L"PluginManagerImpl::RTServerCreate()\n");

    ComPtr<IConnection> spConnection;
    IFR(GetConnection(connectionHandle, &spConnection));

    // Default encoding activation
    ComPtr<IMediaEncodingProfileStatics3> spEncodingProfileStatics;
    IFR(Windows::Foundation::GetActivationFactory(
        Wrappers::HStringReference(RuntimeClass_Windows_Media_MediaProperties_MediaEncodingProfile).Get(),
        &spEncodingProfileStatics));

    ComPtr<IMediaEncodingProfile> spMediaEncodingProfile;
    IFR(spEncodingProfileStatics->CreateHevc(
        ABI::Windows::Media::MediaProperties::VideoEncodingQuality_HD720p,
        &spMediaEncodingProfile));

    ComPtr<RealtimeServerImpl> spRTServer;
    IFR(MakeAndInitialize<RealtimeServerImpl>(&spRTServer,
        spConnection.Get(),
        MFVideoFormat_RGB32,
        spMediaEncodingProfile.Get()));

    auto lock = _lock.Lock();

    ModuleHandle handle = MODULE_HANDLE_INVALID;
    ComPtr<IModule> module;

    if (nullptr == _moduleManager)
    {
        IFR(E_NOT_SET);
    }

    IFR(spRTServer.As(&module));

    IFR(_moduleManager->AddModule(module.Get(), &handle));

    *serverHandle = handle;

    return S_OK;
}

_Use_decl_annotations_
HRESULT PluginManagerImpl::RTServerShutdown(
    ModuleHandle serverHandle)
{
    Log(Log_Level_Info, L"PluginManagerImpl::RTServerShutdown()\n");

    auto lock = _lock.Lock();

    // get capture engine
    ComPtr<IRealtimeServer> spRTServer;
    IFR(GetRealtimeServer(serverHandle, &spRTServer));

    IFR(spRTServer->Shutdown());

    // release the handle
    return _moduleManager->ReleaseModule(serverHandle);
}

_Use_decl_annotations_
HRESULT PluginManagerImpl::RTServerWriteFrame(
    ModuleHandle serverHandle,
    BYTE* pBuffer,
    UINT32 bufferSize)
{
    Log(Log_Level_Info, L"PluginManagerImpl::RTServerWriteFrame()\n");

    auto lock = _lock.Lock();

    // get realtime server
    ComPtr<IRealtimeServer> spRTServer;
    IFR(GetRealtimeServer(serverHandle, &spRTServer));

    ComPtr<IRealtimeServerWriter> spRTServerWriter;
    spRTServer.As(&spRTServerWriter);

    // Create MediaBuffer
    ComPtr<IMFMediaBuffer> spBuffer;

    UINT32 frameWidth, frameHeight;
    IFR(spRTServerWriter->GetCurrentResolution(&frameWidth, &frameHeight));

    const LONG cbWidth = sizeof(DWORD) * frameWidth;
    const DWORD cbBuffer = cbWidth * frameHeight;

    if (cbBuffer != bufferSize)
    {
        return E_INVALIDARG;
    }

    // Create a new memory buffer.
    BYTE *pMFdata = NULL;
    IFR(MFCreateMemoryBuffer(cbBuffer, &spBuffer));

    // Lock the buffer and copy the video frame to the buffer.
    IFR(spBuffer->Lock(&pMFdata, NULL, NULL));

    IFR(MFCopyImage(
        pMFdata,                    // Destination buffer.
        cbWidth,                    // Destination stride.
        pBuffer,						    // First row in source image.
        cbWidth,                    // Source stride.
        cbWidth,                    // Image width in bytes.
        frameHeight                 // Image height in pixels.
    ));

    if (spBuffer)
    {
        spBuffer->Unlock();
    }

    // Set the data length of the buffer.
    IFR(spBuffer->SetCurrentLength(cbBuffer));

    // Send server IMFMediaBuffer
    IFR(spRTServerWriter->WriteFrame(spBuffer.Get()));

    return S_OK;
}

_Use_decl_annotations_
HRESULT PluginManagerImpl::RTPlayerCreate(
    ModuleHandle handle,
    //StateChangedCallback fnCallback,
    PlayerCreatedCallback callback,
    void* pCallbackObject)
    //StreamingMediaPlayerImpl** ppStreamingPlayer)
{
    Log(Log_Level_Info, L"PluginManagerImpl::RTPlayerCreate()\n");

    NULL_CHK(callback);

    auto lock = _lock.Lock();

    // get connection
    ComPtr<IConnection> spConnection;
    IFR(GetConnection(handle, &spConnection));

    UnityGfxRenderer unityGraphics = _unityGraphics->GetRenderer();

    ComPtr<IStreamingMediaPlayer> spPlayer;
    IFR(StreamingMediaPlayerImpl::CreateStreamingMediaPlayer(unityGraphics,
        _unityInterfaces,
        spConnection.Get(),
        //fnCallback,
        &spPlayer));

    // Copy out the streaming player
    IFR(spPlayer.CopyTo(&s_spStreamingPlayer));

    ComPtr<IAsyncAction> spInitAction;
    IFR(spPlayer.As(&spInitAction));

    ComPtr<PluginManagerImpl> spThis(this);
    return StartAsyncThen(
        spInitAction.Get(),
        [this, spThis, spPlayer, callback, pCallbackObject](_In_ HRESULT hr, _In_ IAsyncAction* asyncAction, _In_ AsyncStatus asyncStatus)
    {
        Log(Log_Level_Info, L"PluginManagerImpl::CreateStreamingMediaPlayer() - InitAsync()\n");

        auto lock = _lock.Lock();

        UINT32 width, height;
        IFC(spPlayer->GetCurrentResolution(&width, &height));

    done:
        callback(pCallbackObject,
            hr,
            width,
            height);

        return hr;
    });
}

_Use_decl_annotations_
HRESULT PluginManagerImpl::RTPlayerRelease()
{
    Log(Log_Level_Info, L"PluginManagerImpl::RTPlayerRelease()\n");

    if (s_spStreamingPlayer != nullptr)
    {
        s_spStreamingPlayer.Reset();
        s_spStreamingPlayer = nullptr;
    }

    return S_OK;
}

_Use_decl_annotations_
HRESULT PluginManagerImpl::RTPlayerCreateTexture(_In_ UINT32 width, 
    _In_ UINT32 height,
    _COM_Outptr_ void** ppvTexture)
{
    Log(Log_Level_Info, L"PluginManagerImpl::RTPlayerCreateTexture()\n");

    NULL_CHK(ppvTexture);
    NULL_CHK(s_spStreamingPlayer);

    return s_spStreamingPlayer->CreateStreamingTexture(width, height, ppvTexture);
}

_Use_decl_annotations_
HRESULT  PluginManagerImpl::RTPlayerStart()
{
    Log(Log_Level_Info, L"PluginManagerImpl::RTPlayerStart()\n");

    NULL_CHK(s_spStreamingPlayer);

    return s_spStreamingPlayer->Play();
}

_Use_decl_annotations_
HRESULT  PluginManagerImpl::RTPlayerPause()
{
    Log(Log_Level_Info, L"PluginManagerImpl::RTPlayerPause()\n");

    NULL_CHK(s_spStreamingPlayer);

    return s_spStreamingPlayer->Pause();
}

_Use_decl_annotations_
HRESULT PluginManagerImpl::RTPlayerStop()
{
    Log(Log_Level_Info, L"PluginManagerImpl::RTPlayerStop()\n");

    NULL_CHK(s_spStreamingPlayer);

    return s_spStreamingPlayer->Stop();
}


// internal
_Use_decl_annotations_
void PluginManagerImpl::CompletePluginCallback(
    _In_ PluginCallback callback,
    _In_ void* pCallbackObject,
    _In_ ModuleHandle handle,
    _In_ HRESULT hr)
{
    LOG_RESULT(PluginManagerStaticsImpl::IsOnThread() ? S_OK : RPC_E_WRONG_THREAD);

    LOG_RESULT(ExceptionBoundary([&]() -> HRESULT
    {
        if (FAILED(hr))
        {
            callback(handle, pCallbackObject, hr, ErrorMessage(hr));
        }
        else
        {
            callback(handle, pCallbackObject, S_OK, L"");
        }

        return S_OK;
    }));
}

_Use_decl_annotations_
HRESULT PluginManagerImpl::GetConnection(
    _In_ ModuleHandle handle,
    _Out_ IConnection** ppConnection)
{
    NULL_CHK_HR(_moduleManager, E_NOT_SET);

    // find the module for handle
    ComPtr<IModule> module;
    IFR(_moduleManager->GetModule(handle, &module));

    // make sure its of capture type
    ComPtr<IConnection> spConnection;
    IFR(module.As(&spConnection));

    return spConnection.CopyTo(ppConnection);
}

_Use_decl_annotations_
HRESULT PluginManagerImpl::GetRealtimeServer(
    _In_ ModuleHandle handle,
    _Out_ IRealtimeServer** ppPlaybackEngine)
{
    NULL_CHK_HR(_moduleManager, E_NOT_SET);

    // find the module for handle
    ComPtr<IModule> module;
    IFR(_moduleManager->GetModule(handle, &module));

    // make sure its of capture type
    ComPtr<IRealtimeServer> spRTServer;
    IFR(module.As(&spRTServer));

    return spRTServer.CopyTo(ppPlaybackEngine);
}

_Use_decl_annotations_
void PluginManagerImpl::StoreToken(
    ModuleHandle handle,
    EventRegistrationToken &newToken)
{
    // get the list of tokens for connection
    auto iter = _eventTokens.find(handle);
    if (iter == _eventTokens.end())
    {
        // if none exist, create then list
        _eventTokens.insert(std::move(std::make_pair(handle, std::vector<EventRegistrationToken>())));

        // it should be in the list now
        iter = _eventTokens.find(handle);
    }

    // set eventList
    std::vector<EventRegistrationToken> &eventList = iter->second;

    // register callback and track token
    eventList.emplace_back(std::move(newToken));
}

_Use_decl_annotations_
void PluginManagerImpl::RemoveToken(
    ModuleHandle handle,
    INT64 tokenValue,
    EventRegistrationToken* token)
{
    // get the list of tokens for connection
    auto mapIt = _eventTokens.find(handle);
    if (mapIt != _eventTokens.end())
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
