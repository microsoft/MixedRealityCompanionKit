//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

// A constant buffer that stores the model transform.
cbuffer ModelConstantBuffer : register(b0)
{
    float4x4 model;
    float4x4 normal;
};

// A constant buffer that stores each set of view and projection matrices in column-major format.
cbuffer ViewProjectionConstantBuffer : register(b1)
{
    float4x4 viewProjection[2];
};

cbuffer ColorFilterConstantBuffer : register(b2)
{
    float4 colorFilter;
};

// Per-pixel color data passed through the pixel shader.
struct PixelShaderInput
{
    min16float4 pos         : SV_POSITION;
    min16float3 color       : COLOR0;
    uint        idx         : TEXCOORD0;
};

// The pixel shader applies Blinn-Phong BRDF shading.
min16float4 main(PixelShaderInput input) : SV_TARGET
{
    return min16float4(input.color, 1.f) * min16float4(colorFilter);
}
