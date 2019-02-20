// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "Connection.h"
#include "DataBuffer.h"
#include "DataBundle.h"
#include "DataBundleArgs.h"

#define PARTIAL_PAYLOAD_RECEIVED MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_INTERNET, 0x0967)

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Networking::Sockets;
using namespace winrt::Windows::Storage::Streams;

using namespace RealtimeStreaming::Network::implementation;
using namespace RealtimeStreaming::Common;

using namespace concurrency;

_Use_decl_annotations_
Connection::Connection(_In_ Windows::Networking::Sockets::StreamSocket const& socket)
    : m_concurrentFailedBuffers(0)
    , m_concurrentFailedBundles(0)
    , m_streamSocket(socket)
    , m_receivedBundle(nullptr)
{
    Log(Log_Level_All, L"Connection::Connection - Tid: %d \n", GetCurrentThreadId());

    //auto lock = _lock.Lock();
    slim_lock_guard guard(m_lock);

    ZeroMemory(&m_receivedHeader, sizeof(PayloadHeader));
    m_receivedHeader.ePayloadType = PayloadType::Unknown;

    // Create background task
    RunSocketLoop();
}

_Use_decl_annotations_
Connection::~Connection()
{
    slim_lock_guard guard(m_lock);

    Log(Log_Level_Warning, L"ConnectionImpl::~ConnectionImpl()\n");

    Close();
}

IAsyncAction Connection::ReadPayloadLoopAsync()
{
    co_await winrt::resume_background();

    IBuffer buffer = WaitForPayloadAsync().get();

    if (buffer.Length() != 0)
    {
        HRESULT hr = OnPayloadReceived(buffer);

        if (hr == PARTIAL_PAYLOAD_RECEIVED)
        {
            // We have not read all of the payload yet
            co_await ReadPayloadLoopAsync();
        }
        // whether we succed or fail otherwise, we want to restart SocketReceiveLoop()
    }

    /*
    return concurrency::create_task([=]()
    //.then([this](IBuffer result)
    {
        IBuffer buffer{ WaitForPayloadAsync().get() };

        if (buffer.Length() != 0)
        {
            HRESULT hr = OnPayloadReceived(buffer);
            if (hr == PARTIAL_PAYLOAD_RECEIVED)
            {
                // We have not read all of the payload yet
                return ReadPayloadLoopAsync();
            }
            // whether we succed or fail otherwise, we want to restart SocketReceiveLoop()
        }
        // else is null and we fail and restart back to WaitForHeader
    });*/
}

winrt::fire_and_forget Connection::RunSocketLoop()
{
    Log(Log_Level_All, L"Connection::RunSocketLoop - Tid: %d \n", GetCurrentThreadId());

    co_await winrt::resume_background();

    Log(Log_Level_All, L"Connection::RunSocketLoop - resume_background - Tid: %d \n", GetCurrentThreadId());

    co_await WaitForHeaderAsync();

    HRESULT hr = OnHeaderReceived();

    if (SUCCEEDED(hr))
    {
        co_await ReadPayloadLoopAsync();
    }
    // if failed, then we proceed to continuation which will restart loop (i.e call WaitForHeaderAsync())

    RunSocketLoop();
}

/*
void Connection::ProcessSocketLoop()
{
    // Returned HResults are failed siliently by restarting process to waitforheader
    // Thrown HResult exceptions will abort connection & close socket
    concurrency::create_task(WaitForHeaderAsync()).then([=]() 
    {
        HRESULT hr = OnHeaderReceived(); // uses m_headerBuffer, consider switching to local?
        
        if (SUCCEEDED(hr))
        {
            return ReadPayloadLoopAsync();
        }
        // if failed, then we proceed to continuation which will restart loop (i.e call WaitForHeaderAsync())
    }).then([=](concurrency::task<void> previousTask)
    {
        try 
        {
            previousTask.get();

            ProcessSocketLoop();
        }
        catch (winrt::hresult_error const& ex)
        {   
            LOG_RESULT_MSG(ex.to_abi(), ex.message().data());
            Close();
        }
        catch (concurrency::task_canceled&)
        {
            //close socket?
        }
    });
}*/

/*
void Connection::Bind(_In_ Windows::Networking::Sockets::StreamSocket const& socket)
{
    slim_lock_guard guard(m_lock);

    m_streamSocket = socket;

    WaitForHeader();
}
*/

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
    m_evtDisconnected();
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

    slim_lock_guard guard(m_lock);

    return m_evtDisconnected.add(handler);
}

_Use_decl_annotations_
void Connection::Disconnected(winrt::event_token const& token) noexcept
{
    Log(Log_Level_Info, L"ConnectionImpl::remove_Disconnected()\n");

    slim_lock_guard guard(m_lock);

    m_evtDisconnected.remove(token);
}

_Use_decl_annotations_
event_token Connection::Received(
    EventHandler<RealtimeStreaming::Network::DataBundleArgs> const& handler)
{
    Log(Log_Level_Info, L"ConnectionImpl::add_Received()\n");

    slim_lock_guard guard(m_lock);

    return m_evtBundleReceived.add(handler);
}

_Use_decl_annotations_
void Connection::Received(winrt::event_token const& token) noexcept
{
    Log(Log_Level_Info, L"ConnectionImpl::remove_Received()\n");

    slim_lock_guard guard(m_lock);

    m_evtBundleReceived.remove(token);
}

