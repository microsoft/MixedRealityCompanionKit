// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "Connection.h"

_Use_decl_annotations_
Connection::Connection(StreamSocket socket)
    : _isInitialized(false)
    , m_concurrentFailedBuffers(0)
    , m_concurrentFailedBundles(0)
    , m_streamSocket(socket)
    , m_receivedBundle(nullptr)
{
    auto lock = _lock.Lock();

    ZeroMemory(&m_receivedHeader, sizeof(PayloadHeader));
    m_receivedHeader.ePayloadType = PayloadType_Unknown;

    return WaitForHeader();
}

_Use_decl_annotations_
Connection::~Connection()
{
    Log(Log_Level_Warning, L"ConnectionImpl::~ConnectionImpl()\n");

    Close();
}

_Use_decl_annotations_
void Connection::Close()
{
    Log(Log_Level_Info, L"ConnectionImpl::Close()\n");

    if (nullptr == m_streamSocket)
    {
        return;
    }

    LOG_RESULT(ResetBundle());

    // cleanup socket
    m_streamSocket.Close();
    m_streamSocket = nullptr;

    // Fire disconnect event
    m_evtDisconnected(*this, true);
}

_Use_decl_annotations_
bool Connection::IsConnected()
{
    Log(Log_Level_Info, L"ConnectionImpl::get_IsConnected()\n");

    auto lock = _lock.Lock();

    return nullptr != m_streamSocket;
}
_Use_decl_annotations_
IStreamSocketInformation Connection::ConnectionInfo()
{
    Log(Log_Level_Info, L"ConnectionImpl::get_ConnectionInfo()\n");

    auto lock = _lock.Lock();

    if (m_streamSocket == nullptr)
    {
        return nullptr;
    }

    return m_streamSocket.Information();
}

/* Event Handlers */

_Use_decl_annotations_
winrt::event_token Connection::Disconnected(Windows::Foundation::EventHandler const& handler)
{
    Log(Log_Level_All, L"ConnectionImpl::add_Disconnected() - Tid: %d \n", GetCurrentThreadId());

    auto lock = _lock.Lock();

    return m_evtDisconnected.add(handler);
}

_Use_decl_annotations_
void Connection::Disconnected(winrt::event_token const& token)
{
    Log(Log_Level_Info, L"ConnectionImpl::remove_Disconnected()\n");

    auto lock = _lock.Lock();

    m_evtDisconnected.remove(token);
}

_Use_decl_annotations_
winrt::event_token Connection::Received(Windows::Foundation::EventHandler const& handler)
{
    Log(Log_Level_Info, L"ConnectionImpl::add_Received()\n");

    auto lock = _lock.Lock();

    return m_evtBundleReceived.add(handler);
}

_Use_decl_annotations_
void Connection::Received(winrt::event_token const& token)
{
    Log(Log_Level_Info, L"ConnectionImpl::remove_Received()\n");

    auto lock = _lock.Lock();

    m_evtBundleReceived.remove(token);
}


_Use_decl_annotations_
IAsyncAction Connection::SendPayloadType(
    PayloadType payloadType)
{
    Log(Log_Level_All, L"ConnectionImpl::SendBundleAsync(%d) - Tid: %d \n", payloadType, GetCurrentThreadId());

    // send an PayloadType header, contains no payload.
    DataBuffer buffer = DataBuffer(sizeof(PayloadHeader));
    DataBundle dataBundle = DataBundle(); // TODO: winrt::make()?

    // cast to IBufferByteAccess
    com_ptr<Windows::Storage::Streams::IBufferByteAccess> spByteAccess;
    IFT(dataBuffer.As(&spByteAccess));

    // get the raw byte pointer
    BYTE* pBuffer = nullptr;

    buffer.Buffer(pBuffer);
    //IFT(spByteAccess->Buffer(&buffer));
    NULL_CHK(buffer);

    // cast to the dataPtr
    PayloadHeader* pOpHeader = reinterpret_cast<PayloadHeader*>(pBuffer);
    NULL_CHK(pOpHeader);

    pOpHeader->cbPayloadSize = 0;
    pOpHeader->ePayloadType = payloadType;

    dataBuffer.CurrentLength(sizeof(PayloadHeader));
    dataBundle.AddBuffer(dataBuffer);

    return SendBundle(dataBundle);
}

