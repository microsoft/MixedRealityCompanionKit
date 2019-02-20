#include "pch.h"
#include "Network.Connection.h"

namespace winrt::RealtimeStreaming::Network::implementation
{
    Connection::Connection(Windows::Networking::Sockets::StreamSocket const& streamSocket)
    {
        throw hresult_not_implemented();
    }

    Windows::Foundation::IAsyncAction Connection::SendPayloadTypeAsync(RealtimeStreaming::Common::PayloadType const type)
    {
        throw hresult_not_implemented();
    }

    Windows::Foundation::IAsyncAction Connection::SendBundleAsync(RealtimeStreaming::Network::DataBundle const dataBundle)
    {
        throw hresult_not_implemented();
    }

    bool Connection::IsConnected()
    {
        throw hresult_not_implemented();
    }

    Windows::Networking::Sockets::StreamSocketInformation Connection::ConnectionInfo()
    {
        throw hresult_not_implemented();
    }

    winrt::event_token Connection::Disconnected(RealtimeStreaming::Network::DisconnectedDelegate const& handler)
    {
        throw hresult_not_implemented();
    }

    void Connection::Disconnected(winrt::event_token const& token) noexcept
    {
        throw hresult_not_implemented();
    }

    winrt::event_token Connection::Received(Windows::Foundation::EventHandler<RealtimeStreaming::Network::DataBundleArgs> const& handler)
    {
        throw hresult_not_implemented();
    }

    void Connection::Received(winrt::event_token const& token) noexcept
    {
        throw hresult_not_implemented();
    }

    void Connection::Shutdown()
    {
        throw hresult_not_implemented();
    }
}
