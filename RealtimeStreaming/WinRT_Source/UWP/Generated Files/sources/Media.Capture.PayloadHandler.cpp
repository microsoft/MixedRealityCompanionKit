#include "pch.h"
#include "Media.Capture.PayloadHandler.h"

namespace winrt::CameraCapture::Media::Capture::implementation
{
    HRESULT PayloadHandler::QueuePayload(CameraCapture::Media::Capture::Payload const& payload)
    {
        throw hresult_not_implemented();
    }

    event_token PayloadHandler::OnSample(Windows::Foundation::EventHandler<CameraCapture::Media::Capture::Payload> const& handler)
    {
        throw hresult_not_implemented();
    }

    void PayloadHandler::OnSample(event_token const& token)
    {
        throw hresult_not_implemented();
    }
}
