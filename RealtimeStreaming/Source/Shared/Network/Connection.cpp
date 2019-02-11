// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "Connection.h"

_Use_decl_annotations_
inline HRESULT PrepareRemoteUrl(
    _In_ IStreamSocketInformation* pInfo, 
    _Out_ IUriRuntimeClass** ppUri)
{
    NULL_CHK(pInfo);
    NULL_CHK(ppUri);

    com_ptr<IStreamSocketInformation> spInfo(pInfo);
    
    com_ptr<IHostName> spHostName;
    check_hresult(spInfo->get_RemoteHostName(&spHostName));

    HostNameType spHostNameType;
    check_hresult(spHostName->get_Type(&spHostNameType));

    HString rawName;
    check_hresult(spHostName->get_RawName(rawName.GetAddressOf()));

    UINT32 length = 0;
    WCHAR pszUri[MAX_PATH];
    if (spHostNameType == HostNameType::HostNameType_Ipv4 || spHostNameType == HostNameType::HostNameType_DomainName)
    {
        check_hresult(StringCchPrintf(pszUri, _countof(pszUri), L"%s://%s", c_szNetworkScheme, rawName.GetRawBuffer(&length)));
    }
    else if (spHostNameType == HostNameType::HostNameType_Ipv6)
    {
        check_hresult(StringCchPrintf(pszUri, _countof(pszUri), L"%s://[%s]", c_szNetworkScheme, rawName.GetRawBuffer(&length)));
    }

    length = std::wcslen(pszUri);

    Wrappers::HString uriHString;
    check_hresult(WindowsCreateString(pszUri, length, uriHString.GetAddressOf()));

    com_ptr<IUriRuntimeClassFactory> uriFactory;
    check_hresult(
        Windows::Foundation::GetActivationFactory(
            Wrappers::HStringReference(RuntimeClass_Windows_Foundation_Uri).get(), 
            uriFactory.GetAddressOf())
    );

    com_ptr<IUriRuntimeClass> spUri;
    check_hresult(uriFactory->CreateUri(uriHString.get(), &spUri));

    NULL_CHK_HR(spUri, E_NOT_SET);

    return spUri.CopyTo(ppUri);
}


_Use_decl_annotations_
ConnectionImpl::ConnectionImpl()
    : _isInitialized(false)
    , _concurrentFailedBuffers(0)
    , _concurrentFailedBundles(0)
    , _streamSocket(nullptr)
    , _receivedBundle(nullptr)
{
    ZeroMemory(&_receivedHeader, sizeof(PayloadHeader));
}

_Use_decl_annotations_
ConnectionImpl::~ConnectionImpl()
{
    Log(Log_Level_Warning, L"ConnectionImpl::~ConnectionImpl()\n");

    Uninitialize();
}

_Use_decl_annotations_
HRESULT ConnectionImpl::RuntimeClassInitialize(
    IStreamSocket* socket)
{
    Log(Log_Level_Info, L"ConnectionImpl::RuntimeClassInitialize(socket)\n");

    NULL_CHK(socket);

    auto lock = _lock.Lock();

    _isInitialized = true;

    // store the socket
    com_ptr<IStreamSocket> spSocket(socket);
    check_hresult(spSocket.As(&_streamSocket));

    ZeroMemory(&_receivedHeader, sizeof(PayloadHeader));
    _receivedHeader.ePayloadType = PayloadType_Unknown;

    // create a thread to send data
    com_ptr<IThreadPoolStatics> threadPoolStatics;
    check_hresult(Windows::Foundation::GetActivationFactory(
        Wrappers::HStringReference(RuntimeClass_Windows_System_Threading_ThreadPool).get(),
        &threadPoolStatics));

    check_hresult(threadPoolStatics.As(&m_threadPoolStatics));

    return WaitForHeader();
}


// IModule
_Use_decl_annotations_
HRESULT ConnectionImpl::get_IsInitialized(
    _Out_ boolean* initialized)
{
    Log(Log_Level_Info, L"ConnectionImpl::get_IsInitialized()\n");

    NULL_CHK(initialized);

    auto lock = _lock.Lock();

    *initialized = _isInitialized;

    return S_OK;
};

_Use_decl_annotations_
HRESULT ConnectionImpl::Uninitialize(void)
{
    Log(Log_Level_Info, L"ConnectionImpl::Uninitialize()\n");

    auto lock = _lock.Lock();

    if (!_isInitialized)
    {
        return S_OK;
    }

    _isInitialized = false;

    return Close();
}

