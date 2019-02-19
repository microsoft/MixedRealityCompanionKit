#pragma once

#include "Media/Capture/Payload.g.h"

namespace winrt::CameraCapture::Media::Capture::implementation
{
    struct Payload : PayloadT<Payload>
    {
        Payload() = delete;

        Windows::Media::MediaProperties::IMediaEncodingProperties MediaEncodingProperties();
    };
}
