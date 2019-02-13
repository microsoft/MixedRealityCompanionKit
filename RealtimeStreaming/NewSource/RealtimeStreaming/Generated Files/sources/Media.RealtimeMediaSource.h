#pragma once

#include "Media/RealtimeMediaSource.g.h"

namespace winrt::RealtimeStreaming::Media::implementation
{
    struct RealtimeMediaSource : RealtimeMediaSourceT<RealtimeMediaSource>
    {
        RealtimeMediaSource() = default;

        Windows::Foundation::IAsyncAction InitAsync(RealtimeStreaming::Network::Connection const connection);
        Windows::Media::Core::MediaStreamSource MediaStreamSource();
        Windows::Media::MediaProperties::VideoEncodingProperties VideoProperties();
        void SetProperties(Windows::Foundation::Collections::IPropertySet const& configuration);
    };
}

namespace winrt::RealtimeStreaming::Media::factory_implementation
{
    struct RealtimeMediaSource : RealtimeMediaSourceT<RealtimeMediaSource, implementation::RealtimeMediaSource>
    {
    };
}
