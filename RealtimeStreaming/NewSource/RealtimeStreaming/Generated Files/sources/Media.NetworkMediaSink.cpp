#include "pch.h"
#include "Media.NetworkMediaSink.h"

namespace winrt::RealtimeStreaming::Media::implementation
{
    event_token NetworkMediaSink::Closed(Windows::Foundation::EventHandler<bool> const& handler)
    {
        throw hresult_not_implemented();
    }

    void NetworkMediaSink::Closed(event_token const& token)
    {
        throw hresult_not_implemented();
    }

    HRESULT NetworkMediaSink::OnEndOfStream(uint32_t streamId)
    {
        throw hresult_not_implemented();
    }

    HRESULT NetworkMediaSink::HandleError(HRESULT const& hr)
    {
        throw hresult_not_implemented();
    }

    HRESULT NetworkMediaSink::CheckShutdown()
    {
        throw hresult_not_implemented();
    }

    HRESULT NetworkMediaSink::SendDescription()
    {
        throw hresult_not_implemented();
    }

    void NetworkMediaSink::SetProperties(Windows::Foundation::Collections::IPropertySet const& configuration)
    {
        throw hresult_not_implemented();
    }
}
