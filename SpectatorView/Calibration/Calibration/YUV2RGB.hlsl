// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

Texture2D<float4> Texture : register(t0);
sampler TextureSampler : register(s0);

cbuffer CONVERSION_PARAMETERS : register(b0)
{
	int width;
	int height;
}

float4 GetRGBA(float4 col1, int x)
{
	int u = (int)(col1.r * 255.0f);
	int y0 = (int)(col1.g * 255.0f);
	int v = (int)(col1.b * 255.0f);
	int y1 = (int)(col1.a * 255.0f);

	float r, g, b;

	// Conversion requires > 8 bit precision.
	// https://msdn.microsoft.com/en-us/library/ms893078.aspx
	int c0 = (int)(y0 - 16);
	int c1 = (int)(y1 - 16);
	int d = (int)(u - 128);
	int e = (int)(v - 128);

	int c = c0;

	if (x == 1)
	{
		c = c1;
	}

	b = (float)((298 * c + 409 * e + 128) >> 8);
	g = (float)((298 * c - 100 * d - 208 * e + 128) >> 8);
	r = (float)((298 * c + 516 * d + 128) >> 8);

	b = clamp(b, 0.0f, 255.0f) / 255.0f;
	g = clamp(g, 0.0f, 255.0f) / 255.0f;
	r = clamp(r, 0.0f, 255.0f) / 255.0f;

	return float4(r, g, b, 1);

}
float4 main(float4 color : COLOR0, float2 texCoord : TEXCOORD0) : SV_Target0
{
	uint2 xy = texCoord * float2(width, height);
	uint p = xy.x + (xy.y * width);
	uint pixloc = p / 2;

	float2 uv;
	uv.x = (pixloc % width) / (float)width;
	uv.y = (pixloc / width) / (float)height;

	float4 yuvPixel = Texture.Sample(TextureSampler, uv);
	return GetRGBA(yuvPixel, p % 2).bgra;
}