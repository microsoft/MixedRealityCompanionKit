// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
#pragma once

class BufferedTextureFetch
{
public:

    BufferedTextureFetch()
    {
        textures[0] = textures[1] = nullptr;
        Reset();
    }

    ~BufferedTextureFetch()
    {
        ReleaseTextures();
    }

    void Reset()
    {
        textureIndex = 0;
        dataAvailable = false;
    }

    void PrepareTextureFetch(ID3D11Device* device, ID3D11Texture2D* texture)
    {
        if(!CreateTextureBuffers(device, texture))
            return;

        ID3D11DeviceContext* d3d11DevCon;
        device->GetImmediateContext(&d3d11DevCon);

        textureIndex = 1 - textureIndex;
        d3d11DevCon->CopyResource(textures[textureIndex], texture);
        dataAvailable = true;
    }

    void FetchTextureData(ID3D11Device* device, BYTE* const & bytes, float bpp)
    {
        if (!dataAvailable)
            return;

        ID3D11DeviceContext* d3d11DevCon;
        device->GetImmediateContext(&d3d11DevCon);

        D3D11_MAPPED_SUBRESOURCE  mapResource;
        d3d11DevCon->Map(textures[textureIndex], 0, D3D11_MAP_READ, NULL, &mapResource);

        memcpy(bytes, mapResource.pData, (size_t)(dataSize * bpp));

        d3d11DevCon->Unmap(textures[textureIndex], 0);
        dataAvailable = false;
    }

    bool IsDataAvailable()
    {
        return dataAvailable;
    }

    void ReleaseTextures()
    {
        for (int i = 0; i < 2; i++)
        {
            if (textures[i] != nullptr)
            {
                textures[i]->Release();
                textures[i] = nullptr;
            }
        }
    }

private:

    ID3D11Texture2D* textures[2];
    int textureIndex;
    size_t dataSize;
    bool dataAvailable;

    bool CreateTextureBuffers(ID3D11Device* device, ID3D11Texture2D* texture)
    {
        for (int i = 0; i < 2; i++)
        {
            if (textures[i] == nullptr)
            {
                D3D11_TEXTURE2D_DESC existingDesc;
                texture->GetDesc(&existingDesc);

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

                device->CreateTexture2D(&textureDesc, NULL, &textures[i]);

                dataSize = (size_t)(textureDesc.Width * textureDesc.Height);
            }
        }
        return (textures[0] != nullptr && textures[1] != nullptr);
    }
};

