// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "Generated Files\Network\Listener.g.h"

namespace winrt::RealtimeStreaming::Network::implementation
{
    struct Listener : ListenerT<Listener, winrt::RealtimeStreaming::Plugin::implementation::Module>
    {

        public:
            Listener(_In_ UINT16 port);
            ~Listener();

            // IListener
            Windows::Foundation::IAsyncOperation<Connection> ListenAsync();

            void Close(); // TROY: Todo look at using?

        protected:
            IAsyncAction OnConnectionReceived(
                Windows::Networking::Sockets::StreamSocketListener /* sender */,
                Windows::Networking::Sockets::StreamSocketListenerConnectionReceivedEventArgs args);
            
        private:
            //Wrappers::CriticalSection   _lock;
            slim_mutex m_lock;

            UINT16                      m_port;
            
            winrt::event<Windows::Foundation::EventHandler<bool> > m_evtClosed;

            StreamSocket           m_streamSocketResult;
            StreamSocketListener   m_socketListener;
            winrt::event_token m_connectionReceivedEventToken;
            //EventRegistrationToken          _connectionReceivedEventToken;
        };
}