_Use_decl_annotations_
IAsyncAction Connection::SendBundleAsync(
    DataBundle dataBundle)
{
    Log(Log_Level_Info, L"ConnectionImpl::SendBundleAsync(bundle)\n");

    // get the output stream for socket
    OutputStream outputStream{ nullptr };
    {
        auto lock = _lock.Lock();

        IFT(CheckClosed());
        outputStream = m_streamSocket.OutputStream();
    }

    UINT32 totalLength = 0;
    UINT32 bufferCount = 0;

    std::vector< IAsyncOperationWithProgress< unsigned int, unsigned int > > pendingWrites{};

    // send one buffer at a time syncronously
    for (auto const& currBuffer : dataBundle.Buffer())
    {
        totalLength += currBuffer.Length();

        pendingWrites.push_back(outputStream.WriteAsync(currBuffer));

        bufferCount++;
    }

    co_await outputStream.FlushAsync();

    Log(Log_Level_Info, L"ConnectionImpl::SendBundle() - Buffers: %d - TotalLength: %d\n", bufferCount, totalLength);
}


// IConnectionInternal
_Use_decl_annotations_
IAsyncAction Connection::WaitForHeader()
{
    Log(Log_Level_All, L"ConnectionImpl::WaitForHeader - Tid: %d \n", GetCurrentThreadId());

    IFT(CheckClosed());

    if (PayloadType_Unknown != m_receivedHeader.ePayloadType ||
        0 != m_receivedHeader.cbPayloadSize)
    {
        ResetBundle();
    }

    if (nullptr == m_spHeaderBuffer)
    {
        m_spHeaderBuffer = DataBuffer(sizeof(PayloadHeader));
    }

    // TODO: how to return com_ptr of winrt function?
    com_ptr<IMFMediaBuffer> spMediaBuffer = m_spHeaderBuffer.MediaBuffer();
    //IFT(m_spHeaderBuffer->get_MediaBuffer(&spMediaBuffer));

    DWORD bufferLen = 0;
    IFT(spMediaBuffer->GetMaxLength(&bufferLen)); 

    // should be the same as sizeof(PayloadHeader)
    if (bufferLen != sizeof(PayloadHeader))
    {
        m_spHeaderBuffer.CurrentLength(sizeof(PayloadHeader));
    }

    // get the socket input stream reader
    InputStream spInputStream = m_streamSocket.InputStream();

    auto asyncInfo = co_await spInputStream.ReadAsync(m_spHeaderBuffer, bufferLen, InputStreamOptions::InputStreamOptions_Partial)
    m_spHeaderBuffer = asyncInfo.GetResults();

    return OnHeaderReceived();
}

_Use_decl_annotations_
IAsyncAction Connection::WaitForPayload()
{
    Log(Log_Level_Info, L"ConnectionImpl::WaitForPayload()\n");

    if (FAILED(CheckClosed()))
    {
        return S_OK;
    }

    if (PayloadType_Unknown == m_receivedHeader.ePayloadType ||
        PayloadType_ENDOFLIST <= m_receivedHeader.ePayloadType ||
        0 == m_receivedHeader.cbPayloadSize)
    {
        IFT(HRESULT_FROM_WIN32(ERROR_INVALID_STATE));
    }

    DataBuffer payloadBuffer = DataBuffer(m_receivedHeader.cbPayloadSize);

    // get the underlying buffer and makes sure it the right size
    com_ptr<IMFMediaBuffer> spMediaBuffer = payloadBuffer.MediaBuffer();
    //IFT(payloadBuffer.MediaBuffer(&spMediaBuffer));

    DWORD bufferLen = 0;
    IFT(spMediaBuffer->GetMaxLength(&bufferLen));

    // should be the same as sizeof(PayloadHeader)
    if (bufferLen != m_receivedHeader.cbPayloadSize)
    {
        IFT(spMediaBuffer->SetCurrentLength(m_receivedHeader.cbPayloadSize))
    }

    // get the socket input stream reader
    auto asyncInfo = co_await m_streamSocket.InputStream().ReadAsync(payloadBuffer,
        m_receivedHeader.cbPayloadSize,
        InputStreamOptions::InputStreamOptions_None);

    return OnPayloadReceived(asyncInfo.GetResults());
}

_Use_decl_annotations_
void Connection::ResetBundle()
{
    Log(Log_Level_Info, L"ConnectionImpl::ResetBundle()\n");

    if (nullptr != m_receivedBundle)
    {
        LOG_RESULT(m_receivedBundle.Reset());
    }

    ZeroMemory(&m_receivedHeader, sizeof(PayloadHeader));
}

