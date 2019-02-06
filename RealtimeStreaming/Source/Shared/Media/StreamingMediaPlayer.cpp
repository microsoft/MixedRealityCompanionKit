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

#include "pch.h"
#include "StreamingMediaPlayer.h"
#include "IUnityGraphicsD3D11.h"

using namespace Microsoft::WRL;
using namespace ABI::Windows::Graphics::DirectX::Direct3D11;
using namespace ABI::Windows::Media;
using namespace ABI::Windows::Media::Core;
using namespace ABI::Windows::Media::Playback;
using namespace Windows::Foundation;

_Use_decl_annotations_
HRESULT StreamingMediaPlayerImpl::CreateStreamingMediaPlayer(
	UnityGfxRenderer apiType,
	IUnityInterfaces* pUnityInterfaces,
	IConnection* pConnection,
	//StateChangedCallback fnCallback,
	IStreamingMediaPlayer** ppMediaPlayback)
{
	Log(Log_Level_Info, L"StreamingMediaPlayerImpl::CreateMediaPlayback()");

	NULL_CHK(pUnityInterfaces);
	//NULL_CHK(fnCallback);
	NULL_CHK(ppMediaPlayback);

	*ppMediaPlayback = nullptr;

	if (apiType == kUnityGfxRendererD3D11)
	{
		IUnityGraphicsD3D11* d3d = pUnityInterfaces->Get<IUnityGraphicsD3D11>();
		NULL_CHK_HR(d3d, E_INVALIDARG);

		ComPtr<IConnection> spConnection(pConnection);

		ComPtr<StreamingMediaPlayerImpl> spMediaPlayback(nullptr);
		IFR(MakeAndInitialize<StreamingMediaPlayerImpl>(&spMediaPlayback, 
			spConnection.Get(), 
			//fnCallback, 
			d3d->GetDevice()));

		*ppMediaPlayback = spMediaPlayback.Detach();
	}
	else
	{
		IFR(E_INVALIDARG);
	}

	return S_OK;
}

_Use_decl_annotations_
StreamingMediaPlayerImpl::StreamingMediaPlayerImpl()
	: m_d3dDevice(nullptr)
	, m_mediaDevice(nullptr)
	, m_fnStateCallback(nullptr)
	, m_mediaPlayer(nullptr)
	, m_mediaPlaybackSession(nullptr)
	, m_primaryTexture(nullptr)
	, m_primaryTextureSRV(nullptr)
	, m_primarySharedHandle(INVALID_HANDLE_VALUE)
	, m_primaryMediaTexture(nullptr)
	, m_primaryMediaSurface(nullptr)
{
}

_Use_decl_annotations_
StreamingMediaPlayerImpl::~StreamingMediaPlayerImpl()
{
	ReleaseTextures();

	ReleaseMediaPlayer();

	MFUnlockDXGIDeviceManager();

	ReleaseResources();
}

