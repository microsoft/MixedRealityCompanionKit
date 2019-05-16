// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "Connection.h"
#include "DataBuffer.h"
#include "DataBundle.h"
#include "DataBundleArgs.h"

#include <robuffer.h>

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
    Log(Log_Level_Info, L"Connection::Connection - Tid: %d \n", GetCurrentThreadId());

    slim_lock_guard guard(m_lock);

    m_streamSocket = socket;

    ZeroMemory(&m_receivedHeader, sizeof(PayloadHeader));
    m_receivedHeader.ePayloadType = PayloadType::Unknown;

    // Create background task
    RunSocketLoop();
}

_Use_decl_annotations_
Connection::~Connection()
{
    Log(Log_Level_Info, L"Connection::~Connection()\n");

    Close();
}

winrt::fire_and_forget Connection::RunSocketLoop()
{
    Log(Log_Level_Verbose, L"Connection::RunSocketLoop - Tid: %d \n", GetCurrentThreadId());

    // If our socket is closed, then do not run read loop
    if (FAILED(CheckClosed())) return;

    try
    {
        auto inputStream = m_streamSocket.InputStream();

        // Read in the batch of data (header & payload)
        Buffer headerBuffer(sizeof(PayloadHeader));
        co_await inputStream.ReadAsync(headerBuffer, headerBuffer.Capacity(), InputStreamOptions::None);

        HRESULT hr = OnHeaderReceived(headerBuffer);

        if (SUCCEEDED(hr))
        {
            // If there is a payload associated with this header, read & process that 
            if (m_receivedHeader.cbPayloadSize != 0)
            {
                Buffer payloadBuffer(m_receivedHeader.cbPayloadSize);

                co_await inputStream.ReadAsync(payloadBuffer, payloadBuffer.Capacity(), InputStreamOptions::None);

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

        // Re-run loop to listen for next batch of data
        RunSocketLoop();
    }
    catch (winrt::hresult_error const& ex)
    {
        CloseOnDisconnectedSocketError(ex.code());

        Log(Log_Level_Error, L"Connection::RunSocketLoop Exception Thrown - Tid: %d \n", GetCurrentThreadId());
        LOG_RESULT_MSG(ex.to_abi(), ex.message().data());
    }
}

_Use_decl_annotations_
void Connection::Close()
{
    slim_lock_guard guard(m_lock);

    Log(Log_Level_Info, L"Connection::Close()\n");

    if (m_streamSocket == nullptr)
    {
        Log(Log_Level_Info, L"Connection::Close - Already closed()\n");
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
    Log(Log_Level_Verbose, L"Connection::add_Disconnected() - Tid: %d \n", GetCurrentThreadId());
    slim_lock_guard guard(m_eventLock);
    return m_evtDisconnected.add(handler);
}

_Use_decl_annotations_
void Connection::Disconnected(winrt::event_token const& token) noexcept
{
    Log(Log_Level_Info, L"Connection::remove_Disconnected()\n");
    slim_lock_guard guard(m_eventLock);
    m_evtDisconnected.remove(token);
}

_Use_decl_annotations_
event_token Connection::Received(
    EventHandler<RealtimeStreaming::Network::DataBundleArgs> const& handler)
{
    Log(Log_Level_Info, L"Connection::add_Received()\n");
    slim_lock_guard guard(m_eventLock);
    return m_evtBundleReceived.add(handler);
}

_Use_decl_annotations_
void Connection::Received(winrt::event_token const& token) noexcept
{
    Log(Log_Level_Info, L"Connection::remove_Received()\n");
    slim_lock_guard guard(m_eventLock);
    m_evtBundleReceived.remove(token);
}

_Use_decl_annotations_
IAsyncAction Connection::SendPayloadTypeAsync(
    PayloadType payloadType)
{
    Log(Log_Level_Info, L"Connection::SendPayloadTypeAsync(%d) - Tid: %d \n", payloadType, GetCurrentThreadId());

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

    // If socket is closed, we cannot send anything
    if (FAILED(CheckClosed())) return;

    try
    {
        UINT32 totalLength = 0, bufferCount = 0;

        auto dataBundleImpl = dataBundle.as<implementation::DataBundle>();
        for (auto const& currBuffer : dataBundleImpl->GetBuffers())
        {
            totalLength += currBuffer.Length();
        }

        auto outStream = m_streamSocket.OutputStream();
        for (auto const& currBuffer : dataBundleImpl->GetBuffers())
        {
            co_await outStream.WriteAsync(currBuffer);
            bufferCount++;
        }

        Log(Log_Level_Info, L"Connection::SendBundleAsync() wrote - Buffers: %d - TotalLength(B): %d - TID: %d\n", bufferCount, totalLength, GetCurrentThreadId());
    }
    catch (winrt::hresult_error const& ex)
    {
        CloseOnDisconnectedSocketError(ex.code());

        Log(Log_Level_Error, L"Connection::SendBundleAsync Exception Thrown - Tid: %d \n", GetCurrentThreadId());
        LOG_RESULT_MSG(ex.to_abi(), ex.message().data());
    }
    catch (...)
    {
        Log(Log_Level_Error, L"Connection::SendBundleAsync Exception Thrown - Tid: %d - HRESULT=%d \n", GetCurrentThreadId(), winrt::to_hresult());
    }

    Log(Log_Level_Info, L"Connection::SendBundleAsync() end - Tid: %d \n", GetCurrentThreadId());
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
    Log(Log_Level_Verbose, L"Connection::OnHeaderReceived() - Tid: %d \n", GetCurrentThreadId());

    auto headerBufferImpl = headerBuffer.as<IBufferByte>();
    NULL_CHK(headerBufferImpl);

    HRESULT hr = S_OK;

    UINT32 bytesRead = headerBuffer.Length();
    DWORD bufferSize = headerBuffer.Capacity();

    // makes sure this is the expected size
    if (bytesRead != sizeof(PayloadHeader) ||
        bytesRead != bufferSize)
    {
        Log(Log_Level_Error, L"Connection::OnHeaderReceived() - Invalid read size \n");
        m_concurrentFailedBuffers++;
        goto done;
    }

    // ensure we are waiting for a new payload
    if (m_receivedHeader.ePayloadType != PayloadType::Unknown)
    {
        // should not be receiving a header buffer when waiting for payload buffers
        // bad state; bail
        Log(Log_Level_Error, L"Connection::OnHeaderReceived() - Invalid request received \n");
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
        Log(Log_Level_Error, L"Connection::OnHeaderReceived() - Invalid payload header \n");
        m_concurrentFailedBuffers++;
        goto done;
    }

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
    Log(Log_Level_Verbose, L"Connection::OnPayloadReceived\n");

    NULL_CHK(payloadBuffer);

    UINT32 bytesRead = payloadBuffer.Length();

    // Copy IBuffer data into DataBuffer
    auto payloadDataBuffer = make<Network::implementation::DataBuffer>(payloadBuffer);
    auto receivedBundle = winrt::make<Network::implementation::DataBundle>(payloadDataBuffer);

    DWORD bufferSize = payloadDataBuffer.CurrentLength();

    if (c_cbMaxBundleSize < m_receivedHeader.cbPayloadSize ||
        bytesRead > m_receivedHeader.cbPayloadSize ||
        bytesRead == 0 ||
        bytesRead != bufferSize ||
        m_receivedHeader.ePayloadType == PayloadType::Unknown ||
        m_receivedHeader.ePayloadType >= PayloadType::ENDOFLIST)
    {
        Log(Log_Level_Error, L"Connection::OnPayloadReceived() - Payload read error \n");
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

void Connection::CloseOnDisconnectedSocketError(HRESULT hResult)
{
    auto code = HRESULT_CODE(hResult);

    if ((code == WSAECONNABORTED) || (code == WSAECONNRESET) || (hResult == MF_E_SHUTDOWN))
    {
        // The connection has gone, we need to tidy ourselves up.
        Close();
    }
}


