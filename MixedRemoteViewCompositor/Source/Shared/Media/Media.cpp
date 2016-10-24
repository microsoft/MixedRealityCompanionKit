// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "Media.h"

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
