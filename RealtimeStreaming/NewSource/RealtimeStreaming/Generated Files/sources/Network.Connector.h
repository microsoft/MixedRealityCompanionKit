#pragma once

#include "Network/Connector.g.h"
#include "Plugin.Module.h"

namespace winrt::RealtimeStreaming::Network::implementation
{
    struct Connector : ConnectorT<Connector, RealtimeStreaming::Plugin::implementation::Module>
    {
        Connector() = delete;

        Windows::Foundation::IAsyncOperation<RealtimeStreaming::Network::Connection> ConnectAsync();
        event_token Closed(Windows::Foundation::EventHandler<bool> const& handler);
        void Closed(event_token const& token);
    };
}