_Use_decl_annotations_
HRESULT StreamingMediaPlayerImpl::RuntimeClassInitialize(
	IConnection* pConnection,
	//StateChangedCallback fnCallback,
	ID3D11Device* pDevice)
{
	Log(Log_Level_Info, L"StreamingMediaPlayerImpl::RuntimeClassInitialize()");

	//NULL_CHK(fnCallback);
	NULL_CHK(pDevice);

	// ref count passed in device
	ComPtr<ID3D11Device> spDevice(pDevice);

	// make sure creation of the device is on the same adapter
	ComPtr<IDXGIDevice> spDXGIDevice;
	IFR(spDevice.As(&spDXGIDevice));

	ComPtr<IDXGIAdapter> spAdapter;
	IFR(spDXGIDevice->GetAdapter(&spAdapter));

	// create dx device for media pipeline
	ComPtr<ID3D11Device> spMediaDevice;
	IFR(CreateMediaDevice(spAdapter.Get(), &spMediaDevice));

	// lock the shared dxgi device manager
	// will keep lock open for the life of object
	//     call MFUnlockDXGIDeviceManager when unloading
	UINT uiResetToken;
	ComPtr<IMFDXGIDeviceManager> spDeviceManager;
	IFR(MFLockDXGIDeviceManager(&uiResetToken, &spDeviceManager));

	// associtate the device with the manager
	IFR(spDeviceManager->ResetDevice(spMediaDevice.Get(), uiResetToken));

	// create media player object
	IFR(CreateMediaPlayer());

	// TODO: Need to hook-back up callback?
	//m_fnStateCallback = fnCallback;
	m_d3dDevice.Attach(spDevice.Detach());
	m_mediaDevice.Attach(spMediaDevice.Detach());

	// Start the async operation that this class implements interface
	IFR(Start());

	// Create the media source asyncrhonsouly 
	ComPtr<IConnection> spConnection(pConnection);

	ComPtr<IRealtimeMediaSource> spInteropMediaSource;
	IFR(MakeAndInitialize<RealtimeMediaSourceImpl>(&spInteropMediaSource, spConnection.Get()));
	
	// Save Streaming Media Source locally
	IFR(spInteropMediaSource.CopyTo(&m_RealtimeMediaSource));

	ComPtr<IAsyncAction> spInitSourceAction;
	IFR(spInteropMediaSource.As(&spInitSourceAction));

	// setup callback, only when the source gets the describe message, is it valid
	ComPtr<IStreamingMediaPlayer> spThis(this);
	auto sourceReady = Callback<IAsyncActionCompletedHandler>(
		[this, spThis, spInteropMediaSource](_In_ IAsyncAction *asyncResult, _In_ AsyncStatus asyncStatus) -> HRESULT
	{
		NULL_CHK(asyncResult);

		ComPtr<IMediaStreamSource> spMediaStreamSource;
		ComPtr<IMediaSourceStatics> spMediaSourceStatics;
		ComPtr<IMediaSource2> spMediaSource2;
		ComPtr<IMediaPlaybackItem> spPlaybackItem;
		ComPtr<IMediaPlaybackSource> spMediaPlaybackSource;
		ComPtr<IMediaPlayerSource2> spMediaPlayerSource;

		HRESULT hr = S_OK;

		IFC(spInteropMediaSource->get_MediaStreamSource(&spMediaStreamSource));

		IFC(ABI::Windows::Foundation::GetActivationFactory(
			Wrappers::HStringReference(RuntimeClass_Windows_Media_Core_MediaSource).Get(),
			&spMediaSourceStatics));

		IFC(spMediaSourceStatics->CreateFromMediaStreamSource(
			spMediaStreamSource.Get(),
			&spMediaSource2));

		IFC(CreateMediaPlaybackItem(spMediaSource2.Get(), &spPlaybackItem));

		IFC(spPlaybackItem.As(&spMediaPlaybackSource));

		IFC(m_mediaPlayer.As(&spMediaPlayerSource));
		IFC(spMediaPlayerSource->put_Source(spMediaPlaybackSource.Get()));
	
	done:
		return CompleteAsyncAction(hr);
	});
	
	return spInitSourceAction->put_Completed(sourceReady.Get());
}

HRESULT StreamingMediaPlayerImpl::GetCurrentResolution(
    UINT32* pWidth,
    UINT32* pHeight)
{
    NULL_CHK(pWidth);
    NULL_CHK(pHeight);

    if (m_RealtimeMediaSource == nullptr)
    {
        return E_FAIL;
    }

    return m_RealtimeMediaSource->get_StreamResolution(pWidth, pHeight);
}

_Use_decl_annotations_
HRESULT StreamingMediaPlayerImpl::CreateStreamingTexture(
	UINT32 width,
	UINT32 height,
	void** ppvTexture)
{
	NULL_CHK(ppvTexture);

	if (width < 1 || height < 1)
		IFR(E_INVALIDARG);

	*ppvTexture = nullptr;

	// create the video texture description based on texture format
    m_textureDesc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_B8G8R8A8_UNORM, width, height);
	m_textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	m_textureDesc.MipLevels = 1;
	m_textureDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED | D3D11_RESOURCE_MISC_SHARED_NTHANDLE;
	m_textureDesc.Usage = D3D11_USAGE_DEFAULT;

	IFR(CreateTextures());

	ComPtr<ID3D11ShaderResourceView> spSRV;
	IFR(m_primaryTextureSRV.CopyTo(&spSRV));

	*ppvTexture = spSRV.Detach();

	return S_OK;
}

_Use_decl_annotations_
HRESULT StreamingMediaPlayerImpl::Play()
{
	Log(Log_Level_Info, L"StreamingMediaPlayerImpl::Play()");

	if (nullptr != m_mediaPlayer)
	{
		MediaPlayerState state;
		LOG_RESULT(m_mediaPlayer->get_CurrentState(&state));

		IFR(m_mediaPlayer->Play());
	}

	return S_OK;
}

_Use_decl_annotations_
HRESULT StreamingMediaPlayerImpl::Pause()
{
	Log(Log_Level_Info, L"StreamingMediaPlayerImpl::Pause()");

	if (nullptr != m_mediaPlayer)
	{
		IFR(m_mediaPlayer->Pause());
	}

	return S_OK;
}

