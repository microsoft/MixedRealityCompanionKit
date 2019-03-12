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
{
    Log(Log_Level_Info, L"Listener::Listener()\n");
}

Listener::~Listener()
{
    Log(Log_Level_Info, L"Listener::~Listener()\n");

    // TODO: Close like original?
    // Close();
}

// IListener
_Use_decl_annotations_
IAsyncOperation<RealtimeStreaming::Network::Connection> Listener::ListenAsync()
{
    Log(Log_Level_Info, L"Listener::ListenAsync()\n");

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

    return m_connection;
}

IAsyncAction Listener::DiscoveryListenAsync()
{
    try
    {
        m_serverDatagramSocket.MessageReceived({ this, &Listener::ServerDatagramSocket_MessageReceived });

        co_await m_serverDatagramSocket.BindServiceNameAsync(c_UDP_Communication_Port);
        
        HostName multicastGroupHostName{ c_UDP_Multicast_IP };

        m_serverDatagramSocket.JoinMulticastGroup(multicastGroupHostName);
    }
    catch (winrt::hresult_error const& ex)
    {
        SocketErrorStatus webErrorStatus{ SocketError::GetStatus(ex.to_abi()) };
        // TODO: log error
    }
}

IAsyncAction Listener::ServerDatagramSocket_MessageReceived(
    DatagramSocket sender, 
    DatagramSocketMessageReceivedEventArgs args)
{
    //DataReader dataReader{ args.GetDataReader() };
    //winrt::hstring request{ dataReader.ReadString(dataReader.UnconsumedBufferLength()) };

    // TODO: Should we select random port?
    // TODO: check that we can use args.remoteaddress isntead of multicast IP
    IOutputStream outputStream = co_await sender.GetOutputStreamAsync(args.RemoteAddress(), c_UDP_Communication_Port);
    DataWriter dataWriter{ outputStream };
    dataWriter.WriteUInt16(m_port);

    co_await dataWriter.StoreAsync();
    dataWriter.DetachStream();

    // We need to shut down properly
    m_serverDatagramSocket = nullptr;
}

/* Event Handlers */
_Use_decl_annotations_
event_token Listener::Closed(Windows::Foundation::EventHandler<bool> const& handler)
{
    Log(Log_Level_All, L"Listener::add_Closed() - Tid: %d \n", GetCurrentThreadId());

    slim_lock_guard guard(m_lock);

    return m_evtClosed.add(handler);
}

_Use_decl_annotations_
void Listener::Closed(winrt::event_token const& token)
{
    Log(Log_Level_Info, L"Listener::remove_Closed()\n");

    slim_lock_guard guard(m_lock);

    m_evtClosed.remove(token);
}

_Use_decl_annotations_
void  Listener::OnConnectionReceived(StreamSocketListener /* sender */,
    StreamSocketListenerConnectionReceivedEventArgs args)
{
    Log(Log_Level_Info, L"Listener::OnConnectionReceived()\n");

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
    m_socketListener = nullptr;
    m_streamSocket = nullptr;
    m_serverDatagramSocket = nullptr;
}
