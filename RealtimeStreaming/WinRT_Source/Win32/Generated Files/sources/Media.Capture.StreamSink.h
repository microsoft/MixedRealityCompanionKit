#pragma once

#include "Media/Capture/StreamSink.g.h"

namespace winrt::CameraCapture::Media::Capture::implementation
{
    struct StreamSink : StreamSinkT<StreamSink>
    {
        StreamSink() = delete;
        StreamSink(uint8_t index, Windows::Media::MediaProperties::IMediaEncodingProperties const& encodingProperties, CameraCapture::Media::Capture::Sink const& parent);

        HRESULT Start(int64_t systemTime, int64_t clockStartOffset);
        HRESULT Stop();
        HRESULT Shutdown();
        CameraCapture::Media::Capture::State State();
        void State(CameraCapture::Media::Capture::State const& value);
        void SetProperties(Windows::Foundation::Collections::IPropertySet const& configuration);
    };
}

namespace winrt::CameraCapture::Media::Capture::factory_implementation
{
    struct StreamSink : StreamSinkT<StreamSink, implementation::StreamSink>
    {
    };
}
