#pragma once

#include "Network.Connection.g.h"

namespace winrt::RealtimeStreaming::Network::implementation
{
    struct Connection : ConnectionT<Connection>
    {
        Connection() = default;
        Connection(Windows::Networking::Sockets::StreamSocket const& streamSocket);

        Windows::Foundation::IAsyncAction SendPayloadTypeAsync(RealtimeStreaming::Common::PayloadType const type);
        Windows::Foundation::IAsyncAction SendBundleAsync(RealtimeStreaming::Network::DataBundle const dataBundle);
        bool IsConnected();
        Windows::Networking::Sockets::StreamSocketInformation ConnectionInfo();
        winrt::event_token Disconnected(RealtimeStreaming::Network::DisconnectedDelegate const& handler);
        void Disconnected(winrt::event_token const& token) noexcept;
        winrt::event_token Received(Windows::Foundation::EventHandler<RealtimeStreaming::Network::DataBundleArgs> const& handler);
        void Received(winrt::event_token const& token) noexcept;
        void Shutdown();
    };
}

namespace winrt::RealtimeStreaming::Network::factory_implementation
{
    struct Connection : ConnectionT<Connection, implementation::Connection>
    {
    };
}
