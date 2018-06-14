// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

// If not otherwise indicated, YUV corresponds to UYVY (http://www.fourcc.org/yuv.php#UYVY)

#pragma once

#include <d3d11_1.h>
#include "CompositorShared.h"
#include <amp.h>

class DirectXHelper
{
public:
    // Texture resources.
    // Create a shader resource view to render the input texture.
    static ID3D11ShaderResourceView* CreateShaderResourceView(ID3D11Device* device, ID3D11Texture2D* texture, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM)
    {
        ID3D11ShaderResourceView* srv;
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        D3D11_TEXTURE2D_DESC texDesc;

        texture->GetDesc(&texDesc);
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
        srvDesc.Format = format;

        device->CreateShaderResourceView(texture, &srvDesc, &srv);

        return srv;
    }

    static void UpdateSRV(ID3D11Device* device, ID3D11ShaderResourceView* srv, const byte* bytes, int stride)
    {
        ID3D11Texture2D* tex = NULL;
        srv->GetResource((ID3D11Resource**)(&tex));

        if (tex == NULL)
        {
            return;
        }

        ID3D11DeviceContext *ctx = NULL;
        device->GetImmediateContext(&ctx);

        if (ctx == NULL)
        {
            return;
        }

        ctx->UpdateSubresource(tex, 0, NULL, bytes, stride, 0);
        ctx->Release();
    }

    // Create a texture with the given bytes.
    static ID3D11Texture2D* CreateTexture(ID3D11Device* device, const byte* bytes, int width, int height, int bpp, DXGI_FORMAT textureFormat = DXGI_FORMAT_R8G8B8A8_UNORM)
    {
        if (device == nullptr)
        {
            return nullptr;
        }

        ID3D11Texture2D* tex;

        D3D11_TEXTURE2D_DESC tdesc;
        D3D11_SUBRESOURCE_DATA tbsd;

        // setting up D3D11_SUBRESOURCE_DATA 
        tbsd.pSysMem = (void *)bytes;
        tbsd.SysMemPitch = width * bpp;

        // setting up D3D11_TEXTURE2D_DESC
        tdesc.Width = width;
        tdesc.Height = height;
        tdesc.MipLevels = 1;
        tdesc.ArraySize = 1;
        tdesc.SampleDesc.Count = 1;
        tdesc.SampleDesc.Quality = 0;
        tdesc.Usage = D3D11_USAGE_DEFAULT;
        tdesc.Format = textureFormat;
        tdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        tdesc.CPUAccessFlags = 0;
        tdesc.MiscFlags = 0;

        device->CreateTexture2D(&tdesc, &tbsd, &tex);

        return tex;
    }

    static ID3D11Buffer* CreateBuffer(ID3D11Device* device, const byte* bytes, int width, int height, int bpp)
    {
        ID3D11Buffer* buf;

        D3D11_BUFFER_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.ByteWidth = width * height * bpp;
        desc.StructureByteStride = bpp;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
        desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

        D3D11_SUBRESOURCE_DATA initData;
        initData.pSysMem = bytes;
        initData.SysMemPitch = 0;
        initData.SysMemSlicePitch = 0;

        device->CreateBuffer(&desc, &initData, &buf);

        return buf;
    }

    static void FillBuffer(ID3D11Buffer*& buffer, ID3D11Device* device, const byte* bytes, int width, int height, int bpp)
    {
        if (buffer == nullptr)
        {
            return;
        }

        ID3D11DeviceContext* d3d11DevCon;
        device->GetImmediateContext(&d3d11DevCon);

        d3d11DevCon->UpdateSubresource(buffer, 0, NULL, bytes, width * bpp, 0);
    }

    // Create a texture with the given bytes.
    static ID3D11Texture2D* CreateTexture(ID3D11Device* device, D3D11_TEXTURE2D_DESC textureDescription, const byte* bytes, int width, int height, int bpp, DXGI_FORMAT textureFormat = DXGI_FORMAT_R8G8B8A8_UNORM)
    {
        ID3D11Texture2D* tex;
        D3D11_SUBRESOURCE_DATA tbsd;

        // setting up D3D11_SUBRESOURCE_DATA 
        tbsd.pSysMem = (void *)bytes;
        tbsd.SysMemPitch = width * bpp;

        // setting up D3D11_TEXTURE2D_DESC

        device->CreateTexture2D(&textureDescription, &tbsd, &tex);

        return tex;
    }

