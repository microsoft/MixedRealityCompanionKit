#pragma once

#include "Media/RealtimeMediaPlayer.g.h"
#include "Plugin.Module.h"

namespace winrt::RealtimeStreaming::Media::implementation
{
    struct RealtimeMediaPlayer : RealtimeMediaPlayerT<RealtimeMediaPlayer, RealtimeStreaming::Plugin::implementation::Module>
    {
        RealtimeMediaPlayer() = default;

        HRESULT Play();
        HRESULT Pause();
        HRESULT Stop();
        event_token Closed(Windows::Foundation::EventHandler<RealtimeStreaming::Media::RealtimeMediaPlayer> const& handler);
        void Closed(event_token const& token);
    };
}

namespace winrt::RealtimeStreaming::Media::factory_implementation
{
    struct RealtimeMediaPlayer : RealtimeMediaPlayerT<RealtimeMediaPlayer, implementation::RealtimeMediaPlayer>
    {
    };
}
