// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#if USE_DECKLINK

#include "IFrameProvider.h"
#include "DeckLinkDevice.h"

class DeckLinkManager : public IFrameProvider
{
public:
    DeckLinkManager(bool useCPU = false, bool passthroughOutput = false);
    ~DeckLinkManager();

    HRESULT Initialize(ID3D11ShaderResourceView* colorSRV, ID3D11Texture2D* outputTexture);

    LONGLONG GetTimestamp();
    LONGLONG GetDurationHNS();

    void Update();

    bool IsEnabled();
    bool SupportsOutput();

    void Dispose();

    bool OutputYUV();

    bool IsVideoFrameReady();

private:
    DeckLinkDeviceDiscovery* deckLinkDiscovery = nullptr;
    DeckLinkDevice* deckLinkDevice = nullptr;
    IDeckLink* deckLink = nullptr;
    bool supportsBlackMagic = true;

    bool _useCPU;
    bool _passthroughOutput;
};

#endif