_Use_decl_annotations_
HRESULT StreamingMediaPlayerImpl::Stop()
{
	Log(Log_Level_Info, L"StreamingMediaPlayerImpl::Stop()");

	if (nullptr != m_mediaPlayer)
	{
		ComPtr<IMediaPlayerSource2> spMediaPlayerSource;
		IFR(m_mediaPlayer.As(&spMediaPlayerSource));
		IFR(spMediaPlayerSource->put_Source(nullptr));
	}

	return S_OK;
}

_Use_decl_annotations_
HRESULT StreamingMediaPlayerImpl::CreateMediaPlayer()
{
	Log(Log_Level_Info, L"StreamingMediaPlayerImpl::CreateMediaPlayer()");

	// create media player
	ComPtr<IMediaPlayer> spMediaPlayer;
	IFR(Windows::Foundation::ActivateInstance(
		Wrappers::HStringReference(RuntimeClass_Windows_Media_Playback_MediaPlayer).Get(),
		&spMediaPlayer));

	// setup callbacks
	EventRegistrationToken openedEventToken;
	auto mediaOpened = Microsoft::WRL::Callback<IMediaPlayerEventHandler>(this, &StreamingMediaPlayerImpl::OnOpened);
	IFR(spMediaPlayer->add_MediaOpened(mediaOpened.Get(), &openedEventToken));

	EventRegistrationToken endedEventToken;
	auto mediaEnded = Microsoft::WRL::Callback<IMediaPlayerEventHandler>(this, &StreamingMediaPlayerImpl::OnEnded);
	IFR(spMediaPlayer->add_MediaEnded(mediaEnded.Get(), &endedEventToken));

	EventRegistrationToken failedEventToken;
	auto mediaFailed = Microsoft::WRL::Callback<IFailedEventHandler>(this, &StreamingMediaPlayerImpl::OnFailed);
	IFR(spMediaPlayer->add_MediaFailed(mediaFailed.Get(), &failedEventToken));

	// frameserver mode is on the IMediaPlayer5 interface
	ComPtr<IMediaPlayer5> spMediaPlayer5;
	IFR(spMediaPlayer.As(&spMediaPlayer5));

	// set frameserver mode
	IFR(spMediaPlayer5->put_IsVideoFrameServerEnabled(true));

	// register for frame available callback
	EventRegistrationToken videoFrameAvailableToken;
	auto videoFrameAvailableCallback = Microsoft::WRL::Callback<IMediaPlayerEventHandler>(this, &StreamingMediaPlayerImpl::OnVideoFrameAvailable);
	IFR(spMediaPlayer5->add_VideoFrameAvailable(videoFrameAvailableCallback.Get(), &videoFrameAvailableToken));

	// store the player and token
	m_mediaPlayer.Attach(spMediaPlayer.Detach());
	m_openedEventToken = openedEventToken;
	m_endedEventToken = endedEventToken;
	m_failedEventToken = failedEventToken;
	m_videoFrameAvailableToken = videoFrameAvailableToken;

	IFR(AddStateChanged());

	return S_OK;
}

_Use_decl_annotations_
void StreamingMediaPlayerImpl::ReleaseMediaPlayer()
{
	Log(Log_Level_Info, L"StreamingMediaPlayerImpl::ReleaseMediaPlayer()");

	RemoveStateChanged();

	if (nullptr != m_mediaPlayer)
	{
		ComPtr<IMediaPlayer5> spMediaPlayer5;
		if (SUCCEEDED(m_mediaPlayer.As(&spMediaPlayer5)))
		{
			LOG_RESULT(spMediaPlayer5->remove_VideoFrameAvailable(m_videoFrameAvailableToken));

			spMediaPlayer5.Reset();
			spMediaPlayer5 = nullptr;
		}

		LOG_RESULT(m_mediaPlayer->remove_MediaOpened(m_openedEventToken));
		LOG_RESULT(m_mediaPlayer->remove_MediaEnded(m_endedEventToken));
		LOG_RESULT(m_mediaPlayer->remove_MediaFailed(m_failedEventToken));

		// stop playback
		LOG_RESULT(Stop());

		m_mediaPlayer.Reset();
		m_mediaPlayer = nullptr;
	}
}

