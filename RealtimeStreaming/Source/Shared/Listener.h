// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "Network.Listener.g.h"

namespace winrt::RealtimeStreaming::Network::implementation
{
    struct Listener : ListenerT<Listener>
    {

    public:
        Listener(_In_ UINT16 port);
        ~Listener();

        // IListener
        Windows::Foundation::IAsyncOperation<Network::Connection> ListenAsync(bool listenForMulticast);

        event_token Listener::Closed(Windows::Foundation::EventHandler<bool> const& handler);
        void Listener::Closed(winrt::event_token const& token);

        void Close();

        // IRTModule
        void Shutdown() { Close(); };
    protected:
        Windows::Foundation::IAsyncAction SendUDPDiscoverResponse();

        void OnConnectionReceived(
            Windows::Networking::Sockets::StreamSocketListener /* sender */,
            Windows::Networking::Sockets::StreamSocketListenerConnectionReceivedEventArgs args);

        Windows::Foundation::IAsyncAction ServerDatagramSocket_OnMessageReceived(
            Windows::Networking::Sockets::DatagramSocket sender,
            Windows::Networking::Sockets::DatagramSocketMessageReceivedEventArgs args);

    private:
        slim_mutex m_lock;
        handle m_signal{ nullptr };
        bool m_isListening;
        UINT16 m_port;

        winrt::event<Windows::Foundation::EventHandler<bool> > m_evtClosed;

        Windows::Networking::Sockets::StreamSocket           m_streamSocket;
        Windows::Networking::Sockets::StreamSocketListener   m_socketListener;

        Windows::Networking::HostName m_udpMulticastGroup{ nullptr };
        Windows::Networking::Sockets::DatagramSocket m_serverDatagramSocket;

        winrt::event_token m_udpMessageReceivedEventToken;
        winrt::event_token m_connectionReceivedEventToken;

        Network::Connection m_connection{ nullptr };
    };
}

namespace winrt::RealtimeStreaming::Network::factory_implementation
{
    struct Listener : ListenerT<Listener, implementation::Listener>
    {
    };
}
