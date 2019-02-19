#pragma once

#include "Media/Capture/AudioPayload.g.h"
#include "Media.Capture.Payload.h"

namespace winrt::CameraCapture::Media::Capture::implementation
{
    struct AudioPayload : AudioPayloadT<AudioPayload, CameraCapture::Media::Capture::implementation::Payload>
    {
        AudioPayload() = default;

        Windows::Media::MediaProperties::AudioEncodingProperties AudioProperties();
    };
}

namespace winrt::CameraCapture::Media::Capture::factory_implementation
{
    struct AudioPayload : AudioPayloadT<AudioPayload, implementation::AudioPayload>
    {
    };
}
