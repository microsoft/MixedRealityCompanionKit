// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "stdafx.h"
#include "DeckLinkManager.h"

#if USE_DECKLINK

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
                    deckLinkDevice->StartCapture(bmdModeHD1080i5994);
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
        return deckLinkDevice->GetTimeStamp();
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

