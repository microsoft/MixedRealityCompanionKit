// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "Connector.h"

ActivatableStaticOnlyFactory(ConnectorStaticsImpl);

_Use_decl_annotations_
ConnectorImpl::ConnectorImpl()
    : _isInitialized(false)
    , _hostName(nullptr)
    , _port(-1)
    , _streamSocketResult(nullptr)
{
    Log(Log_Level_Info, L"ConnectorImpl::ConnectorImpl()\n");
}

_Use_decl_annotations_
ConnectorImpl::~ConnectorImpl()
{
    Log(Log_Level_Info, L"ConnectorImpl::~ConnectorImpl()\n");

    Uninitialize();
}

_Use_decl_annotations_
HRESULT ConnectorImpl::RuntimeClassInitialize(
    IHostName* hostName, 
    UINT16 port)
{
    Log(Log_Level_Info, L"ConnectorImpl::RuntimeClassInitialize(IHostName)\n");

    ComPtr<ABI::Windows::Networking::IHostName> spHostName(hostName);
    IFR(spHostName.As(&_hostName));

    _port = port;

    _isInitialized = true;

    return S_OK;
}

_Use_decl_annotations_
HRESULT ConnectorImpl::get_IsInitialized(
    boolean* initialized)
{
    Log(Log_Level_Info, L"ConnectorImpl::get_IsInitialized()\n");

    NULL_CHK(initialized);

    auto lock = _lock.Lock();

    *initialized = _isInitialized;

    return S_OK;
}

_Use_decl_annotations_
HRESULT ConnectorImpl::Uninitialize()
{
    Log(Log_Level_Info, L"ConnectorImpl::Uninitialize()\n");

    auto lock = _lock.Lock();

    if (!_isInitialized)
    {
        return S_OK;
    }

    _isInitialized = false;

    Close();

    return S_OK;
}

// IConnector
_Use_decl_annotations_
HRESULT ConnectorImpl::ConnectAsync(
    IAsyncOperation<Connection*>** operation)
{
    Log(Log_Level_Info, L"ConnectorImpl::ConnectAsync()\n");

    NULL_CHK(operation);

    ComPtr<IAsyncOperation<Connection*>> spThis(this);

    IFR(spThis.CopyTo(operation));

    return Start();
}

_Use_decl_annotations_
HRESULT ConnectorImpl::add_Closed(
    IClosedEventHandler* eventHandler,
    EventRegistrationToken* token)
{
    Log(Log_Level_Info, L"ConnectorImpl::add_Closed()\n");

    NULL_CHK(eventHandler);
    NULL_CHK(token);

    auto lock = _lock.Lock();

    return _evtClosed.Add(eventHandler, token);
}

_Use_decl_annotations_
HRESULT ConnectorImpl::remove_Closed(
    EventRegistrationToken token)
{
    Log(Log_Level_Info, L"ConnectorImpl::remove_Closed()\n");

    auto lock = _lock.Lock();

    return _evtClosed.Remove(token);
}

// IAsyncOperation<IConnection>
_Use_decl_annotations_
HRESULT ConnectorImpl::put_Completed(
    IAsyncOperationCompletedHandler<Connection*>* handler)
{
    NULL_CHK(handler);

    return PutOnComplete(handler);
}

_Use_decl_annotations_
HRESULT ConnectorImpl::get_Completed(
    IAsyncOperationCompletedHandler<Connection*>** handler)
{
    NULL_CHK(handler);

    return GetOnComplete(handler);
}

_Use_decl_annotations_
HRESULT ConnectorImpl::GetResults(
    IConnection** ppConnection)
{
    auto lock = _lock.Lock();

    IFR(AsyncBase::CheckValidStateForResultsCall());

    ComPtr<ConnectionImpl> spConnection;
    IFR(Microsoft::WRL::MakeAndInitialize<ConnectionImpl>(&spConnection, _streamSocketResult.Detach()));

    NULL_CHK_HR(spConnection, E_NOT_SET);

    IFR(spConnection.CopyTo(ppConnection));

    return Close();
}

_Use_decl_annotations_
HRESULT ConnectorImpl::OnStart(void)
{
    // set port as a string
    std::wstring wsPort = to_wstring(_port);

    // activate a stream socket
    ComPtr<IStreamSocket> streamSocket;
    IFR(Windows::Foundation::ActivateInstance(
        Wrappers::HStringReference(RuntimeClass_Windows_Networking_Sockets_StreamSocket).Get(),
        &streamSocket));

    // get the control and set properties
    ComPtr<IStreamSocketControl> streamControl;
    IFR(streamSocket->get_Control(&streamControl));
    IFR(streamControl->put_KeepAlive(true));
    IFR(streamControl->put_NoDelay(true));
    IFR(streamControl->put_QualityOfService(SocketQualityOfService::SocketQualityOfService_LowLatency));

    // setup connection Action
    ComPtr<IAsyncAction> connectAsync;
    IFR(streamSocket->ConnectAsync(
        _hostName.Get(), 
        Wrappers::HStringReference(wsPort.data()).Get(), 
        &connectAsync));

    // set callback and handle result
    ComPtr<ConnectorImpl> spThis(this);
    return StartAsyncThen(
        connectAsync.Get(),
        [this, spThis, streamSocket](_In_ HRESULT hr, _In_ IAsyncAction* pAsyncResult, _In_ AsyncStatus asyncStatus) -> HRESULT
    {
        auto lock = _lock.Lock();

        IFC(hr);

        IFC(streamSocket.As(&_streamSocketResult))

    done:
        if (FAILED(hr))
        {
            TryTransitionToError(hr);
        }

        return FireCompletion();
    });
}

_Use_decl_annotations_
void ConnectorImpl::OnClose(void)
{
    CloseInternal();
}

_Use_decl_annotations_
void ConnectorImpl::OnCancel(void)
{
    CloseInternal();
}

_Use_decl_annotations_
void ConnectorImpl::CloseInternal(void)
{
    Log(Log_Level_Info, L"ConnectorImpl::CloseInternal()\n");

    ComPtr<ABI::Windows::Foundation::IClosable> closeable;
    if (nullptr != _streamSocketResult)
    {
        if SUCCEEDED(_streamSocketResult.As(&closeable))
        {
            LOG_RESULT(closeable->Close());
        }

        ComPtr<IConnector> spThis(this);
        LOG_RESULT(_evtClosed.InvokeAll(spThis.Get()));
    }
    _streamSocketResult.Reset();
    _streamSocketResult = nullptr;
}
