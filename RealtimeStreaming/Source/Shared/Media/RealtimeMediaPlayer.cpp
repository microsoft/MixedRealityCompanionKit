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
#include "RealtimeMediaPlayer.h"
#include "IUnityGraphicsD3D11.h"

using namespace winrt;
using namespace Windows::Graphics::DirectX::Direct3D11;
using namespace Windows::Media;
using namespace Windows::Media::Core;
using namespace Windows::Media::Playback;
using namespace Windows::Foundation;

_Use_decl_annotations_
RealtimeMediaPlayer::RealtimeMediaPlayer(
    UnityGfxRenderer apiType,
    IUnityInterfaces* pUnityInterfaces)
    //StateChangedCallback fnCallback,
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
    Log(Log_Level_Info, L"RealtimeMediaPlayer::RealtimeMediaPlayer()\n");

    NULL_CHK(pUnityInterfaces);
    //NULL_CHK(fnCallback);

    com_ptr<ID3D11Device> spDevice;

    if (apiType == kUnityGfxRendererD3D11)
    {
        IUnityGraphicsD3D11* d3d = pUnityInterfaces->Get<IUnityGraphicsD3D11>();
        
        NULL_CHK_HR(d3d, E_INVALIDARG);

        spDevice.attach(d3d->GetDevice());
    }
    else
    {
        check_hresult(E_INVALIDARG);
    }

    // make sure creation of the device is on the same adapter
    com_ptr<IDXGIDevice> spDXGIDevice;
    check_hresult(spDevice.as(spDXGIDevice.put()));

    com_ptr<IDXGIAdapter> spAdapter;
    check_hresult(spDXGIDevice->GetAdapter(spAdapter.put()));

    // create dx device for media pipeline
    com_ptr<ID3D11Device> spMediaDevice;
    check_hresult(CreateMediaDevice(spAdapter.get(), spMediaDevice.put()));

    // lock the shared dxgi device manager
    // will keep lock open for the life of object
    //     call MFUnlockDXGIDeviceManager when unloading
    UINT uiResetToken;
    com_ptr<IMFDXGIDeviceManager> spDeviceManager;
    check_hresult(MFLockDXGIDeviceManager(&uiResetToken, spDeviceManager.put()));

    // associtate the device with the manager
    check_hresult(spDeviceManager->ResetDevice(spMediaDevice.get(), uiResetToken));

    // create media player object
    CreateMediaPlayer();

    // TODO: Need to hook-back up callback?
    //m_fnStateCallback = fnCallback;
    m_d3dDevice.attach(spDevice.detach());
    m_mediaDevice.attach(spMediaDevice.detach());
}

IAsyncAction RealtimeMediaPlayer::InitAsync(
    Connection connection)
{
    m_RealtimeMediaSource = RealtimeMediaSource(connection);

    co_await m_RealtimeMediaSource.connectasync();

    MediaStreamSource mediaStreamSource = m_RealtimeMediaSource.GetMediaStreamSource();

    MediaSource source = MediaSource.CreateFromMediaStreamSource(mediaStreamSource);

    MediaPlaybackItem item = MediaPlaybackItem(source);

    m_mediaPlayer.Source(item);
}

_Use_decl_annotations_
RealtimeMediaPlayer::~RealtimeMediaPlayer()
{
    ReleaseTextures();

    ReleaseMediaPlayer();

    MFUnlockDXGIDeviceManager();

    ReleaseResources();
}


HRESULT RealtimeMediaPlayer::GetCurrentResolution(
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
HRESULT RealtimeMediaPlayer::CreateStreamingTexture(
    UINT32 width,
    UINT32 height,
    void** ppvTexture)
{
    Log(Log_Level_Info, L"RealtimeMediaPlayerImpl::CreateStreamingTexture()\n");

    NULL_CHK(ppvTexture);

    if (width < 1 || height < 1)
        check_hresult(E_INVALIDARG);

    *ppvTexture = nullptr;

    // create the video texture description based on texture format
    m_textureDesc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_B8G8R8A8_UNORM, width, height);
    m_textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    m_textureDesc.MipLevels = 1;
    m_textureDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED | D3D11_RESOURCE_MISC_SHARED_NTHANDLE;
    m_textureDesc.Usage = D3D11_USAGE_DEFAULT;

    check_hresult(CreateTextures());

    com_ptr<ID3D11ShaderResourceView> spSRV;
    check_hresult(m_primaryTextureSRV.CopyTo(&spSRV));

    *ppvTexture = spSRV.detach();

    return S_OK;
}

_Use_decl_annotations_
void RealtimeMediaPlayer::Play()
{
    Log(Log_Level_Info, L"RealtimeMediaPlayerImpl::Play()\n");

    if (nullptr != m_mediaPlayer)
    {
        //LOG_RESULT(m_mediaPlayer.PlaybackSession().State);

        m_mediaPlayer.Play();
    }
}