_Use_decl_annotations_
HRESULT ConnectionImpl::Close(void)
{
    Log(Log_Level_Info, L"ConnectionImpl::Close()\n");

    if (nullptr == _streamSocket)
    {
        return S_OK;
    }

    LOG_RESULT(ResetBundle());

    // cleanup socket
    com_ptr<ABI::Windows::Foundation::IClosable> closeable;
    if SUCCEEDED(_streamSocket.As(&closeable))
    {
        LOG_RESULT(closeable->Close());
    }

    _streamSocket.Reset();
    _streamSocket = nullptr;

    com_ptr<IConnection> spThis(this);
    return _evtDisconnected.InvokeAll(spThis.get());
}


// IConnection
_Use_decl_annotations_
HRESULT ConnectionImpl::get_IsConnected(
    boolean* connected)
{
    Log(Log_Level_Info, L"ConnectionImpl::get_IsConnected()\n");

    NULL_CHK(connected);

    auto lock = _lock.Lock();

    if (nullptr == _streamSocket)
    {
        return MF_E_SHUTDOWN;
    }

    *connected = (nullptr != _streamSocket);

    return S_OK;
}

_Use_decl_annotations_
HRESULT ConnectionImpl::add_Disconnected(
    IDisconnectedEventHandler* eventHandler, 
    EventRegistrationToken* token)
{
    //Log(Log_Level_Info, L"ConnectionImpl::add_Disconnected()\n");
     Log(Log_Level_All, L"ConnectionImpl::add_Disconnected() - Tid: %d \n", GetCurrentThreadId());

    NULL_CHK(eventHandler);
    NULL_CHK(token);

    auto lock = _lock.Lock();

    if (nullptr == _streamSocket)
    {
        return MF_E_SHUTDOWN;
    }

    return _evtDisconnected.Add(eventHandler, token);
}

_Use_decl_annotations_
HRESULT ConnectionImpl::remove_Disconnected(
    EventRegistrationToken token)
{
    Log(Log_Level_Info, L"ConnectionImpl::remove_Disconnected()\n");

    auto lock = _lock.Lock();

    return _evtDisconnected.Remove(token);
}

_Use_decl_annotations_
HRESULT ConnectionImpl::add_Received(
    IBundleReceivedEventHandler* eventHandler,
    EventRegistrationToken* token)
{
    Log(Log_Level_Info, L"ConnectionImpl::add_Received()\n");

    NULL_CHK(eventHandler);
    NULL_CHK(token);

    auto lock = _lock.Lock();

    if (nullptr == _streamSocket)
    {
        return MF_E_SHUTDOWN;
    }

    return _evtBundleReceived.Add(eventHandler, token);
}

_Use_decl_annotations_
HRESULT ConnectionImpl::remove_Received(
    EventRegistrationToken token)
{
    Log(Log_Level_Info, L"ConnectionImpl::remove_Received()\n");

    auto lock = _lock.Lock();

    return _evtBundleReceived.Remove(token);
}

_Use_decl_annotations_
HRESULT ConnectionImpl::SendPayloadType(
    PayloadType payloadType)
{
    //Log(Log_Level_Info, L"ConnectionImpl::SendRequest(%d)\n", payloadType);
    Log(Log_Level_All, L"ConnectionImpl::SendBundleAsync(%d) - Tid: %d \n", payloadType, GetCurrentThreadId());

    // send an PayloadType header, contains no payload.
    com_ptr<IDataBuffer> dataBuffer;
    check_hresult(MakeAndInitialize<DataBufferImpl>(&dataBuffer, sizeof(PayloadHeader)));

    com_ptr<IDataBundle> dataBundle;
    check_hresult(MakeAndInitialize<DataBundleImpl>(&dataBundle));

    // cast to IBufferByteAccess
    com_ptr<Windows::Storage::Streams::IBufferByteAccess> spByteAccess;
    check_hresult(dataBuffer.As(&spByteAccess));

    // get the raw byte pointer
    BYTE* buffer = nullptr;
    check_hresult(spByteAccess->Buffer(&buffer));
    NULL_CHK(buffer);

    // cast to the dataPtr
    PayloadHeader* pOpHeader = reinterpret_cast<PayloadHeader*>(buffer);
    NULL_CHK(pOpHeader);

    pOpHeader->cbPayloadSize = 0;
    pOpHeader->ePayloadType = payloadType;

    check_hresult(dataBuffer->put_CurrentLength(sizeof(PayloadHeader)));

    check_hresult(dataBundle->AddBuffer(dataBuffer.get()));

    return SendBundle(dataBundle.get());
}

