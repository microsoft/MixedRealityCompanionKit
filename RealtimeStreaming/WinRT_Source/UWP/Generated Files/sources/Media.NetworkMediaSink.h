#pragma once

#include "Media.NetworkMediaSink.g.h"

namespace winrt::RealtimeStreaming::Media::implementation
{
    struct NetworkMediaSink : NetworkMediaSinkT<NetworkMediaSink>
    {
        NetworkMediaSink() = default;
        NetworkMediaSink(Windows::Media::MediaProperties::AudioEncodingProperties const& audioEncodingProperties, Windows::Media::MediaProperties::VideoEncodingProperties const& videoEncodingProperties, RealtimeStreaming::Network::Connection const& connection);

        winrt::event_token Closed(Windows::Foundation::EventHandler<bool> const& handler);
        void Closed(winrt::event_token const& token) noexcept;
        winrt::hresult OnEndOfStream(uint32_t streamId);
        void SetProperties(Windows::Foundation::Collections::IPropertySet const& configuration);
    };
}

namespace winrt::RealtimeStreaming::Media::factory_implementation
{
    struct NetworkMediaSink : NetworkMediaSinkT<NetworkMediaSink, implementation::NetworkMediaSink>
    {
    };
}
