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

using namespace concurrency;

using IBufferByte = ::Windows::Storage::Streams::IBufferByteAccess;
using implDataBuffer = winrt::RealtimeStreaming::Network::implementation::DataBuffer;

const UINT32 c_cbPayloadHeaderSize = sizeof(PayloadHeader);

/* static helper */
RealtimeStreaming::Network::DataBuffer Connection::CreatePayloadHeaderBuffer(
    _In_ RealtimeStreaming::Common::PayloadType payloadType,
    _In_ UINT32 cbPayloadSize)
{
    auto headerDataBuffer = winrt::make<implDataBuffer>(c_cbPayloadHeaderSize);
    headerDataBuffer.CurrentLength(c_cbPayloadHeaderSize);

    // get the buffer pointer
    BYTE* pHeaderBuffer = implDataBuffer::GetBufferPointer(headerDataBuffer);

    // Prepare payload header 8 bytes - type and size of payload to follow
    PayloadHeader* pOpHeader = reinterpret_cast<PayloadHeader*>(pHeaderBuffer);
    NULL_THROW(pOpHeader);
    pOpHeader->ePayloadType = payloadType;
    pOpHeader->cbPayloadSize = cbPayloadSize;

    return headerDataBuffer;
}

_Use_decl_annotations_
Connection::Connection(_In_ winrt::Windows::Networking::Sockets::StreamSocket const socket)
    : m_concurrentFailedBuffers(0)
    , m_concurrentFailedBundles(0)
{
    Log(Log_Level_All, L"Connection::Connection - Tid: %d \n", GetCurrentThreadId());

    slim_lock_guard guard(m_lock);

    m_streamSocket = socket;
    m_dataReader = DataReader(socket.InputStream());

    ZeroMemory(&m_receivedHeader, sizeof(PayloadHeader));
    m_receivedHeader.ePayloadType = PayloadType::Unknown;

    Log(Log_Level_All, L"Connection::Connection - ISS: %d\n", m_dataReader.InputStreamOptions());

    // Create background task
    RunSocketLoop();
}

_Use_decl_annotations_
Connection::~Connection()
{
    Log(Log_Level_Warning, L"Connection::~Connection()\n");

    Close();
}

winrt::fire_and_forget Connection::RunSocketLoop()
{
    Log(Log_Level_All, L"Connection::RunSocketLoop - Tid: %d \n", GetCurrentThreadId());

    //co_await winrt::resume_background();
    //Log(Log_Level_All, L"Connection::RunSocketLoop - resume_background - Tid: %d \n", GetCurrentThreadId());

    try
    {
        IFT(CheckClosed());

        // Read size of next batch of data to process
        co_await m_dataReader.LoadAsync(sizeof(UINT32));
        UINT32 cbData = m_dataReader.ReadUInt32();

        // Read in the batch of data (header & payload)
        co_await m_dataReader.LoadAsync(cbData);
        auto headerBuffer = m_dataReader.ReadBuffer(sizeof(PayloadHeader));

        HRESULT hr = OnHeaderReceived(headerBuffer);
        if (SUCCEEDED(hr))
        {
            // If there is a payload associated with this header, read & process that 
            if (m_receivedHeader.cbPayloadSize != 0)
            {
                auto payloadBuffer = m_dataReader.ReadBuffer(m_receivedHeader.cbPayloadSize);

                hr = OnPayloadReceived(payloadBuffer);
            }
            else
            {
                // We received no payload, so push on notification
                NotifyBundleComplete(m_receivedHeader.ePayloadType, nullptr);
            }
        }

        // Reset header for next read
        ZeroMemory(&m_receivedHeader, sizeof(PayloadHeader));
        m_receivedHeader.ePayloadType = PayloadType::Unknown;
    }
    catch (winrt::hresult_error const& ex)
    {
        Log(Log_Level_All, L"Connection::RunSocketLoop Exception Thrown - Tid: %d \n", GetCurrentThreadId());
        LOG_RESULT_MSG(ex.to_abi(), ex.message().data());
        
        Close();
        return;
    }

    // Re-run loop to listen for next batch of data
    RunSocketLoop();
}

