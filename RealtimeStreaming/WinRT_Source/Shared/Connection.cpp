// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "Connection.h"
#include "DataBuffer.h"
#include "DataBundle.h"
#include "DataBundleArgs.h"

#include <robuffer.h>

#define PARTIAL_PAYLOAD_RECEIVED MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_INTERNET, 0x0967)

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Networking::Sockets;
using namespace winrt::Windows::Storage::Streams;

using namespace RealtimeStreaming::Network::implementation;
using namespace RealtimeStreaming::Common;

using IBufferByte = ::Windows::Storage::Streams::IBufferByteAccess;

using namespace concurrency;

_Use_decl_annotations_
Connection::Connection(_In_ winrt::Windows::Networking::Sockets::StreamSocket const& socket)
    : m_concurrentFailedBuffers(0)
    , m_concurrentFailedBundles(0)
    , m_streamSocket(socket)
    , m_dataReader(socket.InputStream())
    , m_receivedBundle(nullptr)
{
    Log(Log_Level_All, L"Connection::Connection - Tid: %d \n", GetCurrentThreadId());

    slim_lock_guard guard(m_lock);

    ZeroMemory(&m_receivedHeader, sizeof(PayloadHeader));
    m_receivedHeader.ePayloadType = PayloadType::Unknown;

    Log(Log_Level_All, L"Connection::Connection - ISS: %d\n", m_dataReader.InputStreamOptions());

    //m_dataReader.InputStreamOptions(Windows::Storage::Streams::InputStreamOptions::Partial);

    // Create background task
    RunSocketLoop();
}

_Use_decl_annotations_
Connection::~Connection()
{
    slim_lock_guard guard(m_lock);

    Log(Log_Level_Warning, L"Connection::~Connection()\n");

    Close();
}

void Connection::ReadPayloadLoop()
{
    Log(Log_Level_All, L"Connection::ReadPayloadLoopAsync[%d] - Tid: %d \n", m_streamSocket.Information().LocalPort(), GetCurrentThreadId());

    IBuffer buffer = WaitForPayloadAsync().get();

    if (buffer.Length() != 0)
    {
        HRESULT hr = OnPayloadReceived(buffer);

        if (hr == PARTIAL_PAYLOAD_RECEIVED)
        {
            // We have not read all of the payload yet
            ReadPayloadLoop();
        }
        // whether we succed or fail otherwise, we want to restart SocketReceiveLoop()
    }
}

winrt::fire_and_forget Connection::RunSocketLoop()
{
    Log(Log_Level_All, L"Connection::RunSocketLoop[%d] - Tid: %d \n", m_streamSocket.Information().LocalPort(), GetCurrentThreadId());

    co_await winrt::resume_background();

    Log(Log_Level_All, L"Connection::RunSocketLoop - resume_background - Tid: %d \n", GetCurrentThreadId());

    auto headerBuffer{ co_await WaitForHeaderAsync() };

    if (headerBuffer == nullptr)
    {
        // TODO: Close and cancel loop?
        return;
    }

    Log(Log_Level_All, L"Connection::RunSocketLoop - UnConsumed: %d - Tid: %d \n", 
        m_dataReader.UnconsumedBufferLength(), 
        GetCurrentThreadId());

    HRESULT hr = OnHeaderReceived(headerBuffer);

    if (SUCCEEDED(hr))
    {
        //co_await ReadPayloadLoopAsync();
        ReadPayloadLoop();
    }
    // if failed, then we proceed to continuation which will restart loop (i.e call WaitForHeaderAsync())

    RunSocketLoop();
}

_Use_decl_annotations_
void Connection::Close()
{
    Log(Log_Level_Info, L"Connection::Close()\n");

    if (nullptr == m_streamSocket)
    {
        return;
    }
    
    ResetBundle();

    // cleanup socket
    m_streamSocket.Close();
    m_streamSocket = nullptr;

    // Fire disconnect event
    m_evtDisconnected();
}

