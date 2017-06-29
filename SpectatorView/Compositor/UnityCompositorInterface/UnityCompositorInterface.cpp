// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "stdafx.h"
#include "CompositorShared.h"
#include <Windows.h>
#include <ppltasks.h>

#include "DirectXHelper.h"
#include "CompositorInterface.h"
#include "HologramQueue.h"

#include "PluginAPI\IUnityInterface.h"
#include "PluginAPI\IUnityGraphics.h"
#include "PluginAPI\IUnityGraphicsD3D11.h"


#define UNITYDLL EXTERN_C __declspec(dllexport)

static CompositorInterface* ci = NULL;
static bool isRecording = false;
static bool videoInitialized = false;

static BYTE* colorBytes = new BYTE[FRAME_BUFSIZE];
static BYTE* holoBytes = new BYTE[FRAME_BUFSIZE];
#if USE_CANON_SDK
static BYTE* hiResHoloBytes = new BYTE[HOLOGRAM_BUFSIZE_HIRES];
#endif
#if HARDWARE_ENCODE_VIDEO
byte* videoBytes = new byte[(int)(1.5f * FRAME_WIDTH * FRAME_HEIGHT)];
#else
byte* videoBytes = new byte[FRAME_BUFSIZE];
#endif

static ID3D11Texture2D* g_holoRenderTexture = nullptr;
#if USE_CANON_SDK
static ID3D11Texture2D* g_hiResHoloRenderTexture = nullptr;
#endif

static ID3D11Texture2D* g_colorTexture = nullptr;
static ID3D11Texture2D* g_holoTexture = nullptr;
static ID3D11Texture2D* g_videoTexture = nullptr;
static ID3D11Texture2D* g_outputTexture = nullptr;

static ID3D11ShaderResourceView* g_UnityColorSRV = nullptr;
static ID3D11ShaderResourceView* g_UnityHoloSRV = nullptr;

static ID3D11Device* g_pD3D11Device = NULL;

static float _frameOffset = INITIAL_FRAME_OFFSET;
static LONGLONG colorTime = INVALID_TIMESTAMP;

static bool takePicture = false;
static bool takeHiResPicture = false;

static CRITICAL_SECTION lock;

static IUnityInterfaces *s_UnityInterfaces = nullptr;
static IUnityGraphics *s_Graphics = nullptr;

static void UNITY_INTERFACE_API OnGraphicsDeviceEvent(UnityGfxDeviceEventType eventType)
{
    switch (eventType)
    {
    case kUnityGfxDeviceEventInitialize:
    {
        IUnityGraphicsD3D11* d3d11 = s_UnityInterfaces->Get<IUnityGraphicsD3D11>();
        if (d3d11 != nullptr)
        {
            g_pD3D11Device = d3d11->GetDevice();
        }
    }
    break;
    case kUnityGfxDeviceEventShutdown:
        g_pD3D11Device = NULL;
        break;
    }
}

void UNITY_INTERFACE_API UnityPluginLoad(IUnityInterfaces *unityInterfaces)
{
    InitializeCriticalSection(&lock);

    s_UnityInterfaces = unityInterfaces;
    s_Graphics = s_UnityInterfaces->Get<IUnityGraphics>();
    s_Graphics->RegisterDeviceEventCallback(OnGraphicsDeviceEvent);

    OnGraphicsDeviceEvent(kUnityGfxDeviceEventInitialize);
}

void UNITY_INTERFACE_API UnityPluginUnload()
{
    s_Graphics->UnregisterDeviceEventCallback(OnGraphicsDeviceEvent);
    OnGraphicsDeviceEvent(kUnityGfxDeviceEventShutdown);

    DeleteCriticalSection(&lock);
}

UNITYDLL void UpdateCompositor()
{
    if (ci == NULL)
    {
        return;
    }

    ci->Update();
}

// Expose queue state so Unity can determine whether to render the spectator view's render texture or the queued hologram texture.
UNITYDLL bool QueueingHoloFrames()
{
    return QUEUE_FRAMES;
}

struct HoloLensPose
{
public:
    HoloLensPose()
    {
        _rotX = 0;
        _rotY = 0;
        _rotZ = 0;
        _rotW = 0;

        _posX = 0;
        _posY = 0;
        _posZ = 0;

        _timestamp = 0;
        _colorTime = 0;
    }

    HoloLensPose(HoloLensPose& copy)
    {
        _rotX = copy._rotX;
        _rotY = copy._rotY;
        _rotZ = copy._rotZ;
        _rotW = copy._rotW;

        _posX = copy._posX;
        _posY = copy._posY;
        _posZ = copy._posZ;

        _timestamp = copy._timestamp;
        _colorTime = copy._colorTime;
    }

