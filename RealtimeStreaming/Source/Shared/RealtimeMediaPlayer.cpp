#include "pch.h"
#include "RealtimeMediaPlayer.h"

#include <winrt/Windows.Media.Core.h>
#include <winrt/Windows.Graphics.DirectX.Direct3D11.h>
#include <Windows.Graphics.DirectX.Direct3D11.h>

#include "RealtimeMediaSource.h"

using namespace winrt;
using namespace RealtimeStreaming::Media::implementation;

using namespace Windows::Foundation;
using namespace Windows::Media::Core;
using namespace Windows::Media::MediaProperties;
using namespace Windows::Media::Playback;

using winrtPlaybackManager = RealtimeStreaming::Media::RealtimeMediaPlayer;

void RealtimeMediaPlayer::Initialize(std::weak_ptr<IUnityDeviceResource> const& unityDevice)
{
    m_primaryBuffer = std::make_shared<SharedTextureBuffer>();
    m_deviceResources = unityDevice;
}

_Use_decl_annotations_
RealtimeMediaPlayer::~RealtimeMediaPlayer()
{
    Log(Log_Level_Info, L"RealtimeMediaPlayer::~RealtimeMediaPlayer()\n");

    Shutdown();
}

void RealtimeMediaPlayer::Shutdown()
{
    Log(Log_Level_Info, L"RealtimeMediaPlayer::Shutdown()\n");

    if (m_mediaPlayer == nullptr)
    {
        Log(Log_Level_Info, L"RealtimeMediaPlayer::Shutdown() - Already shutdown\n");
        return;
    }

    m_primaryBuffer.reset();

    ReleaseMediaPlayer();

    ReleaseResources();

    m_RealtimeMediaSource = nullptr;

    m_deviceResources.reset();

    if (m_closedEvent)
    {
        m_closedEvent(nullptr, *this);
    }
}

IAsyncOperation<VideoEncodingProperties> RealtimeMediaPlayer::InitAsync(
    RealtimeStreaming::Network::Connection connection)
{
    IFT(CreateMediaPlayer());

    m_RealtimeMediaSource = winrt::make<Media::implementation::RealtimeMediaSource>();

    co_await m_RealtimeMediaSource.InitAsync(connection);

    MediaStreamSource mediaStreamSource = m_RealtimeMediaSource.MediaStreamSource();
    MediaSource source = MediaSource::CreateFromMediaStreamSource(mediaStreamSource);
    m_mediaPlayer.Source(source);

    // TODO: Turn this into MediaEncodingProfile when supporting audio
    return m_RealtimeMediaSource.VideoProperties();
}

_Use_decl_annotations_
VideoEncodingProperties RealtimeMediaPlayer::GetVideoProperties()
{
    if (m_RealtimeMediaSource != nullptr)
    {
        return m_RealtimeMediaSource.VideoProperties();
    }

    return nullptr;
}

_Use_decl_annotations_
event_token RealtimeMediaPlayer::Closed(Windows::Foundation::EventHandler<winrtPlaybackManager> const& handler)
{
    return m_closedEvent.add(handler);
}

_Use_decl_annotations_
void RealtimeMediaPlayer::Closed(event_token const& token)
{
    m_closedEvent.remove(token);
}

_Use_decl_annotations_
void RealtimeMediaPlayer::SetStateChangeCallback(RealtimeStreaming::Media::StateChangedCallback callback, 
    void* pManagedCallbackObject)
{
    m_stateChangedCallback = callback;
    m_pManagedCallbackObject = pManagedCallbackObject;
}

_Use_decl_annotations_
HRESULT RealtimeMediaPlayer::CreateStreamingTexture(
    UINT32 width,
    UINT32 height,
    void** ppvTexture_L,
    void** ppvTexture_UV)
{
    NULL_CHK_HR(ppvTexture_L, E_INVALIDARG);
    NULL_CHK_HR(ppvTexture_UV, E_INVALIDARG);

    if (width < 1 || height < 1)
    {
        IFR(E_INVALIDARG);
    }

    *ppvTexture_L = nullptr;
    *ppvTexture_UV = nullptr;

    m_primaryBuffer->Reset();

    // TODO: Look at incorporating to have all modules be like
    // https://github.com/carmines/UnityPluginCollection/blob/master/VideoPlayer/VideoPlayer/Source/Shared/Plugin.Module.h
    auto resources = m_deviceResources.lock();
    NULL_CHK_HR(resources, E_POINTER);

    com_ptr<ID3D11DeviceResource> spD3D11Resources = nullptr;
    IFR(resources->QueryInterface(__uuidof(ID3D11DeviceResource), spD3D11Resources.put_void()));

    // make sure we have created our own d3d device
    IFR(CreateResources(spD3D11Resources->GetDevice()));

    IFR(SharedTextureBuffer::Create(spD3D11Resources->GetDevice().get(),
        m_dxgiDeviceManager.get(),
        width,
        height,
        m_primaryBuffer));

    com_ptr<ID3D11ShaderResourceView> spSRV_L = nullptr;
    m_primaryBuffer->frameTextureSRV_Luma.copy_to(spSRV_L.put());

    com_ptr<ID3D11ShaderResourceView> spSRV_UV = nullptr;
    m_primaryBuffer->frameTextureSRV_Chroma.copy_to(spSRV_UV.put());

    *ppvTexture_L = spSRV_L.detach();
    *ppvTexture_UV = spSRV_UV.detach();

    return S_OK;
}

