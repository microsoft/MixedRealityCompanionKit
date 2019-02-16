#pragma once

#include "Media.NetworkMediaSinkStream.g.h"

namespace winrt::RealtimeStreaming::Media::implementation
{
    struct NetworkMediaSinkStream : NetworkMediaSinkStreamT<NetworkMediaSinkStream>
    {
        NetworkMediaSinkStream() = delete;

        winrt::hresult Start(int64_t start);
        winrt::hresult Restart();
        winrt::hresult Stop();
        winrt::hresult Pause();
        winrt::hresult Shutdown();
        void SetProperties(Windows::Foundation::Collections::IPropertySet const& configuration);
    };
}