_Use_decl_annotations_
HRESULT ConnectionImpl::SendBundle(
    IDataBundle* dataBundle)
{
    Log(Log_Level_Info, L"ConnectionImpl::SendBundle(bundle)\n");

    NULL_CHK(dataBundle);

    // get the output stream for socket
    com_ptr<IOutputStream> spOutputStream;
    {
        auto lock = _lock.Lock();

        check_hresult(CheckClosed());

        check_hresult(_streamSocket->get_OutputStream(&spOutputStream));
    }

    DataBundleImpl* bundleImpl = static_cast<DataBundleImpl*>(dataBundle);
    NULL_CHK_HR(bundleImpl, E_INVALIDARG);

    DataBundleImpl::Container buffers;
    check_hresult(bundleImpl->get_Buffers(&buffers));


    UINT32 totalLength = 0;
    UINT32 bufferCount = 0;
    // send one buffer at a time syncronously
    DataBundleImpl::Iterator iter = buffers.begin();
    for (; iter != buffers.end(); ++iter)
    {
        com_ptr<IBuffer> rawBuffer;
        check_hresult((*iter).As(&rawBuffer));

        UINT32 bufferLen;
        check_hresult(rawBuffer->get_Length(&bufferLen));
        totalLength += bufferLen;

        com_ptr<IStreamWriteOperation> writeOperation;
        check_hresult(spOutputStream->WriteAsync(rawBuffer.get(), &writeOperation));

        HRESULT hr = SyncWait<UINT32, UINT32>(writeOperation.get(), 1);
        check_hresult(hr);

        bufferCount++;
    }

    Log(Log_Level_Info, L"ConnectionImpl::SendBundle() - Buffers: %d - TotalLength: %d\n", bufferCount, totalLength);

    return S_OK;
}

_Use_decl_annotations_
HRESULT ConnectionImpl::SendBundleAsync(
    IDataBundle *dataBundle,
    IAsyncAction **sendAction)
{
    Log(Log_Level_All, L"ConnectionImpl::SendBundleAsync - Tid: %d \n", GetCurrentThreadId());

    NULL_CHK(dataBundle);
    NULL_CHK(sendAction);

    {
        auto lock = _lock.Lock();

        check_hresult(CheckClosed());
    }

    UINT32 bufferCount = 0;
    check_hresult(dataBundle->get_BufferCount(&bufferCount));

    com_ptr<WriteCompleteImpl> spWriteAction;
    check_hresult(MakeAndInitialize<WriteCompleteImpl>(&spWriteAction, bufferCount));

    // define workitem
    com_ptr<IDataBundle> spDataBundle(dataBundle);
    com_ptr<ConnectionImpl> spThis(this);
    auto workItem =
        Callback<ABI::Windows::System::Threading::IWorkItemHandler>(
            [this, spThis, spDataBundle, spWriteAction](IAsyncAction* asyncAction) -> HRESULT
    {
        com_ptr<IOutputStream> spOutputStream;
        DataBundleImpl::Container buffers;
        DataBundleImpl::Iterator iter;

        HRESULT hr = S_OK;

        {
            auto lock = _lock.Lock();

            IFC(CheckClosed());

            // get the output stream for socket
            if (nullptr != _streamSocket)
            {
                IFC(_streamSocket->get_OutputStream(&spOutputStream));
            }
            else
            {
                IFC(E_UNEXPECTED);
            }
        }

        DataBundleImpl* bundleImpl = static_cast<DataBundleImpl*>(spDataBundle.get());
        if (nullptr == bundleImpl)
        {
            IFC(E_INVALIDARG);
        }

        // send one buffer at a time syncronously
        IFC(bundleImpl->get_Buffers(&buffers));
        iter = buffers.begin();

        UINT32 totalLength = 0;
        UINT32 bufferCount = 0;

        for (; iter != buffers.end(); ++iter)
        {
            com_ptr<IStreamWriteOperation> spWriteOperation;
            com_ptr<IBuffer> rawBuffer;
            IFC((*iter).As(&rawBuffer));

            UINT32 bufferLen;
            check_hresult(rawBuffer->get_Length(&bufferLen));
            totalLength += bufferLen;

            bufferCount++;

            IFC(spOutputStream->WriteAsync(rawBuffer.get(), &spWriteOperation));

            // start async write operation
            IFC(StartAsyncThen(
                spWriteOperation.get(),
                [this, spThis, spWriteAction](_In_ HRESULT hr, _In_ IStreamWriteOperation *asyncResult, _In_ AsyncStatus asyncStatus) -> HRESULT
            {
                LOG_RESULT(hr);

                spWriteAction->SignalCompleted(hr);

                return S_OK;
            }));
        }

        Log(Log_Level_Info, L"ConnectionImpl::SendBundleAsync() - Buffers: %d - TotalLength: %d\n", bufferCount, totalLength);

    done:
        if (FAILED(hr))
        {
            spWriteAction->SignalCompleted(hr);
        }

        return S_OK;
    });

    com_ptr<IAsyncAction> workerAsync;
    check_hresult(m_threadPoolStatics->RunAsync(workItem.get(), &workerAsync));

    check_hresult(StartAsyncThen(
        workerAsync.get(),
        [this, spThis, spWriteAction](_In_ HRESULT hr, _In_ IAsyncAction *asyncResult, _In_ AsyncStatus asyncStatus) -> HRESULT
    {
        if (FAILED(hr))
        {
            spWriteAction->SignalCompleted(hr);
        }

        return S_OK;
    }));

    // hand off async op
    return spWriteAction.CopyTo(sendAction);
}

