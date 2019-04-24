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

    frameProvider = NULL;
}

void CompositorInterface::SetFrameProvider(IFrameProvider::ProviderType type)
{
    if (frameProvider)
    {
        if (frameProvider->GetProviderType() == type)
            return;
        frameProvider->Dispose();
        delete frameProvider;
        frameProvider = NULL;
    }

    if(type == IFrameProvider::ProviderType::Elgato)
        frameProvider = new ElgatoFrameProvider();
    if (type == IFrameProvider::ProviderType::BlackMagic)
        frameProvider = new DeckLinkManager();
#if USE_OPENCV
    if (type == IFrameProvider::ProviderType::OpenCV)
        frameProvider = new OpenCVFrameProvider();
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

    return SUCCEEDED(frameProvider->Initialize(colorSRV, outputTexture));
}

void CompositorInterface::UpdateFrameProvider()
{
    if (frameProvider != nullptr)
    {
        frameProvider->Update(compositeFrameIndex);
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

LONGLONG CompositorInterface::GetTimestamp(int frame)
{
    if (frameProvider != nullptr)
    {
        return frameProvider->GetTimestamp(frame);
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

int CompositorInterface::GetCaptureFrameIndex()
{
    if (frameProvider != nullptr)
    {
        return frameProvider->GetCaptureFrameIndex();
    }

    return 0;
}

int CompositorInterface::GetPixelChange(int frame)
{
    if (frameProvider != nullptr)
    {
        return frameProvider->GetPixelChange(frame);
    }

    return 0;
}

int CompositorInterface::GetNumQueuedOutputFrames()
{
    if (frameProvider != nullptr)
    {
        return frameProvider->GetNumQueuedOutputFrames();
    }

    return 0;
}



void CompositorInterface::SetCompositeFrameIndex(int index)
{
    compositeFrameIndex = index;
}

void CompositorInterface::TakePicture(ID3D11Device* device, int width, int height, int bpp, BYTE* bytes)
{
    if (device == nullptr)
    {
        return;
    }

    ID3D11DeviceContext* context;
    device->GetImmediateContext(&context);

    photoIndex++;
    std::wstring photoPath = DirectoryHelper::FindUniqueFileName(outputPath, L"Photo", L".png", photoIndex);

    ID3D11Texture2D* tex = DirectXHelper::CreateTexture(device, bytes, width, height, bpp);
    DirectX::SaveWICTextureToFile(context, tex, GUID_ContainerFormatPng, photoPath.c_str());
}

bool CompositorInterface::InitializeVideoEncoder(ID3D11Device* device)
{
    videoEncoder = new VideoEncoder(FRAME_WIDTH, FRAME_HEIGHT, FRAME_WIDTH * FRAME_BPP, VIDEO_FPS,
        AUDIO_SAMPLE_RATE, AUDIO_CHANNELS, AUDIO_BPS);

    return videoEncoder->Initialize(device);
}

void CompositorInterface::StartRecording()
{
    if (videoEncoder == nullptr)
    {
        return;
    }

    videoIndex++;
    videoRecordingStartTime = compositeFrameIndex * GetColorDuration();
    audioRecordingStartTime = -1.0;

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

void CompositorInterface::RecordFrameAsync(BYTE* videoFrame, LONGLONG frameTime, int numFrames)
{
    if (frameProvider == nullptr || videoEncoder == nullptr)
    {
        return;
    }
    if (numFrames < 1) 
        numFrames = 1;
    else if (numFrames > 5) 
        numFrames = 5;

    videoEncoder->QueueVideoFrame(videoFrame, frameTime, numFrames * frameProvider->GetDurationHNS());
}

void CompositorInterface::RecordAudioFrameAsync(BYTE* audioFrame, int audioSize, double audioTime)
{
    if (videoEncoder == nullptr)
    {
        return;
    }

    if (audioRecordingStartTime < 0)
        audioRecordingStartTime = audioTime;

    LONGLONG frameTime = videoRecordingStartTime + (LONGLONG)((audioTime - audioRecordingStartTime) * QPC_MULTIPLIER);

    videoEncoder->QueueAudioFrame(audioFrame, audioSize, frameTime);
}

bool CompositorInterface::OutputYUV()
{
    if (frameProvider == nullptr)
    {
        return false;
    }

    return frameProvider->OutputYUV();
}