    static void CopyTexture(ID3D11Device* device, ID3D11Texture2D* pDst, ID3D11Texture2D* pSrc)
    {
        ID3D11DeviceContext* d3d11DevCon;
        device->GetImmediateContext(&d3d11DevCon);

        d3d11DevCon->CopyResource(pDst, pSrc);
    }

    static void GetBytesFromBuffer(ID3D11Device* device, ID3D11Buffer* buffer, float bpp, BYTE* const & bytes)
    {
        D3D11_BUFFER_DESC existingDesc;
        buffer->GetDesc(&existingDesc);

        ID3D11Buffer* tmpBuf;
        D3D11_BUFFER_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.ByteWidth = existingDesc.ByteWidth;
        desc.StructureByteStride = existingDesc.StructureByteStride;
        desc.Usage = D3D11_USAGE_STAGING;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        desc.MiscFlags = 0;
        desc.BindFlags = 0;

        device->CreateBuffer(&desc, NULL, &tmpBuf);

        ID3D11DeviceContext* d3d11DevCon;
        device->GetImmediateContext(&d3d11DevCon);

        d3d11DevCon->CopyResource(tmpBuf, buffer);
        D3D11_MAPPED_SUBRESOURCE  mapResource;
        d3d11DevCon->Map(tmpBuf, 0, D3D11_MAP_READ, NULL, &mapResource);

        memcpy(bytes, mapResource.pData, (size_t)(FRAME_WIDTH * FRAME_HEIGHT * bpp));

        d3d11DevCon->Unmap(tmpBuf, 0);
        if (tmpBuf != nullptr)
        {
            tmpBuf->Release();
            tmpBuf = nullptr;
        }
    }

    static void GetBytesFromTexture(ID3D11Device* device, ID3D11Texture2D* texture, float bpp, BYTE* const & bytes)
    {
        D3D11_TEXTURE2D_DESC existingDesc;
        texture->GetDesc(&existingDesc);

        ID3D11Texture2D* textureBuf;
        D3D11_TEXTURE2D_DESC textureDesc;
        ZeroMemory(&textureDesc, sizeof(textureDesc));
        textureDesc.Width = existingDesc.Width;
        textureDesc.Height = existingDesc.Height;
        textureDesc.MipLevels = existingDesc.MipLevels;
        textureDesc.ArraySize = existingDesc.ArraySize;
        textureDesc.Format = existingDesc.Format;
        textureDesc.SampleDesc.Count = existingDesc.SampleDesc.Count;
        textureDesc.SampleDesc.Quality = existingDesc.SampleDesc.Quality;
        textureDesc.Usage = D3D11_USAGE_STAGING;
        textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        textureDesc.MiscFlags = 0;

        device->CreateTexture2D(&textureDesc, NULL, &textureBuf);

        ID3D11DeviceContext* d3d11DevCon;
        device->GetImmediateContext(&d3d11DevCon);

        d3d11DevCon->CopyResource(textureBuf, texture);
        D3D11_MAPPED_SUBRESOURCE  mapResource;
        d3d11DevCon->Map(textureBuf, 0, D3D11_MAP_READ, NULL, &mapResource);

        memcpy(bytes, mapResource.pData, (size_t)(textureDesc.Width * textureDesc.Height * bpp));

        d3d11DevCon->Unmap(textureBuf, 0);
        if (textureBuf != nullptr)
        {
            textureBuf->Release();
            textureBuf = nullptr;
        }
    }

    static void FlipHorizontally(BYTE*& bytes, int height, int stride, bool rgba = false)
    {
        BYTE* swap = new BYTE[stride];
        for (int i = 0, j = height - 1; i < height / 2.0f; i++, j--)
        {
            int topRow = stride * i;
            int bottomRow = stride * j;

            CopyMemory(swap, bytes + topRow, stride);
            CopyMemory(bytes + topRow, bytes + bottomRow, stride);
            CopyMemory(bytes + bottomRow, swap, stride);
        }

        delete[] swap;
    }