// IConnectionInternal
_Use_decl_annotations_
HRESULT ConnectionImpl::WaitForHeader()
{
    //Log(Log_Level_Info, L"ConnectionImpl::WaitForHeader()\n");
    Log(Log_Level_All, L"ConnectionImpl::WaitForHeader - Tid: %d \n", GetCurrentThreadId());

    check_hresult(CheckClosed());

    if (PayloadType_Unknown != _receivedHeader.ePayloadType
        ||
        0 != _receivedHeader.cbPayloadSize)
    {
        ResetBundle();
    }

    if (nullptr == _spHeaderBuffer)
    {
        check_hresult(MakeAndInitialize<DataBufferImpl>(&_spHeaderBuffer, sizeof(PayloadHeader)));
    }

    com_ptr<IMFMediaBuffer> spMediaBuffer;
    check_hresult(_spHeaderBuffer->get_MediaBuffer(&spMediaBuffer));

    DWORD bufferLen = 0;
    check_hresult(spMediaBuffer->GetMaxLength(&bufferLen)); 

    // should be the same as sizeof(PayloadHeader)
    if (bufferLen != sizeof(PayloadHeader))
    {
       check_hresult(_spHeaderBuffer->put_CurrentLength(sizeof(PayloadHeader)))
    }

    // get the socket input stream reader
    com_ptr<IInputStream> spInputStream;
    check_hresult(_streamSocket->get_InputStream(&spInputStream));

    // set the read operation and wait for data
    com_ptr<IStreamReadOperation> readAsyncOperation;
    check_hresult(spInputStream->ReadAsync(_spHeaderBuffer.get(), bufferLen, InputStreamOptions::InputStreamOptions_Partial, &readAsyncOperation));

    com_ptr<ConnectionImpl> spThis(this);
    return StartAsyncThen(
        readAsyncOperation.get(),
        [this, spThis](_In_ HRESULT hr, _In_ IStreamReadOperation *asyncResult, _In_ AsyncStatus asyncStatus) -> HRESULT
    {
        auto lock = _lock.Lock();

        if (FAILED(CheckClosed()) || FAILED(hr))
        {
            LOG_RESULT(hr);

            return Close();
        }

        return OnHeaderReceived(asyncResult, asyncStatus);
    });
}

