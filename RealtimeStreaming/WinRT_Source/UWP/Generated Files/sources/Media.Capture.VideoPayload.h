#pragma once

#include "Media/Capture/VideoPayload.g.h"
#include "Media.Capture.Payload.h"

namespace winrt::CameraCapture::Media::Capture::implementation
{
    struct VideoPayload : VideoPayloadT<VideoPayload, CameraCapture::Media::Capture::implementation::Payload>
    {
        VideoPayload() = default;

        Windows::Media::MediaProperties::VideoEncodingProperties VideoProperties();
    };
}

namespace winrt::CameraCapture::Media::Capture::factory_implementation
{
    struct VideoPayload : VideoPayloadT<VideoPayload, implementation::VideoPayload>
    {
    };
}
