// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "MediaUtils.h"
#include <windows.graphics.directx.direct3d11.interop.h> // inline helpers

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Graphics::DirectX::Direct3D11;
using namespace winrt::Windows::Media::Playback;

HRESULT SharedTextureBuffer::Create(
    _In_ ID3D11Device* d3dDevice,
    _In_ IMFDXGIDeviceManager* dxgiDeviceManager,
    _In_ uint32_t width,
    _In_ uint32_t height,
    _In_ std::weak_ptr<SharedTextureBuffer> outputBuffer)
{
    NULL_CHK_HR(d3dDevice, E_INVALIDARG);
    NULL_CHK_HR(dxgiDeviceManager, E_INVALIDARG);
    if (width < 1 && height < 1)
    {
        IFR(E_INVALIDARG);
    }

    auto buffer = outputBuffer.lock();
    NULL_CHK_HR(buffer, E_INVALIDARG);

    HANDLE deviceHandle;
    IFR(dxgiDeviceManager->OpenDeviceHandle(&deviceHandle));

    com_ptr<ID3D11Device1> mediaDevice = nullptr;
    IFR(dxgiDeviceManager->LockDevice(deviceHandle, __uuidof(ID3D11Device1), mediaDevice.put_void(), TRUE));

    // since the device is locked, unlock before we exit function
    HRESULT hr = S_OK;

    auto textureDesc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_B8G8R8A8_UNORM, width, height);
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    textureDesc.MipLevels = 1;
    textureDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED | D3D11_RESOURCE_MISC_SHARED_NTHANDLE;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;

    // create a texture
    com_ptr<ID3D11Texture2D> spTexture = nullptr;
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    com_ptr<ID3D11ShaderResourceView> spSRV = nullptr;
    com_ptr<IDXGIResource1> spDXGIResource = nullptr;
    com_ptr<ID3D11Texture2D> spMediaTexture = nullptr;
    HANDLE sharedHandle = INVALID_HANDLE_VALUE;

    IDirect3DSurface mediaSurface;
    com_ptr<IMFMediaBuffer> dxgiMediaBuffer = nullptr;
    com_ptr<IMFSample> mediaSample = nullptr;

    IFG(d3dDevice->CreateTexture2D(&textureDesc, nullptr, spTexture.put()), done);

    // srv for the texture
    srvDesc = CD3D11_SHADER_RESOURCE_VIEW_DESC(spTexture.get(), D3D11_SRV_DIMENSION_TEXTURE2D);
    IFG(d3dDevice->CreateShaderResourceView(spTexture.get(), &srvDesc, spSRV.put()), done);

    IFG(spTexture->QueryInterface(__uuidof(IDXGIResource1), spDXGIResource.put_void()), done);

    // create shared texture 
    IFG(spDXGIResource->CreateSharedHandle(
        nullptr,
        DXGI_SHARED_RESOURCE_READ | DXGI_SHARED_RESOURCE_WRITE,
        nullptr,
        &sharedHandle), done);

    IFG(mediaDevice->OpenSharedResource1(sharedHandle, __uuidof(ID3D11Texture2D), spMediaTexture.put_void()), done);

    IFG(GetSurfaceFromTexture(spMediaTexture.get(), mediaSurface), done);

    // create a media buffer for the texture
    IFG(MFCreateDXGISurfaceBuffer(__uuidof(ID3D11Texture2D), spMediaTexture.get(), 0, /*fBottomUpWhenLinear*/false, dxgiMediaBuffer.put()), done);

    // create a sample with the buffer
    IFG(MFCreateSample(mediaSample.put()), done);

    IFG(mediaSample->AddBuffer(dxgiMediaBuffer.get()), done);

    buffer->frameTextureDesc = textureDesc;
    buffer->frameTexture.attach(spTexture.detach());
    buffer->frameTextureSRV.attach(spSRV.detach());
    buffer->sharedTextureHandle = sharedHandle;
    buffer->mediaTexture.attach(spMediaTexture.detach());
    buffer->mediaSurface = mediaSurface;
    buffer->mediaBuffer.attach(dxgiMediaBuffer.detach());
    buffer->mediaSample.attach(mediaSample.detach());

done:
    if (FAILED(hr))
    {
        if (sharedHandle != INVALID_HANDLE_VALUE)
        {
            CloseHandle(sharedHandle);
        }
    }

    dxgiDeviceManager->UnlockDevice(deviceHandle, FALSE);

    return hr;
}

