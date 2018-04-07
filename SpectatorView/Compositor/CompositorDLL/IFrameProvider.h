// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#include "stdafx.h"

class IFrameProvider
{
public:
    virtual HRESULT Initialize(ID3D11ShaderResourceView* srv) = 0;
    
    virtual LONGLONG GetTimestamp(int frame) = 0;
    virtual LONGLONG GetDurationHNS() = 0;

    virtual bool IsEnabled() = 0;

    virtual void Update(int compositeFrameIndex) = 0;

    virtual void Dispose() = 0;

    virtual bool OutputYUV() = 0;

    virtual void SetOutputTexture(ID3D11Texture2D* outputTexture) { }

    virtual int GetCaptureFrameIndex() { return 0; }
};