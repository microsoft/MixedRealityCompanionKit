#pragma once

#include "Media.RTSchemeHandler.g.h"

namespace winrt::RealtimeStreaming::Media::implementation
{
    struct RTSchemeHandler : RTSchemeHandlerT<RTSchemeHandler>
    {
        RTSchemeHandler() = default;

        RealtimeStreaming::Network::Connection DataConnection();
        void DataConnection(RealtimeStreaming::Network::Connection const& value);
        void SetProperties(Windows::Foundation::Collections::IPropertySet const& configuration);
    };
}

namespace winrt::RealtimeStreaming::Media::factory_implementation
{
    struct RTSchemeHandler : RTSchemeHandlerT<RTSchemeHandler, implementation::RTSchemeHandler>
    {
    };
}
