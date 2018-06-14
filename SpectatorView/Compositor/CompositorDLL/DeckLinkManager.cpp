// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "stdafx.h"
#include "DeckLinkManager.h"

#if USE_DECKLINK || USE_DECKLINK_SHUTTLE

DeckLinkManager::DeckLinkManager()
{
    deckLinkDiscovery = new DeckLinkDeviceDiscovery();
    if (!deckLinkDiscovery->Enable())
    {
        supportsBlackMagic = false;
        OutputDebugString(L"Please install the Blackmagic Desktop Video drivers.\n");
    }
}


DeckLinkManager::~DeckLinkManager()
{
}

HRESULT DeckLinkManager::Initialize(ID3D11ShaderResourceView* srv)
{
    if (deckLinkDiscovery == nullptr)
    {
        deckLinkDiscovery = new DeckLinkDeviceDiscovery();
        if (!deckLinkDiscovery->Enable())
        {
            supportsBlackMagic = false;
            OutputDebugString(L"Please install the Blackmagic Desktop Video drivers.\n");
        }
    }

    if (supportsBlackMagic && (deckLinkDevice == nullptr || deckLink == nullptr))
    {
        deckLink = deckLinkDiscovery->GetDeckLink();
        if (deckLink != nullptr)
        {
            deckLinkDevice = new DeckLinkDevice(deckLink);
            if (deckLinkDevice != nullptr)
            {
                deckLinkDevice->Init(srv);

                if (!deckLinkDevice->IsCapturing())
                {
                    // Note: this will use the resolution that the camera is outputting.  Ensure you set the camera to the settings you want.
                    // The capture card will detect when camera display settings are changed, and update accordingly.

                    //TODO: The DeckLink device must have a valid starting format to autodetect the actual format.
                    //      However, if you select a valid format that is less than your output format, your frames will be downsized.
                    //      Update the videoDisplayMode if your camera's output does not meet this selection criteria.
                    BMDDisplayMode videoDisplayMode;
                    if (FRAME_HEIGHT < 1080)
                    {
                        videoDisplayMode = bmdModeHD720p5994;
                    }
                    else if (FRAME_HEIGHT >= 1080 && FRAME_HEIGHT < 2160)
                    {
                        videoDisplayMode = bmdModeHD1080p5994;
#if USE_DECKLINK_SHUTTLE
                        videoDisplayMode = bmdModeHD1080p2398;
#endif
                    }
                    else if (FRAME_HEIGHT == 2160)
                    {
                        videoDisplayMode = bmdMode4K2160p5994;
#if USE_DECKLINK_SHUTTLE
                        videoDisplayMode = bmdMode4K2160p2398;
#endif
                    }
                    else if (FRAME_HEIGHT > 2160)
                    {
                        videoDisplayMode = bmdMode4kDCI2398;
                    }

                    deckLinkDevice->StartCapture(videoDisplayMode);
                    return S_OK;
                }
            }
        }
    }

    return E_PENDING;
}

LONGLONG DeckLinkManager::GetTimestamp(int frame)
{
    if (IsEnabled())
    {
        return deckLinkDevice->GetTimestamp(frame);
    }

    return 0;
}

LONGLONG DeckLinkManager::GetDurationHNS()
{
    if (IsEnabled())
    {
        return deckLinkDevice->GetDurationHNS();
    }

    return (LONGLONG)((1.0f / 30.0f) * S2HNS);
}

bool DeckLinkManager::IsEnabled()
{
    if (deckLinkDevice == nullptr)
    {
        return false;
    }

    return deckLinkDevice->IsCapturing();
}

void DeckLinkManager::Update(int compositeFrameIndex)
{
    if (deckLinkDevice != nullptr)
    {
        deckLinkDevice->Update(compositeFrameIndex);
    }
}

void DeckLinkManager::Dispose()
{
    if (deckLink != nullptr && deckLinkDevice != nullptr)
    {
        deckLinkDevice->StopCapture();
    }

    SafeRelease(deckLink);
    SafeRelease(deckLinkDevice);
    SafeRelease(deckLinkDiscovery);
}

bool DeckLinkManager::OutputYUV()
{
    if (!IsEnabled())
    {
        return false;
    }

    return deckLinkDevice->OutputYUV();
}

void DeckLinkManager::SetOutputTexture(ID3D11Texture2D* outputTexture)
{
    deckLinkDevice->SetOutputTexture(outputTexture);
}

int DeckLinkManager::GetCaptureFrameIndex()
{
    if (IsEnabled())
    {
        return deckLinkDevice->GetCaptureFrameIndex();
    }

    return 0;
}

#endif

