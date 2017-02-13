// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "stdafx.h"
#include "CompositorShared.h"
#include <Windows.h>
#include <ppltasks.h>

#include "DirectXHelper.h"
#include "CompositorInterface.h"
#include "HologramQueue.h"

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

static LONGLONG prevFrameDelta = 0;
static LONGLONG prevFrameDeltaNoModification = 0;
static LONGLONG lastColorTime = INVALID_TIMESTAMP;
static LONGLONG lastHoloTime = INVALID_TIMESTAMP;
static LONGLONG prevColorTime = INVALID_TIMESTAMP;
// Flag to indicate to Unity that a new color frame is ready.
static bool newColorFrameExternal = false;

static float _frameOffset = INITIAL_FRAME_OFFSET;
static LONGLONG _networkLatency = 0;

static bool takePicture = false;
static bool takeHiResPicture = false;

static CRITICAL_SECTION lock;

// https://docs.unity3d.com/430/Documentation/Manual/NativePluginInterface.html
enum GfxDeviceRenderer {
    kGfxRendererOpenGL = 0,              // OpenGL
    kGfxRendererD3D9 = 1,                // Direct3D 9
    kGfxRendererD3D11 = 2,               // Direct3D 11
    kGfxRendererGCM = 3,                 // Sony PlayStation 3 GCM
    kGfxRendererNull = 4,                // "null" device (used in batch mode)
    kGfxRendererHollywood = 5,           // Nintendo Wii
    kGfxRendererXenon = 6,               // Xbox 360
    kGfxRendererOpenGLES = 7,            // OpenGL ES 1.1
    kGfxRendererOpenGLES20Mobile = 8,    // OpenGL ES 2.0 mobile variant
    kGfxRendererMolehill = 9,            // Flash 11 Stage3D
    kGfxRendererOpenGLES20Desktop = 10,  // OpenGL ES 2.0 desktop variant (i.e. NaCl)
};

enum GfxDeviceEventType {
    kGfxDeviceEventInitialize = 0,
    kGfxDeviceEventShutdown = 1,
    kGfxDeviceEventBeforeReset = 2,
    kGfxDeviceEventAfterReset = 3,
};

// Certain Unity APIs (GL.IssuePluginEvent, CommandBuffer.IssuePluginEvent) can callback into native plugins.
// Provide them with an address to a function of this signature.
typedef void(_stdcall * UnityRenderingEvent)(int eventId);

// Get the GraphicsDevice so we can get texture data.
UNITYDLL void UnitySetGraphicsDevice(void* device, int deviceType, int eventType)
{
    if (g_pD3D11Device != NULL)
    {
        return;
    }

    if (deviceType == GfxDeviceRenderer::kGfxRendererD3D11)
    {
        if (eventType == GfxDeviceEventType::kGfxDeviceEventInitialize)
        {
            g_pD3D11Device = (ID3D11Device*)device;
            InitializeCriticalSection(&lock);
        }
        else if (eventType == GfxDeviceEventType::kGfxDeviceEventShutdown)
        {
            g_pD3D11Device = NULL;
            DeleteCriticalSection(&lock);
        }
    }
}

UNITYDLL void UpdateCompositor()
{
    if (ci == NULL)
    {
        return;
    }

    ci->Update();

    prevColorTime = lastColorTime;
    lastColorTime = ci->GetColorTime();

    // These flags are only queried from Unity's update loop, so a lock is not needed.
    if (prevColorTime != lastColorTime)
    {
        newColorFrameExternal = true;
    }
}

UNITYDLL void SetHoloTime()
{
    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);

    time.QuadPart;
    lastHoloTime = time.QuadPart;
}

UNITYDLL void SetExplicitHoloTime(LONGLONG holoTime)
{
    lastHoloTime = holoTime;
}

// Expose queue state so Unity can determine whether to render the spectator view's render texture or the queued hologram texture.
UNITYDLL bool QueueingHoloFrames()
{
    return QUEUE_FRAMES;
}

// Plugin function to handle a specific rendering event
static void __stdcall OnRenderEvent(int eventID)
{
    if (ci == nullptr)
    {
        return;
    }

#if QUEUE_FRAMES
    // Get the time of the current render event.
    LARGE_INTEGER time;

    QueryPerformanceCounter(&time);

    // Update Hologram Queue
    if (ci != nullptr && g_holoRenderTexture != nullptr && g_holoTexture != nullptr && g_pD3D11Device != nullptr)
    {
        LONGLONG frameDuration = ci->GetColorDuration();
        auto timestamp = (time.QuadPart) + (LONGLONG)(_frameOffset * (float)frameDuration);

        const auto hologramFrame = ci->GetNextHologramFrame(timestamp);
        if (hologramFrame != nullptr)
        {
            DirectXHelper::CopyTexture(g_pD3D11Device, hologramFrame->holoTexture, g_holoRenderTexture);

            const auto closestHologramFrame = ci->FindClosestHologramFrame(ci->GetColorTime(), prevFrameDeltaNoModification);
            if (closestHologramFrame != nullptr)
            {
                if (closestHologramFrame->GetId() != hologramFrame->GetId())
                {
                    DirectXHelper::CopyTexture(g_pD3D11Device, g_holoTexture, closestHologramFrame->holoTexture);
                }
                else
                {
                    DirectXHelper::CopyTexture(g_pD3D11Device, g_holoTexture, g_holoRenderTexture);
                }
            }
        }
    }
#endif

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

        LONGLONG cachedFrameTime = ci->GetColorTime();
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

UNITYDLL void SetNetworkLatency(LONGLONG networkLatency)
{
    _networkLatency = networkLatency;
}

UNITYDLL int GetFrameDelta()
{
    if (ci == nullptr)
    {
        return 0;
    }

    if (lastColorTime == INVALID_TIMESTAMP)
    {
        return 0;
    }

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);

    LONGLONG frameDuration = ci->GetColorDuration();
    LONGLONG frameDelta = (lastHoloTime - lastColorTime);

    if (frameDelta > 0) { frameDelta *= -1; }

    frameDelta -= (LONGLONG)(_frameOffset * (float)frameDuration);
    prevFrameDeltaNoModification = frameDelta;

    frameDelta -= (LONGLONG)(_networkLatency / 2.0f);

    frameDelta *= QPC_MULTIPLIER;
    frameDelta /= frequency.QuadPart;

    prevFrameDelta = frameDelta;

    return (int)frameDelta;
}

UNITYDLL LONGLONG GetColorDuration()
{
    if (ci != nullptr)
    {
        return ci->GetColorDuration();
    }

    return (LONGLONG)((1.0f / 30.0f) * QPC_MULTIPLIER);
}

UNITYDLL bool NewColorFrame()
{
    bool ret = newColorFrameExternal;

    if (newColorFrameExternal)
    {
        newColorFrameExternal = false;
    }

    return ret;
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
    if (g_UnityColorSRV == nullptr)
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
    if (g_UnityHoloSRV == nullptr)
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