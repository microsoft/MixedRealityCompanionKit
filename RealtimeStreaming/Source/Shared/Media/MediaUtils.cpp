// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "MediaUtils.h"
#include <windows.graphics.directx.direct3d11.interop.h> // inline helpers

using namespace ABI::Windows::Media::Core;
using namespace ABI::Windows::Media::Playback;
using namespace ABI::Windows::Graphics::DirectX::Direct3D11;

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

    IFR(pSourceMediaType->GetMajorType(&guidMajorType));

    IFR(pSourceMediaType->GetGUID(MF_MT_SUBTYPE, &guidSubtype));

    MediaTypeValidationDescriptor typeDescriptor;
    IFR(FindMediaTypeDescriptor(guidMajorType, guidSubtype, &typeDescriptor));

    UINT32 cAttributes = 0;
    IFR(pSourceMediaType->GetCount(&cAttributes));

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
            IFR(hr);
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
    IFR(FindMediaTypeDescriptor(guidMajorType, guidSubtype, &typeDescriptor));

    UINT32 cAttributes = 0;
    IFR(pMediaType->GetCount(&cAttributes));

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
            IFR(hr);
        }
    }

    return S_OK;
}

_Use_decl_annotations_
HRESULT GetVideoResolution(
	 IMediaEncodingProfile* mediaEncodingProfile,
	 UINT32* width,
	 UINT32* height)
{
	 NULL_CHK(mediaEncodingProfile);
	 NULL_CHK(width);
	 NULL_CHK(height);

	 ComPtr<IVideoEncodingProperties> spVideoEncodingProperties;
	 IFR(mediaEncodingProfile->get_Video(&spVideoEncodingProperties));

	 IFR(spVideoEncodingProperties->get_Height(height));
	 IFR(spVideoEncodingProperties->get_Width(width));

	 return S_OK;
}

_Use_decl_annotations_
HRESULT CreateMediaPlaybackItem(
	 IMediaSource2* pMediaSource,
	 IMediaPlaybackItem** ppMediaPlaybackItem)
{
	 // create playbackitem from source
	 ComPtr<IMediaPlaybackItemFactory> spItemFactory;
	 IFR(ABI::Windows::Foundation::GetActivationFactory(
		  Wrappers::HStringReference(RuntimeClass_Windows_Media_Playback_MediaPlaybackItem).Get(),
		  &spItemFactory));

	 ComPtr<IMediaPlaybackItem> spItem;
	 IFR(spItemFactory->Create(pMediaSource, &spItem));

	 *ppMediaPlaybackItem = spItem.Detach();

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
	 IDirect3DSurface** ppSurface)
{
	 NULL_CHK(pTexture);
	 NULL_CHK(ppSurface);

	 *ppSurface = nullptr;

	 ComPtr<ID3D11Texture2D> spTexture(pTexture);

	 ComPtr<IDXGISurface> dxgiSurface;
	 IFR(spTexture.As(&dxgiSurface));

	 ComPtr<IInspectable> inspectableSurface;
	 IFR(CreateDirect3D11SurfaceFromDXGISurface(dxgiSurface.Get(), &inspectableSurface));

	 ComPtr<IDirect3DSurface> spSurface;
	 IFR(inspectableSurface.As(&spSurface));

	 *ppSurface = spSurface.Detach();

	 return S_OK;
}

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

	 ComPtr<ID3D11Device> spDevice;
	 ComPtr<ID3D11DeviceContext> spContext;

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

		  IFR(hr);
	 }
	 else
	 {
		  // workaround for nvidia GPU's, cast to ID3D11VideoDevice
		  Microsoft::WRL::ComPtr<ID3D11VideoDevice> videoDevice;
		  spDevice.As(&videoDevice);
	 }

	 // Turn multithreading on 
	 ComPtr<ID3D10Multithread> spMultithread;
	 if (SUCCEEDED(spContext.As(&spMultithread)))
	 {
		  spMultithread->SetMultithreadProtected(TRUE);
	 }

	 *ppDevice = spDevice.Detach();

	 return S_OK;
}