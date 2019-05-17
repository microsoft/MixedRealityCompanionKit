// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "MrcVideoEffectDefinition.h"

MrcVideoEffectDefinitionImpl::MrcVideoEffectDefinitionImpl()
    : _streamType(ABI::Windows::Media::Capture::MediaStreamType_VideoRecord)
    , _hologramCompositionEnabled(true)
    , _videoStabilizationEnabled(false)
    , _videoStabilizationBufferLength(15)
    , _maxSupportedVsBuffer(30)
    , _globalOpacityCoefficient(0.9f)
    , _recordingIndicatorEnabled(true)
{
}

MrcVideoEffectDefinitionImpl::~MrcVideoEffectDefinitionImpl()
{
}

HRESULT MrcVideoEffectDefinitionImpl::RuntimeClassInitialize()
{
    // Initialize Activatable Class ID.
    IFR(_activatableClassId.Set(L"Windows.Media.MixedRealityCapture.MixedRealityCaptureVideoEffect"));

    // Create Property Value Factory
    IFR(Windows::Foundation::GetActivationFactory(
        Wrappers::HStringReference(RuntimeClass_Windows_Foundation_PropertyValue).Get(), 
        _spFactory.GetAddressOf()));

    // Initialize the property set with default settings
    IFR(Windows::Foundation::ActivateInstance(
        Wrappers::HStringReference(RuntimeClass_Windows_Foundation_Collections_PropertySet).Get(),
        &_spPropertySet));

    // Set Default values
    IFR(put_StreamType(_streamType));
    IFR(put_HologramComposition(_hologramCompositionEnabled));
    IFR(put_RecordingIndicatorEnabled(_recordingIndicatorEnabled));
    IFR(put_VideoStabilization(_videoStabilizationEnabled));
    IFR(put_VideoStabilizationBufferLength(_videoStabilizationBufferLength));
    IFR(put_GlobalOpacityCoefficient(_globalOpacityCoefficient));

    return S_OK;
}

_Use_decl_annotations_
HRESULT MrcVideoEffectDefinitionImpl::get_ActivatableClassId(
    HSTRING* value)
{
    NULL_CHK(value);

    auto lock = _lock.Lock();

    NULL_CHK_HR(_activatableClassId, E_NOT_SET);

    return _activatableClassId.CopyTo(value);
}

_Use_decl_annotations_
HRESULT MrcVideoEffectDefinitionImpl::get_Properties(
    ABI::Windows::Foundation::Collections::IPropertySet** value)
{
    NULL_CHK(value);

    auto lock = _lock.Lock();

    NULL_CHK_HR(_spPropertySet, E_NOT_SET);

    return _spPropertySet.CopyTo(value);
}

_Use_decl_annotations_
HRESULT MrcVideoEffectDefinitionImpl::get_StreamType(
    ABI::Windows::Media::Capture::MediaStreamType* value)
{
    NULL_CHK(value);

    auto lock = _lock.Lock();

    *value = _streamType;

    return S_OK;
}
_Use_decl_annotations_
HRESULT MrcVideoEffectDefinitionImpl::put_StreamType(
    ABI::Windows::Media::Capture::MediaStreamType value)
{
    IFR((
        value == ABI::Windows::Media::Capture::MediaStreamType_Photo
        ||
        value == ABI::Windows::Media::Capture::MediaStreamType_VideoPreview
        ||
        value == ABI::Windows::Media::Capture::MediaStreamType_VideoRecord) ? S_OK : E_INVALIDARG);

    auto lock = _lock.Lock();

    ComPtr<ABI::Windows::Foundation::Collections::IMap<HSTRING, IInspectable *>> spMap;
    IFR(_spPropertySet.As(&spMap));

    ComPtr<ABI::Windows::Foundation::IPropertyValue> spPropVal;
    IFR(_spFactory->CreateUInt32(value, &spPropVal));

    boolean fReplaced = false;
    IFR(spMap->Insert(Wrappers::HStringReference(L"StreamType").Get(), spPropVal.Get(), &fReplaced));

    _streamType = value;

    return S_OK;
}

_Use_decl_annotations_
HRESULT MrcVideoEffectDefinitionImpl::get_HologramComposition(
    boolean* pValue)
{
    NULL_CHK(pValue);

    auto lock = _lock.Lock();

   * pValue = _hologramCompositionEnabled;

    return S_OK;
}
_Use_decl_annotations_
HRESULT MrcVideoEffectDefinitionImpl::put_HologramComposition(
    boolean value)
{
    auto lock = _lock.Lock();

    ComPtr<ABI::Windows::Foundation::Collections::IMap<HSTRING, IInspectable *>> spMap;
    IFR(_spPropertySet.As(&spMap));

    ComPtr<ABI::Windows::Foundation::IPropertyValue> spPropVal;
    IFR(_spFactory->CreateBoolean(value, &spPropVal));

    boolean fReplaced = false;
    IFR(spMap->Insert(Wrappers::HStringReference(L"HologramCompositionEnabled").Get(), spPropVal.Get(), &fReplaced));

    _hologramCompositionEnabled = value;

    return S_OK;
}

