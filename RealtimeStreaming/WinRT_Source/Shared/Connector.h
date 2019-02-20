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
            event_token Connector::Closed(Windows::Foundation::EventHandler<bool> const& handler);
            void Connector::Closed(winrt::event_token const& token);

            void Shutdown() {};
        private:
            void Close();

        private:
            //Wrappers::CriticalSec\tion _lock;
            slim_mutex m_lock;

            Windows::Networking::HostName m_hostName{ nullptr };
            UINT16 m_port;
     
            Windows::Networking::Sockets::StreamSocket m_streamSocket;

            winrt::event<Windows::Foundation::EventHandler<bool> > m_evtClosed;
    };
}
