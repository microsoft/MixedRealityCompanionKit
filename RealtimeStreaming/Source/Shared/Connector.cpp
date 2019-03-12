// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "Connector.h"

#include "Connection.h"

using namespace winrt;
using namespace RealtimeStreaming::Network::implementation;
using namespace Windows::Foundation;
using namespace Windows::Networking;
using namespace Windows::Networking::Sockets;
using namespace Windows::Storage::Streams;

_Use_decl_annotations_
Connector::Connector(_In_ Windows::Networking::HostName hostName,
    _In_ UINT16 port)
    : m_hostName(hostName)
    , m_port(port)
{
    Log(Log_Level_Info, L"Connector::Connector()\n");
}

_Use_decl_annotations_
Connector::~Connector()
{
    Log(Log_Level_Info, L"Connector::~Connector()\n");

    //Close();
}

// IConnector
_Use_decl_annotations_
IAsyncOperation<RealtimeStreaming::Network::Connection> Connector::ConnectAsync()
{
    Log(Log_Level_Info, L"Connector::ConnectAsync()\n");

    // activate a stream socket
    m_streamSocket.Control().KeepAlive(true);
    m_streamSocket.Control().NoDelay(true);
    m_streamSocket.Control().QualityOfService(SocketQualityOfService::LowLatency);

    std::string port = std::to_string(m_port);

    co_await m_streamSocket.ConnectAsync(m_hostName, winrt::to_hstring(port));

    return winrt::make<Network::implementation::Connection>(m_streamSocket);
}

IAsyncOperation<RealtimeStreaming::Network::Connection> Connector::AutoConnectAsync()
{
    // TODO: check state we are not already connecting, don't call twice etc
    try
    {
        m_clientDatagramSocket.MessageReceived({ this, &Connector::ClientDatagramSocket_MessageReceived });

        // Establish a connection to the echo server.
        // The server hostname that we will be establishing a connection to. In this example, the server and client are in the same process.
        HostName hostName{ c_UDP_Multicast_IP };

        co_await m_clientDatagramSocket.BindServiceNameAsync(c_UDP_Communication_Port);

        auto outputStream = co_await m_clientDatagramSocket.GetOutputStreamAsync(hostName, c_UDP_Communication_Port);

        winrt::hstring request{ L"Hello, World!" };
        DataWriter dataWriter{ outputStream };
        dataWriter.WriteString(request);

        co_await dataWriter.StoreAsync();
        dataWriter.DetachStream();

        m_signal = handle(::CreateEvent(nullptr, true, false, nullptr));

        // Wait for server to be discovered
        co_await winrt::resume_on_signal(m_signal.get());

        // TODO: need to check this if we are closing down connector? this thread will hang?

        auto connection = co_await ConnectAsync();
        return connection;
    }
    catch (winrt::hresult_error const& ex)
    {
        Windows::Networking::Sockets::SocketErrorStatus webErrorStatus{ Windows::Networking::Sockets::SocketError::GetStatus(ex.to_abi()) };
    }
}

IAsyncAction Connector::ClientDatagramSocket_MessageReceived(
    DatagramSocket const& sender, 
    DatagramSocketMessageReceivedEventArgs const& args)
{
    DataReader dataReader{ args.GetDataReader() };
    m_port = dataReader.ReadUInt16();
    m_hostName = args.RemoteAddress();

    // need to clean up and remove message received reference?
    m_clientDatagramSocket = nullptr;

    // Signal completion of UDP Multicast for server discovery()
    SetEvent(m_signal.get());
}

/* Event Handlers */
_Use_decl_annotations_
event_token Connector::Closed(Windows::Foundation::EventHandler<bool> const& handler)
{
    Log(Log_Level_All, L"Connector::add_Closed() - Tid: %d \n", GetCurrentThreadId());

    slim_lock_guard guard(m_lock);

    return m_evtClosed.add(handler);
}

_Use_decl_annotations_
void Connector::Closed(winrt::event_token const& token)
{
    Log(Log_Level_Info, L"Connector::remove_Closed()\n");

    //auto lock = _lock.Lock();
    slim_lock_guard guard(m_lock);

    m_evtClosed.remove(token);
}

_Use_decl_annotations_
void Connector::Close()
{
    Log(Log_Level_Info, L"Connector::Close()\n");

    /*
    com_ptr<Windows::Foundation::IClosable> closeable;
    if (nullptr != m_streamSocketResult)
    {
        if SUCCEEDED(m_streamSocketResult.As(&closeable))
        {
            LOG_RESULT(closeable->Close());
        }

        com_ptr<IConnector> spThis(this);
        LOG_RESULT(_evtClosed.InvokeAll(spThis.get()));
    }
    m_streamSocketResult.Reset();
    m_streamSocketResult = nullptr;
    */
}
