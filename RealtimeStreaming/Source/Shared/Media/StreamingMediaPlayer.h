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
		
		typedef ABI::Windows::Foundation::ITypedEventHandler<ABI::Windows::Media::Playback::MediaPlayer*, IInspectable*> IMediaPlayerEventHandler;
		typedef ABI::Windows::Foundation::ITypedEventHandler<ABI::Windows::Media::Playback::MediaPlayer*, ABI::Windows::Media::Playback::MediaPlayerFailedEventArgs*> IFailedEventHandler;
		typedef ABI::Windows::Foundation::ITypedEventHandler<ABI::Windows::Media::Playback::MediaPlaybackSession*, IInspectable*> IMediaPlaybackSessionEventHandler;

		DECLARE_INTERFACE_IID_(IStreamingMediaPlayer, IUnknown, "eea215a4-53ee-4f57-8dda-10467d628180")
		{
            STDMETHOD(GetCurrentResolution)(_Out_ UINT32* pWidth, _Out_ UINT32* pHeight) PURE;
			STDMETHOD(CreateStreamingTexture)(_In_ UINT32 width, _In_ UINT32 height, _COM_Outptr_ void** ppvTexture) PURE;
			STDMETHOD(Play)() PURE;
			STDMETHOD(Pause)() PURE;
			STDMETHOD(Stop)() PURE;
		};

		class StreamingMediaPlayerImpl
			: public Microsoft::WRL::RuntimeClass
			//< Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::ClassicCom>
			<RuntimeClassFlags<RuntimeClassType::WinRtClassicComMix>
			, IStreamingMediaPlayer
			, ABI::Windows::Foundation::IAsyncAction
			, AsyncBase<ABI::Windows::Foundation::IAsyncActionCompletedHandler>
			, Microsoft::WRL::FtmBase>
		{
		public:
			static HRESULT CreateStreamingMediaPlayer(
				_In_ UnityGfxRenderer apiType,
				_In_ IUnityInterfaces* pUnityInterfaces,
				_In_ ABI::RealtimeStreaming::Network::IConnection *pConnection,
				//_In_ StateChangedCallback fnCallback,
				_COM_Outptr_ IStreamingMediaPlayer** ppMediaPlayback);

			StreamingMediaPlayerImpl();
			~StreamingMediaPlayerImpl();

			HRESULT RuntimeClassInitialize(
				_In_ ABI::RealtimeStreaming::Network::IConnection *pConnection,
				//_In_ StateChangedCallback fnCallback,
				_In_ ID3D11Device* pDevice);

			// IStreamingMediaPlayer
            IFACEMETHOD(GetCurrentResolution)(
                _Out_ UINT32* pWidth, 
                _Out_ UINT32* pHeight);

			IFACEMETHOD(CreateStreamingTexture)(
				_In_ UINT32 width,
				_In_ UINT32 height,
				_COM_Outptr_ void** ppvTexture);

			IFACEMETHOD(Play)();
			IFACEMETHOD(Pause)();
			IFACEMETHOD(Stop)();

			// IAsyncAction
			IFACEMETHOD(put_Completed)(
				_In_ ABI::Windows::Foundation::IAsyncActionCompletedHandler *handler)
			{
				return PutOnComplete(handler);
			}
			IFACEMETHOD(get_Completed)(
				_Out_ ABI::Windows::Foundation::IAsyncActionCompletedHandler** handler)
			{
				return GetOnComplete(handler);
			}
			IFACEMETHOD(GetResults)(void)
			{
				return S_OK;
			}

			// AsyncBase
			virtual HRESULT OnStart(void) { return S_OK; }
			virtual void OnClose(void) {};
			virtual void OnCancel(void) {};

		protected:
			// Callbacks - IMediaPlayer2
			HRESULT OnOpened(
				_In_ ABI::Windows::Media::Playback::IMediaPlayer* sender,
				_In_ IInspectable* args);
			HRESULT OnEnded(
				_In_ ABI::Windows::Media::Playback::IMediaPlayer* sender,
				_In_ IInspectable* args);
			HRESULT OnFailed(
				_In_ ABI::Windows::Media::Playback::IMediaPlayer* sender,
				_In_ ABI::Windows::Media::Playback::IMediaPlayerFailedEventArgs* args);

			// Callbacks - IMediaPlayer5 - frameserver
			HRESULT OnVideoFrameAvailable(
				_In_ ABI::Windows::Media::Playback::IMediaPlayer* sender,
				_In_ IInspectable* args);

			// Callbacks - IMediaPlaybackSession
			HRESULT OnStateChanged(
				_In_ ABI::Windows::Media::Playback::IMediaPlaybackSession* sender,
				_In_ IInspectable* args);

			HRESULT CompleteAsyncAction(
				_In_ HRESULT hResult);

		private:
			HRESULT CreateMediaPlayer();
			void ReleaseMediaPlayer();

			HRESULT CreateTextures();
			void ReleaseTextures();

			HRESULT AddStateChanged();
			void RemoveStateChanged();

			void ReleaseResources();

		private:
			Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice;
			Microsoft::WRL::ComPtr<ID3D11Device> m_mediaDevice;

			StateChangedCallback m_fnStateCallback;

			Microsoft::WRL::ComPtr<ABI::Windows::Media::Playback::IMediaPlayer> m_mediaPlayer;
			EventRegistrationToken m_openedEventToken;
			EventRegistrationToken m_endedEventToken;
			EventRegistrationToken m_failedEventToken;
			EventRegistrationToken m_videoFrameAvailableToken;

			 Microsoft::WRL::ComPtr<IRealtimeMediaSource> m_RealtimeMediaSource;

			Microsoft::WRL::ComPtr<ABI::Windows::Media::Playback::IMediaPlaybackSession> m_mediaPlaybackSession;
			EventRegistrationToken m_stateChangedEventToken;

			CD3D11_TEXTURE2D_DESC m_textureDesc;
			Microsoft::WRL::ComPtr<ID3D11Texture2D> m_primaryTexture;
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_primaryTextureSRV;

			HANDLE m_primarySharedHandle;
			Microsoft::WRL::ComPtr<ID3D11Texture2D> m_primaryMediaTexture;
			Microsoft::WRL::ComPtr<ABI::Windows::Graphics::DirectX::Direct3D11::IDirect3DSurface> m_primaryMediaSurface;
		};

	}
}