_Use_decl_annotations_
HRESULT StreamingMediaPlayerImpl::CreateTextures()
{
	if (nullptr != m_primaryTexture || nullptr != m_primaryTextureSRV)
		ReleaseTextures();

	// create staging texture on unity device
	ComPtr<ID3D11Texture2D> spTexture;
	IFR(m_d3dDevice->CreateTexture2D(&m_textureDesc, nullptr, &spTexture));

	auto srvDesc = CD3D11_SHADER_RESOURCE_VIEW_DESC(spTexture.Get(), D3D11_SRV_DIMENSION_TEXTURE2D);
	ComPtr<ID3D11ShaderResourceView> spSRV;
	IFR(m_d3dDevice->CreateShaderResourceView(spTexture.Get(), &srvDesc, &spSRV));

	// create shared texture from the unity texture
	ComPtr<IDXGIResource1> spDXGIResource;
	IFR(spTexture.As(&spDXGIResource));

	HANDLE sharedHandle = INVALID_HANDLE_VALUE;
	ComPtr<ID3D11Texture2D> spMediaTexture;
	ComPtr<IDirect3DSurface> spMediaSurface;
	HRESULT hr = spDXGIResource->CreateSharedHandle(
		nullptr,
		DXGI_SHARED_RESOURCE_READ | DXGI_SHARED_RESOURCE_WRITE,
		L"SharedTextureHandle",
		&sharedHandle);
	if (SUCCEEDED(hr))
	{
		ComPtr<ID3D11Device1> spMediaDevice;
		hr = m_mediaDevice.As(&spMediaDevice);
		if (SUCCEEDED(hr))
		{
			hr = spMediaDevice->OpenSharedResource1(sharedHandle, IID_PPV_ARGS(&spMediaTexture));
			if (SUCCEEDED(hr))
			{
				hr = GetSurfaceFromTexture(spMediaTexture.Get(), &spMediaSurface);
			}
		}
	}

	// if anything failed, clean up and return
	if (FAILED(hr))
	{
		if (sharedHandle != INVALID_HANDLE_VALUE)
			CloseHandle(sharedHandle);

		IFR(hr);
	}

	m_primaryTexture.Attach(spTexture.Detach());
	m_primaryTextureSRV.Attach(spSRV.Detach());

	m_primarySharedHandle = sharedHandle;
	m_primaryMediaTexture.Attach(spMediaTexture.Detach());
	m_primaryMediaSurface.Attach(spMediaSurface.Detach());

	return hr;
}

_Use_decl_annotations_
void StreamingMediaPlayerImpl::ReleaseTextures()
{
	Log(Log_Level_Info, L"StreamingMediaPlayerImpl::ReleaseTextures()");

	// primary texture
	if (m_primarySharedHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_primarySharedHandle);
		m_primarySharedHandle = INVALID_HANDLE_VALUE;
	}

	m_primaryMediaSurface.Reset();
	m_primaryMediaSurface = nullptr;

	m_primaryMediaTexture.Reset();
	m_primaryMediaTexture = nullptr;

	m_primaryTextureSRV.Reset();
	m_primaryTextureSRV = nullptr;

	m_primaryTexture.Reset();
	m_primaryTexture = nullptr;

	ZeroMemory(&m_textureDesc, sizeof(m_textureDesc));
}

_Use_decl_annotations_
HRESULT StreamingMediaPlayerImpl::AddStateChanged()
{
	ComPtr<IMediaPlayer3> spMediaPlayer3;
	IFR(m_mediaPlayer.As(&spMediaPlayer3));

	ComPtr<IMediaPlaybackSession> spSession;
	IFR(spMediaPlayer3->get_PlaybackSession(&spSession));

	EventRegistrationToken stateChangedToken;
	auto stateChanged = Microsoft::WRL::Callback<IMediaPlaybackSessionEventHandler>(this, &StreamingMediaPlayerImpl::OnStateChanged);
	IFR(spSession->add_PlaybackStateChanged(stateChanged.Get(), &stateChangedToken));
	m_stateChangedEventToken = stateChangedToken;
	m_mediaPlaybackSession.Attach(spSession.Detach());

	return S_OK;
}

_Use_decl_annotations_
void StreamingMediaPlayerImpl::RemoveStateChanged()
{
	// remove playback session callbacks
	if (nullptr != m_mediaPlaybackSession)
	{
		LOG_RESULT(m_mediaPlaybackSession->remove_PlaybackStateChanged(m_stateChangedEventToken));

		m_mediaPlaybackSession.Reset();
		m_mediaPlaybackSession = nullptr;
	}
}

_Use_decl_annotations_
void StreamingMediaPlayerImpl::ReleaseResources()
{
	m_fnStateCallback = nullptr;

	// release dx devices
	m_mediaDevice.Reset();
	m_mediaDevice = nullptr;

	m_d3dDevice.Reset();
	m_d3dDevice = nullptr;

    m_RealtimeMediaSource.Reset();
    m_RealtimeMediaSource = nullptr;
}

