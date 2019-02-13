#pragma once

#include "Media/NetworkMediaSink.g.h"

namespace winrt::RealtimeStreaming::Media::implementation
{
    struct NetworkMediaSink : NetworkMediaSinkT<NetworkMediaSink>
    {
        NetworkMediaSink() = delete;

        event_token Closed(Windows::Foundation::EventHandler<bool> const& handler);
        void Closed(event_token const& token);
        HRESULT OnEndOfStream(uint32_t streamId);
        HRESULT HandleError(HRESULT const& hr);
        HRESULT CheckShutdown();
        HRESULT SendDescription();
        void SetProperties(Windows::Foundation::Collections::IPropertySet const& configuration);
    };
}