    HoloLensPose(
        float rotX, float rotY, float rotZ, float rotW,
        float posX, float posY, float posZ,
        LONGLONG timestamp, LONGLONG colorTime)
    {
        _rotX = rotX;
        _rotY = rotY;
        _rotZ = rotZ;
        _rotW = rotW;

        _posX = posX;
        _posY = posY;
        _posZ = posZ;

        _timestamp = timestamp;
        _colorTime = colorTime;
    }

    float _rotX, _rotY, _rotZ, _rotW, _posX, _posY, _posZ;
    LONGLONG _timestamp;
    LONGLONG _colorTime;
};

static HoloLensPose thirdPose = HoloLensPose::HoloLensPose();

UNITYDLL void GetEarliestHologramPose(
    float& rotX, float& rotY, float& rotZ, float& rotW,
    float& posX, float& posY, float& posZ, float& timestamp)
{
    rotX = thirdPose._rotX;
    rotY = thirdPose._rotY;
    rotZ = thirdPose._rotZ;
    rotW = thirdPose._rotW;

    posX = thirdPose._posX;
    posY = thirdPose._posY;
    posZ = thirdPose._posZ;

    timestamp = thirdPose._timestamp;
}

// Set hologram poses when we get them on the network.
UNITYDLL void SetHologramPose(
    float rotX, float rotY, float rotZ, float rotW,
    float posX, float posY, float posZ, float msOffset)
{
    if (ci == nullptr)
    {
        return;
    }

    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);

    LONGLONG timestamp = time.QuadPart;

    // Convert offset to microseconds.
    LONGLONG offset = (LONGLONG)(msOffset * 1000.0f);

    auto hologramFrame = ci->GetNextHologramFrame(timestamp - offset);
    if (hologramFrame != nullptr)
    {
        hologramFrame->rotX = rotX;
        hologramFrame->rotY = rotY;
        hologramFrame->rotZ = rotZ;
        hologramFrame->rotW = rotW;

        hologramFrame->posX = posX;
        hologramFrame->posY = posY;
        hologramFrame->posZ = posZ;
    }
}

static bool newColorFrame = false;
UNITYDLL bool NewColorFrame()
{
    bool value = newColorFrame;

    if (value == true)
    {
        newColorFrame = false;
    }

    return value;
}

UNITYDLL void UpdateSpectatorView()
{
    if (ci == nullptr)
    {
        return;
    }

    LONGLONG cachedTime = colorTime;
    colorTime = ci->GetTimestamp();
    if (cachedTime != colorTime)
    {
        LONGLONG frameDuration = ci->GetColorDuration();
        // Find a pose on the leading end of this color frame.
        const auto frame = ci->FindClosestHologramFrame(
            colorTime - frameDuration, (LONGLONG)(_frameOffset * (float)frameDuration));

        if (frame != nullptr)
        {
            thirdPose._rotX = frame->rotX;
            thirdPose._rotY = frame->rotY;
            thirdPose._rotZ = frame->rotZ;
            thirdPose._rotW = frame->rotW;
            thirdPose._posX = frame->posX;
            thirdPose._posY = frame->posY;
            thirdPose._posZ = frame->posZ;
            thirdPose._timestamp = frame->timeStamp;
            thirdPose._colorTime = colorTime;
        }

        newColorFrame = true;
    }
}

