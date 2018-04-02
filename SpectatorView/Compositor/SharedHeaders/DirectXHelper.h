// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

// If not otherwise indicated, YUV corresponds to UYVY (http://www.fourcc.org/yuv.php#UYVY)

#pragma once

#include <d3d11_1.h>
#include "CompositorConstants.h"
#include <ppltasks.h>

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

	static ID3D11Texture2D* CreateTexture(ID3D11Device* device, int width, int height, int bpp, DXGI_FORMAT textureFormat = DXGI_FORMAT_R8G8B8A8_UNORM)
	{
		if (device == nullptr)
		{
			return nullptr;
		}

		ID3D11Texture2D* tex;

		D3D11_TEXTURE2D_DESC tdesc;

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

		device->CreateTexture2D(&tdesc, NULL, &tex);

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

		ID3D11Texture2D* textureBuf = nullptr;
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
		if (textureBuf == nullptr)
		{
			return;
		}

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
};