_Use_decl_annotations_
HRESULT ConnectionImpl::WaitForPayload()
{
    Log(Log_Level_Info, L"ConnectionImpl::WaitForPayload()\n");

    if (FAILED(CheckClosed()))
    {
        return S_OK;
    }

    if (PayloadType_Unknown == _receivedHeader.ePayloadType
        ||
        PayloadType_ENDOFLIST <= _receivedHeader.ePayloadType
        ||
        0 == _receivedHeader.cbPayloadSize)
    {
        check_hresult(HRESULT_FROM_WIN32(ERROR_INVALID_STATE));
    }

    com_ptr<DataBufferImpl> payloadBuffer;
    check_hresult(MakeAndInitialize<DataBufferImpl>(&payloadBuffer, _receivedHeader.cbPayloadSize));

    // get the underlying buffer and makes sure it the right size
    com_ptr<IMFMediaBuffer> spMediaBuffer;
    check_hresult(payloadBuffer->get_MediaBuffer(&spMediaBuffer));

    DWORD bufferLen = 0;
    check_hresult(spMediaBuffer->GetMaxLength(&bufferLen));

    // should be the same as sizeof(PayloadHeader)
    if (bufferLen != _receivedHeader.cbPayloadSize)
    {
        check_hresult(spMediaBuffer->SetCurrentLength(_receivedHeader.cbPayloadSize))
    }

    // get the socket input stream reader
    com_ptr<IInputStream> spInputStream;
    check_hresult(_streamSocket->get_InputStream(&spInputStream));

    // set the read operation and wait for data
    com_ptr<IStreamReadOperation> readOperation;
    check_hresult(spInputStream->ReadAsync(payloadBuffer.get(), _receivedHeader.cbPayloadSize, InputStreamOptions::InputStreamOptions_None, &readOperation));

    com_ptr<ConnectionImpl> spThis(this);
    return StartAsyncThen(
        readOperation.get(),
        [this, spThis](_In_ HRESULT hr, _In_ IStreamReadOperation *asyncResult, _In_ AsyncStatus asyncStatus) -> HRESULT
    {
        auto lock = _lock.Lock();

        if (FAILED(CheckClosed()) || FAILED(hr))
        {
            LOG_RESULT(hr);

            return Close();
        }

        return OnPayloadReceived(asyncResult, asyncStatus);
    });
}


_Use_decl_annotations_
HRESULT ConnectionImpl::ResetBundle()
{
    Log(Log_Level_Info, L"ConnectionImpl::ResetBundle()\n");

    if (nullptr != _receivedBundle)
    {
        LOG_RESULT(_receivedBundle->Reset());
    }

    ZeroMemory(&_receivedHeader, sizeof(PayloadHeader));

    return S_OK;
}

_Use_decl_annotations_
HRESULT ConnectionImpl::ProcessHeaderBuffer(
    PayloadHeader* header,
    IDataBuffer* dataBuffer)
{
    NULL_CHK(header);
    NULL_CHK(dataBuffer);

    com_ptr<IDataBundle> dataBundle;
    check_hresult(MakeAndInitialize<DataBundleImpl>(&dataBundle));

    // add buffer
    check_hresult(dataBundle->AddBuffer(dataBuffer));

    LOG_RESULT(NotifyBundleComplete(header->ePayloadType, dataBundle.get()));

    // reset anything we created
    return dataBundle.Reset();
}

_Use_decl_annotations_
HRESULT ConnectionImpl::NotifyBundleComplete(
    PayloadType payloadType,
    IDataBundle* dataBundle)
{
    Log(Log_Level_Info, L"ConnectionImpl::OnCompleteBundle\n");

    NULL_CHK(dataBundle);

    {
        auto lock = _lock.Lock();

        if (FAILED(CheckClosed()))
        {
            return S_OK;
        }
    }

    com_ptr<IStreamSocketInformation> spInfo;
    check_hresult(_streamSocket->get_Information(&spInfo));

    com_ptr<IUriRuntimeClass> spUri;
    check_hresult(PrepareRemoteUrl(spInfo.get(), &spUri));

    com_ptr<IBundleReceivedArgs> args;
    check_hresult(MakeAndInitialize<DataBundleArgsImpl>(&args, payloadType, this, dataBundle, spUri.get()));

    return _evtBundleReceived.InvokeAll(this, args.get());
}