    // Conversions.
    // Convert a YUV input buffer to a BGRA output buffer.
    static void ConvertYUVtoBGRA(BYTE* input, BYTE* alphaInput, BYTE*& output, int width, int height, bool rgba = false)
    {
        ConvertYUVtoBGRA_CPU(input, alphaInput, output, width, height, rgba);
    }

    static void ConvertYUVtoBGRA(BYTE* input, BYTE*& output, int width, int height, bool rgba = false)
    {
        ConvertYUVtoBGRA_CPU(input, nullptr, output, width, height, rgba);
    }

    // Convert a BGRA input buffer to a YUV output buffer.
    static void ConvertBGRAtoYUV(BYTE* input, BYTE*& output, BYTE*& alphaOut, int width, int height)
    {
        ConvertBGRAtoYUV_CPU(input, output, alphaOut, width, height);
    }

    static void ConvertRGBAtoYUV(BYTE* input, BYTE*& output, int width, int height)
    {
        ConvertRGBAtoYUV_CPU(input, output, width, height);
    }

    static void ConvertBGRAtoYUV(BYTE* input, BYTE*& output, int width, int height)
    {
        ConvertBGRAtoYUV_CPU(input, output, width, height);
    }

    static void ConvertRGBAtoNV12(BYTE* input, BYTE*& outputYUV, int width, int height)
    {
        for (int i = 0, j = 0, k = 0; i < width * height * FRAME_BPP - 4 * FRAME_BPP; i += 4 * FRAME_BPP, j += FRAME_BPP_RAW * 2)
        {
            int row = (int)(i / (width * FRAME_BPP));

            int b1, g1, r1, a1;
            int b2, g2, r2, a2;
            int b3, g3, r3, a3;
            int b4, g4, r4, a4;

            r1 = (int)input[i];
            g1 = (int)input[i + 1];
            b1 = (int)input[i + 2];
            a1 = (int)input[i + 3];

            r2 = (int)input[i + 4];
            g2 = (int)input[i + 5];
            b2 = (int)input[i + 6];
            a2 = (int)input[i + 7];

            r3 = (int)input[i + 8];
            g3 = (int)input[i + 9];
            b3 = (int)input[i + 10];
            a3 = (int)input[i + 11];

            r4 = (int)input[i + 12];
            g4 = (int)input[i + 13];
            b4 = (int)input[i + 14];
            a4 = (int)input[i + 15];

            int u, y0, v, y1;
            int u2, y02, v2, y12;

            GetYUV(r1, g1, b1, r2, g2, b2, u, y0, v, y1);
            GetYUV(r3, g3, b3, r4, g4, b4, u2, y02, v2, y12);

            outputYUV[j] = (byte)Clamp(y0);
            outputYUV[j + 1] = (byte)Clamp(y1);
            outputYUV[j + 2] = (byte)Clamp(y02);
            outputYUV[j + 3] = (byte)Clamp(y12);

            if (row % 2 == 0)
            {
                outputYUV[width * height + k] = (byte)Clamp(u);
                outputYUV[width * height + k + 1] = (byte)Clamp(v);
                outputYUV[width * height + k + 2] = (byte)Clamp(u2);
                outputYUV[width * height + k + 3] = (byte)Clamp(v2);

                k += 4;
            }
        }
    }

    // Swap B and R components and force alpha to 255.
    static void ConvertBGRAtoRGBA(BYTE*& bytes, int width, int height, bool forceOpaque = true)
    {
        for (int i = 0; i < width * height * FRAME_BPP - FRAME_BPP; i += 4)
        {
            byte swap = bytes[i];
            bytes[i] = bytes[i + 2];
            bytes[i + 2] = swap;

            if (forceOpaque)
            {
                bytes[i + 3] = 255;
            }
        }
    }

