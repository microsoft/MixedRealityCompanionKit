#pragma once

#include "Media/Capture/PayloadHandler.g.h"

namespace winrt::CameraCapture::Media::Capture::implementation
{
    struct PayloadHandler : PayloadHandlerT<PayloadHandler>
    {
        PayloadHandler() = default;

        HRESULT QueuePayload(CameraCapture::Media::Capture::Payload const& payload);
        event_token OnSample(Windows::Foundation::EventHandler<CameraCapture::Media::Capture::Payload> const& handler);
        void OnSample(event_token const& token);
    };
}

namespace winrt::CameraCapture::Media::Capture::factory_implementation
{
    struct PayloadHandler : PayloadHandlerT<PayloadHandler, implementation::PayloadHandler>
    {
    };
}
