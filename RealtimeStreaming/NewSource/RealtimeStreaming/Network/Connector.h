// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "Generated Files/Network/Connector.g.h"
#include "Connection.h"
#include "Plugin\Module.h"

namespace winrt::RealtimeStreaming::Network::implementation
{
    struct Connector : ConnectorT<Connector, winrt::RealtimeStreaming::Plugin::implementation::Module>
    {
        public:
            Connector(_In_ Windows::Networking::HostName hostName,
                _In_ UINT16 port);
            ~Connector();

            // IConnector
            Windows::Foundation::IAsyncOperation<RealtimeStreaming::Network::Connection> ConnectAsync();

        private:
            void Close();

        private:
            //Wrappers::CriticalSec\tion _lock;
            slim_mutex m_lock;

            Windows::Networking::HostName m_hostName{ nullptr };
            UINT16 m_port;
     
            Windows::Networking::Sockets::StreamSocket m_streamSocketResult;

            winrt::event<Windows::Foundation::EventHandler<bool> > m_evtClosed;
    };
}
