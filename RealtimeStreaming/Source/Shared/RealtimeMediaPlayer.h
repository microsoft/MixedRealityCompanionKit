#pragma once

#include "Media.RealtimeMediaPlayer.g.h"

#include "D3D11DeviceResources.h"
#include "MediaUtils.h"
#include "Unity\IUnityInterface.h"

#include <winrt/Windows.Media.Playback.h>

// TODO: Move this to RealtimeStreaming.idl
namespace winrt::RealtimeStreaming::Media
{
    enum class StateType : UINT16
    {
        None = 0,
        Opened,
        StateChanged,
        Failed,
    };

    // Matches Windows.Media.Playback.MediaPlaybackState enum
    enum class PlaybackState : UINT16
    {
        None = 0,
        Opening,
        Buffering,
        Playing,
        Paused,
    };

#pragma pack(push, 4)
    typedef struct _MEDIA_DESCRIPTION
    {
        UINT32 width;
        UINT32 height;
    } MEDIA_DESCRIPTION;
#pragma pack(pop)

#pragma pack(push, 4)
    typedef struct _PLAYBACK_STATE
    {
        StateType type;
        union {
            PlaybackState state;
            HRESULT hresult;
            MEDIA_DESCRIPTION description;
        } value;
    } PLAYBACK_STATE;
#pragma pack(pop)

    extern "C" typedef void(UNITY_INTERFACE_API *StateChangedCallback)(
        _In_ void* pCallbackObject,
        _In_ PLAYBACK_STATE args);
}

namespace winrt::RealtimeStreaming::Media::implementation
{
    struct RealtimeMediaPlayer : RealtimeMediaPlayerT<RealtimeMediaPlayer>
    {
        RealtimeMediaPlayer() = default;
        ~RealtimeMediaPlayer();

        void Initialize(_In_ std::weak_ptr<IUnityDeviceResource> const& unityDevice);

        Windows::Foundation::IAsyncOperation<Windows::Media::MediaProperties::VideoEncodingProperties> InitAsync(_In_ Network::Connection connection);

        Windows::Media::MediaProperties::VideoEncodingProperties GetVideoProperties();

        void Shutdown();

        winrt::hresult Play();
        winrt::hresult Pause();
        winrt::hresult Stop();

        event_token Closed(_In_ Windows::Foundation::EventHandler<Media::RealtimeMediaPlayer> const& handler);
        void Closed(_In_ event_token const& token);

        // IPlaybackManagerPriv
        STDOVERRIDEMETHODIMP CreateStreamingTexture(
            _In_ UINT32 width,
            _In_ UINT32 height,
            _COM_Outptr_ void** ppvTexture_L,
            _COM_Outptr_ void** ppvTexture_UV);

        void SetStateChangeCallback(
            _In_ RealtimeStreaming::Media::StateChangedCallback callback, 
            _In_ void* pManagedCallbackObject);

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

        Windows::Media::Playback::MediaPlayer m_mediaPlayer{ nullptr };
        event_token m_endedToken;
        event_token m_failedToken;
        event_token m_openedToken;
        event_token m_videoFrameAvailableToken;

        Windows::Media::Playback::MediaPlaybackSession m_mediaPlaybackSession{ nullptr };
        event_token m_stateChangedEventToken;

        std::shared_ptr<SharedTextureBuffer> m_primaryBuffer;
        Media::RealtimeMediaSource m_RealtimeMediaSource{ nullptr };

        event<Windows::Foundation::EventHandler<Media::RealtimeMediaPlayer>> m_closedEvent;
        StateChangedCallback m_stateChangedCallback;
        void* m_pManagedCallbackObject;
    };
}

namespace winrt::RealtimeStreaming::Media::factory_implementation
{
    struct RealtimeMediaPlayer : RealtimeMediaPlayerT<RealtimeMediaPlayer, implementation::RealtimeMediaPlayer>
    {
    };
}