// Plugin function to handle a specific rendering event
static void __stdcall OnRenderEvent(int eventID)
{
    if (ci == nullptr)
    {
        return;
    }

    // Update hologram texture from the spectator view camera.
    DirectXHelper::CopyTexture(g_pD3D11Device, g_holoTexture, g_holoRenderTexture);

    EnterCriticalSection(&lock);

    if (g_pD3D11Device != nullptr)
    {
        ci->UpdateFrameProvider();

        if (!videoInitialized && ci != nullptr)
        {
            videoInitialized = ci->InitializeVideoEncoder(g_pD3D11Device);
        }

        // Update hi res holo bytes
#if USE_CANON_SDK
        if (g_hiResHoloRenderTexture != nullptr)
        {
            if (ci != nullptr && takeHiResPicture)
            {
                takeHiResPicture = false;

                DirectXHelper::GetBytesFromTexture(g_pD3D11Device, g_hiResHoloRenderTexture, FRAME_BPP, hiResHoloBytes);
                DirectXHelper::FlipHorizontally(hiResHoloBytes, HOLOGRAM_HEIGHT_HIRES, HOLOGRAM_WIDTH_HIRES * FRAME_BPP);
                ci->TakeCanonPicture(g_pD3D11Device, hiResHoloBytes);
            }
        }
#endif

        if (takePicture && g_colorTexture != nullptr)
        {
            takePicture = false;

            BYTE* colorBytes = new BYTE[FRAME_BUFSIZE];
            BYTE* holoBytes = new BYTE[FRAME_BUFSIZE];
            BYTE* mergedBytes = new BYTE[FRAME_BUFSIZE];
            BYTE* colorBytesRaw = new BYTE[FRAME_BUFSIZE_RAW];

            DirectXHelper::GetBytesFromTexture(g_pD3D11Device, g_colorTexture, FRAME_BPP, colorBytes);

            if (ci->OutputYUV())
            {
                memcpy(colorBytesRaw, colorBytes, FRAME_BUFSIZE_RAW);
                DirectXHelper::ConvertYUVtoBGRA(colorBytesRaw, colorBytes, FRAME_WIDTH, FRAME_HEIGHT, true);
            }
            else
            {
                DirectXHelper::ConvertBGRAtoRGBA(colorBytes, FRAME_WIDTH, FRAME_HEIGHT, true);
            }

            DirectXHelper::GetBytesFromTexture(g_pD3D11Device, g_holoRenderTexture, FRAME_BPP, holoBytes);
            DirectXHelper::FlipHorizontally(holoBytes, FRAME_HEIGHT, FRAME_WIDTH * FRAME_BPP);

            memcpy(mergedBytes, colorBytes, FRAME_BUFSIZE);
            DirectXHelper::AlphaBlend(mergedBytes, holoBytes, FRAME_BUFSIZE, ci->GetAlpha());

            ci->TakePicture(g_pD3D11Device, FRAME_WIDTH, FRAME_HEIGHT, FRAME_BPP, mergedBytes, colorBytes, holoBytes);

            delete[] colorBytes;
            delete[] colorBytesRaw;
            delete[] holoBytes;
            delete[] mergedBytes;
        }

        LONGLONG cachedFrameTime = ci->GetTimestamp();
        if (isRecording &&
            g_videoTexture != nullptr &&
            cachedFrameTime != INVALID_TIMESTAMP &&
            ci->IsVideoFrameReady())
        {
#if HARDWARE_ENCODE_VIDEO
            DirectXHelper::GetBytesFromTexture(g_pD3D11Device, g_videoTexture, 1.5f, videoBytes);
#else
            DirectXHelper::GetBytesFromTexture(g_pD3D11Device, g_videoTexture, FRAME_BPP, videoBytes);
#endif
            ci->RecordFrameAsync(videoBytes, cachedFrameTime);
        }
    }

    LeaveCriticalSection(&lock);
}

UNITYDLL LONGLONG GetColorDuration()
{
    if (ci != nullptr)
    {
        return ci->GetColorDuration();
    }

    return (LONGLONG)((1.0f / 30.0f) * QPC_MULTIPLIER);
}

// Function to pass a callback to plugin-specific scripts
EXTERN_C UnityRenderingEvent __declspec(dllexport) __stdcall GetRenderEventFunc()
{
    return OnRenderEvent;
}

UNITYDLL int GetFrameWidth()
{
    return HOLOGRAM_WIDTH;
}

UNITYDLL int GetFrameHeight()
{
    return HOLOGRAM_HEIGHT;
}

UNITYDLL int GetFrameWidthHiRes()
{
    return HOLOGRAM_WIDTH_HIRES;
}

UNITYDLL int GetFrameHeightHiRes()
{
    return HOLOGRAM_HEIGHT_HIRES;
}

UNITYDLL bool CaptureHiResHolograms()
{
    return USE_CANON_SDK;
}

UNITYDLL bool InitializeFrameProvider()
{
    if (g_outputTexture == nullptr ||
        g_UnityColorSRV == nullptr ||
        g_pD3D11Device == nullptr)
    {
        return false;
    }

    if (ci == nullptr)
    {
        ci = new CompositorInterface();
    }

    bool setColorSRV = false;
    if (ci != nullptr)
    {
        setColorSRV = ci->Initialize(g_pD3D11Device, g_UnityColorSRV, g_outputTexture);
    }

    return ci != nullptr && setColorSRV;
}

UNITYDLL void StopFrameProvider()
{
    if (ci != NULL)
    {
        ci->StopFrameProvider();
    }
}

UNITYDLL void SetAudioData(BYTE* audioData)
{
    if (!isRecording)
    {
        return;
    }

#if ENCODE_AUDIO
    // Get the time for the audio frame.
    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);

    if (ci != nullptr)
    {
        ci->RecordAudioFrameAsync(audioData, time.QuadPart);
    }
#endif
}

UNITYDLL void TakePicture()
{
    takePicture = true;
}

UNITYDLL void TakeCanonPicture()
{
#if USE_CANON_SDK
    takeHiResPicture = true;
#endif
}

UNITYDLL void StartRecording()
{
    if (videoInitialized && ci != nullptr)
    {
        ci->StartRecording();
        isRecording = true;
    }
}

UNITYDLL void StopRecording()
{
    if (videoInitialized && ci != nullptr)
    {
        ci->StopRecording();
        isRecording = false;
    }
}

