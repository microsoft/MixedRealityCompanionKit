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
using namespace Windows::System::Threading;

_Use_decl_annotations_
Connector::~Connector()
{
    Log(Log_Level_Info, L"Connector::~Connector()\n");
    Close();
}

// IConnector
_Use_decl_annotations_
IAsyncOperation<RealtimeStreaming::Network::Connection> Connector::ConnectAsync(_In_ Windows::Networking::HostName hostName,
    _In_ UINT16 port)
{
    Log(Log_Level_Info, L"Connector::ConnectAsync()\n");

    if (!m_isConnecting)
    {
        try
        {
            m_hostName = hostName;
            m_port = port;
            m_isConnecting = true;

            // activate a stream socket
            m_streamSocket.Control().KeepAlive(true);
            m_streamSocket.Control().NoDelay(true);
            m_streamSocket.Control().QualityOfService(SocketQualityOfService::LowLatency);

            std::string port = std::to_string(m_port);

            co_await m_streamSocket.ConnectAsync(m_hostName, winrt::to_hstring(port));

            auto connection = winrt::make<Network::implementation::Connection>(m_streamSocket);
            m_isConnecting = false;
            return connection;
        }
        catch (winrt::hresult_error const& ex)
        {
            Log(Log_Level_Error, L"Connector::ConnectAsync Exception Thrown - Tid: %d \n", GetCurrentThreadId());
            LOG_RESULT_MSG(ex.to_abi(), ex.message().data());

            m_isConnecting = false;
        }
    }

    return nullptr;
}

_Use_decl_annotations_
IAsyncOperation<RealtimeStreaming::Network::Connection> Connector::DiscoverAsync()
{
    Log(Log_Level_Info, L"Connector::DiscoverAsync()\n");

    if (!m_isConnecting)
    {
        try
        {
            // Initialize variables for UDP Discovery
            m_isConnecting = true;
            m_didUDPDiscoverySucceed = false;
            m_discoveryAttempts = 0;
            m_signal = handle(::CreateEvent(nullptr, true, false, nullptr));
            m_udpMulticastGroup = HostName{ c_UDP_Multicast_IP };
            m_clientDatagramSocket = DatagramSocket();
            m_clientDatagramSocket.Control().MulticastOnly(true);
            m_udpMessageReceivedEventToken = m_clientDatagramSocket.MessageReceived({ this, &Connector::ClientDatagramSocket_MessageReceived });

            co_await m_clientDatagramSocket.BindServiceNameAsync(c_UDP_Communication_Port);

            m_clientDatagramSocket.JoinMulticastGroup(m_udpMulticastGroup);

            m_udpMulticastSignalTimer = ThreadPoolTimer::CreatePeriodicTimer({ this, &Connector::OnTimer }, m_udpSignalPeriod);

            co_await SendUDPDiscovery();

            // Wait for server to be discovered
            co_await winrt::resume_on_signal(m_signal.get());

            m_isConnecting = false;

            // If we timed out on UDP attempts, then cancel discovery
            if (!m_didUDPDiscoverySucceed) return nullptr;

            RealtimeStreaming::Network::Connection connection = co_await ConnectAsync(m_hostName, m_port);
            return connection;
        }
        catch (winrt::hresult_error const& ex)
        {
            SocketErrorStatus webErrorStatus{ SocketError::GetStatus(ex.to_abi()) };

            Log(Log_Level_Error, L"Connector::AutoConnectAsync Exception Thrown - Tid: %d \n", GetCurrentThreadId());
            LOG_RESULT_MSG(ex.to_abi(), ex.message().data());

            m_isConnecting = false;
        }
    }

    Log(Log_Level_Info, L"Connector::AutoConnectAsync() - Can't Connect \n");
    return nullptr;
}

_Use_decl_annotations_
void Connector::OnTimer(ThreadPoolTimer const& timer)
{
    m_discoveryAttempts++;
    if (m_discoveryAttempts >= c_MaxDiscoveryAttempts)
    {
        OnDiscoveryFinished(false);
    }
    else
    {
        SendUDPDiscovery();
    }
}

