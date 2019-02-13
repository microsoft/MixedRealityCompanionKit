#include "pch.h"
#include "Network.Listener.h"

namespace winrt::RealtimeStreaming::Network::implementation
{
    Listener::Listener(uint16_t port)
    {
        throw hresult_not_implemented();
    }

    Windows::Foundation::IAsyncOperation<RealtimeStreaming::Network::Connection> Listener::ListenAsync()
    {
        throw hresult_not_implemented();
    }

    event_token Listener::Closed(Windows::Foundation::EventHandler<bool> const& handler)
    {
        throw hresult_not_implemented();
    }

    void Listener::Closed(event_token const& token)
    {
        throw hresult_not_implemented();
    }
}
