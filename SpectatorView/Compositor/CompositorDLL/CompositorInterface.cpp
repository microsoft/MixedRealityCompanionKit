// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "stdafx.h"
#include "CompositorInterface.h"


CompositorInterface::CompositorInterface()
{
    wchar_t myDocumentsPath[1024];
    SHGetFolderPathW(0, CSIDL_MYDOCUMENTS, 0, 0, myDocumentsPath);
    outputPath = std::wstring(myDocumentsPath) + L"\\HologramCapture\\";
    outputPathCanon = std::wstring(myDocumentsPath) + L"\\HologramCapture\\CanonChannels\\";
    channelPath = std::wstring(myDocumentsPath) + L"\\HologramCapture\\LoResChannels\\";

    DirectoryHelper::CreateOutputDirectory(outputPath);
    DirectoryHelper::CreateOutputDirectory(outputPathCanon);
    DirectoryHelper::CreateOutputDirectory(channelPath);

#if USE_CANON_SDK
    InitializeCriticalSection(&canonLock);
#endif

#if USE_ELGATO
    frameProvider = new ElgatoFrameProvider();
#endif
#if USE_DECKLINK
    frameProvider = new DeckLinkManager();
#endif
#if USE_OPENCV
    frameProvider = new OpenCVFrameProvider();
#endif

#if USE_CANON_SDK
    canonManager = new CanonSDKManager();
#endif
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

    hologramQueue = new HologramQueue(device);

    return SUCCEEDED(frameProvider->Initialize(colorSRV, outputTexture));
}

void CompositorInterface::UpdateFrameProvider()
{
    if (frameProvider != nullptr)
    {
        frameProvider->Update();
    }

#if USE_CANON_SDK
    //Note: this is done on the UI thread.
    if (takingCanonPicture &&
        canonPictureDownloaded && 
        canonPhotoPath != L"" && 
        cachedHiResHoloBytes != nullptr &&
        _device != nullptr)
    {
        EnterCriticalSection(&canonLock);
        photoIndex++;
        std::wstring photoPath = DirectoryHelper::FindUniqueFileName(outputPath, L"Photo", L".png", photoIndex);

        ID3D11DeviceContext* context;
        _device->GetImmediateContext(&context);

        ID3D11ShaderResourceView* srv;
        ID3D11Texture2D* canonColorTexture;
        DirectX::CreateWICTextureFromFile(_device, canonPhotoPath.c_str(), (ID3D11Resource**)&canonColorTexture, &srv);

        DirectXHelper::GetBytesFromTexture(_device, canonColorTexture, FRAME_BPP, canonColorBytes);

        DirectXHelper::AlphaBlend(canonColorBytes, cachedHiResHoloBytes, HOLOGRAM_BUFSIZE_HIRES, alpha);
        ID3D11Texture2D* tex = DirectXHelper::CreateTexture(_device, canonColorBytes, HOLOGRAM_WIDTH_HIRES, HOLOGRAM_HEIGHT_HIRES, FRAME_BPP);
        DirectX::SaveWICTextureToFile(context, tex, GUID_ContainerFormatPng, photoPath.c_str());

        canonPictureDownloaded = false;
        takingCanonPicture = false;
        LeaveCriticalSection(&canonLock);
    }
#endif
}

void CompositorInterface::Update()
{
    if (videoEncoder != nullptr)
    {
        videoEncoder->Update();
    }

#if USE_CANON_SDK
    if (canonManager != nullptr)
    {
        canonManager->Update();
    }
#endif
}

void CompositorInterface::StopFrameProvider()
{
    if (frameProvider != nullptr)
    {
        frameProvider->Dispose();
    }

#if USE_CANON_SDK
    EnterCriticalSection(&canonLock);
    canonPictureDownloaded = false;
    takingCanonPicture = false;
    LeaveCriticalSection(&canonLock);
#endif
}

LONGLONG CompositorInterface::GetColorTime()
{
    if (frameProvider != nullptr)
    {
        return frameProvider->GetTimestamp();
    }

    return INVALID_TIMESTAMP;
}

LONGLONG CompositorInterface::GetColorDuration()
{
    if (frameProvider != nullptr)
    {
        return frameProvider->GetDurationHNS();
    }

    return (LONGLONG)((1.0f / 30.0f) * QPC_MULTIPLIER);
}