_Use_decl_annotations_
bool Connection::IsConnected()
{
    Log(Log_Level_Info, L"Connection::get_IsConnected()\n");

    //auto lock = _lock.Lock();
    slim_shared_lock_guard const guard(m_lock);

    return nullptr != m_streamSocket;
}
_Use_decl_annotations_
StreamSocketInformation Connection::ConnectionInfo()
{
    Log(Log_Level_Info, L"Connection::get_ConnectionInfo()\n");

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
    Log(Log_Level_All, L"Connection::add_Disconnected() - Tid: %d \n", GetCurrentThreadId());

    slim_lock_guard guard(m_lock);

    return m_evtDisconnected.add(handler);
}

_Use_decl_annotations_
void Connection::Disconnected(winrt::event_token const& token) noexcept
{
    Log(Log_Level_Info, L"Connection::remove_Disconnected()\n");

    slim_lock_guard guard(m_lock);

    m_evtDisconnected.remove(token);
}

_Use_decl_annotations_
event_token Connection::Received(
    EventHandler<RealtimeStreaming::Network::DataBundleArgs> const& handler)
{
    Log(Log_Level_Info, L"Connection::add_Received()\n");

    slim_lock_guard guard(m_lock);

    return m_evtBundleReceived.add(handler);
}

_Use_decl_annotations_
void Connection::Received(winrt::event_token const& token) noexcept
{
    Log(Log_Level_Info, L"Connection::remove_Received()\n");

    slim_lock_guard guard(m_lock);

    m_evtBundleReceived.remove(token);
}

_Use_decl_annotations_
IAsyncAction Connection::SendPayloadTypeAsync(
    PayloadType payloadType)
{
    // TODO: Consider switch to co_await for sendbundle at bottom
    // TODO: Also re-write this to direct write one buffer instead of creating DataBundle

    Log(Log_Level_All, L"Connection::SendPayloadTypeAsync(%d)[%d] - Tid: %d \n",  payloadType, m_streamSocket.Information().LocalPort(), GetCurrentThreadId());

    // Create send buffer
    auto dataBuffer = winrt::make<Network::implementation::DataBuffer>(sizeof(PayloadHeader));

    // cast to the dataPtr
    PayloadHeader* pOpHeader = reinterpret_cast<PayloadHeader*>(DataBuffer::GetBufferPointer(dataBuffer));
    NULL_THROW(pOpHeader);

    pOpHeader->cbPayloadSize = 0;
    pOpHeader->ePayloadType = payloadType;

    // Since we set new data into the buffer, we need to indicate how much data was written
    dataBuffer.CurrentLength(sizeof(PayloadHeader));

    auto dataBundle = winrt::make<Network::implementation::DataBundle>(dataBuffer);
    //dataBundle.AddBuffer(dataBuffer);

    co_await SendBundleAsync(dataBundle);
}

_Use_decl_annotations_
IAsyncAction Connection::SendBundleAsync(
    RealtimeStreaming::Network::DataBundle const dataBundle)
{
    co_await winrt::resume_background();

    Log(Log_Level_Info, L"Connection::SendBundleAsync()[%d] - Tid: %d \n", m_streamSocket.Information().LocalPort(), GetCurrentThreadId());

    try
    {

    // get the output stream for socket
    DataWriter dataWriter{ nullptr };
    {
        slim_lock_guard guard(m_lock);

        IFT(CheckClosed());
        dataWriter = DataWriter(m_streamSocket.OutputStream());
    }

    UINT32 totalLength = 0;
    UINT32 bufferCount = 0;

    std::vector< IAsyncOperationWithProgress< unsigned int, unsigned int > > pendingWrites{};

    auto dataBundleImpl = dataBundle.as<implementation::DataBundle>();
    for (auto const& currBuffer : dataBundleImpl->GetBuffers())
    {
        totalLength += currBuffer.Length();

        // TODO: Create cleaner way to send data?
        // TOOD: Re-try multiple packets flushasync approach*

        BYTE* pBuffer = DataBuffer::GetBufferPointer(currBuffer);

        // a view over(and not a copy of) the input data
        array_view<byte const> bufferArrayView{ pBuffer, pBuffer + currBuffer.Length() };

        dataWriter.WriteBytes(bufferArrayView);
        co_await dataWriter.StoreAsync();
        //outputStream.WriteAsync(currBuffer).get();
        //pendingWrites.push_back(outputStream.WriteAsync(currBuffer));

        bufferCount++;
    }

    //co_await dataWriter.StoreAsync();
    //co_await dataWriter.FlushAsync();
    //co_await outputStream.FlushAsync();
    dataWriter.DetachStream();

    Log(Log_Level_Info, L"Connection::SendBundleAsync() - Buffers: %d - TotalLength: %d - TID: %d\n", bufferCount, totalLength, GetCurrentThreadId());

    }
    catch (winrt::hresult_error const& ex)
    {
        Log(Log_Level_All, L"Connection::SendBundleAsync Exception Thrown - Tid: %d \n", GetCurrentThreadId());
        LOG_RESULT_MSG(ex.to_abi(), ex.message().data());
    }
}


