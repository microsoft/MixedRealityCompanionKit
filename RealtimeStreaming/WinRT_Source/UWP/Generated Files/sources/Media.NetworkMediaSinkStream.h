#pragma once

#include "Media.NetworkMediaSinkStream.g.h"

namespace winrt::RealtimeStreaming::Media::implementation
{
    struct NetworkMediaSinkStream : NetworkMediaSinkStreamT<NetworkMediaSinkStream>
    {
        NetworkMediaSinkStream() = default;
        NetworkMediaSinkStream(uint32_t streamId, RealtimeStreaming::Network::Connection const& dataConnection, RealtimeStreaming::Media::NetworkMediaSink const& parentMediaSink);

        winrt::hresult Start(int64_t start);
        winrt::hresult Restart();
        winrt::hresult Stop();
        winrt::hresult Pause();
        winrt::hresult Shutdown();
        void SetProperties(Windows::Foundation::Collections::IPropertySet const& configuration);
    };
}

namespace winrt::RealtimeStreaming::Media::factory_implementation
{
    struct NetworkMediaSinkStream : NetworkMediaSinkStreamT<NetworkMediaSinkStream, implementation::NetworkMediaSinkStream>
    {
    };
}