    static void ConvertRGBtoBGRA(BYTE* input, BYTE*& output, int width, int height, bool rgba)
    {
        for (int i = 0, j = 0; i <= width * height * 3 - 3 * 4; i += 3 * 4, j += 4 * 4)
        {
            byte r, g, b;
            byte r2, g2, b2;
            byte r3, g3, b3;
            byte r4, g4, b4;

            r = input[i];
            g = input[i + 1];
            b = input[i + 2];

            r2 = input[i + 3];
            g2 = input[i + 4];
            b2 = input[i + 5];

            r3 = input[i + 6];
            g3 = input[i + 7];
            b3 = input[i + 8];

            r4 = input[i + 9];
            g4 = input[i + 10];
            b4 = input[i + 11];


            output[j] = b;
            output[j + 1] = g;
            output[j + 2] = r;
            output[j + 3] = 255;

            output[j + 4] = b2;
            output[j + 5] = g2;
            output[j + 6] = r2;
            output[j + 7] = 255;

            output[j + 8] = b3;
            output[j + 9] = g3;
            output[j + 10] = r3;
            output[j + 11] = 255;

            output[j + 12] = b4;
            output[j + 13] = g4;
            output[j + 14] = r4;
            output[j + 15] = 255;

            if (rgba)
            {
                output[j] = r;
                output[j + 2] = b;

                output[j + 4] = r2;
                output[j + 6] = b2;

                output[j + 8] = r3;
                output[j + 10] = b3;

                output[j + 12] = r4;
                output[j + 14] = b4;
            }
        }
    }

    static void AlphaBlend(/*[in out]*/ BYTE*& back, const BYTE* front, int bufferSize, float alpha)
    {
        for (int i = 0; i < bufferSize - FRAME_BPP; i += FRAME_BPP)
        {
            byte br, bg, bb, ba;
            byte fr, fg, fb, fa;

            br = back[i];
            bg = back[i + 1];
            bb = back[i + 2];
            ba = back[i + 3];

            fr = front[i];
            fg = front[i + 1];
            fb = front[i + 2];
            fa = front[i + 3];


            float frontAlpha = Saturate(fa);

            br = (byte)DirectXHelper::Clamp((int)
                (((1 - alpha * frontAlpha) * (float)br) +
                (alpha * (float)fr)));
            bg = (byte)DirectXHelper::Clamp((int)
                (((1 - alpha * frontAlpha) * (float)bg) +
                (alpha * (float)fg)));
            bb = (byte)DirectXHelper::Clamp((int)
                (((1 - alpha * frontAlpha) * (float)bb) +
                (alpha * (float)fb)));

            back[i] = br;
            back[i + 1] = bg;
            back[i + 2] = bb;
            back[i + 3] = 255;
        }
    }

    static void AlphaAsRGBA(BYTE* input, BYTE*& output, int width, int height)
    {
        for (int i = 0; i < width * height * FRAME_BPP - 4; i += 4)
        {
            byte a = input[i + 3];

            output[i] = a;
            output[i + 1] = a;
            output[i + 2] = a;
            output[i + 3] = a;
        }
    }

    // Byte value sanitation.
    // Flatten overflow or underflow values to a valid byte.
    static unsigned int Clamp(int input)
    {
        if (input > 255)
        {
            input = 255;
        }
        else if (input < 0)
        {
            input = 0;
        }

        return input;
    }

    static float Saturate(int input)
    {
        return ((float)input / 255.0f);
    }

    static void GetYUV(
        int r, int g, int b,
        int r2, int g2, int b2,
        int& u, int& y, int& v, int& y2)
    {
        // Conversion requires > 8 bit precision.
        // https://msdn.microsoft.com/en-us/library/ms893078.aspx
        y = (int)((float)((66 * r + 129 * g + 25 * b + 128) >> 8) + 16);
        y2 = (int)((float)((66 * r2 + 129 * g2 + 25 * b2 + 128) >> 8) + 16);
        u = (int)((float)((-38 * r - 74 * g + 112 * b + 128) >> 8) + 128);
        v = (int)((float)((112 * r - 94 * g - 18 * b + 128) >> 8) + 128);
    }