_Use_decl_annotations_
HRESULT Connection::ProcessHeaderBuffer(
    PayloadHeader* header,
    DataBuffer dataBuffer)
{
    NULL_CHK(header);

    DataBundle dataBundle = DataBundle();
    
    // add buffer
    IFR(dataBundle.AddBuffer(dataBuffer));

    LOG_RESULT(NotifyBundleComplete(header->ePayloadType, dataBundle));

    return S_OK;
}

_Use_decl_annotations_
HRESULT Connection::NotifyBundleComplete(
    PayloadType payloadType,
    DataBundle dataBundle)
{
    Log(Log_Level_Info, L"ConnectionImpl::OnCompleteBundle\n");

    {
        auto lock = _lock.Lock();

        if (FAILED(CheckClosed()))
        {
            return S_OK;
        }
    }

    BundleReceivedArgs> args = DataBundleArgs(payloadType, this, dataBundle);
    m_evtBundleReceived(*this, args);
}

// Callbacks
_Use_decl_annotations_
IAsyncAction Connection::OnHeaderReceived()
{
    // TODO: Troy lock? or pass in buffer via method entry

    Log(Log_Level_All, L"ConnectionImpl::OnHeaderReceived - Tid: %d \n", GetCurrentThreadId());

    HRESULT hr = S_OK;

    UINT32 bytesRead = m_spHeaderBuffer.Length()
    DWORD bufferSize = m_spHeaderBuffer.CurrentLength();
    
    // makes sure this is the expected size
    if (bytesRead != sizeof(PayloadHeader) 
        ||
        bytesRead != bufferSize)
    {
        m_concurrentFailedBuffers++;

        goto done;
    }

    // ensure we are waiting for a new payload
    if (m_receivedHeader.ePayloadType != PayloadType_Unknown)
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
        m_concurrentFailedBuffers++;

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
            m_concurrentFailedBuffers++;
        }
        else
        {
            // store the payload header info
            m_receivedHeader = *header;

            // start the process to receive payload data
            return WaitForPayload();
        }
    }

    m_concurrentFailedBuffers = 0;

    IFC(ProcessHeaderBuffer(header, dataBuffer));

done:
    if (m_concurrentFailedBuffers > c_cbMaxBufferFailures)
    {
        LOG_RESULT(hr);

        return Close();
    }

    return WaitForHeader(); // go back to waiting for header
}

_Use_decl_annotations_
Windows::Foundation::IAsyncAction Connection::OnPayloadReceived(IBuffer payloadBuffer)
{
    Log(Log_Level_All, L"ConnectionImpl::OnPayloadReceived\n");

    UINT32 bytesRead = buffer.Length();
    DWORD bufferSize = buffer.CurrentLength();

    // makes sure this is the expected size
    if (c_cbMaxBundleSize < m_receivedHeader.cbPayloadSize ||
        bytesRead > m_receivedHeader.cbPayloadSize ||
        bytesRead == 0 ||
        bytesRead != bufferSize)
    {
        _concurrentFailedBundles++;

        goto done;
    }

    // still have a valid payload type
    if (m_receivedHeader.ePayloadType == PayloadType_Unknown ||
        m_receivedHeader.ePayloadType >= PayloadType_ENDOFLIST)
    {
        _concurrentFailedBundles++;

        goto done;
    }

    // create bundle to hold all buffers
    if (nullptr == m_receivedBundle)
    {
        m_receivedBundle = DataBundle();
    }

    ULONG bundleSize = m_receivedBundle.TotalSize();


    // before adding the new buffer, make sure that doesn't exceed the payload size
    ULONG expectedSize = m_receivedHeader.cbPayloadSize - bundleSize;
    if (bufferSize > expectedSize)
    {
        DataBuffer trimmed;
        payloadBuffer.TrimRight(expectedSize, trimmed);
    };

    // add the buffer to the bundle
    m_receivedBundle.AddBuffer(payloadBuffer);

    // recalculate the bundle size
    bundleSize = m_receivedBundle.TotalSize();

    // do we have a complete bundle?
    if (bundleSize < m_receivedHeader.cbPayloadSize)
    {
        return WaitForPayload();
    }

    // store the bundle data to be used for notification
    LOG_RESULT(NotifyBundleComplete(m_receivedHeader.ePayloadType, m_receivedBundle));

done:
    ResetBundle();

    if (_concurrentFailedBundles > c_cbMaxBundleFailures)
    {
        LOG_RESULT(HRESULT_FROM_WIN32(ERROR_CONNECTION_UNAVAIL));

        return Close();
    }

    return WaitForHeader();
}

