#include "pch.h"
#include "Media.Capture.AudioPayload.h"

namespace winrt::CameraCapture::Media::Capture::implementation
{
    Windows::Media::MediaProperties::AudioEncodingProperties AudioPayload::AudioProperties()
    {
        throw hresult_not_implemented();
    }
}