_Use_decl_annotations_
void RealtimeMediaPlayer::Pause()
{
    Log(Log_Level_Info, L"RealtimeMediaPlayerImpl::Pause()\n");

    if (nullptr != m_mediaPlayer)
    {
        m_mediaPlayer.Pause();
    }
}

_Use_decl_annotations_
void RealtimeMediaPlayer::Stop()
{
    Log(Log_Level_Info, L"RealtimeMediaPlayerImpl::Stop()\n");

    if (nullptr != m_mediaPlayer)
    {
        Pause();
        m_mediaPlayer.Source = nullptr;
    }
}

_Use_decl_annotations_
void RealtimeMediaPlayer::CreateMediaPlayer()
{
    Log(Log_Level_Info, L"RealtimeMediaPlayerImpl::CreateMediaPlayer()\n");

    // setup callbacks
    m_mediaPlayer = MediaPlayer();
    m_mediaPlayer.IsVideoFrameServerEnabled = true;

    // Subscribe to events
    m_openedEventToken = m_mediaPlayer.MediaOpened({ this, &RealtimeMediaPlayer::OnOpened });
    m_endedEventToken = m_mediaPlayer.MediaEnded({ this, &RealtimeMediaPlayer::OnEnded });
    m_failedEventToken = m_mediaPlayer.MediaFailed({ this, &RealtimeMediaPlayer::OnFailed });
    m_videoFrameAvailableToken = m_mediaPlayer.VideoFrameAvailable({ this, &RealtimeMediaPlayer::OnVideoFrameAvailable });

    m_mediaPlaybackSession = m_mediaPlayer.PlaybackSession;
    m_stateChangedEventToken = m_mediaPlaybackSession.PlaybackStateChanged({ this, &RealtimeMediaPlayer::OnStateChanged });)
}

_Use_decl_annotations_
void RealtimeMediaPlayer::ReleaseMediaPlayer()
{
    Log(Log_Level_Info, L"RealtimeMediaPlayerImpl::ReleaseMediaPlayer()\n");

    // stop playback
    Stop();

    // Unsubscribe from event handlers
    if (nullptr != m_mediaPlaybackSession)
    {
        m_mediaPlaybackSession.PlaybackStateChanged(m_stateChangedEventToken);
    }

    if (nullptr != m_mediaPlayer)
    {
        m_mediaPlayer.MediaOpened(m_openedEventToken);
        m_mediaPlayer.MediaEnded(m_endedEventToken);
        m_mediaPlayer.MediaFailed(m_failedEventToken);
        m_mediaPlayer.VideoFrameAvailable(m_videoFrameAvailableToken);
    }
}

_Use_decl_annotations_
HRESULT RealtimeMediaPlayer::CreateTextures()
{
    Log(Log_Level_Info, L"RealtimeMediaPlayerImpl::CreateTextures()\n");

    if (nullptr != m_primaryTexture || nullptr != m_primaryTextureSRV)
    {
        ReleaseTextures();
    }

    // create staging texture on unity device
    com_ptr<ID3D11Texture2D> spTexture;
    check_hresult(m_d3dDevice->CreateTexture2D(&m_textureDesc, nullptr, &spTexture));

    auto srvDesc = CD3D11_SHADER_RESOURCE_VIEW_DESC(spTexture.get(), D3D11_SRV_DIMENSION_TEXTURE2D);
    com_ptr<ID3D11ShaderResourceView> spSRV;
    check_hresult(m_d3dDevice->CreateShaderResourceView(spTexture.get(), &srvDesc, &spSRV));

    // create shared texture from the unity texture
    com_ptr<IDXGIResource1> spDXGIResource;
    check_hresult(spTexture.As(&spDXGIResource));

    HANDLE sharedHandle = INVALID_HANDLE_VALUE;
    com_ptr<ID3D11Texture2D> spMediaTexture;
    com_ptr<IDirect3DSurface> spMediaSurface;
    HRESULT hr = spDXGIResource->CreateSharedHandle(
        nullptr,
        DXGI_SHARED_RESOURCE_READ | DXGI_SHARED_RESOURCE_WRITE,
        L"SharedTextureHandle",
        &sharedHandle);

    if (SUCCEEDED(hr))
    {
        com_ptr<ID3D11Device1> spMediaDevice;
        hr = m_mediaDevice.As(&spMediaDevice);
        if (SUCCEEDED(hr))
        {
            hr = spMediaDevice->OpenSharedResource1(sharedHandle, IID_PPV_ARGS(&spMediaTexture));
            if (SUCCEEDED(hr))
            {
                hr = GetSurfaceFromTexture(spMediaTexture.get(), &spMediaSurface);
            }
        }
    }

    // if anything failed, clean up and return
    if (FAILED(hr))
    {
        if (sharedHandle != INVALID_HANDLE_VALUE)
            CloseHandle(sharedHandle);

        check_hresult(hr);
    }

    m_primaryTexture.attach(spTexture.detach());
    m_primaryTextureSRV.attach(spSRV.detach());

    m_primarySharedHandle = sharedHandle;
    m_primaryMediaTexture.attach(spMediaTexture.detach());
    m_primaryMediaSurface.attach(spMediaSurface.detach());

    return hr;
}

