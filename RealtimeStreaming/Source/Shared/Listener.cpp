// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "Listener.h"
#include "Connection.h"

using namespace winrt;
using namespace RealtimeStreaming::Network::implementation;
using namespace Windows::Foundation;
using namespace Windows::Networking;
using namespace Windows::Networking::Sockets;
using namespace winrt::Windows::Storage::Streams;

Listener::Listener(UINT16 port)
    : m_port(port)
    , m_streamSocket(nullptr)
    , m_isListening(false)
{
    Log(Log_Level_Info, L"Listener::Listener() - Port=%d\n", m_port);
}

Listener::~Listener()
{
    Log(Log_Level_Info, L"Listener::~Listener() - Port=%d\n", m_port);
    Close();
}

// IListener
_Use_decl_annotations_
IAsyncOperation<RealtimeStreaming::Network::Connection> Listener::ListenAsync(bool listenForMulticast)
{
    Log(Log_Level_Info, L"Listener::ListenAsync(%d) - Port=%d \n", listenForMulticast, m_port);

    if (m_isListening) return nullptr;

    m_isListening = true;

    // TODO: make this separate async command so we can immediately start listening on TCP connections?
    if (listenForMulticast)
    {
        try
        {
            m_udpMulticastGroup = HostName{ c_UDP_Multicast_IP };
            m_serverDatagramSocket.Control().MulticastOnly(true);

            m_udpMessageReceivedEventToken = m_serverDatagramSocket.MessageReceived({ this, &Listener::ServerDatagramSocket_OnMessageReceived });

            co_await m_serverDatagramSocket.BindServiceNameAsync(c_UDP_Communication_Port);

            m_serverDatagramSocket.JoinMulticastGroup(m_udpMulticastGroup);
        }
        catch (winrt::hresult_error const& ex)
        {
            SocketErrorStatus webErrorStatus{ SocketError::GetStatus(ex.to_abi()) };
            Log(Log_Level_Error, L"Listener::ListenAsync UDP Exception Thrown - Port=%d \n", m_port);

            LOG_RESULT_MSG(ex.to_abi(), ex.message().data());
        }
    }

    // create a listener
    m_connectionReceivedEventToken = m_socketListener.ConnectionReceived({ this, &Listener::OnConnectionReceived });
    m_signal = handle(::CreateEvent(nullptr, true, false, nullptr));

    // activate a stream socket before listening
    m_socketListener.Control().KeepAlive(true);
    m_socketListener.Control().NoDelay(true);
    m_socketListener.Control().QualityOfService(SocketQualityOfService::LowLatency);

    // Convert our port to a string
    std::string port = std::to_string(m_port);

    co_await m_socketListener.BindServiceNameAsync(winrt::to_hstring(port));

    co_await winrt::resume_on_signal(m_signal.get());

    m_isListening = false;

    return m_connection;
}

_Use_decl_annotations_
IAsyncAction Listener::ServerDatagramSocket_OnMessageReceived(
    DatagramSocket sender,
    DatagramSocketMessageReceivedEventArgs args)
{
    Log(Log_Level_Verbose, L"Listener::ServerDatagramSocket_MessageReceived() - - Port=%d , Remote=%s \n", m_port, args.RemoteAddress().ToString().c_str());

    if (m_serverDatagramSocket == nullptr) return;

    DataReader dataReader{ args.GetDataReader() };
    UINT32 source = dataReader.ReadUInt32();

    if (source == CONNECTOR_UDP_SOURCE)
    {
        Log(Log_Level_Verbose, L"Listener::ServerDatagramSocket_MessageReceived() - CONNECTOR - Port=%d \n", m_port);

        co_await SendUDPDiscoverResponse();
    }
    else if (source == LISTENER_UDP_SOURCE)
    {
        Log(Log_Level_Verbose, L"Listener::ServerDatagramSocket_MessageReceived() - LISTENER - Port=%d \n", m_port);

        // Read out rest of buffer on reader since we are multicast
        dataReader.ReadUInt16();
    }
}

_Use_decl_annotations_
IAsyncAction Listener::SendUDPDiscoverResponse()
{
    try
    {
        Log(Log_Level_Verbose, L"Listener::SendUDPDiscoverResponse() - Port=%d\n", m_port);

        IOutputStream outputStream = co_await m_serverDatagramSocket.GetOutputStreamAsync(m_udpMulticastGroup, c_UDP_Communication_Port);

        DataWriter dataWriter{ outputStream };
        dataWriter.WriteUInt32(LISTENER_UDP_SOURCE);
        dataWriter.WriteUInt16(m_port);

        co_await dataWriter.StoreAsync();
        co_await dataWriter.FlushAsync();
        dataWriter.DetachStream();
    }
    catch (winrt::hresult_error const& ex)
    {
        SocketErrorStatus webErrorStatus{ SocketError::GetStatus(ex.to_abi()) };
        Log(Log_Level_Error, L"Listener::SendUDPDiscoverResponse UDP Exception Thrown - Port=%d \n", m_port);

        LOG_RESULT_MSG(ex.to_abi(), ex.message().data());
    }
}

/* Event Handlers */
_Use_decl_annotations_
event_token Listener::Closed(Windows::Foundation::EventHandler<bool> const& handler)
{
    Log(Log_Level_Verbose, L"Listener::add_Closed() - Port=%d \n", m_port);

    slim_lock_guard guard(m_lock);

    return m_evtClosed.add(handler);
}

_Use_decl_annotations_
void Listener::Closed(winrt::event_token const& token)
{
    Log(Log_Level_Verbose, L"Listener::remove_Closed()\n");

    slim_lock_guard guard(m_lock);

    m_evtClosed.remove(token);
}

_Use_decl_annotations_
void  Listener::OnConnectionReceived(StreamSocketListener /* sender */,
    StreamSocketListenerConnectionReceivedEventArgs args)
{
    Log(Log_Level_Info, L"Listener::OnConnectionReceived() - Port=%d \n", m_port);

    slim_lock_guard guard(m_lock);

    if (m_connection == nullptr)
    {
        m_connection = winrt::make<implementation::Connection>(args.Socket());

        // Signal completion of ListenAsync()
        SetEvent(m_signal.get());
    }
};

_Use_decl_annotations_
void Listener::Close()
{
    Log(Log_Level_Info, L"Listener::Close() - Port=%d\n", m_port);

    if (m_socketListener == nullptr) return;

    m_socketListener.Close();
    m_socketListener = nullptr;

    m_streamSocket = nullptr;

    m_serverDatagramSocket = nullptr;
}
