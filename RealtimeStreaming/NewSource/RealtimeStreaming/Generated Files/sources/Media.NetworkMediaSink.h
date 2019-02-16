#pragma once

#include "Media.NetworkMediaSink.g.h"

namespace winrt::RealtimeStreaming::Media::implementation
{
    struct NetworkMediaSink : NetworkMediaSinkT<NetworkMediaSink>
    {
        NetworkMediaSink() = delete;

        winrt::event_token Closed(Windows::Foundation::EventHandler<bool> const& handler);
        void Closed(winrt::event_token const& token) noexcept;
        winrt::hresult OnEndOfStream(uint32_t streamId);
        winrt::hresult HandleError(winrt::hresult const& hr);
        winrt::hresult CheckShutdown();
        winrt::hresult SendDescription();
        void SetProperties(Windows::Foundation::Collections::IPropertySet const& configuration);
    };
}
