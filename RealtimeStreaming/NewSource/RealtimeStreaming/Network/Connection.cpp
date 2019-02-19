// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "Connection.h"
#include "DataBuffer.h"
#include "DataBundle.h"
#include "DataBundleArgs.h"
//#include <robuffer.h> // IBufferByteAccess 

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Networking::Sockets;
using namespace winrt::Windows::Storage::Streams;

using namespace RealtimeStreaming::Network::implementation;
using namespace RealtimeStreaming::Common;
//using IBufferByteAccess = ::Windows::Storage::Streams::IBufferByteAccess; //IBufferByteAccess

_Use_decl_annotations_
Connection::Connection(StreamSocket const& socket)
    : m_concurrentFailedBuffers(0)
    , m_concurrentFailedBundles(0)
    , m_streamSocket(socket)
    , m_receivedBundle(nullptr)
{
    //auto lock = _lock.Lock();
    slim_lock_guard guard(m_lock);

    ZeroMemory(&m_receivedHeader, sizeof(PayloadHeader));
    m_receivedHeader.ePayloadType = PayloadType::Unknown;

    WaitForHeader();
}

_Use_decl_annotations_
Connection::~Connection()
{
    slim_lock_guard guard(m_lock);

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

    ResetBundle();

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

    //auto lock = _lock.Lock();
    slim_shared_lock_guard const guard(m_lock);

    return nullptr != m_streamSocket;
}
_Use_decl_annotations_
StreamSocketInformation Connection::ConnectionInfo()
{
    Log(Log_Level_Info, L"ConnectionImpl::get_ConnectionInfo()\n");

    slim_shared_lock_guard const guard(m_lock);

    if (m_streamSocket == nullptr)
    {
        return nullptr;
    }

    return m_streamSocket.Information();
}

/* Event Handlers */

_Use_decl_annotations_
event_token Connection::Disconnected(
    RealtimeStreaming::Network::DisconnectedDelegate const& handler)
{
    Log(Log_Level_All, L"ConnectionImpl::add_Disconnected() - Tid: %d \n", GetCurrentThreadId());

    //auto lock = _lock.Lock();
    slim_lock_guard guard(m_lock);

    return m_evtDisconnected.add(handler);
}

_Use_decl_annotations_
void Connection::Disconnected(winrt::event_token const& token)
{
    Log(Log_Level_Info, L"ConnectionImpl::remove_Disconnected()\n");

    //auto lock = _lock.Lock();
    slim_lock_guard guard(m_lock);

    m_evtDisconnected.remove(token);
}

_Use_decl_annotations_
event_token Connection::Received(
    EventHandler<RealtimeStreaming::Network::DataBundleArgs> const& handler)
{
    Log(Log_Level_Info, L"ConnectionImpl::add_Received()\n");

    //auto lock = _lock.Lock();
    slim_lock_guard guard(m_lock);

    return m_evtBundleReceived.add(handler);
}

_Use_decl_annotations_
void Connection::Received(winrt::event_token const& token)
{
    Log(Log_Level_Info, L"ConnectionImpl::remove_Received()\n");

    //auto lock = _lock.Lock();
    slim_lock_guard guard(m_lock);

    m_evtBundleReceived.remove(token);
}

_Use_decl_annotations_
IAsyncAction Connection::SendPayloadTypeAsync(
    PayloadType payloadType)
{
    Log(Log_Level_All, L"ConnectionImpl::SendBundleAsync(%d) - Tid: %d \n", payloadType, GetCurrentThreadId());

    // Create send buffer
    DataBuffer dataBuffer = DataBuffer(sizeof(PayloadHeader));
    
    // get the buffer pointer
    BYTE* pBuffer;
    dataBuffer.Buffer(&pBuffer);
    NULL_THROW(pBuffer);

    DataBundle dataBundle = DataBundle(dataBuffer);

    // cast to the dataPtr
    PayloadHeader* pOpHeader = reinterpret_cast<PayloadHeader*>(pBuffer);
    NULL_THROW(pOpHeader);

    pOpHeader->cbPayloadSize = 0;
    pOpHeader->ePayloadType = payloadType;

    dataBuffer.CurrentLength(sizeof(PayloadHeader));
    dataBundle.AddBuffer(dataBuffer);

    return SendBundleAsync(dataBundle);
}