void CompositorInterface::TakePicture(ID3D11Device* device, int width, int height, int bpp, 
    BYTE* bytes, BYTE* colorBytes, BYTE* holoBytes)
{
    if (device == nullptr)
    {
        return;
    }

    ID3D11DeviceContext* context;
    device->GetImmediateContext(&context);

    photoIndex++;
    std::wstring photoPath = DirectoryHelper::FindUniqueFileName(outputPath, L"Photo", L".png", photoIndex);

    std::wstring colorPath = DirectoryHelper::FindUniqueFileName(channelPath, L"color", L".png", photoIndex);
    std::wstring holoPath = DirectoryHelper::FindUniqueFileName(channelPath, L"holo", L".png", photoIndex);
    std::wstring alphaPath = DirectoryHelper::FindUniqueFileName(channelPath, L"alpha", L".png", photoIndex);

    ID3D11Texture2D* tex = DirectXHelper::CreateTexture(device, bytes, width, height, bpp);
    DirectX::SaveWICTextureToFile(context, tex, GUID_ContainerFormatPng, photoPath.c_str());

    ID3D11Texture2D* colorTex = DirectXHelper::CreateTexture(device, colorBytes, width, height, bpp);
    DirectX::SaveWICTextureToFile(context, colorTex, GUID_ContainerFormatPng, colorPath.c_str());

    ID3D11Texture2D* holoTex = DirectXHelper::CreateTexture(device, holoBytes, width, height, bpp);
    DirectX::SaveWICTextureToFile(context, holoTex, GUID_ContainerFormatPng, holoPath.c_str());

    BYTE* alphaBytes = new BYTE[FRAME_BUFSIZE];
    DirectXHelper::AlphaAsRGBA(holoBytes, alphaBytes, FRAME_WIDTH, FRAME_HEIGHT);

    ID3D11Texture2D* alphaTex = DirectXHelper::CreateTexture(device, alphaBytes, width, height, bpp);
    DirectX::SaveWICTextureToFile(context, alphaTex, GUID_ContainerFormatPng, alphaPath.c_str());

    delete[] alphaBytes;
}

void CompositorInterface::TakeCanonPicture(ID3D11Device* device, BYTE* bytes)
{
#if USE_CANON_SDK
    if (device == nullptr || takingCanonPicture)
    {
        return;
    }

    ID3D11DeviceContext* context;
    device->GetImmediateContext(&context);

    EnterCriticalSection(&canonLock);
    takingCanonPicture = true;

    canonPhotoIndex++;
    canonPhotoPath = DirectoryHelper::FindUniqueFileName(outputPathCanon, L"Color", L".jpg", canonPhotoIndex);

    memcpy(cachedHiResHoloBytes, bytes, HOLOGRAM_BUFSIZE_HIRES);

    concurrency::create_task([=]
    {
        canonManager->TakePictureAsync(canonPhotoPath);

        // Wait for picture to finish.
        while (canonManager->CurrentlyDownloadingPicture(canonPhotoPath))
        {
            Sleep(10);
        }

        canonPictureDownloaded = true;
    });

    LeaveCriticalSection(&canonLock);

    std::wstring holoPath = DirectoryHelper::FindUniqueFileName(outputPathCanon, L"holo", L".png", canonPhotoIndex);
    std::wstring alphaPath = DirectoryHelper::FindUniqueFileName(outputPathCanon, L"alpha", L".png", canonPhotoIndex);

    ID3D11Texture2D* holoTex = DirectXHelper::CreateTexture(device, bytes, HOLOGRAM_WIDTH_HIRES, HOLOGRAM_HEIGHT_HIRES, FRAME_BPP);
    DirectX::SaveWICTextureToFile(context, holoTex, GUID_ContainerFormatPng, holoPath.c_str());

    BYTE* alphaBytes = new BYTE[HOLOGRAM_BUFSIZE_HIRES];
    DirectXHelper::AlphaAsRGBA(bytes, alphaBytes, HOLOGRAM_WIDTH_HIRES, HOLOGRAM_HEIGHT_HIRES);
    ID3D11Texture2D* alphaTex = DirectXHelper::CreateTexture(device, alphaBytes, HOLOGRAM_WIDTH_HIRES, HOLOGRAM_HEIGHT_HIRES, FRAME_BPP);
    DirectX::SaveWICTextureToFile(context, alphaTex, GUID_ContainerFormatPng, alphaPath.c_str());
    delete[] alphaBytes;
#endif
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

    return frameProvider->IsVideoFrameReady();
}

void CompositorInterface::RecordFrameAsync(BYTE* videoFrame, LONGLONG frameTime)
{
    if (frameProvider == nullptr || videoEncoder == nullptr)
    {
        return;
    }

    videoEncoder->QueueVideoFrame(videoFrame, frameTime, frameProvider->GetDurationHNS());
}

void CompositorInterface::RecordAudioFrameAsync(BYTE* audioFrame, LONGLONG frameTime)
{
    if (videoEncoder == nullptr)
    {
        return;
    }

    videoEncoder->QueueAudioFrame(audioFrame, frameTime);
}

bool CompositorInterface::OutputYUV()
{
    if (frameProvider == nullptr)
    {
        return false;
    }

    return frameProvider->OutputYUV();
}

FrameMessage* CompositorInterface::GetNextHologramFrame(LONGLONG timeStamp)
{
    if (hologramQueue == nullptr)
    {
        return nullptr;
    }

    return hologramQueue->GetNextFrame(timeStamp);
}

FrameMessage* CompositorInterface::FindClosestHologramFrame(LONGLONG timeStamp, LONGLONG frameOffset)
{
    if (hologramQueue == nullptr)
    {
        return nullptr;
    }

    return hologramQueue->FindClosestFrame(timeStamp, frameOffset);
}

