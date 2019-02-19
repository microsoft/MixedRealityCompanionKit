#include "pch.h"
#include "Media.Capture.StreamSink.h"

namespace winrt::CameraCapture::Media::Capture::implementation
{
    StreamSink::StreamSink(uint8_t index, Windows::Media::MediaProperties::IMediaEncodingProperties const& encodingProperties, CameraCapture::Media::Capture::Sink const& parent)
    {
        throw hresult_not_implemented();
    }

    HRESULT StreamSink::Start(int64_t systemTime, int64_t clockStartOffset)
    {
        throw hresult_not_implemented();
    }

    HRESULT StreamSink::Stop()
    {
        throw hresult_not_implemented();
    }

    HRESULT StreamSink::Shutdown()
    {
        throw hresult_not_implemented();
    }

    CameraCapture::Media::Capture::State StreamSink::State()
    {
        throw hresult_not_implemented();
    }

    void StreamSink::State(CameraCapture::Media::Capture::State const& value)
    {
        throw hresult_not_implemented();
    }

    void StreamSink::SetProperties(Windows::Foundation::Collections::IPropertySet const& configuration)
    {
        throw hresult_not_implemented();
    }
}