SharedTextureBuffer::SharedTextureBuffer()
    : frameTextureDesc{}
    , frameTexture(nullptr)
    , frameTextureSRV(nullptr)
    , sharedTextureHandle(INVALID_HANDLE_VALUE)
    , mediaTexture(nullptr)
    , mediaSurface(nullptr)
    , mediaBuffer(nullptr)
    , mediaSample(nullptr)
{}

SharedTextureBuffer::~SharedTextureBuffer()
{
    Reset();
}

void SharedTextureBuffer::Reset()
{
    // primary texture
    if (sharedTextureHandle != INVALID_HANDLE_VALUE)
    {
        if (CloseHandle(sharedTextureHandle))
        {
            sharedTextureHandle = INVALID_HANDLE_VALUE;
        }
    }

    mediaSample = nullptr;
    mediaBuffer = nullptr;
    mediaSurface = nullptr;
    mediaTexture = nullptr;
    frameTextureSRV = nullptr;
    frameTexture = nullptr;

    ZeroMemory(&frameTextureDesc, sizeof(CD3D11_TEXTURE2D_DESC));
}

// Function validating just a type of the data.
template <VARTYPE vt>
bool ValidateDataType(_In_ PROPVARIANT* pValue)
{
    return (pValue->vt == vt);
}

// Function validating a BLOB apart from the type it checks the size as well.
bool ValidateBlob(_In_ PROPVARIANT* pValue)
{
    return ValidateDataType<MF_ATTRIBUTE_BLOB>(pValue) && pValue->caub.cElems < 128;
}

// Description of an attribute for validation
struct AttributeValidationDescriptor
{
    REFGUID guidKey;
    bool(*IsValid)(_In_ PROPVARIANT* pValue);
};

// Description of an media type for validation
struct MediaTypeValidationDescriptor
{
    GUID guidSubtype;
    bool fVideo;
    size_t cAttributes;
    const AttributeValidationDescriptor *attributes;
};

// Valid attibutes for H.264 decoder.
const AttributeValidationDescriptor H264ValidAttributes[] = {
    { MF_MT_FRAME_SIZE, ValidateDataType<MF_ATTRIBUTE_UINT64> },
    { MF_MT_AVG_BITRATE, ValidateDataType<MF_ATTRIBUTE_UINT32> },
    { MF_MT_MPEG_SEQUENCE_HEADER, ValidateBlob },
    { MF_MT_MPEG2_PROFILE, ValidateDataType<MF_ATTRIBUTE_UINT32> },
    { MF_MT_FRAME_RATE, ValidateDataType<MF_ATTRIBUTE_UINT64> },
    { MF_MT_PIXEL_ASPECT_RATIO, ValidateDataType<MF_ATTRIBUTE_UINT64> },
    { MF_MT_INTERLACE_MODE, ValidateDataType<MF_ATTRIBUTE_UINT32> },
};

// Valid attributes for AAC decoder
const AttributeValidationDescriptor AACValidAttributes[] = {
    { MF_MT_AUDIO_AVG_BYTES_PER_SECOND, ValidateDataType<MF_ATTRIBUTE_UINT32> },
    { MF_MT_AVG_BITRATE, ValidateDataType<MF_ATTRIBUTE_UINT32> },
    { MF_MT_AUDIO_BLOCK_ALIGNMENT, ValidateDataType<MF_ATTRIBUTE_UINT32> },
    { MF_MT_AUDIO_NUM_CHANNELS, ValidateDataType<MF_ATTRIBUTE_UINT32> },
    { MF_MT_COMPRESSED, ValidateDataType<MF_ATTRIBUTE_UINT32> },
    { MF_MT_AUDIO_SAMPLES_PER_SECOND, ValidateDataType<MF_ATTRIBUTE_UINT32> },
    { MF_MT_AAC_AUDIO_PROFILE_LEVEL_INDICATION, ValidateDataType<MF_ATTRIBUTE_UINT32> },
    { MF_MT_AUDIO_PREFER_WAVEFORMATEX, ValidateDataType<MF_ATTRIBUTE_UINT32> },
    { MF_MT_USER_DATA, ValidateDataType<MF_ATTRIBUTE_BLOB> },
    { MF_MT_FIXED_SIZE_SAMPLES, ValidateDataType<MF_ATTRIBUTE_UINT32> },
    { MF_MT_AAC_PAYLOAD_TYPE, ValidateDataType<MF_ATTRIBUTE_UINT32> },
    { MF_MT_AUDIO_BITS_PER_SAMPLE, ValidateDataType<MF_ATTRIBUTE_UINT32> },
};