UNITYDLL bool IsRecording()
{
    return isRecording;
}

UNITYDLL void SetFrameOffset(float frameOffset)
{
    _frameOffset = frameOffset;
}

UNITYDLL float GetFrameOffset()
{
    return _frameOffset;
}

UNITYDLL void SetAlpha(float alpha)
{
    if (ci != NULL)
    {
        ci->SetAlpha(alpha);
    }
}

UNITYDLL float GetAlpha()
{
    if (ci != NULL)
    {
        return ci->GetAlpha();
    }

    return 0;
}

UNITYDLL void Reset()
{
    EnterCriticalSection(&lock);
    g_colorTexture = nullptr;
    g_holoTexture = nullptr;
    g_videoTexture = nullptr;
    g_outputTexture = nullptr;

    g_holoRenderTexture = nullptr;
#if USE_CANON_SDK
    g_hiResHoloRenderTexture = nullptr;
#endif

    g_UnityColorSRV = nullptr;
    g_UnityHoloSRV = nullptr;

    LeaveCriticalSection(&lock);
}

UNITYDLL bool OutputYUV()
{
    if (ci == nullptr)
    {
        return false;
    }

    return ci->OutputYUV();
}

UNITYDLL LONGLONG GetCurrentUnityTime()
{
    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);
    return time.QuadPart;
}

UNITYDLL bool HardwareEncodeVideo()
{
    return HARDWARE_ENCODE_VIDEO;
}

#pragma region CreateExternalTextures
UNITYDLL bool SetHoloTexture(ID3D11Texture2D* holoTexture)
{
    // We have already set a texture ptr.
    if (g_holoRenderTexture != nullptr)
    {
        return true;
    }

    if (g_holoRenderTexture == nullptr)
    {
        g_holoRenderTexture = holoTexture;
    }

    return g_holoRenderTexture != nullptr;
}

UNITYDLL bool SetHoloTextureHiRes(ID3D11Texture2D* holoTexture)
{
#if USE_CANON_SDK
    // We have already set a texture ptr.
    if (g_hiResHoloRenderTexture != nullptr)
    {
        return true;
    }

    if (g_hiResHoloRenderTexture == nullptr)
    {
        g_hiResHoloRenderTexture = holoTexture;
    }

    return g_hiResHoloRenderTexture != nullptr;
#else

    return true;
#endif
}

UNITYDLL bool SetVideoRenderTexture(ID3D11Texture2D* tex)
{
    if (g_videoTexture == nullptr)
    {
        g_videoTexture = tex;
    }

    return g_videoTexture != nullptr;
}

UNITYDLL bool SetOutputRenderTexture(ID3D11Texture2D* tex)
{
    if (g_outputTexture == nullptr)
    {
        g_outputTexture = tex;
    }

    return g_outputTexture != nullptr;
}

UNITYDLL bool CreateUnityColorTexture(ID3D11ShaderResourceView*& srv)
{
    if (g_UnityColorSRV == nullptr && g_pD3D11Device != nullptr)
    {
        g_colorTexture = DirectXHelper::CreateTexture(g_pD3D11Device, colorBytes, FRAME_WIDTH, FRAME_HEIGHT, FRAME_BPP);

        if (g_colorTexture == nullptr)
        {
            return false;
        }

        g_UnityColorSRV = DirectXHelper::CreateShaderResourceView(g_pD3D11Device, g_colorTexture);
        if (g_UnityColorSRV == nullptr)
        {
            return false;
        }
    }

    srv = g_UnityColorSRV;
    return true;
}

UNITYDLL bool CreateUnityHoloTexture(ID3D11ShaderResourceView*& srv)
{
    if (g_UnityHoloSRV == nullptr && g_pD3D11Device != nullptr)
    {
        g_holoTexture = DirectXHelper::CreateTexture(g_pD3D11Device, holoBytes, FRAME_WIDTH, FRAME_HEIGHT, FRAME_BPP);

        if (g_holoTexture == nullptr)
        {
            return false;
        }

        g_UnityHoloSRV = DirectXHelper::CreateShaderResourceView(g_pD3D11Device, g_holoTexture);
        if (g_UnityHoloSRV == nullptr)
        {
            return false;
        }
    }

    srv = g_UnityHoloSRV;
    return true;
}
#pragma endregion CreateExternalTextures

// These functions are included for backwards compatibility
#pragma region BackCompat
UNITYDLL void SetHoloTime()
{
}

UNITYDLL void SetExplicitHoloTime(LONGLONG holoTime)
{
}

UNITYDLL int GetFrameDelta()
{
    return 0;
}

static LONGLONG _networkLatency = 0;

// HNS
UNITYDLL void SetNetworkLatency(LONGLONG networkLatency)
{
    _networkLatency = networkLatency;
}
#pragma endregion BackCompat
