// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "stdafx.h"
#include "CompositorShared.h"
#include <Windows.h>
#include <ppltasks.h>

#include "DirectXHelper.h"
#include "CompositorInterface.h"

#include "PluginAPI\IUnityInterface.h"
#include "PluginAPI\IUnityGraphics.h"
#include "PluginAPI\IUnityGraphicsD3D11.h"
#include "BufferedTextureFetch.h"


#define UNITYDLL EXTERN_C __declspec(dllexport)

static CompositorInterface* ci = NULL;
static bool isRecording = false;
static bool videoInitialized = false;

static BYTE* colorBytes = new BYTE[FRAME_BUFSIZE];
static BYTE* holoBytes = new BYTE[FRAME_BUFSIZE];

#define NUM_VIDEO_BUFFERS 10

static byte** videoBytes = nullptr;
static int videoBufferIndex = 0;

void AllocateVideoBuffers()
{
    if (videoBytes != nullptr)
        return;

    videoBytes = new byte*[NUM_VIDEO_BUFFERS];

    for (int i = 0; i < NUM_VIDEO_BUFFERS; i++)
    {
#if HARDWARE_ENCODE_VIDEO
        videoBytes[i] = new byte[(int)(1.5f * FRAME_WIDTH * FRAME_HEIGHT)];
#else
        videoBytes[i] = new byte[FRAME_BUFSIZE];
#endif
    }
}

void FreeVideoBuffers()
{
    if (videoBytes == nullptr)
        return;

    for (int i = 0; i < NUM_VIDEO_BUFFERS; i++)
    {
        delete[] videoBytes[i];
    }
    delete[] videoBytes;
    videoBytes = nullptr;
}


static ID3D11Texture2D* g_holoRenderTexture = nullptr;

static ID3D11Texture2D* g_colorTexture = nullptr;
static ID3D11Texture2D* g_compositeTexture = nullptr;
static ID3D11Texture2D* g_videoTexture = nullptr;
static ID3D11Texture2D* g_outputTexture = nullptr;

static ID3D11ShaderResourceView* g_UnityColorSRV = nullptr;

static ID3D11Device* g_pD3D11Device = NULL;

static LONGLONG colorTime = INVALID_TIMESTAMP;

static bool takePicture = false;
static bool takeRawPicture = false;
static std::wstring rawPicturePath;

static CRITICAL_SECTION lock;

static IUnityInterfaces *s_UnityInterfaces = nullptr;
static IUnityGraphics *s_Graphics = nullptr;

static int lastRecordedVideoFrame = -1;
static int lastVideoFrame = -1;
static BufferedTextureFetch VideoTextureBuffer;

static bool isInitialized = false;


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
        VideoTextureBuffer.ReleaseTextures();
        g_pD3D11Device = NULL;
        break;
    }
}

void UNITY_INTERFACE_API UnityPluginLoad(IUnityInterfaces *unityInterfaces)
{
    AllocateVideoBuffers();

    InitializeCriticalSection(&lock);

    s_UnityInterfaces = unityInterfaces;
    s_Graphics = s_UnityInterfaces->Get<IUnityGraphics>();
    s_Graphics->RegisterDeviceEventCallback(OnGraphicsDeviceEvent);

    OnGraphicsDeviceEvent(kUnityGfxDeviceEventInitialize);
}

void UNITY_INTERFACE_API UnityPluginUnload()
{
    FreeVideoBuffers();
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

    //Update video encoder
    ci->Update();
}

UNITYDLL void UpdateSpectatorView()
{
    if (ci == nullptr)
    {
        return;
    }

    colorTime = ci->GetTimestamp(ci->GetCaptureFrameIndex());
}

static LONGLONG queuedVideoFrameTime;
static int queuedVideoFrameCount = 0;

void UpdateVideoRecordingFrame()
{
    //We have an old frame, lets get the data and queue it now
    if (VideoTextureBuffer.IsDataAvailable())
    {
        videoBufferIndex = (videoBufferIndex + 1) % NUM_VIDEO_BUFFERS;
#if HARDWARE_ENCODE_VIDEO
        float bpp = 1.5f;
#else
        float bpp = FRAME_BPP;
#endif

        VideoTextureBuffer.FetchTextureData(g_pD3D11Device, videoBytes[videoBufferIndex], bpp);
        ci->RecordFrameAsync(videoBytes[videoBufferIndex], queuedVideoFrameTime, queuedVideoFrameCount);
    }


    if (lastVideoFrame >= 0 && lastRecordedVideoFrame != lastVideoFrame)
    {
        queuedVideoFrameCount = ci->compositeFrameIndex - lastVideoFrame;
        if (queuedVideoFrameCount <= 0)
            queuedVideoFrameCount = lastVideoFrame - lastRecordedVideoFrame;
        if (queuedVideoFrameCount <= 0)
            queuedVideoFrameCount = 1;

        lastRecordedVideoFrame = lastVideoFrame;
        queuedVideoFrameTime = lastVideoFrame * ci->GetColorDuration();
        VideoTextureBuffer.PrepareTextureFetch(g_pD3D11Device, g_videoTexture);
    }

    lastVideoFrame = ci->compositeFrameIndex;
}

