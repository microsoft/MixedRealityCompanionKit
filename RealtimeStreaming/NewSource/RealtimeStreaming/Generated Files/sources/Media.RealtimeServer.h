#pragma once

#include "Media.RealtimeServer.g.h"
#include "Plugin.Module.h"

namespace winrt::RealtimeStreaming::Media::implementation
{
    struct RealtimeServer : RealtimeServerT<RealtimeServer, RealtimeStreaming::Plugin::implementation::Module>
    {
        RealtimeServer() = delete;
        RealtimeServer(RealtimeStreaming::Network::Connection const& connection, winrt::guid const& inputMediaType, Windows::Media::MediaProperties::MediaEncodingProfile const& mediaEncodingProperties);

        void WriteFrame(uint32_t bufferSize, array_view<uint8_t const> pBuffer);
        Windows::Media::MediaProperties::VideoEncodingProperties VideoProperties();
    };
}

namespace winrt::RealtimeStreaming::Media::factory_implementation
{
    struct RealtimeServer : RealtimeServerT<RealtimeServer, implementation::RealtimeServer>
    {
    };
}
