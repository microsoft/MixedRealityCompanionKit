#pragma once

#include "Media/Capture/Sink.g.h"

namespace winrt::CameraCapture::Media::Capture::implementation
{
    struct Sink : SinkT<Sink>
    {
        Sink() = delete;
        Sink(Windows::Media::MediaProperties::MediaEncodingProfile const& encodingProfile);

        HRESULT OnEndOfStream();
        CameraCapture::Media::Capture::State State();
        CameraCapture::Media::Capture::PayloadHandler PayloadHandler();
        void PayloadHandler(CameraCapture::Media::Capture::PayloadHandler const& value);
        void SetProperties(Windows::Foundation::Collections::IPropertySet const& configuration);
    };
}

namespace winrt::CameraCapture::Media::Capture::factory_implementation
{
    struct Sink : SinkT<Sink, implementation::Sink>
    {
    };
}
