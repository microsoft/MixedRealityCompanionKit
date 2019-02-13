#include "pch.h"
#include "Media.NetworkMediaSinkStream.h"

namespace winrt::RealtimeStreaming::Media::implementation
{
    HRESULT NetworkMediaSinkStream::Start(int64_t start)
    {
        throw hresult_not_implemented();
    }

    HRESULT NetworkMediaSinkStream::Restart()
    {
        throw hresult_not_implemented();
    }

    HRESULT NetworkMediaSinkStream::Stop()
    {
        throw hresult_not_implemented();
    }

    HRESULT NetworkMediaSinkStream::Pause()
    {
        throw hresult_not_implemented();
    }

    HRESULT NetworkMediaSinkStream::Shutdown()
    {
        throw hresult_not_implemented();
    }

    void NetworkMediaSinkStream::SetProperties(Windows::Foundation::Collections::IPropertySet const& configuration)
    {
        throw hresult_not_implemented();
    }
}