// List of valid media types.
const MediaTypeValidationDescriptor ValidMediaTypes[] = {
    { MFVideoFormat_H264, true, _countof(H264ValidAttributes), H264ValidAttributes },
    { MFAudioFormat_AAC, false, _countof(AACValidAttributes), AACValidAttributes }
};

// Finds media type desciptor based on major type and subtype.
HRESULT FindMediaTypeDescriptor(
    _In_ REFGUID guidMajorType,
    _In_ REFGUID guidSubtype,
    _Out_ MediaTypeValidationDescriptor* pValidationDescriptor)
{
    NULL_CHK(pValidationDescriptor);

    for (UINT32 nIndex = 0; nIndex < _countof(ValidMediaTypes); ++nIndex)
    {
        if (ValidMediaTypes[nIndex].guidSubtype == guidSubtype)
        {
            if (guidMajorType == MFMediaType_Video && ValidMediaTypes[nIndex].fVideo
                ||
                guidMajorType == MFMediaType_Audio && !ValidMediaTypes[nIndex].fVideo)
            {
               * pValidationDescriptor = ValidMediaTypes[nIndex];
                return S_OK;
            }
        }
    }

    return MF_E_INVALIDMEDIATYPE;
}

// Checks if attribute is valid for a given media type. If it returns FALSE attrubute is optional and shouldn't be used during communication
// between sink and source, if it returns error it means that attribute is correct but its value is somehow corrupted.
HRESULT IsAttributeValid(
    _In_ MediaTypeValidationDescriptor validationDescriptor, 
    _In_ REFGUID guidKey, 
    _In_ PROPVARIANT* pValue)
{
    HRESULT hr = E_INVALIDARG;

    for (UINT32 nIndex = 0; nIndex < validationDescriptor.cAttributes; ++nIndex)
    {
        if (validationDescriptor.attributes[nIndex].guidKey == guidKey)
        {
            if (validationDescriptor.attributes[nIndex].IsValid(pValue))
            {
                hr = S_OK;
            }
            else
            {
                hr = MF_E_INVALIDMEDIATYPE;
            }
            break;
        }
    }

    return hr;
}

_Use_decl_annotations_
HRESULT FilterOutputMediaType(
    IMFMediaType* pSourceMediaType, 
    IMFMediaType* pDestinationMediaType)
{
    GUID guidMajorType;
    GUID guidSubtype;

    IFT(pSourceMediaType->GetMajorType(&guidMajorType));

    IFT(pSourceMediaType->GetGUID(MF_MT_SUBTYPE, &guidSubtype));

    MediaTypeValidationDescriptor typeDescriptor;
    IFT(FindMediaTypeDescriptor(guidMajorType, guidSubtype, &typeDescriptor));

    UINT32 cAttributes = 0;
    IFT(pSourceMediaType->GetCount(&cAttributes));

    // Remove optional and not necessary attributes.
    for (UINT32 nIndex = 0; nIndex < cAttributes; ++nIndex)
    {
        GUID guidKey;
        PROPVARIANT val;
        PropVariantInit(&val);
        HRESULT hr = pSourceMediaType->GetItemByIndex(nIndex, &guidKey, &val);
        if (SUCCEEDED(hr))
        {
            hr = IsAttributeValid(typeDescriptor, guidKey, &val);
            if (SUCCEEDED(hr))
            {
                // Copy only attributes that can be sent otherwise skip it.
                hr = pDestinationMediaType->SetItem(guidKey, val);
            }
        }
        PropVariantClear(&val);
        if (MF_E_INVALIDMEDIATYPE == hr)
        {
            IFT(hr);
        }
    }

    return S_OK;
}

_Use_decl_annotations_
HRESULT ValidateInputMediaType(
    REFGUID guidMajorType, 
    REFGUID guidSubtype, 
    IMFMediaType* pMediaType)
{
    NULL_CHK(pMediaType);

    MediaTypeValidationDescriptor typeDescriptor;
    IFT(FindMediaTypeDescriptor(guidMajorType, guidSubtype, &typeDescriptor));

    UINT32 cAttributes = 0;
    IFT(pMediaType->GetCount(&cAttributes));

    HRESULT hr = S_OK;
    GUID guidKey;
    for (UINT32 nIndex = 0; nIndex < cAttributes; ++nIndex)
    {
        PROPVARIANT val;
        PropVariantInit(&val);
        hr = pMediaType->GetItemByIndex(nIndex, &guidKey, &val);
        if(SUCCEEDED(hr))
        {
            hr = IsAttributeValid(typeDescriptor, guidKey, &val);
            if(FAILED(hr))
            {
                hr = MF_E_INVALIDMEDIATYPE;
            }
        }

        PropVariantClear(&val);
        if (FAILED(hr))
        {
            IFT(hr);
        }
    }

    return S_OK;
}

