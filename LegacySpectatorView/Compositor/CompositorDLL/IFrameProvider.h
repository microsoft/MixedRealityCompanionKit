// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#include "stdafx.h"

class IFrameProvider
{
public:
    // Set up the FrameProvider to start delivering frames.
    virtual HRESULT Initialize(ID3D11ShaderResourceView* colorSRV, ID3D11Texture2D* outputTexture) = 0;

    // 4 frames are caches for reliable hologram stability:
    // Get the timestamp of the currently rendered cached frame.
    virtual LONGLONG GetTimestamp() = 0;

    virtual LONGLONG GetDurationHNS() = 0;

    virtual void Update() = 0;

    // Check if the FrameProvider is currently delivering frames.
    virtual bool IsEnabled() = 0;
    // Check if this capture device supports outputting video.
    virtual bool SupportsOutput() = 0;
    // Stop capturing frames from the FrameProvider.
    virtual void Dispose() = 0;

    // Return true if function outputs YUV frames, false otherwise.
    virtual bool OutputYUV() = 0;

    virtual bool IsVideoFrameReady() = 0;
};