// IConnectionInternal
_Use_decl_annotations_
IAsyncOperation<IBuffer> Connection::WaitForHeaderAsync()
{
    Log(Log_Level_All, L"Connection::WaitForHeaderAsync()[%d] - Tid: %d \n", m_streamSocket.Information().LocalPort(), GetCurrentThreadId());

    if (FAILED(CheckClosed()))
    {
        return nullptr;
    }

    if (PayloadType::Unknown != m_receivedHeader.ePayloadType ||
        0 != m_receivedHeader.cbPayloadSize)
    {
        ResetBundle();
    }

    try
    {
        // TODO: Optimize this by re-using local member buffer instead of consistent construction?
        Buffer tempBuffer = Buffer(sizeof(PayloadHeader));
        UINT32 bufferLen = tempBuffer.Capacity();

        UINT32 bytesLoaded = co_await m_dataReader.LoadAsync(bufferLen);

        Log(Log_Level_All, L"Connection::WaitForHeaderAsync() Unconsumed: %d - Tid: %d \n", m_dataReader.UnconsumedBufferLength(), GetCurrentThreadId());
        Log(Log_Level_All, L"Connection::WaitForHeaderAsync() Loaded: %d - Tid: %d \n", bytesLoaded, GetCurrentThreadId());
        // UGHGHGHGGH

        return m_dataReader.ReadBuffer(bufferLen);

        /*
        // get the socket input stream reader
        auto bufferResult = co_await m_streamSocket.InputStream().ReadAsync(tempBuffer,
                bufferLen,
                InputStreamOptions::Partial);

        return bufferResult;
        */
    }
    catch (winrt::hresult_error const& ex)
    {
        // TODO: close?
        Log(Log_Level_All, L"Connection::WaitForHeader Exception Thrown - Tid: %d \n", GetCurrentThreadId());
        LOG_RESULT_MSG(ex.to_abi(), ex.message().data());
    }

    return nullptr;
}

_Use_decl_annotations_
IAsyncOperation<IBuffer> Connection::WaitForPayloadAsync()
{
    Log(Log_Level_Info, L"Connection::WaitForPayloadAsync()[%d]\n", m_streamSocket.Information().LocalPort());

    if (FAILED(CheckClosed()))
    {
        return nullptr;
    }

    if (PayloadType::Unknown == m_receivedHeader.ePayloadType ||
        PayloadType::ENDOFLIST <= m_receivedHeader.ePayloadType ||
        0 == m_receivedHeader.cbPayloadSize)
    {
        IFT(HRESULT_FROM_WIN32(ERROR_INVALID_STATE));
    }

    try
    {
        // TODO: Optimize this by re-using local member buffer instead of consistent construction?
        Buffer tempBuffer = Buffer(m_receivedHeader.cbPayloadSize);
        UINT32 bufferLen = tempBuffer.Capacity();

        co_await m_dataReader.LoadAsync(bufferLen);
        return m_dataReader.ReadBuffer(bufferLen);
    }
    catch (winrt::hresult_error const& ex)
    {
        Log(Log_Level_All, L"Connection::WaitForPayloadAsync Exception Thrown - Tid: %d \n", GetCurrentThreadId());
        LOG_RESULT_MSG(ex.to_abi(), ex.message().data());
    }

    return nullptr;
    //co_await OnPayloadReceived(asyncInfo);
}

_Use_decl_annotations_
void Connection::ResetBundle()
{
    Log(Log_Level_Info, L"Connection::ResetBundle()\n");

    if (nullptr != m_receivedBundle)
    {
        m_receivedBundle.Reset();
    }

    ZeroMemory(&m_receivedHeader, sizeof(PayloadHeader));
}

