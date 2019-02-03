// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

// Filters media type's attribute before they can be sent over the network.
HRESULT FilterOutputMediaType(_In_ IMFMediaType* pSourceMediaType, _In_ IMFMediaType* pDestinationMediaType);

// Used to validate media type after receiving it from the network.
HRESULT ValidateInputMediaType(_In_ REFGUID guidMajorType, _In_ REFGUID guidSubtype, _In_ IMFMediaType* pMediaType);

HRESULT GetVideoResolution(
	 _In_ IMediaEncodingProfile* mediaEncodingProfile,
	 _Out_ UINT32* width,
	 _Out_ UINT32* height);

HRESULT CreateMediaPlaybackItem(
	 _In_ ABI::Windows::Media::Core::IMediaSource2* pMediaSource,
	 _COM_Outptr_ ABI::Windows::Media::Playback::IMediaPlaybackItem** ppMediaPlaybackItem);

HRESULT GetSurfaceFromTexture(
	 _In_ ID3D11Texture2D* pTexture,
	 _COM_Outptr_ ABI::Windows::Graphics::DirectX::Direct3D11::IDirect3DSurface** ppSurface);

HRESULT CreateMediaDevice(
	 _In_ IDXGIAdapter* pDXGIAdapter,
	 _COM_Outptr_ ID3D11Device** ppDevice);