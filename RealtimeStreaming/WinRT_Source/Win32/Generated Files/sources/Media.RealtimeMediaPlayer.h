#pragma once

#include "Media.RealtimeMediaPlayer.g.h"

namespace winrt::RealtimeStreaming::Media::implementation
{
    struct RealtimeMediaPlayer : RealtimeMediaPlayerT<RealtimeMediaPlayer>
    {
        RealtimeMediaPlayer() = default;

        Windows::Foundation::IAsyncOperation<Windows::Media::MediaProperties::VideoEncodingProperties> InitAsync(RealtimeStreaming::Network::Connection const connection);
        winrt::hresult Play();
        winrt::hresult Pause();
        winrt::hresult Stop();
        winrt::event_token Closed(Windows::Foundation::EventHandler<RealtimeStreaming::Media::RealtimeMediaPlayer> const& handler);
        void Closed(winrt::event_token const& token) noexcept;
        void Shutdown();
    };
}

namespace winrt::RealtimeStreaming::Media::factory_implementation
{
    struct RealtimeMediaPlayer : RealtimeMediaPlayerT<RealtimeMediaPlayer, implementation::RealtimeMediaPlayer>
    {
    };
}
