// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#include <d3d11_1.h>

#include <mfapi.h>
#include <mfidl.h>
#include <mferror.h>
#pragma comment(lib, "mfuuid")

#include <winrt/windows.devices.enumeration.h>
#include <winrt/windows.graphics.directx.direct3d11.h>
#include <winrt/windows.media.devices.h>

struct SharedTextureBuffer
{
    static HRESULT Create(
        _In_ ID3D11Device* d3dDevice,
        _In_ IMFDXGIDeviceManager* dxgiDeviceManager,
        _In_ uint32_t width,
        _In_ uint32_t height,
        _In_ std::weak_ptr<SharedTextureBuffer> outputBuffer);

    SharedTextureBuffer();

    virtual ~SharedTextureBuffer();

    void Reset();

    CD3D11_TEXTURE2D_DESC frameTextureDesc;
    winrt::com_ptr<ID3D11Texture2D> frameTexture;
    winrt::com_ptr<ID3D11ShaderResourceView> frameTextureSRV;
    HANDLE sharedTextureHandle;
    winrt::com_ptr<ID3D11Texture2D> mediaTexture;
    winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DSurface mediaSurface;
    winrt::com_ptr<IMFMediaBuffer> mediaBuffer;
    winrt::com_ptr<IMFSample> mediaSample;
};

// Filters media type's attribute before they can be sent over the network.
HRESULT FilterOutputMediaType(_In_ IMFMediaType* pSourceMediaType,
    _In_ IMFMediaType* pDestinationMediaType);

// Used to validate media type after receiving it from the network.
HRESULT ValidateInputMediaType(_In_ REFGUID guidMajorType, _In_ REFGUID guidSubtype, _In_ IMFMediaType* pMediaType);

HRESULT GetSurfaceFromTexture(
    _In_ ID3D11Texture2D* pTexture,
    _Out_ winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DSurface& ppSurface);

HRESULT CreateMediaDevice(
    _In_opt_ IDXGIAdapter* pDXGIAdapter,
    _COM_Outptr_ ID3D11Device** ppDevice);

HRESULT CreateIMFMediaBuffer(
    _In_ UINT32 width,
    _In_ UINT32 height,
    _In_ UINT32 bufferSize,
    __in_ecount(bufferSize) BYTE* pBuffer,
    _COM_Outptr_ IMFMediaBuffer** ppIMFBuffer);