    static void GetRGB(
        int y0, int y1, int u, int v,
        int& r, int& g, int& b,
        int& r2, int& g2, int& b2)
    {
        // Conversion requires > 8 bit precision.
        // https://msdn.microsoft.com/en-us/library/ms893078.aspx
        int c0 = (int)(y0 - 16);
        int c1 = (int)(y1 - 16);
        int d = (int)(u - 128);
        int e = (int)(v - 128);

        b = (298 * c0 + 409 * e + 128) >> 8;
        g = (298 * c0 - 100 * d - 208 * e + 128) >> 8;
        r = (298 * c0 + 516 * d + 128) >> 8;

        b2 = (298 * c1 + 409 * e + 128) >> 8;
        g2 = (298 * c1 - 100 * d - 208 * e + 128) >> 8;
        r2 = (298 * c1 + 516 * d + 128) >> 8;
    }

private:
    static void ConvertYUVtoBGRA_CPU(BYTE* input, BYTE* alphaInput, BYTE*& output, int width, int height, bool rgba = false)
    {
        for (int i = 0, j = 0, a = 0; i < width * height * FRAME_BPP_RAW - (FRAME_BPP_RAW * 4); i += FRAME_BPP_RAW * 4, j += FRAME_BPP * 4, a += 4)
        {
            int u, y0, v, y1;
            int u2, y02, v2, y12;

            u = (int)input[i];
            y0 = (int)input[i + 1];
            v = (int)input[i + 2];
            y1 = (int)input[i + 3];

            u2 = (int)input[i + 4];
            y02 = (int)input[i + 5];
            v2 = (int)input[i + 6];
            y12 = (int)input[i + 7];

            int b1, g1, r1, a1;
            int b2, g2, r2, a2;
            int b3, g3, r3, a3;
            int b4, g4, r4, a4;

            GetRGB(y0, y1, u, v, r1, g1, b1, r2, g2, b2);
            GetRGB(y02, y12, u2, v2, r3, g3, b3, r4, g4, b4);

            a1 = 255;
            a2 = 255;
            a3 = 255;
            a4 = 255;

            if (alphaInput != nullptr)
            {
                a1 = (int)alphaInput[a];
                a2 = (int)alphaInput[a + 1];
                a3 = (int)alphaInput[a + 2];
                a4 = (int)alphaInput[a + 3];
            }

            if (rgba)
            {
                int swap = r1;
                r1 = b1;
                b1 = swap;

                swap = r2;
                r2 = b2;
                b2 = swap;

                swap = r3;
                r3 = b3;
                b3 = swap;

                swap = r4;
                r4 = b4;
                b4 = swap;
            }

            output[j] = (byte)Clamp(r1);
            output[j + 1] = (byte)Clamp(g1);
            output[j + 2] = (byte)Clamp(b1);
            output[j + 3] = (byte)Clamp(a1);

            output[j + 4] = (byte)Clamp(r2);
            output[j + 5] = (byte)Clamp(g2);
            output[j + 6] = (byte)Clamp(b2);
            output[j + 7] = (byte)Clamp(a2);

            output[j + 8] = (byte)Clamp(r3);
            output[j + 9] = (byte)Clamp(g3);
            output[j + 10] = (byte)Clamp(b3);
            output[j + 11] = (byte)Clamp(a3);

            output[j + 12] = (byte)Clamp(r4);
            output[j + 13] = (byte)Clamp(g4);
            output[j + 14] = (byte)Clamp(b4);
            output[j + 15] = (byte)Clamp(a4);
        }
    }

    static void ConvertBGRAtoYUV_CPU(BYTE* input, BYTE*& output, BYTE*& alphaOut, int width, int height)
    {
        for (int i = 0, j = 0, a = 0; i < width * height * FRAME_BPP - 4 * FRAME_BPP; i += 4 * FRAME_BPP, j += 4 * FRAME_BPP_RAW, a += FRAME_BPP)
        {
            int b1, g1, r1, a1;
            int b2, g2, r2, a2;
            int b3, g3, r3, a3;
            int b4, g4, r4, a4;

            b1 = (int)input[i];
            g1 = (int)input[i + 1];
            r1 = (int)input[i + 2];
            a1 = (int)input[i + 3];

            b2 = (int)input[i + 4];
            g2 = (int)input[i + 5];
            r2 = (int)input[i + 6];
            a2 = (int)input[i + 7];

            b3 = (int)input[i + 8];
            g3 = (int)input[i + 9];
            r3 = (int)input[i + 10];
            a3 = (int)input[i + 11];

            b4 = (int)input[i + 12];
            g4 = (int)input[i + 13];
            r4 = (int)input[i + 14];
            a4 = (int)input[i + 15];

            int u, y0, v, y1;
            int u2, y02, v2, y12;

            GetYUV(r1, g1, b1, r2, g2, b2, u, y0, v, y1);
            GetYUV(r3, g3, b3, r4, g4, b4, u2, y02, v2, y12);

            output[j] = (byte)Clamp(u);
            output[j + 1] = (byte)Clamp(y0);
            output[j + 2] = (byte)Clamp(v);
            output[j + 3] = (byte)Clamp(y1);

            output[j + 4] = (byte)Clamp(u2);
            output[j + 5] = (byte)Clamp(y02);
            output[j + 6] = (byte)Clamp(v2);
            output[j + 7] = (byte)Clamp(y12);

            alphaOut[a] = a1;
            alphaOut[a + 1] = a2;
            alphaOut[a + 2] = a3;
            alphaOut[a + 3] = a4;
        }
    }

