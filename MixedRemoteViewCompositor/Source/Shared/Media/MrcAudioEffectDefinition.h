// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

namespace MixedRemoteViewCompositor
{
    namespace Media
    {

        class MrcAudioEffectDefinitionImpl
            : public RuntimeClass<RuntimeClassFlags<WinRtClassicComMix>
            , ABI::MixedRemoteViewCompositor::Media::IMrcAudioEffectDefinition
            , ABI::Windows::Media::Effects::IAudioEffectDefinition>
        {
            InspectableClass(RuntimeClass_MixedRemoteViewCompositor_Media_MrcAudioEffectDefinition, BaseTrust)

        public:
            MrcAudioEffectDefinitionImpl();
            virtual ~MrcAudioEffectDefinitionImpl();

            HRESULT RuntimeClassInitialize();

            // IAudioEffectDefinition
            IFACEMETHOD(get_ActivatableClassId)(
                _Out_ HSTRING* value);
            IFACEMETHOD(get_Properties)(
                _Out_ ABI::Windows::Foundation::Collections::IPropertySet** value);

            // IMrcAudioEffectDefinition
            IFACEMETHOD(get_MixerMode)(
                _Out_ ABI::MixedRemoteViewCompositor::Media::AudioMixerMode* value);
            IFACEMETHOD(put_MixerMode)(
                _In_ ABI::MixedRemoteViewCompositor::Media::AudioMixerMode value);

        private:
            Wrappers::CriticalSection _lock;

            Wrappers::HString _activatableClassId;

            ComPtr<ABI::Windows::Foundation::IPropertyValueStatics> _spFactory;
            ComPtr<ABI::Windows::Foundation::Collections::IPropertySet> _spPropertySet;

            ABI::MixedRemoteViewCompositor::Media::AudioMixerMode _mixerMode;
        };

    }
}