_Use_decl_annotations_
void Connector::ClientDatagramSocket_MessageReceived(
    DatagramSocket const& sender,
    DatagramSocketMessageReceivedEventArgs const& args)
{
    Log(Log_Level_Info, L"Connector::ClientDatagramSocket_MessageReceived() - %s \n", args.RemoteAddress().ToString().c_str());

    slim_lock_guard guard(m_lock);

    if (m_clientDatagramSocket == nullptr) return;

    DataReader dataReader{ args.GetDataReader() };
    UINT32 source = dataReader.ReadUInt32();

    Log(Log_Level_Info, L"Connector::ClientDatagramSocket_MessageReceived() - source=%d \n", source);

    if (source == LISTENER_UDP_SOURCE)
    {
        m_port = dataReader.ReadUInt16();
        m_hostName = args.RemoteAddress();

        Log(Log_Level_Info, L"Connector::ClientDatagramSocket_MessageReceived() Port=%d - read\n", m_port);

        OnDiscoveryFinished(true);
    }
}

_Use_decl_annotations_
IAsyncAction Connector::SendUDPDiscovery()
{
    try
    {
        Log(Log_Level_Info, L"Connector::SendUDPDiscovery()\n");

        auto outputStream = co_await m_clientDatagramSocket.GetOutputStreamAsync(m_udpMulticastGroup, c_UDP_Communication_Port);

        DataWriter dataWriter{ outputStream };
        dataWriter.WriteUInt32(CONNECTOR_UDP_SOURCE);

        co_await dataWriter.StoreAsync();
        co_await dataWriter.FlushAsync();
        dataWriter.DetachStream();
    }
    catch (winrt::hresult_error const& ex)
    {
        SocketErrorStatus webErrorStatus{ SocketError::GetStatus(ex.to_abi()) };
        Log(Log_Level_Error, L"Connector::SendUDPDiscoverResponse UDP Exception Thrown - Tid: %d \n", GetCurrentThreadId());

        LOG_RESULT_MSG(ex.to_abi(), ex.message().data());
    }
}

void Connector::OnDiscoveryFinished(bool succeeded)
{
    if (m_clientDatagramSocket == nullptr);

    m_didUDPDiscoverySucceed = succeeded;

    // Cancel our timer
    m_udpMulticastSignalTimer.Cancel();
    m_udpMulticastSignalTimer = nullptr;

    // Clean up our UDP socket
    m_clientDatagramSocket.MessageReceived(m_udpMessageReceivedEventToken);
    m_clientDatagramSocket = nullptr;

    // Signal completion of UDP Multicast for server discovery()
    SetEvent(m_signal.get());
}

/* Event Handlers */
_Use_decl_annotations_
event_token Connector::Closed(Windows::Foundation::EventHandler<bool> const& handler)
{
    Log(Log_Level_Verbose, L"Connector::add_Closed() - Tid: %d \n", GetCurrentThreadId());

    slim_lock_guard guard(m_lock);

    return m_evtClosed.add(handler);
}

_Use_decl_annotations_
void Connector::Closed(winrt::event_token const& token)
{
    Log(Log_Level_Verbose, L"Connector::remove_Closed()\n");

    slim_lock_guard guard(m_lock);

    m_evtClosed.remove(token);
}

_Use_decl_annotations_
void Connector::Close()
{
    Log(Log_Level_Info, L"Connector::Close()\n");

    if (m_streamSocket == nullptr && m_clientDatagramSocket == nullptr)
    {
        // Already closed connector
        return;
    }

    if (m_udpMulticastSignalTimer != nullptr)
    {
        m_udpMulticastSignalTimer.Cancel();
        m_udpMulticastSignalTimer = nullptr;
    }

    if (m_isConnecting)
    {
        // If we are in the middle of an attempted connection via discovery, then auto-finish and fail
        m_didUDPDiscoverySucceed = false;
        SetEvent(m_signal.get());
    }

    m_streamSocket = nullptr;
    m_clientDatagramSocket = nullptr;

    m_evtClosed(*this, true);
}
