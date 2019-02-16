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

    winrt::event_token Listener::Closed(Windows::Foundation::EventHandler<bool> const& handler)
    {
        throw hresult_not_implemented();
    }

    void Listener::Closed(winrt::event_token const& token) noexcept
    {
        throw hresult_not_implemented();
    }
}
