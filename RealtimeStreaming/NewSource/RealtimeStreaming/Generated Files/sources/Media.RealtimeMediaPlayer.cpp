#include "pch.h"
#include "Media.RealtimeMediaPlayer.h"

namespace winrt::RealtimeStreaming::Media::implementation
{
    event_token RealtimeMediaPlayer::Closed(Windows::Foundation::EventHandler<RealtimeStreaming::Media::RealtimeMediaPlayer> const& handler)
    {
        throw hresult_not_implemented();
    }

    void RealtimeMediaPlayer::Closed(event_token const& token)
    {
        throw hresult_not_implemented();
    }
}
