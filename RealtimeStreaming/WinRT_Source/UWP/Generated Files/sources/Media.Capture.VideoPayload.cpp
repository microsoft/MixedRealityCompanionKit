#include "pch.h"
#include "Media.Capture.VideoPayload.h"

namespace winrt::CameraCapture::Media::Capture::implementation
{
    Windows::Media::MediaProperties::VideoEncodingProperties VideoPayload::VideoProperties()
    {
        throw hresult_not_implemented();
    }
}
