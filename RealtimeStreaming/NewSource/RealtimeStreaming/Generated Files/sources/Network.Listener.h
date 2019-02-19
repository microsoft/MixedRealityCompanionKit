#pragma once

#include "Network.Listener.g.h"

namespace winrt::RealtimeStreaming::Network::implementation
{
    struct Listener : ListenerT<Listener>
    {
        Listener() = delete;
        Listener(uint16_t port);

        Windows::Foundation::IAsyncOperation<RealtimeStreaming::Network::Connection> ListenAsync();
        winrt::event_token Closed(Windows::Foundation::EventHandler<bool> const& handler);
        void Closed(winrt::event_token const& token) noexcept;
        void Shutdown();
    };
}

namespace winrt::RealtimeStreaming::Network::factory_implementation
{
    struct Listener : ListenerT<Listener, implementation::Listener>
    {
    };
}
