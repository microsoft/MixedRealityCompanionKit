#include "pch.h"
#include "Media.RealtimeMediaPlayer.h"

namespace winrt::RealtimeStreaming::Media::implementation
{
    HRESULT RealtimeMediaPlayer::Play()
    {
        throw hresult_not_implemented();
    }

    HRESULT RealtimeMediaPlayer::Pause()
    {
        throw hresult_not_implemented();
    }

    HRESULT RealtimeMediaPlayer::Stop()
    {
        throw hresult_not_implemented();
    }

    event_token RealtimeMediaPlayer::Closed(Windows::Foundation::EventHandler<RealtimeStreaming::Media::RealtimeMediaPlayer> const& handler)
    {
        throw hresult_not_implemented();
    }

    void RealtimeMediaPlayer::Closed(event_token const& token)
    {
        throw hresult_not_implemented();
    }
}
