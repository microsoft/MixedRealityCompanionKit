// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "Network.Connector.g.h"

namespace winrt::RealtimeStreaming::Network::implementation
{
    struct Connector : ConnectorT<Connector>
    {
        public:
            Connector() = default;
            ~Connector();

            // IConnector
            Windows::Foundation::IAsyncOperation<RealtimeStreaming::Network::Connection> ConnectAsync(_In_ Windows::Networking::HostName hostName,
                _In_ UINT16 port);

            Windows::Foundation::IAsyncOperation<RealtimeStreaming::Network::Connection> DiscoverAsync();

            event_token Connector::Closed(Windows::Foundation::EventHandler<bool> const& handler);
            void Connector::Closed(winrt::event_token const& token);

            void Close();

        private:
            void Connector::OnTimer(Windows::System::Threading::ThreadPoolTimer const& timer);
            Windows::Foundation::IAsyncAction SendUDPDiscovery();

            void ClientDatagramSocket_MessageReceived(
                Windows::Networking::Sockets::DatagramSocket const& sender,
                Windows::Networking::Sockets::DatagramSocketMessageReceivedEventArgs const& args);

            void OnDiscoveryFinished(bool succeeded);

        private:
            slim_mutex m_lock;
            handle m_signal{ nullptr };

            bool m_isConnecting;
            Windows::Networking::HostName m_hostName{ nullptr };
            UINT16 m_port;

            Windows::Networking::Sockets::StreamSocket m_streamSocket;

            Windows::Networking::Sockets::DatagramSocket m_clientDatagramSocket{ nullptr };
            Windows::Networking::HostName m_udpMulticastGroup{ nullptr };
            winrt::event_token m_udpMessageReceivedEventToken;
            bool m_didUDPDiscoverySucceed;
            UINT32 m_discoveryAttempts;
            const UINT32 c_MaxDiscoveryAttempts = 5;

            Windows::Foundation::TimeSpan m_udpSignalPeriod{ 20000000 };// 10,000,000 ticks per second
            Windows::System::Threading::ThreadPoolTimer m_udpMulticastSignalTimer{ nullptr };

            winrt::event<Windows::Foundation::EventHandler<bool> > m_evtClosed;
    };
}
