#include "pch.h"
#include "Media.Capture.Payload.h"

namespace winrt::CameraCapture::Media::Capture::implementation
{
    Windows::Media::MediaProperties::IMediaEncodingProperties Payload::MediaEncodingProperties()
    {
        throw hresult_not_implemented();
    }
}
