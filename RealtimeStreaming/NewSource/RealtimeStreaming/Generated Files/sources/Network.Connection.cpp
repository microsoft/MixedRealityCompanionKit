#include "pch.h"
#include "Network.Connection.h"

namespace winrt::RealtimeStreaming::Network::implementation
{
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

    event_token Connection::Disconnected(RealtimeStreaming::Network::DisconnectedDelegate const& handler)
    {
        throw hresult_not_implemented();
    }

    void Connection::Disconnected(event_token const& token)
    {
        throw hresult_not_implemented();
    }

    event_token Connection::Received(Windows::Foundation::EventHandler<RealtimeStreaming::Network::BundleReceivedArgs> const& handler)
    {
        throw hresult_not_implemented();
    }

    void Connection::Received(event_token const& token)
    {
        throw hresult_not_implemented();
    }
}
