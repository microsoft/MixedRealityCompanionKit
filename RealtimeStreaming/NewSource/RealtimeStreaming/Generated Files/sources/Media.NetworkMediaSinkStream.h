#pragma once

#include "Media/NetworkMediaSinkStream.g.h"

namespace winrt::RealtimeStreaming::Media::implementation
{
    struct NetworkMediaSinkStream : NetworkMediaSinkStreamT<NetworkMediaSinkStream>
    {
        NetworkMediaSinkStream() = delete;

        HRESULT Start(int64_t start);
        HRESULT Restart();
        HRESULT Stop();
        HRESULT Pause();
        HRESULT Shutdown();
        void SetProperties(Windows::Foundation::Collections::IPropertySet const& configuration);
    };
}