    static void ConvertRGBAtoYUV_CPU(BYTE* input, BYTE*& output, int width, int height)
    {
        for (int i = 0, j = 0; i < width * height * FRAME_BPP - 4 * FRAME_BPP; i += 4 * FRAME_BPP, j += 4 * FRAME_BPP_RAW)
        {
            int b1, g1, r1, a1;
            int b2, g2, r2, a2;
            int b3, g3, r3, a3;
            int b4, g4, r4, a4;

            r1 = (int)input[i];
            g1 = (int)input[i + 1];
            b1 = (int)input[i + 2];
            a1 = (int)input[i + 3];

            r2 = (int)input[i + 4];
            g2 = (int)input[i + 5];
            b2 = (int)input[i + 6];
            a2 = (int)input[i + 7];

            r3 = (int)input[i + 8];
            g3 = (int)input[i + 9];
            b3 = (int)input[i + 10];
            a3 = (int)input[i + 11];

            r4 = (int)input[i + 12];
            g4 = (int)input[i + 13];
            b4 = (int)input[i + 14];
            a4 = (int)input[i + 15];

            int u, y0, v, y1;
            int u2, y02, v2, y12;

            GetYUV(r1, g1, b1, r2, g2, b2, u, y0, v, y1);
            GetYUV(r3, g3, b3, r4, g4, b4, u2, y02, v2, y12);

            output[j] = (byte)Clamp(u);
            output[j + 1] = (byte)Clamp(y0);
            output[j + 2] = (byte)Clamp(v);
            output[j + 3] = (byte)Clamp(y1);

            output[j + 4] = (byte)Clamp(u2);
            output[j + 5] = (byte)Clamp(y02);
            output[j + 6] = (byte)Clamp(v2);
            output[j + 7] = (byte)Clamp(y12);
        }
    }

    static void ConvertBGRAtoYUV_CPU(BYTE* input, BYTE*& output, int width, int height)
    {
        for (int i = 0, j = 0; i < width * height * FRAME_BPP - 4 * FRAME_BPP; i += 4 * FRAME_BPP, j += 4 * FRAME_BPP_RAW)
        {
            int b1, g1, r1, a1;
            int b2, g2, r2, a2;
            int b3, g3, r3, a3;
            int b4, g4, r4, a4;

            b1 = (int)input[i];
            g1 = (int)input[i + 1];
            r1 = (int)input[i + 2];
            a1 = (int)input[i + 3];

            b2 = (int)input[i + 4];
            g2 = (int)input[i + 5];
            r2 = (int)input[i + 6];
            a2 = (int)input[i + 7];

            b3 = (int)input[i + 8];
            g3 = (int)input[i + 9];
            r3 = (int)input[i + 10];
            a3 = (int)input[i + 11];

            b4 = (int)input[i + 12];
            g4 = (int)input[i + 13];
            r4 = (int)input[i + 14];
            a4 = (int)input[i + 15];

            int u, y0, v, y1;
            int u2, y02, v2, y12;

            GetYUV(r1, g1, b1, r2, g2, b2, u, y0, v, y1);
            GetYUV(r3, g3, b3, r4, g4, b4, u2, y02, v2, y12);

            output[j] = (byte)Clamp(u);
            output[j + 1] = (byte)Clamp(y0);
            output[j + 2] = (byte)Clamp(v);
            output[j + 3] = (byte)Clamp(y1);

            output[j + 4] = (byte)Clamp(u2);
            output[j + 5] = (byte)Clamp(y02);
            output[j + 6] = (byte)Clamp(v2);
            output[j + 7] = (byte)Clamp(y12);
        }
    }
};