#if defined(_DEBUG)
// Check for SDK Layer support.
inline bool SdkLayersAvailable()
{
	 HRESULT hr = D3D11CreateDevice(
		  nullptr,
		  D3D_DRIVER_TYPE_NULL,       // There is no need to create a real hardware device.
		  0,
		  D3D11_CREATE_DEVICE_DEBUG,  // Check for the SDK layers.
		  nullptr,                    // Any feature level will do.
		  0,
		  D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Windows Store apps.
		  nullptr,                    // No need to keep the D3D device reference.
		  nullptr,                    // No need to know the feature level.
		  nullptr                     // No need to keep the D3D device context reference.
	 );

	 return SUCCEEDED(hr);
}
#endif

_Use_decl_annotations_
HRESULT GetSurfaceFromTexture(
    ID3D11Texture2D* pTexture,
    IDirect3DSurface& direct3dSurface)
{
    com_ptr<IDXGISurface> dxgiSurface = nullptr;
    IFR(pTexture->QueryInterface(__uuidof(IDXGISurface), dxgiSurface.put_void()));

    com_ptr<::IInspectable> spInspectable = nullptr;
    IFR(CreateDirect3D11SurfaceFromDXGISurface(dxgiSurface.get(), spInspectable.put()));

    IFR(spInspectable->QueryInterface(
        guid_of<IDirect3DSurface>(),
        reinterpret_cast<void**>(put_abi(direct3dSurface))));

    return S_OK;
}

/*
_Use_decl_annotations_
HRESULT CreateMediaDevice(
	 IDXGIAdapter* pDXGIAdapter,
	 ID3D11Device** ppDevice)
{
	 NULL_CHK(ppDevice);

	 // Create the Direct3D 11 API device object and a corresponding context.
	 D3D_FEATURE_LEVEL featureLevel;

	 // This flag adds support for surfaces with a different color channel ordering
	 // than the API default. It is required for compatibility with Direct2D.
	 UINT creationFlags = D3D11_CREATE_DEVICE_VIDEO_SUPPORT | D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
	 if (SdkLayersAvailable())
	 {
		  // If the project is in a debug build, enable debugging via SDK Layers with this flag.
		  creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
	 }
#endif
	 D3D_FEATURE_LEVEL featureLevels[] =
	 {
		 D3D_FEATURE_LEVEL_11_1,
		 D3D_FEATURE_LEVEL_11_0,
		 D3D_FEATURE_LEVEL_10_1,
		 D3D_FEATURE_LEVEL_10_0,
	 };

	 com_ptr<ID3D11Device> spDevice;
	 com_ptr<ID3D11DeviceContext> spContext;

	 D3D_DRIVER_TYPE driverType = (nullptr != pDXGIAdapter) ? D3D_DRIVER_TYPE_UNKNOWN : D3D_DRIVER_TYPE_HARDWARE;

	 // Create a device using the hardware graphics driver if adapter is not supplied
	 HRESULT hr = D3D11CreateDevice(
		  pDXGIAdapter,               // if nullptr will use default adapter.
		  driverType,
		  0,                          // Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
		  creationFlags,              // Set debug and Direct2D compatibility flags.
		  featureLevels,              // List of feature levels this app can support.
		  ARRAYSIZE(featureLevels),   // Size of the list above.
		  D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Windows Store apps.
		  &spDevice,                  // Returns the Direct3D device created.
		  &featureLevel,              // Returns feature level of device created.
		  &spContext                  // Returns the device immediate context.
	 );

	 if (FAILED(hr))
	 {
		  // fallback to WARP if we are not specifying an adapter
		  if (nullptr == pDXGIAdapter)
		  {
				// If the initialization fails, fall back to the WARP device.
				// For more information on WARP, see: 
				// http://go.microsoft.com/fwlink/?LinkId=286690
				hr = D3D11CreateDevice(
					 nullptr,
					 D3D_DRIVER_TYPE_WARP, // Create a WARP device instead of a hardware device.
					 0,
					 creationFlags,
					 featureLevels,
					 ARRAYSIZE(featureLevels),
					 D3D11_SDK_VERSION,
					 &spDevice,
					 &featureLevel,
					 &spContext);
		  }

		  IFT(hr);
	 }
	 else
	 {
		  // workaround for nvidia GPU's, cast to ID3D11VideoDevice
		  com_ptr<ID3D11VideoDevice> videoDevice;
		  spDevice.As(&videoDevice);
	 }

	 // Turn multithreading on 
	 com_ptr<ID3D10Multithread> spMultithread;
	 if (SUCCEEDED(spContext.As(&spMultithread)))
	 {
		  spMultithread->SetMultithreadProtected(TRUE);
	 }

	 *ppDevice = spDevice.detach();

	 return S_OK;
}*/

