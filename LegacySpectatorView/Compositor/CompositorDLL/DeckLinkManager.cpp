// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "stdafx.h"
#include "DeckLinkManager.h"

#if USE_DECKLINK || USE_DECKLINK_SHUTTLE

DeckLinkManager::DeckLinkManager(bool useCPU, bool passthroughOutput)
{
    _useCPU = useCPU;
    _passthroughOutput = passthroughOutput;

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

void DeckLinkManager::Update()
{
    if (deckLinkDevice != nullptr)
    {
        deckLinkDevice->Update();
    }
}

HRESULT DeckLinkManager::Initialize(ID3D11ShaderResourceView* colorSRV, ID3D11Texture2D* outputTexture)
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
                deckLinkDevice->Init(colorSRV, outputTexture, _useCPU, _passthroughOutput);

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

bool DeckLinkManager::SupportsOutput()
{
    if (IsEnabled())
    {
        return deckLinkDevice->supportsOutput;
    }

    return false;
}

bool DeckLinkManager::OutputYUV()
{
    if (!IsEnabled())
    {
        return false;
    }

    return deckLinkDevice->OutputYUV();
}

bool DeckLinkManager::IsVideoFrameReady()
{
    if (!IsEnabled())
    {
        return false;
    }

    return deckLinkDevice->IsVideoFrameReady();
}

LONGLONG DeckLinkManager::GetTimestamp()
{
    if (IsEnabled())
    {
        return deckLinkDevice->GetTimestamp();
    }

    return 0;
}

LONGLONG DeckLinkManager::GetDurationHNS()
{
    if (IsEnabled())
    {
        return deckLinkDevice->GetDurationHNS();
    }

    return (LONGLONG)((1.0f / 30.0f) * QPC_MULTIPLIER);
}

bool DeckLinkManager::IsEnabled()
{
    if (deckLinkDevice == nullptr)
    {
        return false;
    }
    
    return deckLinkDevice->IsCapturing();
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

#endif

