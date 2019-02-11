//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************
#pragma once

namespace RealtimeStreaming
{
    namespace Media
    {
        enum class StateType : UINT16
        {
            StateType_None = 0,
            StateType_Opened,
            StateType_StateChanged,
            StateType_Failed,
        };

        enum class PlaybackState : UINT16
        {
            PlaybackState_None = 0,
            PlaybackState_Opening,
            PlaybackState_Buffering,
            PlaybackState_Playing,
            PlaybackState_Paused,
            PlaybackState_Ended
        };

#pragma pack(push, 4)
        typedef struct _MEDIA_DESCRIPTION
        {
            UINT32 width;
            UINT32 height;
            INT64 duration;
            byte canSeek;
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
            _In_ PLAYBACK_STATE args);

        struct RealtimeMediaPlayer
        {
        public:
            RealtimeMediaPlayer(
                _In_ UnityGfxRenderer apiType,
                _In_ IUnityInterfaces* pUnityInterfaces);
            ~RealtimeMediaPlayer();

            IAsyncAction InitAsync(_In_ Connection connection);

            // IRealtimeMediaPlayer
            HRESULT GetCurrentResolution(
                _Out_ UINT32* pWidth,
                _Out_ UINT32* pHeight);

            HRESULT CreateStreamingTexture(
                _In_ UINT32 width,
                _In_ UINT32 height,
                _COM_Outptr_ void** ppvTexture);

            void Play();
            void Pause();
            void Stop();

        protected:
            // Callbacks - MediaPlayer
            void OnOpened(
                _In_ Windows::Media::Playback::MediaPlayer sender,
                _In_ IInspectable args);
            void OnEnded(
                _In_ Windows::Media::Playback::MediaPlayer sender,
                _In_ Object args);
            void OnFailed(
                _In_ Windows::Media::Playback::MediaPlayer sender,
                _In_ Windows::Media::Playback::MediaPlayerFailedEventArgs args);

            void OnVideoFrameAvailable(
                _In_ Windows::Media::Playback::MediaPlayer sender,
                _In_ IInspectable args);

            // Callbacks - IMediaPlaybackSession
            void OnStateChanged(
                _In_ Windows::Media::Playback::MediaPlaybackSession sender,
                _In_ IInspectable args);

        private:
            void CreateMediaPlayer();
            void ReleaseMediaPlayer();

            HRESULT CreateTextures();
            void ReleaseTextures();

            void ReleaseResources();

        private:
            com_ptr<ID3D11Device> m_d3dDevice;
            com_ptr<ID3D11Device> m_mediaDevice;

            StateChangedCallback m_fnStateCallback;

            Windows::Media::Playback::MediaPlayer m_mediaPlayer{ nullptr };
            winrt::event_token m_openedEventToken;
            winrt::event_token m_endedEventToken;
            winrt::event_token m_failedEventToken;
            winrt::event_token m_videoFrameAvailableToken;

            RealtimeMediaSource m_RealtimeMediaSource{ nullptr };

            Windows::Media::Playback::MediaPlaybackSession m_mediaPlaybackSession;
            winrt::event_token m_stateChangedEventToken;

            CD3D11_TEXTURE2D_DESC m_textureDesc;
            com_ptr<ID3D11Texture2D> m_primaryTexture;
            com_ptr<ID3D11ShaderResourceView> m_primaryTextureSRV;

            HANDLE m_primarySharedHandle;
            com_ptr<ID3D11Texture2D> m_primaryMediaTexture;
            com_ptr<ABI::Windows::Graphics::DirectX::Direct3D11::IDirect3DSurface> m_primaryMediaSurface;
        };

    }
}