_Use_decl_annotations_
HRESULT StreamingMediaPlayerImpl::OnVideoFrameAvailable(IMediaPlayer* sender, IInspectable* arg)
{
	ComPtr<IMediaPlayer> spMediaPlayer(sender);

	ComPtr<IMediaPlayer5> spMediaPlayer5;
	IFR(spMediaPlayer.As(&spMediaPlayer5));

	if (nullptr != m_primaryMediaSurface)
	{
		IFR(spMediaPlayer5->CopyFrameToVideoSurface(m_primaryMediaSurface.Get()));
	}

	return S_OK;
}

_Use_decl_annotations_
HRESULT StreamingMediaPlayerImpl::OnOpened(
	IMediaPlayer* sender,
	IInspectable* args)
{
	ComPtr<IMediaPlayer> spMediaPlayer(sender);

	ComPtr<IMediaPlayer3> spMediaPlayer3;
	IFR(spMediaPlayer.As(&spMediaPlayer3));

	ComPtr<IMediaPlaybackSession> spSession;
	IFR(spMediaPlayer3->get_PlaybackSession(&spSession));

	// width & height of video
	UINT32 width = 0;
	IFR(spSession->get_NaturalVideoWidth(&width));

	UINT32 height = 0;
	IFR(spSession->get_NaturalVideoHeight(&height));

	boolean canSeek = false;
	IFR(spSession->get_CanSeek(&canSeek));

	ABI::Windows::Foundation::TimeSpan duration;
	IFR(spSession->get_NaturalDuration(&duration));

	PLAYBACK_STATE playbackState;
	ZeroMemory(&playbackState, sizeof(playbackState));
	playbackState.type = StateType::StateType_Opened;
	playbackState.value.description.width = width;
	playbackState.value.description.height = height;
	playbackState.value.description.canSeek = canSeek;
	playbackState.value.description.duration = duration.Duration;

	if (m_fnStateCallback != nullptr)
		m_fnStateCallback(playbackState);

	return S_OK;
}

_Use_decl_annotations_
HRESULT StreamingMediaPlayerImpl::OnEnded(
	IMediaPlayer* sender,
	IInspectable* args)
{
	PLAYBACK_STATE playbackState;
	ZeroMemory(&playbackState, sizeof(playbackState));
	playbackState.type = StateType::StateType_StateChanged;
	playbackState.value.state = PlaybackState::PlaybackState_Ended;

	if (m_fnStateCallback != nullptr)
		m_fnStateCallback(playbackState);

	return S_OK;
}

_Use_decl_annotations_
HRESULT StreamingMediaPlayerImpl::OnFailed(
	IMediaPlayer* sender,
	IMediaPlayerFailedEventArgs* args)
{
	HRESULT hr = S_OK;
	IFR(args->get_ExtendedErrorCode(&hr));

	/*
	// TODO: Troy fix to get back logging
	ComPtr<HSTRING> errorMessage;
	IFR(args->get_ErrorMessage(errorMessage.Get()));
	
	//LOG_RESULT_MSG(hr, WindowsGetStringRawBuffer(*errorMessage, nullptr));
		//errorMessage->c_str());
	*/

	PLAYBACK_STATE playbackState;
	ZeroMemory(&playbackState, sizeof(playbackState));
	playbackState.type = StateType::StateType_Failed;
	playbackState.value.hresult = hr;

	if (m_fnStateCallback != nullptr)
		m_fnStateCallback(playbackState);

	return S_OK;
}

_Use_decl_annotations_
HRESULT StreamingMediaPlayerImpl::OnStateChanged(
	IMediaPlaybackSession* sender,
	IInspectable* args)
{
	MediaPlaybackState state;
	IFR(sender->get_PlaybackState(&state));

	PLAYBACK_STATE playbackState;
	ZeroMemory(&playbackState, sizeof(playbackState));
	playbackState.type = StateType::StateType_StateChanged;
	playbackState.value.state = static_cast<PlaybackState>(state);

	if (m_fnStateCallback != nullptr)
		m_fnStateCallback(playbackState);

	return S_OK;
}

_Use_decl_annotations_
HRESULT StreamingMediaPlayerImpl::CompleteAsyncAction(HRESULT hr)
{
	Log(Log_Level_Info, L"StreamingMediaPlayerImpl::CompleteAsyncAction()\n");

	ABI::Windows::Foundation::AsyncStatus status;
	IFC(get_Status(&status));

	if (AsyncStatus::Completed == status)
	{
		return S_OK;
	}

	if (AsyncStatus::Started != status)
	{
		IFC(hr);

		IFC(get_ErrorCode(&hr));
	}

done:
	if (FAILED(hr))
	{
		LOG_RESULT(TryTransitionToError(hr));
	}

	return FireCompletion();
}