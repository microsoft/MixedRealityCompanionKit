#pragma once

#include "Network.Connector.g.h"

namespace winrt::RealtimeStreaming::Network::implementation
{
    struct Connector : ConnectorT<Connector>
    {
        Connector() = delete;

        Windows::Foundation::IAsyncOperation<RealtimeStreaming::Network::Connection> ConnectAsync();
        winrt::event_token Closed(Windows::Foundation::EventHandler<bool> const& handler);
        void Closed(winrt::event_token const& token) noexcept;
        void Shutdown();
    };
}
