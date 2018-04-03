// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#include "stdafx.h"

class IFrameProvider
{
public:
    virtual HRESULT Initialize(ID3D11ShaderResourceView* srv) = 0;
    
    // Timestamp of latest color frame.
    virtual LONGLONG GetTimestamp() = 0;
    virtual LONGLONG GetDurationHNS() = 0;

    // Capture delay from camera to final presentation.
    //TODO: Should this be a frame delay instead of MS?  Try with a 60FPS camera.
    //TODO: You may need to calibrate this on your capture pipeline by:
    //      1. Point your camera at your output display to see recursive frames.
    //      2. Hold up a high-precision stop watch in front of your camera.
    //      3. Start the compositor.
    //      4. Take a photo or video of the capture with stopwatches visible on all recursive frames.
    //      5. Subtract the time on the stopwatch from the time on the first visible frame.
    //      6. Convert this time to milliseconds.
    virtual LONGLONG GetFrameDelayMS() = 0;

    virtual bool IsEnabled() = 0;

    virtual void Update() = 0;

    virtual void Dispose() = 0;

    virtual bool OutputYUV() = 0;

    virtual bool IsVideoFrameReady() = 0;

    virtual void SetOutputTexture(ID3D11Texture2D* outputTexture) { }
};