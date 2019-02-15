#pragma once

#include "Generated Files\Media\RealtimeMediaPlayer.g.h"
#include "Plugin/Module.h"
#include "Common/D3D11DeviceResources.h"
#include "MediaUtils.h"
#include "Unity\IUnityInterface.h"

#include <winrt/Windows.Media.Playback.h>

struct __declspec(uuid("2d67f54a-2f43-11e9-b210-d663bd873d93")) IPlaybackManagerPriv : ::IUnknown
{
    STDMETHOD(CreatePlaybackTexture)(_In_ uint32_t width, _In_ uint32_t height, _COM_Outptr_ void** ppvTexture) PURE;
};

namespace winrt::RealtimeStreaming::Media::implementation
{
    struct RealtimeMediaPlayer : RealtimeMediaPlayerT<RealtimeMediaPlayer, RealtimeStreaming::Plugin::Module, IPlaybackManagerPriv>
    {
        
        static Plugin::IModule Create(
            _In_ std::weak_ptr<IUnityDeviceResource> const& unityDevice);
            //_In_ StateChangedCallback fnCallback);
        

        RealtimeMediaPlayer();
        
        IAsyncAction InitAsync(_In_ Network::Connection connection);

        virtual void Shutdown();

        HRESULT Play();
        HRESULT Pause();
        HRESULT Stop();

        event_token Closed(Windows::Foundation::EventHandler<Media::RealtimeMediaPlayer> const& handler);
        void Closed(event_token const& token);

        // IPlaybackManagerPriv
        STDOVERRIDEMETHODIMP CreatePlaybackTexture(_In_ UINT32 width, _In_ UINT32 height, _COM_Outptr_ void** ppvTexture);

    private:
        HRESULT CreateMediaPlayer();
        void ReleaseMediaPlayer();

        HRESULT CreateResources(com_ptr<ID3D11Device> const& unityDevice);
        void ReleaseResources();

    private:
        std::weak_ptr<IUnityDeviceResource> m_deviceResources;

        com_ptr<ID3D11Device> m_d3dDevice;
        uint32_t m_resetToken;
        com_ptr<IMFDXGIDeviceManager> m_dxgiDeviceManager;

        Windows::Media::Playback::MediaPlayer m_mediaPlayer;
        event_token m_endedToken;
        event_token m_failedToken;
        event_token m_openedToken;
        event_token m_videoFrameAvailableToken;

        Windows::Media::Playback::MediaPlaybackSession m_mediaPlaybackSession;
        event_token m_stateChangedEventToken;

        std::shared_ptr<SharedTextureBuffer> m_primaryBuffer;
        Media::RealtimeMediaSource m_RealtimeMediaSource{ nullptr };

        event<Windows::Foundation::EventHandler<Media::RealtimeMediaPlayer>> m_closedEvent;
    };
}

namespace winrt::RealtimeStreaming::Media::factory_implementation
{
    struct RealtimeMediaPlayer : RealtimeMediaPlayerT<RealtimeMediaPlayer, implementation::RealtimeMediaPlayer>
    {
    };
}
