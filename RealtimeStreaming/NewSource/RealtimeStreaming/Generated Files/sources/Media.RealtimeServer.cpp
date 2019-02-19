#include "pch.h"
#include "Media.RealtimeServer.h"

namespace winrt::RealtimeStreaming::Media::implementation
{
    RealtimeServer::RealtimeServer(RealtimeStreaming::Network::Connection const& connection, winrt::guid const& inputMediaType, Windows::Media::MediaProperties::MediaEncodingProfile const& mediaEncodingProperties)
    {
        throw hresult_not_implemented();
    }

    void RealtimeServer::WriteFrame(uint32_t bufferSize, array_view<uint8_t const> pBuffer)
    {
        throw hresult_not_implemented();
    }

    Windows::Media::MediaProperties::VideoEncodingProperties RealtimeServer::VideoProperties()
    {
        throw hresult_not_implemented();
    }

    void RealtimeServer::Shutdown()
    {
        throw hresult_not_implemented();
    }
}
