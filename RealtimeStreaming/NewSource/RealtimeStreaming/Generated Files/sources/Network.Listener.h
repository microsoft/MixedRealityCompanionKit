#pragma once

#include "Network/Listener.g.h"
#include "Plugin.Module.h"

namespace winrt::RealtimeStreaming::Network::implementation
{
    struct Listener : ListenerT<Listener, RealtimeStreaming::Plugin::implementation::Module>
    {
        Listener() = delete;
        Listener(uint16_t port);

        Windows::Foundation::IAsyncOperation<RealtimeStreaming::Network::Connection> ListenAsync();
        event_token Closed(Windows::Foundation::EventHandler<bool> const& handler);
        void Closed(event_token const& token);
    };
}

namespace winrt::RealtimeStreaming::Network::factory_implementation
{
    struct Listener : ListenerT<Listener, implementation::Listener>
    {
    };
}