_Use_decl_annotations_
void Connection::Close()
{
    slim_lock_guard guard(m_lock);

    Log(Log_Level_Info, L"Connection::Close()\n");

    if (nullptr == m_streamSocket)
    {
        return;
    }

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
    Log(Log_Level_All, L"Connection::SendPayloadTypeAsync(%d) - Tid: %d \n",  payloadType, GetCurrentThreadId());

    auto headerDataBuffer = Connection::CreatePayloadHeaderBuffer(payloadType, 0);

    auto dataBundle = winrt::make<Network::implementation::DataBundle>(headerDataBuffer);

    co_await SendBundleAsync(dataBundle);
}

_Use_decl_annotations_
IAsyncAction Connection::SendBundleAsync(
    RealtimeStreaming::Network::DataBundle const dataBundle)
{
    co_await winrt::resume_background();

    Log(Log_Level_Info, L"Connection::SendBundleAsync() - Tid: %d \n", GetCurrentThreadId());
    
    /*
    {
        // Check that we still have an active socket
        slim_lock_guard guard(m_lock);
        IFT(CheckClosed());
    }
    */

    // TODO: remove try/catch?
    try
    {
        DataWriter dataWriter{ m_streamSocket.OutputStream() };
        UINT32 totalLength = 0, bufferCount = 0;

        //std::vector< IAsyncOperationWithProgress< unsigned int, unsigned int > > pendingWrites{};

        auto dataBundleImpl = dataBundle.as<implementation::DataBundle>();
        for (auto const& currBuffer : dataBundleImpl->GetBuffers())
        {
            totalLength += currBuffer.Length();
        }

        // Write size of data coming
        dataWriter.WriteUInt32(totalLength);

        for (auto const& currBuffer : dataBundleImpl->GetBuffers())
        {
            BYTE* pBuffer = DataBuffer::GetBufferPointer(currBuffer);

            // a view over(and not a copy of) the input data
            array_view<byte const> bufferArrayView{ pBuffer, pBuffer + currBuffer.Length() };

            dataWriter.WriteBytes(bufferArrayView);
            //pendingWrites.push_back(outputStream.WriteAsync(currBuffer));

            bufferCount++;
        }

        co_await dataWriter.StoreAsync();
        co_await dataWriter.FlushAsync();

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
void Connection::NotifyBundleComplete(
    RealtimeStreaming::Common::PayloadType payloadType,
    RealtimeStreaming::Network::DataBundle dataBundle)
{
    Log(Log_Level_Info, L"Connection::OnCompleteBundle\n");

    auto args = winrt::make<Network::implementation::DataBundleArgs>(payloadType, *this, dataBundle);

    m_evtBundleReceived(*this, args);
}

// Callbacks
_Use_decl_annotations_
HRESULT Connection::OnHeaderReceived(IBuffer const& headerBuffer)
{
    Log(Log_Level_All, L"Connection::OnHeaderReceived() - Tid: %d \n", GetCurrentThreadId());

    auto headerBufferImpl = headerBuffer.as<IBufferByte>();
    NULL_CHK(headerBufferImpl);

    HRESULT hr = S_OK;

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

    // todo: crc checks
    // can we trust the payload size?
    if (header->cbPayloadSize > c_cbMaxBundleSize)
    {
        // just incase it was corrupt we wait for the next one
        m_concurrentFailedBuffers++;
        goto done;
    }

    // store the payload header info
    m_receivedHeader = *header;
    m_concurrentFailedBuffers = 0;

done:
    if (m_concurrentFailedBuffers > c_cbMaxBufferFailures)
    {
        // Throw HRESULT so we can close socket due to multiple failed buffers
        IFT(hr);
    }

    return hr; 
}

_Use_decl_annotations_
HRESULT Connection::OnPayloadReceived(IBuffer const& payloadBuffer)
{
    Log(Log_Level_All, L"Connection::OnPayloadReceived\n");

    NULL_CHK(payloadBuffer);

    UINT32 bytesRead = payloadBuffer.Length();

    // Copy IBuffer data into DataBuffer
    auto payloadDataBuffer = make<Network::implementation::DataBuffer>(payloadBuffer);
    auto receivedBundle = winrt::make<Network::implementation::DataBundle>(payloadDataBuffer);

    DWORD bufferSize = payloadDataBuffer.CurrentLength();

    // TODO: Look closer at what to do in these cases
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

    // store the bundle data to be used for notification
    NotifyBundleComplete(m_receivedHeader.ePayloadType, receivedBundle);

done:
    if (m_concurrentFailedBundles > c_cbMaxBundleFailures)
    {
        IFT(HRESULT_FROM_WIN32(ERROR_CONNECTION_UNAVAIL));
    }

    return S_OK;
}

