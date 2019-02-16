#include "pch.h"
#include "Network.Connector.h"

namespace winrt::RealtimeStreaming::Network::implementation
{
    Windows::Foundation::IAsyncOperation<RealtimeStreaming::Network::Connection> Connector::ConnectAsync()
    {
        throw hresult_not_implemented();
    }

    winrt::event_token Connector::Closed(Windows::Foundation::EventHandler<bool> const& handler)
    {
        throw hresult_not_implemented();
    }

    void Connector::Closed(winrt::event_token const& token) noexcept
    {
        throw hresult_not_implemented();
    }
}
