#include "pch.h"
#include "Media.NetworkMediaSink.h"

namespace winrt::RealtimeStreaming::Media::implementation
{
    NetworkMediaSink::NetworkMediaSink(Windows::Media::MediaProperties::AudioEncodingProperties const& audioEncodingProperties, Windows::Media::MediaProperties::VideoEncodingProperties const& videoEncodingProperties, RealtimeStreaming::Network::Connection const& connection)
    {
        throw hresult_not_implemented();
    }

    winrt::event_token NetworkMediaSink::Closed(Windows::Foundation::EventHandler<bool> const& handler)
    {
        throw hresult_not_implemented();
    }

    void NetworkMediaSink::Closed(winrt::event_token const& token) noexcept
    {
        throw hresult_not_implemented();
    }

    winrt::hresult NetworkMediaSink::OnEndOfStream(uint32_t streamId)
    {
        throw hresult_not_implemented();
    }

    void NetworkMediaSink::SetProperties(Windows::Foundation::Collections::IPropertySet const& configuration)
    {
        throw hresult_not_implemented();
    }
}
