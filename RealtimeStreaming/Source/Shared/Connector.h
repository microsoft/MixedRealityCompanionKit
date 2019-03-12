// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "Network.Connector.g.h"

namespace winrt::RealtimeStreaming::Network::implementation
{
    struct Connector : ConnectorT<Connector>
    {
        public:
            Connector(_In_ Windows::Networking::HostName hostName,
                _In_ UINT16 port);
            ~Connector();

            // IConnector
            Windows::Foundation::IAsyncOperation<RealtimeStreaming::Network::Connection> ConnectAsync();
            Windows::Foundation::IAsyncOperation<RealtimeStreaming::Network::Connection> Connector::AutoConnectAsync();

            event_token Connector::Closed(Windows::Foundation::EventHandler<bool> const& handler);
            void Connector::Closed(winrt::event_token const& token);

            void Shutdown() {};
        private:
            void Close();

            Windows::Foundation::IAsyncAction ClientDatagramSocket_MessageReceived(
                Windows::Networking::Sockets::DatagramSocket const& sender,
                Windows::Networking::Sockets::DatagramSocketMessageReceivedEventArgs const& args);

        private:
            slim_mutex m_lock;
            handle m_signal{ nullptr };

            Windows::Networking::HostName m_hostName{ nullptr };
            UINT16 m_port;
     
            Windows::Networking::Sockets::StreamSocket m_streamSocket;
            Windows::Networking::Sockets::DatagramSocket m_clientDatagramSocket;

            winrt::event<Windows::Foundation::EventHandler<bool> > m_evtClosed;
    };
}