_Use_decl_annotations_
winrt::hresult RealtimeMediaPlayer::Play()
{
    NULL_CHK_HR(m_mediaPlayer, MF_E_NOT_INITIALIZED);

    HRESULT hr = S_OK;

    try
    {
        m_mediaPlayer.Play();
    }
    catch (hresult_error const & e)
    {
        hr = e.code();
    }

    return hr;
}

_Use_decl_annotations_
winrt::hresult RealtimeMediaPlayer::Pause()
{
    NULL_CHK_HR(m_mediaPlayer, MF_E_NOT_INITIALIZED);

    HRESULT hr = S_OK;

    try
    {
        m_mediaPlayer.Pause();
    }
    catch (hresult_error const & e)
    {
        hr = e.code();
    }

    return hr;
}

_Use_decl_annotations_
winrt::hresult RealtimeMediaPlayer::Stop()
{
    NULL_CHK_HR(m_mediaPlayer, MF_E_NOT_INITIALIZED);

    HRESULT hr = S_OK;

    try
    {
        m_mediaPlayer.Source(nullptr);
    }
    catch (hresult_error const & e)
    {
        hr = e.code();
    }

    return hr;
}

_Use_decl_annotations_
HRESULT RealtimeMediaPlayer::CreateMediaPlayer()
{
    if (m_mediaPlayer != nullptr)
    {
        ReleaseMediaPlayer();
    }

    m_mediaPlayer = Windows::Media::Playback::MediaPlayer();
    m_mediaPlayer.IsVideoFrameServerEnabled(true);

    m_endedToken = m_mediaPlayer.MediaEnded([=](Windows::Media::Playback::MediaPlayer const& sender,
        Windows::Foundation::IInspectable const& args)
    {
        UNREFERENCED_PARAMETER(sender);
        UNREFERENCED_PARAMETER(args);

        PLAYBACK_STATE state{};
        ZeroMemory(&state, sizeof(PLAYBACK_STATE));

        state.type = StateType::Opened;
        state.value.state = PlaybackState::None;

        if (m_stateChangedCallback)
        {
            m_stateChangedCallback(m_pManagedCallbackObject, state);
        }
    });

    m_failedToken = m_mediaPlayer.MediaFailed([=](Windows::Media::Playback::MediaPlayer const& sender,
        Windows::Media::Playback::MediaPlayerFailedEventArgs const& args)
    {
        Log(Log_Level_Info, L"RealtimeMediaPlayer::MediaFailed()\n");

        UNREFERENCED_PARAMETER(sender);
        UNREFERENCED_PARAMETER(args);

        PLAYBACK_STATE state{};
        ZeroMemory(&state, sizeof(PLAYBACK_STATE));

        state.type = StateType::Failed;
        state.value.hresult = args.ExtendedErrorCode();

        if (m_stateChangedCallback)
        {
            m_stateChangedCallback(m_pManagedCallbackObject, state);
        }
    });

    m_openedToken = m_mediaPlayer.MediaOpened([=](Windows::Media::Playback::MediaPlayer const& sender,
        Windows::Foundation::IInspectable const& args)
    {
        Log(Log_Level_Info, L"RealtimeMediaPlayer::MediaOpened()\n");

        UNREFERENCED_PARAMETER(sender);
        UNREFERENCED_PARAMETER(args);

        if (m_mediaPlaybackSession == nullptr)
        {
            return;
        }

        PLAYBACK_STATE state{};
        ZeroMemory(&state, sizeof(PLAYBACK_STATE));

        state.type = StateType::Opened;
        state.value.state = static_cast<PlaybackState>(m_mediaPlaybackSession.PlaybackState());
        state.value.description.width = static_cast<int32_t>(m_mediaPlaybackSession.NaturalVideoWidth());
        state.value.description.height = static_cast<int32_t>(m_mediaPlaybackSession.NaturalVideoHeight());

        if (m_stateChangedCallback)
        {
            m_stateChangedCallback(m_pManagedCallbackObject, state);
        }
    });

    m_videoFrameAvailableToken = m_mediaPlayer.VideoFrameAvailable([=](Windows::Media::Playback::MediaPlayer const& sender,
        Windows::Foundation::IInspectable const& args)
    {
        Log(Log_Level_Info, L"RealtimeMediaPlayer::VideoFrameAvailable()\n");

        try
        {
            UNREFERENCED_PARAMETER(sender);
            UNREFERENCED_PARAMETER(args);

            if (nullptr != m_primaryBuffer->mediaSurface)
            {
                m_mediaPlayer.CopyFrameToVideoSurface(m_primaryBuffer->mediaSurface);
            }
        }
        catch (winrt::hresult_error const& ex)
        {
            Log(Log_Level_Error, L"RealtimeMediaPlayer::VideoFrameAvailable Exception Thrown - Tid: %d \n", GetCurrentThreadId());
            LOG_RESULT_MSG(ex.to_abi(), ex.message().data());
        }
    });

    m_mediaPlaybackSession = m_mediaPlayer.PlaybackSession();
    m_stateChangedEventToken = m_mediaPlaybackSession.PlaybackStateChanged([=](Windows::Media::Playback::MediaPlaybackSession const& sender,
        Windows::Foundation::IInspectable const& args)
    {
        Log(Log_Level_Info, L"RealtimeMediaPlayer::PlaybackStateChanged()\n");

        UNREFERENCED_PARAMETER(sender);
        UNREFERENCED_PARAMETER(args);

        if (nullptr == m_mediaPlaybackSession)
        {
            return;
        }

        PLAYBACK_STATE state{};
        ZeroMemory(&state, sizeof(PLAYBACK_STATE));

        state.type = StateType::StateChanged;
        state.value.state = static_cast<PlaybackState>(m_mediaPlaybackSession.PlaybackState());

        if (m_stateChangedCallback)
        {
            m_stateChangedCallback(m_pManagedCallbackObject, state);
        }
    });

    return S_OK;
}