_Use_decl_annotations_
HRESULT Connection::ProcessHeaderBuffer(
    RealtimeStreaming::Common::PayloadHeader* header,
    IBuffer dataBuffer)
{
    NULL_CHK(header);

    // TODO: Figure out how to transform IBuffer to DataBuffer. May need to just copy bytes? wasteful
    //Network::DataBuffer db = dataBuffer.as<Network::DataBuffer>();
    //auto dataBundle = winrt::make<Network::implementation::DataBundle>(db);
    auto dataBundle = winrt::make<Network::implementation::DataBundle>();

    IFR(NotifyBundleComplete(header->ePayloadType, dataBundle));

    return S_OK;
}

_Use_decl_annotations_
HRESULT Connection::NotifyBundleComplete(
    RealtimeStreaming::Common::PayloadType payloadType,
    RealtimeStreaming::Network::DataBundle dataBundle)
{
    Log(Log_Level_Info, L"Connection::OnCompleteBundle\n");

    {
        slim_shared_lock_guard const guard(m_lock);

        if (FAILED(CheckClosed()))
        {
            return S_OK;
        }
    }

    auto args = winrt::make<Network::implementation::DataBundleArgs>(payloadType, *this, dataBundle);

    m_evtBundleReceived(*this, args);
}

// Callbacks
_Use_decl_annotations_
HRESULT Connection::OnHeaderReceived(IBuffer const& headerBuffer)
{
    Log(Log_Level_All, L"Connection::OnHeaderReceived()[%d] - Tid: %d \n", m_streamSocket.Information().LocalPort(), GetCurrentThreadId());

    auto headerBufferImpl = headerBuffer.as<IBufferByte>();
    NULL_CHK(headerBufferImpl);

    HRESULT hr = E_FAIL; // assume we fail 

    UINT32 bytesRead = headerBuffer.Length();
    DWORD bufferSize = headerBuffer.Capacity();
    
    // makes sure this is the expected size
    if (bytesRead != sizeof(PayloadHeader) ||
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
    if (header->ePayloadType == PayloadType::Unknown ||
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
            goto done;
        }
        else
        {
            // store the payload header info
            m_receivedHeader = *header;

            // start the process to receive payload data
            //return WaitForPayload();
            return S_OK;
        }
    }

    m_concurrentFailedBuffers = 0;

    IFC(ProcessHeaderBuffer(header, headerBuffer));

done:
    if (m_concurrentFailedBuffers > c_cbMaxBufferFailures)
    {
        IFT(hr);
        //LOG_RESULT(hr);
        //Close();
    }

    return hr; 
    //return WaitForHeader(); // go back to waiting for header
}

_Use_decl_annotations_
HRESULT Connection::OnPayloadReceived(IBuffer const& payloadBuffer)
{
    Log(Log_Level_All, L"Connection::OnPayloadReceived\n");

    NULL_CHK(payloadBuffer);

    UINT32 bytesRead = payloadBuffer.Length();

    // Copy IBuffer data into DataBuffer
    auto payloadDataBuffer = make<Network::implementation::DataBuffer>(payloadBuffer);
    //auto payloadDataBuffer = payloadBuffer.as<RealtimeStreaming::Network::DataBuffer>();
    NULL_CHK(payloadDataBuffer);

    HRESULT hr = S_OK;

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

    // create bundle to hold all buffers, lazy load
    if (nullptr == m_receivedBundle)
    {
        m_receivedBundle = winrt::make<Network::implementation::DataBundle>();
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
        return PARTIAL_PAYLOAD_RECEIVED;
        //return WaitForPayload();
    }

    // store the bundle data to be used for notification
    LOG_RESULT(NotifyBundleComplete(m_receivedHeader.ePayloadType, m_receivedBundle));

done:
    ResetBundle();

    if (m_concurrentFailedBundles > c_cbMaxBundleFailures)
    {
        IFT(HRESULT_FROM_WIN32(ERROR_CONNECTION_UNAVAIL));
    }

    return hr;
    /*
    if (m_concurrentFailedBundles > c_cbMaxBundleFailures)
    {
        LOG_RESULT(HRESULT_FROM_WIN32(ERROR_CONNECTION_UNAVAIL));
        Close();
    }
    else
    {
        return WaitForHeader();
    }*/
}