// Plugin function to handle a specific rendering event
static void __stdcall OnRenderEvent(int eventID)
{
    if (ci == nullptr)
    {
        return;
    }

    //  Update hologram texture from the spectator view camera.
    ci->UpdateFrameProvider();

    EnterCriticalSection(&lock);

    if (g_pD3D11Device != nullptr)
    {
        if (!videoInitialized && ci != nullptr)
        {
            videoInitialized = ci->InitializeVideoEncoder(g_pD3D11Device);
        }

        if (isRecording &&
            g_videoTexture != nullptr)
        {
            UpdateVideoRecordingFrame();
        }

        if (takePicture && g_colorTexture != nullptr)
        {
            takePicture = false;

            DirectXHelper::GetBytesFromTexture(g_pD3D11Device, g_compositeTexture, FRAME_BPP, holoBytes);

            DirectXHelper::FlipHorizontally(holoBytes, FRAME_HEIGHT, FRAME_WIDTH * FRAME_BPP);
            ci->TakePicture(g_pD3D11Device, FRAME_WIDTH, FRAME_HEIGHT, FRAME_BPP, holoBytes);
        }

        if (takeRawPicture && g_colorTexture != nullptr)
        {
            takeRawPicture = false;

            DirectXHelper::GetBytesFromTexture(g_pD3D11Device, g_compositeTexture, FRAME_BPP, holoBytes);

            std::ofstream strm;
            strm.open(rawPicturePath, std::ios::out | std::ios::binary | std::ios::trunc);
            strm.write((const char*)(holoBytes), FRAME_BUFSIZE);
            strm.close();
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

UNITYDLL int GetCaptureFrameIndex()
{
    if (ci != nullptr)
    {
        return ci->GetCaptureFrameIndex();
    }

    return 0;
}

UNITYDLL int GetPixelChange(int frame)
{
    if (ci != nullptr)
    {
        return ci->GetPixelChange(frame);
    }

    return 0;
}

UNITYDLL int GetNumQueuedOutputFrames()
{
    if (ci != nullptr)
    {
        return ci->GetNumQueuedOutputFrames();
    }

    return 0;
}

UNITYDLL void SetCompositeFrameIndex(int index)
{
    if (ci != nullptr)
        return ci->SetCompositeFrameIndex(index);
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

UNITYDLL bool InitializeFrameProviderOnDevice(int providerId)
{
    if (g_outputTexture == nullptr ||
        g_UnityColorSRV == nullptr ||
        g_pD3D11Device == nullptr)
    {
        return false;
    }

    if (isInitialized)
    {
        return true;
    }

    if (ci == nullptr)
    {
        ci = new CompositorInterface();
    }

    ci->SetFrameProvider((IFrameProvider::ProviderType) providerId);
    isInitialized = ci->Initialize(g_pD3D11Device, g_UnityColorSRV, g_outputTexture);

    return isInitialized;
}

UNITYDLL void StopFrameProvider()
{
    if (ci != NULL)
    {
        ci->StopFrameProvider();
    }
}

UNITYDLL void SetAudioData(BYTE* audioData, int audioSize, double audioTime)
{
    if (!isRecording)
    {
        return;
    }
    
#if ENCODE_AUDIO
    if (ci != nullptr)
    {
        ci->RecordAudioFrameAsync(audioData, audioSize, audioTime);
    }
#endif    
}

UNITYDLL void TakePicture()
{
    takePicture = true;

}

UNITYDLL void TakeRawPicture(LPCWSTR lpFilePath)
{
    takeRawPicture = true;
    rawPicturePath = lpFilePath;
}

UNITYDLL void StartRecording()
{
    if (videoInitialized && ci != nullptr)
    {
        lastVideoFrame = -1;
        VideoTextureBuffer.Reset();
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
    g_compositeTexture = nullptr;
    g_videoTexture = nullptr;
    g_outputTexture = nullptr;

    g_holoRenderTexture = nullptr;

    g_UnityColorSRV = nullptr;

    isInitialized = false;

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
    if (g_compositeTexture != nullptr)
    {
        return true;
    }

    if (g_compositeTexture == nullptr)
    {
		g_compositeTexture = holoTexture;
    }

    return g_compositeTexture != nullptr;
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
#pragma endregion CreateExternalTextures
