#include "pch.h"
#include "Media.NetworkMediaSinkStream.h"

namespace winrt::RealtimeStreaming::Media::implementation
{
    NetworkMediaSinkStream::NetworkMediaSinkStream(uint32_t streamId, RealtimeStreaming::Network::Connection const& dataConnection, RealtimeStreaming::Media::NetworkMediaSink const& parentMediaSink)
    {
        throw hresult_not_implemented();
    }

    winrt::hresult NetworkMediaSinkStream::Start(int64_t start)
    {
        throw hresult_not_implemented();
    }

    winrt::hresult NetworkMediaSinkStream::Restart()
    {
        throw hresult_not_implemented();
    }

    winrt::hresult NetworkMediaSinkStream::Stop()
    {
        throw hresult_not_implemented();
    }

    winrt::hresult NetworkMediaSinkStream::Pause()
    {
        throw hresult_not_implemented();
    }

    winrt::hresult NetworkMediaSinkStream::Shutdown()
    {
        throw hresult_not_implemented();
    }

    void NetworkMediaSinkStream::SetProperties(Windows::Foundation::Collections::IPropertySet const& configuration)
    {
        throw hresult_not_implemented();
    }
}