_Use_decl_annotations_
IAsyncAction Connection::SendBundleAsync(
    RealtimeStreaming::Network::DataBundle dataBundle)
{
    Log(Log_Level_Info, L"ConnectionImpl::SendBundleAsync(bundle)\n");

    // get the output stream for socket
    IOutputStream outputStream{ nullptr };
    {
        slim_lock_guard guard(m_lock);

        IFT(CheckClosed());
        outputStream = m_streamSocket.OutputStream();
    }

    UINT32 totalLength = 0;
    UINT32 bufferCount = 0;

    std::vector< IAsyncOperationWithProgress< unsigned int, unsigned int > > pendingWrites{};

    // send one buffer at a time syncronously
    auto dataBundleImpl = dataBundle.as<implementation::DataBundle>();
    for (auto const& currBuffer : dataBundleImpl->GetBuffers())
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

    if (PayloadType::Unknown != m_receivedHeader.ePayloadType ||
        0 != m_receivedHeader.cbPayloadSize)
    {
        ResetBundle();
    }

    // TODO: Look at why using member variable???
    // Maybe create one and then pass through to OnheaderReceived*?
    if (nullptr == m_spHeaderBuffer)
    {
        m_spHeaderBuffer = DataBuffer(sizeof(PayloadHeader));
    }

    auto headerBufferImpl = m_spHeaderBuffer.as<implementation::DataBuffer>();
    com_ptr<IMFMediaBuffer> spMediaBuffer;
    IFT(headerBufferImpl->GetMediaBuffer(spMediaBuffer.put()));

    DWORD bufferLen = headerBufferImpl->Capacity();

    // should be the same as sizeof(PayloadHeader)
    if (bufferLen != sizeof(PayloadHeader))
    {
        m_spHeaderBuffer.CurrentLength(sizeof(PayloadHeader));
    }

    // get the socket input stream reader
    IInputStream inputStream = m_streamSocket.InputStream();

    auto bufferResult = co_await inputStream.ReadAsync(m_spHeaderBuffer, bufferLen, InputStreamOptions::Partial);
    
    m_spHeaderBuffer = nullptr; //unseat current header buffer
    m_spHeaderBuffer = bufferResult.as<RealtimeStreaming::Network::DataBuffer>();

    co_await OnHeaderReceived();
}

_Use_decl_annotations_
IAsyncAction Connection::WaitForPayload()
{
    Log(Log_Level_Info, L"ConnectionImpl::WaitForPayload()\n");

    if (FAILED(CheckClosed()))
    {
        return;
    }

    if (PayloadType::Unknown == m_receivedHeader.ePayloadType ||
        PayloadType::ENDOFLIST <= m_receivedHeader.ePayloadType ||
        0 == m_receivedHeader.cbPayloadSize)
    {
        IFT(HRESULT_FROM_WIN32(ERROR_INVALID_STATE));
    }

    DataBuffer payloadBuffer = DataBuffer(m_receivedHeader.cbPayloadSize);

    // get the underlying buffer and makes sure it the right size
    com_ptr<IMFMediaBuffer> spMediaBuffer;
    payloadBuffer.GetMediaBuffer(spMediaBuffer.put());
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
        InputStreamOptions::None);

    co_await OnPayloadReceived(asyncInfo);
}

_Use_decl_annotations_
void Connection::ResetBundle()
{
    Log(Log_Level_Info, L"ConnectionImpl::ResetBundle()\n");

    if (nullptr != m_receivedBundle)
    {
        m_receivedBundle.Reset();
    }

    ZeroMemory(&m_receivedHeader, sizeof(PayloadHeader));
}

