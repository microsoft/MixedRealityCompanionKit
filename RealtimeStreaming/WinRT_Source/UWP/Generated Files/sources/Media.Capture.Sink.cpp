#include "pch.h"
#include "Media.Capture.Sink.h"

namespace winrt::CameraCapture::Media::Capture::implementation
{
    Sink::Sink(Windows::Media::MediaProperties::MediaEncodingProfile const& encodingProfile)
    {
        throw hresult_not_implemented();
    }

    HRESULT Sink::OnEndOfStream()
    {
        throw hresult_not_implemented();
    }

    CameraCapture::Media::Capture::State Sink::State()
    {
        throw hresult_not_implemented();
    }

    CameraCapture::Media::Capture::PayloadHandler Sink::PayloadHandler()
    {
        throw hresult_not_implemented();
    }

    void Sink::PayloadHandler(CameraCapture::Media::Capture::PayloadHandler const& value)
    {
        throw hresult_not_implemented();
    }

    void Sink::SetProperties(Windows::Foundation::Collections::IPropertySet const& configuration)
    {
        throw hresult_not_implemented();
    }
}