_Use_decl_annotations_
void RealtimeMediaPlayer::ReleaseTextures()
{
    Log(Log_Level_Info, L"RealtimeMediaPlayerImpl::ReleaseTextures()");

    // primary texture
    if (m_primarySharedHandle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_primarySharedHandle);
        m_primarySharedHandle = INVALID_HANDLE_VALUE;
    }

    m_primaryMediaSurface = nullptr;
    m_primaryMediaTexture = nullptr;
    m_primaryTextureSRV = nullptr;
    m_primaryTexture = nullptr;

    ZeroMemory(&m_textureDesc, sizeof(m_textureDesc));
}

_Use_decl_annotations_
void RealtimeMediaPlayer::ReleaseResources()
{
    m_fnStateCallback = nullptr;

    // release dx devices
    m_mediaDevice = nullptr;
    m_d3dDevice = nullptr;

    m_RealtimeMediaSource = nullptr;
}

_Use_decl_annotations_
HRESULT RealtimeMediaPlayer::OnVideoFrameAvailable(IMediaPlayer* sender, IInspectable* arg)
{
    com_ptr<IMediaPlayer> spMediaPlayer(sender);

    com_ptr<IMediaPlayer5> spMediaPlayer5;
    check_hresult(spMediaPlayer.As(&spMediaPlayer5));

    if (nullptr != m_primaryMediaSurface)
    {
        check_hresult(spMediaPlayer5->CopyFrameToVideoSurface(m_primaryMediaSurface.get()));
    }

    return S_OK;
}

_Use_decl_annotations_
void RealtimeMediaPlayer::OnOpened(
    MediaPlayer sender,
    IInspectable args)
{
    PLAYBACK_STATE playbackState;
    ZeroMemory(&playbackState, sizeof(playbackState));
    playbackState.type = StateType::StateType_Opened;
    playbackState.value.description.width = playbackSession.NaturalVideoWidth;
    playbackState.value.description.height = playbackSession.NaturalVideoHeight;
    playbackState.value.description.canSeek = playbackSession.CanSeek;
    playbackState.value.description.duration = playbackSession.NaturalDuration.Duration;

    if (m_fnStateCallback != nullptr)
    {
        m_fnStateCallback(playbackState);
    }
}

_Use_decl_annotations_
void RealtimeMediaPlayer::OnEnded(
    IMediaPlayer sender,
    IInspectable args)
{
    PLAYBACK_STATE playbackState;
    ZeroMemory(&playbackState, sizeof(playbackState));
    playbackState.type = StateType::StateType_StateChanged;
    playbackState.value.state = PlaybackState::PlaybackState_Ended;

    if (m_fnStateCallback != nullptr)
    {
        m_fnStateCallback(playbackState);
    }

}

_Use_decl_annotations_
void RealtimeMediaPlayer::OnFailed(
    MediaPlayer sender,
    MediaPlayerFailedEventArgs args)
{
    /*
    // TODO: Troy fix to get back logging
    com_ptr<HSTRING> errorMessage;
    check_hresult(args->get_ErrorMessage(errorMessage.get()));

    //LOG_RESULT_MSG(hr, WindowsGetStringRawBuffer(*errorMessage, nullptr));
        //errorMessage->c_str());
    */

    PLAYBACK_STATE playbackState;
    ZeroMemory(&playbackState, sizeof(playbackState));
    playbackState.type = StateType::StateType_Failed;
    playbackState.value.hresult = args.ExtendedErrorCode;

    if (m_fnStateCallback != nullptr)
    {
        m_fnStateCallback(playbackState);
    }
}

_Use_decl_annotations_
void RealtimeMediaPlayer::OnStateChanged(
    MediaPlaybackSession sender,
    IInspectable args)
{
    PLAYBACK_STATE playbackState;
    ZeroMemory(&playbackState, sizeof(playbackState));
    playbackState.type = StateType::StateType_StateChanged;
    playbackState.value.state = static_cast<PlaybackState>(sender.PlaybackState);

    if (m_fnStateCallback != nullptr)
    {
        m_fnStateCallback(playbackState);
    }
}