// Callbacks
_Use_decl_annotations_
HRESULT ConnectionImpl::OnHeaderReceived(
    IAsyncOperationWithProgress<IBuffer*, UINT32>* asyncResult,
    AsyncStatus asyncStatus)
{
    Log(Log_Level_All, L"ConnectionImpl::OnHeaderReceived - Tid: %d \n", GetCurrentThreadId());

    NULL_CHK(asyncResult);

    com_ptr<IAsyncOperationWithProgress<IBuffer*, UINT32>> spAsynResult(asyncResult);

    // get any error codes
    HRESULT hr = S_OK;

    com_ptr<IBuffer> spBuffer;
    com_ptr<IDataBuffer> dataBuffer;

    IFC(asyncResult->GetResults(&spBuffer));

    UINT32 bytesRead = -1;
    IFC(spBuffer->get_Length(&bytesRead));

    IFC(spBuffer.As(&dataBuffer));

    DWORD bufferSize = -1;
    IFC(dataBuffer->get_CurrentLength(&bufferSize));

    // makes sure this is the expected size
    if (bytesRead != sizeof(PayloadHeader) 
        ||
        bytesRead != bufferSize)
    {
        _concurrentFailedBuffers++;

        goto done;
    }

    // ensure we are waiting for a new payload
    if (_receivedHeader.ePayloadType != PayloadType_Unknown)
    {
        // should not be receiving a header buffer when waiting for payload buffers
        // bad state; bail
        IFC(-1073740758); // STATUS_REQUEST_OUT_OF_SEQUENCE
    }

    // cast buffer to PayloadHeader*
    PayloadHeader* header = GetDataType<PayloadHeader*>(spBuffer.get());
    IFC(nullptr != header ? S_OK : E_UNEXPECTED);

    // is header type in a range we understand
    if (header->ePayloadType == PayloadType_Unknown
        ||
        header->ePayloadType > PayloadType_ENDOFLIST)
    {
        _concurrentFailedBuffers++;

        goto done;
    }

    // if there is a payload with this buffer, we need to handle that seperately
    if (header->cbPayloadSize != 0)
    {
        // todo: crc checks
        // can we trust the payload size?
        if (header->cbPayloadSize > c_cbMaxBundleSize)
        {
            // just incase it was corrupt we wait for the next one
            _concurrentFailedBuffers++;
        }
        else
        {
            // store the payload header info
            _receivedHeader = *header;

            // start the process to receive payload data
            return WaitForPayload();
        }
    }

    _concurrentFailedBuffers = 0;

    IFC(ProcessHeaderBuffer(header, dataBuffer.get()));

done:
    if (_concurrentFailedBuffers > c_cbMaxBufferFailures)
    {
        LOG_RESULT(hr);

        LOG_RESULT(Close());

        return S_OK;
    }

    return WaitForHeader(); // go back to waiting for header
}

_Use_decl_annotations_
HRESULT ConnectionImpl::OnPayloadReceived(
    IAsyncOperationWithProgress<IBuffer*, UINT32>* asyncResult,
    AsyncStatus asyncStatus)
{
    Log(Log_Level_All, L"ConnectionImpl::OnPayloadReceived\n");

    com_ptr<IBuffer> buffer;
    check_hresult(asyncResult->GetResults(&buffer));

    UINT32 bytesRead = -1;
    check_hresult(buffer->get_Length(&bytesRead));

    com_ptr<IDataBuffer> dataBuffer;
    check_hresult(buffer.As(&dataBuffer));

    DWORD bufferSize = -1;
    check_hresult(dataBuffer->get_CurrentLength(&bufferSize));

    // makes sure this is the expected size
    if (c_cbMaxBundleSize < _receivedHeader.cbPayloadSize
        ||
        bytesRead > _receivedHeader.cbPayloadSize
        ||
        bytesRead == 0
        ||
        bytesRead != bufferSize)
    {
        _concurrentFailedBundles++;

        goto done;
    }

    // still have a valid payload type
    if (_receivedHeader.ePayloadType == PayloadType_Unknown
        ||
        _receivedHeader.ePayloadType >= PayloadType_ENDOFLIST)
    {
        _concurrentFailedBundles++;

        goto done;
    }

    // create bundle to hold all buffers
    if (nullptr == _receivedBundle)
    {
        check_hresult(MakeAndInitialize<DataBundleImpl>(&_receivedBundle));
    }

    ULONG bundleSize = 0;
    check_hresult(_receivedBundle->get_TotalSize(&bundleSize));

    // before adding the new buffer, make sure that doesn't exceed the payload size
    ULONG expectedSize = _receivedHeader.cbPayloadSize - bundleSize;
    if (bufferSize > expectedSize)
    {
        com_ptr<IDataBuffer> spTrimmed;
        check_hresult(dataBuffer->TrimRight(expectedSize, &spTrimmed))
    };

    // add the buffer to the bundle
    _receivedBundle->AddBuffer(dataBuffer.get());

    // recalculate the bundle size
    bundleSize = 0;
    check_hresult(_receivedBundle->get_TotalSize(&bundleSize));

    // do we have a complete bundle?
    if (bundleSize < _receivedHeader.cbPayloadSize)
    {
        return WaitForPayload();
    }

    // store the bundle data to be used for notification
    PayloadType payloadType = _receivedHeader.ePayloadType;
    LOG_RESULT(NotifyBundleComplete(payloadType, _receivedBundle.get()));

done:
    ResetBundle();

    if (_concurrentFailedBundles > c_cbMaxBundleFailures)
    {
        LOG_RESULT(HRESULT_FROM_WIN32(ERROR_CONNECTION_UNAVAIL));

        return Close();
    }

    return WaitForHeader();
}