_Use_decl_annotations_
HRESULT CreateMediaDevice(
    IDXGIAdapter* pDXGIAdapter,
    ID3D11Device** ppDevice)
{
    NULL_CHK_HR(ppDevice, E_INVALIDARG);

    // Create the Direct3D 11 API device object and a corresponding context.
    D3D_FEATURE_LEVEL featureLevel;

    // This flag adds support for surfaces with a different color channel ordering
    // than the API default. It is required for compatibility with Direct2D.
    UINT creationFlags = D3D11_CREATE_DEVICE_VIDEO_SUPPORT | D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
    // If the project is in a debug build, enable debugging via SDK Layers with this flag.
    if (SdkLayersAvailable())
    {
        creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
    }
#endif

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };

    com_ptr<ID3D11Device> spDevice;
    com_ptr<ID3D11DeviceContext> spContext;

    D3D_DRIVER_TYPE driverType = (nullptr != pDXGIAdapter) ? D3D_DRIVER_TYPE_UNKNOWN : D3D_DRIVER_TYPE_HARDWARE;

    // Create a device using the hardware graphics driver if adapter is not supplied
    HRESULT hr = D3D11CreateDevice(
        pDXGIAdapter,               // if nullptr will use default adapter.
        driverType,
        0,                          // Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
        creationFlags,              // Set debug and Direct2D compatibility flags.
        featureLevels,              // List of feature levels this app can support.
        ARRAYSIZE(featureLevels),   // Size of the list above.
        D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Windows Store apps.
        spDevice.put(),             // Returns the Direct3D device created.
        &featureLevel,              // Returns feature level of device created.
        spContext.put()             // Returns the device immediate context.
    );

    if (FAILED(hr))
    {
        // fallback to WARP if we are not specifying an adapter
        if (nullptr == pDXGIAdapter)
        {
            // If the initialization fails, fall back to the WARP device.
            // For more information on WARP, see: 
            // http://go.microsoft.com/fwlink/?LinkId=286690
            hr = D3D11CreateDevice(
                nullptr,
                D3D_DRIVER_TYPE_WARP, // Create a WARP device instead of a hardware device.
                0,
                creationFlags,
                featureLevels,
                ARRAYSIZE(featureLevels),
                D3D11_SDK_VERSION,
                spDevice.put(),
                &featureLevel,
                spContext.put());
        }

        IFR(hr);
    }
    else
    {
        // workaround for nvidia GPU's, cast to ID3D11VideoDevice
        auto videoDevice = spDevice.as<ID3D11VideoDevice>();
    }

    // Turn multithreading on 
    auto spMultithread = spContext.as<ID3D10Multithread>();
    NULL_CHK_HR(spMultithread, E_POINTER);

    spMultithread->SetMultithreadProtected(TRUE);

    spDevice.copy_to(ppDevice);
    //*ppDevice = spDevice.detach();

    return S_OK;
}

_Use_decl_annotations_
HRESULT CreateIMFMediaBuffer(
    UINT32 width,
    UINT32 height,
    UINT32 bufferSize,
    BYTE* pBuffer,
    IMFMediaBuffer** ppIMFBuffer)
{
    com_ptr<IMFMediaBuffer> spBuffer;

    const LONG cbWidth = sizeof(DWORD) * width;
    const DWORD cbBuffer = cbWidth * height;

    if (cbBuffer != bufferSize)
    {
        return E_INVALIDARG;
    }

    // Create a new memory buffer.
    BYTE *pMFdata = NULL;
    IFT(MFCreateMemoryBuffer(cbBuffer, spBuffer.put()));

    // Lock the buffer and copy the video frame to the buffer.
    IFT(spBuffer->Lock(&pMFdata, NULL, NULL));

    IFT(MFCopyImage(
        pMFdata,                    // Destination buffer.
        cbWidth,                    // Destination stride.
        pBuffer,						    // First row in source image.
        cbWidth,                    // Source stride.
        cbWidth,                    // Image width in bytes.
        height                 // Image height in pixels.
    ));

    if (spBuffer)
    {
        spBuffer->Unlock();
    }

    // Set the data length of the buffer.
    IFT(spBuffer->SetCurrentLength(cbBuffer));

    spBuffer.copy_to(ppIMFBuffer);
    //*ppIMFBuffer = spBuffer.detach();

    return S_OK;
}