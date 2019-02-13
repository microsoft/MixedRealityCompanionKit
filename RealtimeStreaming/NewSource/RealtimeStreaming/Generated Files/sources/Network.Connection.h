#pragma once

#include "Network/Connection.g.h"
#include "Plugin.Module.h"

namespace winrt::RealtimeStreaming::Network::implementation
{
    struct Connection : ConnectionT<Connection, RealtimeStreaming::Plugin::implementation::Module>
    {
        Connection() = delete;

        Windows::Foundation::IAsyncAction SendPayloadTypeAsync(RealtimeStreaming::PayloadType const type);
        Windows::Foundation::IAsyncAction SendBundleAsync(RealtimeStreaming::Network::DataBundle const dataBundle);
        bool IsConnected();
        Windows::Networking::Sockets::StreamSocketInformation ConnectionInfo();
        event_token Disconnected(RealtimeStreaming::Network::DisconnectedDelegate const& handler);
        void Disconnected(event_token const& token);
    };
}