_Use_decl_annotations_
HRESULT MrcVideoEffectDefinitionImpl::get_VideoStabilization(
    boolean* pValue)
{
    NULL_CHK(pValue);

    auto lock = _lock.Lock();

   * pValue = _videoStabilizationEnabled;

    return S_OK;
}
_Use_decl_annotations_
HRESULT MrcVideoEffectDefinitionImpl::put_VideoStabilization(
    boolean value)
{
    auto lock = _lock.Lock();

    ComPtr<ABI::Windows::Foundation::Collections::IMap<HSTRING, IInspectable *>> spMap;
    IFR(_spPropertySet.As(&spMap));

    ComPtr<ABI::Windows::Foundation::IPropertyValue> spPropVal;
    IFR(_spFactory->CreateBoolean(value, &spPropVal));

    boolean fReplaced = false;
    IFR(spMap->Insert(Wrappers::HStringReference(L"VideoStabilizationEnabled").Get(), spPropVal.Get(), &fReplaced));

    _videoStabilizationEnabled = value;

    return S_OK;
}

_Use_decl_annotations_
HRESULT MrcVideoEffectDefinitionImpl::get_VideoStabilizationBufferLength(
    UINT32* pValue)
{
    NULL_CHK(pValue);

    auto lock = _lock.Lock();

   * pValue = _videoStabilizationBufferLength;

    return S_OK;
}
_Use_decl_annotations_
HRESULT MrcVideoEffectDefinitionImpl::put_VideoStabilizationBufferLength(
    UINT32 value)
{
    auto lock = _lock.Lock();

    ComPtr<ABI::Windows::Foundation::Collections::IMap<HSTRING, IInspectable *>> spMap;
    IFR(_spPropertySet.As(&spMap));

    ComPtr<ABI::Windows::Foundation::IPropertyValue> spPropVal;
    IFR(_spFactory->CreateUInt32(value, &spPropVal));

    boolean fReplaced = false;
    IFR(spMap->Insert(Wrappers::HStringReference(L"VideoStabilizationBufferLength").Get(), spPropVal.Get(), &fReplaced));

    _videoStabilizationBufferLength = value;

    return S_OK;
}

_Use_decl_annotations_
HRESULT MrcVideoEffectDefinitionImpl::get_VideoStabilizationMaximumBufferLength(
    UINT32* pValue)
{
    NULL_CHK(pValue);

    auto lock = _lock.Lock();

   * pValue = _maxSupportedVsBuffer;

    return S_OK;
}

_Use_decl_annotations_
HRESULT MrcVideoEffectDefinitionImpl::put_GlobalOpacityCoefficient(
    float value)
{
    ComPtr<ABI::Windows::Foundation::Collections::IMap<HSTRING, IInspectable *>> spMap;
    IFR(_spPropertySet.As(&spMap));

    ComPtr<ABI::Windows::Foundation::IPropertyValue> spPropVal;
    IFR(_spFactory->CreateSingle(value, &spPropVal));

    boolean fReplaced = false;
    IFR(spMap->Insert(Wrappers::HStringReference(L"GlobalOpacityCoefficient").Get(), spPropVal.Get(), &fReplaced));

    _globalOpacityCoefficient = value;

    return S_OK;
}
_Use_decl_annotations_
HRESULT MrcVideoEffectDefinitionImpl::get_GlobalOpacityCoefficient(
    float* pValue)
{
    NULL_CHK(pValue);

    auto lock = _lock.Lock();

   * pValue = _globalOpacityCoefficient;

    return S_OK;
}

_Use_decl_annotations_
HRESULT MrcVideoEffectDefinitionImpl::get_RecordingIndicatorEnabled(
    boolean* pValue)
{
    NULL_CHK(pValue);

    auto lock = _lock.Lock();

   * pValue = _recordingIndicatorEnabled;

    return S_OK;
}
_Use_decl_annotations_
HRESULT MrcVideoEffectDefinitionImpl::put_RecordingIndicatorEnabled(
    boolean value)
{
    ComPtr<ABI::Windows::Foundation::Collections::IMap<HSTRING, IInspectable *>> spMap;
    IFR(_spPropertySet.As(&spMap));

    ComPtr<ABI::Windows::Foundation::IPropertyValue> spPropVal;
    IFR(_spFactory->CreateBoolean(value, &spPropVal));

    boolean fReplaced = false;
    IFR(spMap->Insert(Wrappers::HStringReference(L"RecordingIndicatorEnabled").Get(), spPropVal.Get(), &fReplaced));

    _recordingIndicatorEnabled = value;

    return S_OK;
}
