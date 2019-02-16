#include "pch.h"
#include "Media.NetworkMediaSink.h"

namespace winrt::RealtimeStreaming::Media::implementation
{
    winrt::event_token NetworkMediaSink::Closed(Windows::Foundation::EventHandler<bool> const& handler)
    {
        throw hresult_not_implemented();
    }

    void NetworkMediaSink::Closed(winrt::event_token const& token) noexcept
    {
        throw hresult_not_implemented();
    }

    winrt::hresult NetworkMediaSink::OnEndOfStream(uint32_t streamId)
    {
        throw hresult_not_implemented();
    }

    winrt::hresult NetworkMediaSink::HandleError(winrt::hresult const& hr)
    {
        throw hresult_not_implemented();
    }

    winrt::hresult NetworkMediaSink::CheckShutdown()
    {
        throw hresult_not_implemented();
    }

    winrt::hresult NetworkMediaSink::SendDescription()
    {
        throw hresult_not_implemented();
    }

    void NetworkMediaSink::SetProperties(Windows::Foundation::Collections::IPropertySet const& configuration)
    {
        throw hresult_not_implemented();
    }
}