_Use_decl_annotations_
IAsyncAction Connection::SendPayloadTypeAsync(
    PayloadType payloadType)
{
    Log(Log_Level_All, L"ConnectionImpl::SendBundleAsync(%d) - Tid: %d \n", payloadType, GetCurrentThreadId());

    // Create send buffer
    auto dataBuffer = winrt::make<Network::implementation::DataBuffer>(sizeof(PayloadHeader));
    
    // get the buffer pointer
    BYTE* pBuffer;
    dataBuffer.as<DataBuffer>()->Buffer(&pBuffer);
    NULL_THROW(pBuffer);

    // cast to the dataPtr
    PayloadHeader* pOpHeader = reinterpret_cast<PayloadHeader*>(pBuffer);
    NULL_THROW(pOpHeader);

    pOpHeader->cbPayloadSize = 0;
    pOpHeader->ePayloadType = payloadType;

    dataBuffer.CurrentLength(sizeof(PayloadHeader));

    auto dataBundle = winrt::make<Network::implementation::DataBundle>(dataBuffer);
    //dataBundle.AddBuffer(dataBuffer);

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

        Log(Log_Level_Info, m_streamSocket.Information().LocalPort().data());
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

    Log(Log_Level_Info, L"Connection::SendBundleAsync() - Buffers: %d - TotalLength: %d\n", bufferCount, totalLength);
}


// IConnectionInternal
_Use_decl_annotations_
IAsyncAction Connection::WaitForHeaderAsync()
{
    Log(Log_Level_All, L"Connection::WaitForHeader - Tid: %d \n", GetCurrentThreadId());

    IFT(CheckClosed());

    if (PayloadType::Unknown != m_receivedHeader.ePayloadType ||
        0 != m_receivedHeader.cbPayloadSize)
    {
        ResetBundle();
    }

    try
    {

    //auto headerBuffer = winrt::make<Network::implementation::DataBuffer>(sizeof(PayloadHeader));
    m_spHeaderBuffer = nullptr; //reseat local member buffer
    m_spHeaderBuffer = winrt::make<Network::implementation::DataBuffer>(sizeof(PayloadHeader));

    DWORD bufferLen = m_spHeaderBuffer.Capacity();

    // should be the same as sizeof(PayloadHeader)
    if (bufferLen != sizeof(PayloadHeader))
    {
        m_spHeaderBuffer.CurrentLength(sizeof(PayloadHeader));
    }

    // get the socket input stream reader
    IInputStream inputStream = m_streamSocket.InputStream();
    
    Log(Log_Level_Info, m_streamSocket.Information().LocalPort().data());
    Log(Log_Level_Info, L" - ");
    Log(Log_Level_Info, m_streamSocket.Information().RemotePort().data());
    Log(Log_Level_Info, L"\n");

    Buffer test = Buffer(bufferLen);
    auto bufferResult = co_await inputStream.ReadAsync(test,
            bufferLen,
            InputStreamOptions::Partial);

    m_spHeaderBuffer = nullptr; //unseat current header buffer
    m_spHeaderBuffer = bufferResult.as<RealtimeStreaming::Network::DataBuffer>();

    }
    catch (winrt::hresult_error const& ex)
    {
        LOG_RESULT_MSG(ex.to_abi(), ex.message().data());
        int a = 4;
    }

    //co_await OnHeaderReceived();
}

_Use_decl_annotations_
IAsyncOperation<IBuffer> Connection::WaitForPayloadAsync()
{
    Log(Log_Level_Info, L"ConnectionImpl::WaitForPayload()\n");

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

    auto payloadBuffer = winrt::make<Network::implementation::DataBuffer>(m_receivedHeader.cbPayloadSize);

    // get the underlying buffer and makes sure it the right size
    com_ptr<IMFMediaBuffer> spMediaBuffer;
    payloadBuffer.as<DataBuffer>()->GetMediaBuffer(spMediaBuffer.put());
    //IFT(payloadBuffer.MediaBuffer(&spMediaBuffer));

    DWORD bufferLen = 0;
    IFT(spMediaBuffer->GetMaxLength(&bufferLen));

    // should be the same as sizeof(PayloadHeader)
    if (bufferLen != m_receivedHeader.cbPayloadSize)
    {
        IFT(spMediaBuffer->SetCurrentLength(m_receivedHeader.cbPayloadSize))
    }

    // get the socket input stream reader
    auto asyncResult = co_await m_streamSocket.InputStream().ReadAsync(payloadBuffer,
        m_receivedHeader.cbPayloadSize,
        InputStreamOptions::None);

    return asyncResult;
    //co_await OnPayloadReceived(asyncInfo);
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

    auto dataBundle = winrt::make<Network::implementation::DataBundle>(dataBuffer);

    IFR(NotifyBundleComplete(header->ePayloadType, dataBundle));

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

    auto args = winrt::make<Network::implementation::DataBundleArgs>(payloadType, *this, dataBundle);
    m_evtBundleReceived(*this, args);
}

// Callbacks
_Use_decl_annotations_
HRESULT Connection::OnHeaderReceived()
{
    Log(Log_Level_All, L"Connection::OnHeaderReceived - Tid: %d \n", GetCurrentThreadId());

    auto headerBufferImpl = m_spHeaderBuffer.as<implementation::DataBuffer>();
    
    HRESULT hr = E_FAIL; // assume we fail 

    UINT32 bytesRead = m_spHeaderBuffer.Length();
    DWORD bufferSize = m_spHeaderBuffer.CurrentLength();
    
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

    IFC(ProcessHeaderBuffer(header, m_spHeaderBuffer));

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
HRESULT Connection::OnPayloadReceived(IBuffer payloadBuffer)
{
    Log(Log_Level_All, L"ConnectionImpl::OnPayloadReceived\n");

    NULL_CHK(payloadBuffer);

    UINT32 bytesRead = payloadBuffer.Length();

    auto payloadDataBuffer = payloadBuffer.as<RealtimeStreaming::Network::DataBuffer>();
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

    // create bundle to hold all buffers
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

