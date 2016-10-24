// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "MrcAudioEffectDefinition.h"

MrcAudioEffectDefinitionImpl::MrcAudioEffectDefinitionImpl() :
    _mixerMode(AudioMixerMode_MicAndLoopback)
{
}

MrcAudioEffectDefinitionImpl::~MrcAudioEffectDefinitionImpl()
{
}

HRESULT MrcAudioEffectDefinitionImpl::RuntimeClassInitialize()
{
    // Initialize Activatable Class ID.
    IFR(_activatableClassId.Set(L"Windows.Media.MixedRealityCapture.MixedRealityCaptureAudioEffect"));

    // Create Property Value Factory
    IFR(Windows::Foundation::GetActivationFactory(
        Wrappers::HStringReference(RuntimeClass_Windows_Foundation_PropertyValue).Get(),
        _spFactory.GetAddressOf()));

    // Initialize the property set
    IFR(Windows::Foundation::ActivateInstance(
        Wrappers::HStringReference(RuntimeClass_Windows_Foundation_Collections_PropertySet).Get(),
        &_spPropertySet));

    // Set Default value
    return put_MixerMode(_mixerMode);
}

HRESULT MrcAudioEffectDefinitionImpl::get_ActivatableClassId(
    HSTRING* value)
{
    auto lock = _lock.Lock();

    NULL_CHK_HR(_activatableClassId, E_NOT_SET);

    return _activatableClassId.CopyTo(value);
}

HRESULT MrcAudioEffectDefinitionImpl::get_Properties(
    IPropertySet** value)
{
    auto lock = _lock.Lock();

    NULL_CHK_HR(_spPropertySet, E_NOT_SET);

    return _spPropertySet.CopyTo(value);
}

HRESULT MrcAudioEffectDefinitionImpl::get_MixerMode(
    AudioMixerMode* value)
{
    auto lock = _lock.Lock();

    *value = _mixerMode;

    return S_OK;
}

HRESULT MrcAudioEffectDefinitionImpl::put_MixerMode(AudioMixerMode value)
{
    IFR((
        value == AudioMixerMode_Mic 
            || 
        value == AudioMixerMode_Loopback 
            || 
        value == AudioMixerMode_MicAndLoopback) ? S_OK : E_INVALIDARG);

    auto lock = _lock.Lock();

    ComPtr<ABI::Windows::Foundation::Collections::IMap<HSTRING, IInspectable *>> spMap;
    IFR(_spPropertySet.As(&spMap));

    // Set Audio Mixer mode
    ComPtr<ABI::Windows::Foundation::IPropertyValue> spPropVal;
    IFR(_spFactory->CreateUInt32(static_cast<UINT32>(value), &spPropVal));

    boolean fReplaced = false;
    spMap->Insert(Wrappers::HStringReference(L"MixerMode").Get(), spPropVal.Get(), &fReplaced);

    _mixerMode = value;

    return S_OK;
}
