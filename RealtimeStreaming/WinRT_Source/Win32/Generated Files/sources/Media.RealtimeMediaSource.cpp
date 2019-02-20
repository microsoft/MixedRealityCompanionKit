#include "pch.h"
#include "Media.RealtimeMediaSource.h"

namespace winrt::RealtimeStreaming::Media::implementation
{
    Windows::Foundation::IAsyncAction RealtimeMediaSource::InitAsync(RealtimeStreaming::Network::Connection const connection)
    {
        throw hresult_not_implemented();
    }

    Windows::Media::Core::MediaStreamSource RealtimeMediaSource::MediaStreamSource()
    {
        throw hresult_not_implemented();
    }

    Windows::Media::MediaProperties::VideoEncodingProperties RealtimeMediaSource::VideoProperties()
    {
        throw hresult_not_implemented();
    }

    void RealtimeMediaSource::SetProperties(Windows::Foundation::Collections::IPropertySet const& configuration)
    {
        throw hresult_not_implemented();
    }
}
