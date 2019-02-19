#pragma once

#include "Media.RealtimeServer.g.h"

namespace winrt::RealtimeStreaming::Media::implementation
{
    struct RealtimeServer : RealtimeServerT<RealtimeServer>
    {
        RealtimeServer() = delete;
        RealtimeServer(RealtimeStreaming::Network::Connection const& connection, winrt::guid const& inputMediaType, Windows::Media::MediaProperties::MediaEncodingProfile const& mediaEncodingProperties);

        void WriteFrame(uint32_t bufferSize, array_view<uint8_t const> pBuffer);
        Windows::Media::MediaProperties::VideoEncodingProperties VideoProperties();
        void Shutdown();
    };
}

namespace winrt::RealtimeStreaming::Media::factory_implementation
{
    struct RealtimeServer : RealtimeServerT<RealtimeServer, implementation::RealtimeServer>
    {
    };
}
