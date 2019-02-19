#include "pch.h"
#include "Media.RealtimeMediaPlayer.h"

namespace winrt::RealtimeStreaming::Media::implementation
{
    Windows::Foundation::IAsyncOperation<Windows::Media::MediaProperties::VideoEncodingProperties> RealtimeMediaPlayer::InitAsync(RealtimeStreaming::Network::Connection const connection)
    {
        throw hresult_not_implemented();
    }

    winrt::hresult RealtimeMediaPlayer::Play()
    {
        throw hresult_not_implemented();
    }

    winrt::hresult RealtimeMediaPlayer::Pause()
    {
        throw hresult_not_implemented();
    }

    winrt::hresult RealtimeMediaPlayer::Stop()
    {
        throw hresult_not_implemented();
    }

    winrt::event_token RealtimeMediaPlayer::Closed(Windows::Foundation::EventHandler<RealtimeStreaming::Media::RealtimeMediaPlayer> const& handler)
    {
        throw hresult_not_implemented();
    }

    void RealtimeMediaPlayer::Closed(winrt::event_token const& token) noexcept
    {
        throw hresult_not_implemented();
    }

    void RealtimeMediaPlayer::Shutdown()
    {
        throw hresult_not_implemented();
    }
}