_Use_decl_annotations_
HRESULT Connection::ProcessHeaderBuffer(
    RealtimeStreaming::Common::PayloadHeader* header,
    RealtimeStreaming::Network::DataBuffer dataBuffer)
{
    NULL_CHK(header);

    DataBundle dataBundle = DataBundle(dataBuffer);

    LOG_RESULT(NotifyBundleComplete(header->ePayloadType, dataBundle));

    return S_OK;
}

_Use_decl_annotations_
HRESULT Connection::NotifyBundleComplete(
    RealtimeStreaming::Common::PayloadType payloadType,
    RealtimeStreaming::Network::DataBundle dataBundle)
{
    Log(Log_Level_Info, L"ConnectionImpl::OnCompleteBundle\n");

    {
        slim_shared_lock_guard const guard(m_lock);

        if (FAILED(CheckClosed()))
        {
            return S_OK;
        }
    }

    DataBundleArgs args = DataBundleArgs(payloadType, *this, dataBundle);
    m_evtBundleReceived(*this, args);
}

// Callbacks
_Use_decl_annotations_
IAsyncAction Connection::OnHeaderReceived()
{
    // TODO: Troy lock? or pass in buffer via method entry

    Log(Log_Level_All, L"ConnectionImpl::OnHeaderReceived - Tid: %d \n", GetCurrentThreadId());

    HRESULT hr = S_OK;

    auto headerBufferImpl = m_spHeaderBuffer.as<implementation::DataBuffer>();
    UINT32 bytesRead = m_spHeaderBuffer.Length();
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
    if (m_receivedHeader.ePayloadType != PayloadType::Unknown)
    {
        // should not be receiving a header buffer when waiting for payload buffers
        // bad state; bail
        IFC(-1073740758); // STATUS_REQUEST_OUT_OF_SEQUENCE
    }

    // cast buffer to PayloadHeader*
    BYTE* pBuffer;
    headerBufferImpl->Buffer(&pBuffer);

    PayloadHeader* header = reinterpret_cast<PayloadHeader*>(pBuffer);
    IFC(nullptr != header ? S_OK : E_UNEXPECTED);

    // is header type in a range we understand
    if (header->ePayloadType == PayloadType::Unknown
        ||
        header->ePayloadType > PayloadType::ENDOFLIST)
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

    IFC(ProcessHeaderBuffer(header, m_spHeaderBuffer));

done:
    if (m_concurrentFailedBuffers > c_cbMaxBufferFailures)
    {
        LOG_RESULT(hr);
        Close();
    }
    else
    {
        return WaitForHeader(); // go back to waiting for header
    }
}

_Use_decl_annotations_
Windows::Foundation::IAsyncAction Connection::OnPayloadReceived(IBuffer payloadBuffer)
{
    Log(Log_Level_All, L"ConnectionImpl::OnPayloadReceived\n");

    NULL_THROW(payloadBuffer);

    DataBuffer payloadDataBuffer = DataBuffer(payloadBuffer.Capacity());
    payloadBuffer.as<DataBuffer>(payloadDataBuffer);

    UINT32 bytesRead = payloadDataBuffer.Length();
    DWORD bufferSize = payloadDataBuffer.CurrentLength();

    // makes sure this is the expected size
    if (c_cbMaxBundleSize < m_receivedHeader.cbPayloadSize ||
        bytesRead > m_receivedHeader.cbPayloadSize ||
        bytesRead == 0 ||
        bytesRead != bufferSize)
    {
        m_concurrentFailedBundles++;

        goto done;
    }

    // still have a valid payload type
    if (m_receivedHeader.ePayloadType == PayloadType::Unknown ||
        m_receivedHeader.ePayloadType >= PayloadType::ENDOFLIST)
    {
        m_concurrentFailedBundles++;

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
        payloadDataBuffer.TrimRight(expectedSize);
    };

    // add the buffer to the bundle
    m_receivedBundle.AddBuffer(payloadDataBuffer);

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

    if (m_concurrentFailedBundles > c_cbMaxBundleFailures)
    {
        LOG_RESULT(HRESULT_FROM_WIN32(ERROR_CONNECTION_UNAVAIL));
        Close();
    }
    else
    {
        return WaitForHeader();
    }
}

