// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "stdafx.h"
#include "CompositorInterface.h"


CompositorInterface::CompositorInterface()
{
    wchar_t myDocumentsPath[1024];
    SHGetFolderPathW(0, CSIDL_MYDOCUMENTS, 0, 0, myDocumentsPath);
    outputPath = std::wstring(myDocumentsPath) + L"\\HologramCapture\\";

    DirectoryHelper::CreateOutputDirectory(outputPath);

#if USE_DECKLINK || USE_DECKLINK_SHUTTLE
    frameProvider = new DeckLinkManager();
#endif
#if USE_ELGATO
    frameProvider = new ElgatoFrameProvider();
#endif
#if USE_OPENCV
    frameProvider = new OpenCVFrameProvider();
#endif

}

CompositorInterface::~CompositorInterface()
{
    delete[] photoBytes;
}

bool CompositorInterface::Initialize(ID3D11Device* device, ID3D11ShaderResourceView* colorSRV, ID3D11Texture2D* outputTexture)
{
    if (frameProvider == nullptr)
    {
        return false;
    }

    if (frameProvider->IsEnabled())
    {
        return true;
    }

    _device = device;

    HRESULT hr = frameProvider->Initialize(colorSRV);
    if (SUCCEEDED(hr))
    {
        frameProvider->SetOutputTexture(outputTexture);
    }
    return SUCCEEDED(hr);
}

// Call from render thread.
void CompositorInterface::UpdateFrameProvider()
{
    if (frameProvider != nullptr)
    {
        frameProvider->Update();
    }
}

void CompositorInterface::Update()
{
    if (videoEncoder != nullptr)
    {
        videoEncoder->Update();
    }
}

void CompositorInterface::StopFrameProvider()
{
    if (frameProvider != nullptr)
    {
        frameProvider->Dispose();
    }
}

LONGLONG CompositorInterface::GetTimestamp()
{
    if (frameProvider != nullptr)
    {
        return frameProvider->GetTimestamp();
    }

    return INVALID_TIMESTAMP;
}

LONGLONG CompositorInterface::GetFrameDelayMS()
{
    if (frameProvider != nullptr)
    {
        return frameProvider->GetFrameDelayMS();
    }

    return 0;
}

#pragma region
void CompositorInterface::TakePicture(ID3D11Texture2D* outputTexture)
{
    if (outputTexture == nullptr || _device == nullptr)
    {
        return;
    }

    ID3D11DeviceContext* context;
    _device->GetImmediateContext(&context);

    // Get bytes from texture because screengrab does not support texture format provided by Unity.
    DirectXHelper::GetBytesFromTexture(_device, outputTexture, FRAME_BPP, photoBytes);
    ID3D11Texture2D* tex = DirectXHelper::CreateTexture(_device, photoBytes, FRAME_WIDTH, FRAME_HEIGHT, FRAME_BPP, DXGI_FORMAT_B8G8R8A8_UNORM);

    photoIndex++;
    std::wstring photoPath = DirectoryHelper::FindUniqueFileName(outputPath, L"Photo", L".png", photoIndex);

    //TODO: Save each channel?
    DirectX::SaveWICTextureToFile(context, tex, GUID_ContainerFormatPng, photoPath.c_str());
}

bool CompositorInterface::InitializeVideoEncoder(ID3D11Device* device)
{
    videoEncoder = new VideoEncoder(FRAME_WIDTH, FRAME_HEIGHT, FRAME_WIDTH * FRAME_BPP, VIDEO_FPS,
        AUDIO_BUFSIZE, AUDIO_SAMPLE_RATE, AUDIO_CHANNELS, AUDIO_BPS);

    return videoEncoder->Initialize(device);
}

void CompositorInterface::StartRecording()
{
    if (videoEncoder == nullptr)
    {
        return;
    }

    videoIndex++;
    std::wstring videoPath = DirectoryHelper::FindUniqueFileName(outputPath, L"Video", L".mp4", videoIndex);
    videoEncoder->StartRecording(videoPath.c_str(), ENCODE_AUDIO);
}

void CompositorInterface::StopRecording()
{
    if (videoEncoder == nullptr)
    {
        return;
    }

    videoEncoder->StopRecording();
}

bool CompositorInterface::IsVideoFrameReady()
{
    if (frameProvider == nullptr)
    {
        return false;
    }

    // Allow for stub video frames when frame provider is not enabled.
    // A frame provider may not be enabled if it is reporting that it is not enabled or 
    // if its reported frame time is invalid.
    LONGLONG frameTime = frameProvider->GetTimestamp();
    if (!frameProvider->IsEnabled() ||
        frameTime == 0 || frameTime == INVALID_TIMESTAMP)
    {
        LARGE_INTEGER time;
        LARGE_INTEGER freq;
        QueryPerformanceCounter(&freq);
        QueryPerformanceCounter(&time);

        // If elapsed time exceeds frame duration, allow for a video frame.
        LONGLONG elapsedTime = ((time.QuadPart - stubVideoTime) * S2HNS) / freq.QuadPart;
        if (elapsedTime >= (LONGLONG)((1.0f / 30.0f) * S2HNS))
        {
            stubVideoTime = time.QuadPart;
            return true;
        }
    }

    return frameProvider->IsVideoFrameReady();
}

void CompositorInterface::RecordFrameAsync(ID3D11Texture2D* texture)
{
    if (frameProvider == nullptr || videoEncoder == nullptr || _device == nullptr)
    {
        return;
    }

    DirectXHelper::GetBytesFromTexture(_device, texture, 1.5f, videoBytes);
    LONGLONG frameTime = GetTimestamp();
    if (frameTime == INVALID_TIMESTAMP ||
        frameTime == 0)
    {
        // To record frames before a camera source is present, we must have a valid timestamp.
        LARGE_INTEGER time;
        QueryPerformanceCounter(&time);
        frameTime = time.QuadPart;
    }

    videoEncoder->QueueVideoFrame(videoBytes, frameTime, frameProvider->GetDurationHNS());
}

void CompositorInterface::RecordAudioFrameAsync(BYTE* audioFrame, LONGLONG frameTime)
{
    if (videoEncoder == nullptr)
    {
        return;
    }

    videoEncoder->QueueAudioFrame(audioFrame, frameTime);
}
#pragma endregion Recording