_Use_decl_annotations_
void RealtimeMediaPlayer::ReleaseMediaPlayer()
{
    Log(Log_Level_Info, L"RealtimeMediaPlayer::ReleaseMediaPlayer()\n");

    if (m_mediaPlaybackSession != nullptr)
    {
        m_mediaPlaybackSession.PlaybackStateChanged(m_stateChangedEventToken);

        m_mediaPlaybackSession = nullptr;
    }

    if (m_mediaPlayer != nullptr)
    {
        m_mediaPlayer.MediaEnded(m_endedToken);
        m_mediaPlayer.MediaFailed(m_failedToken);
        m_mediaPlayer.MediaOpened(m_openedToken);
        m_mediaPlayer.VideoFrameAvailable(m_videoFrameAvailableToken);

        m_mediaPlayer = nullptr;
    }
}

_Use_decl_annotations_
HRESULT RealtimeMediaPlayer::CreateResources(com_ptr<ID3D11Device> const& unityDevice)
{
    if (m_d3dDevice != nullptr)
    {
        return S_OK;
    }

    NULL_CHK_HR(unityDevice, E_INVALIDARG);

    auto sdxgiDevice = unityDevice.as<IDXGIDevice>();
    NULL_CHK_HR(sdxgiDevice, E_POINTER);

    com_ptr<IDXGIAdapter> dxgiAdapter = nullptr;
    IFR(sdxgiDevice->GetAdapter(dxgiAdapter.put()));

    // create dx device for media pipeline
    com_ptr<ID3D11Device> d3dDevice = nullptr;
    IFR(CreateMediaDevice(dxgiAdapter.get(), d3dDevice.put()));

    // lock the shared dxgi device manager
    // will keep lock open for the life of object
    //     call MFUnlockDXGIDeviceManager when unloading
    uint32_t resetToken;
    com_ptr<IMFDXGIDeviceManager> dxgiDeviceManager;
    IFR(MFLockDXGIDeviceManager(&resetToken, dxgiDeviceManager.put()));

    // associtate the device with the manager
    IFR(dxgiDeviceManager->ResetDevice(d3dDevice.get(), resetToken));

    m_d3dDevice.attach(d3dDevice.detach());
    m_resetToken = resetToken;
    m_dxgiDeviceManager = dxgiDeviceManager;

    return S_OK;
}

_Use_decl_annotations_
void RealtimeMediaPlayer::ReleaseResources()
{
    Log(Log_Level_Info, L"RealtimeMediaPlayer::ReleaseResources()\n");

    if (m_dxgiDeviceManager != nullptr)
    {
        m_dxgiDeviceManager->ResetDevice(m_d3dDevice.get(), m_resetToken);

        // release the default media foundation dxgi manager
        MFUnlockDXGIDeviceManager();

        m_dxgiDeviceManager = nullptr;
    }

    if (m_d3dDevice != nullptr)
    {
        m_d3dDevice = nullptr;
    }
}
