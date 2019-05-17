// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

namespace MixedRemoteViewCompositor
{
    namespace Media
    {

        class MrcVideoEffectDefinitionImpl
            : public RuntimeClass<RuntimeClassFlags<WinRtClassicComMix>
            , ABI::MixedRemoteViewCompositor::Media::IMrcVideoEffectDefinition
            , ABI::Windows::Media::Effects::IVideoEffectDefinition>
        {
            InspectableClass(RuntimeClass_MixedRemoteViewCompositor_Media_MrcVideoEffectDefinition, BaseTrust)

        public:
            MrcVideoEffectDefinitionImpl();
            virtual ~MrcVideoEffectDefinitionImpl();

            HRESULT RuntimeClassInitialize();

            // IVideoEffectDefinition
            IFACEMETHOD(get_ActivatableClassId)(
                _Out_ HSTRING* value);
            IFACEMETHOD(get_Properties)(
                _Out_ ABI::Windows::Foundation::Collections::IPropertySet** value);

            // IMrcVideoEffectDefinition
            IFACEMETHOD(get_StreamType)(
                _Out_ ABI::Windows::Media::Capture::MediaStreamType *value);

            IFACEMETHOD(put_StreamType)(
                _In_ ABI::Windows::Media::Capture::MediaStreamType value);

            IFACEMETHOD(get_HologramComposition)(
                _Out_ boolean *value);

            IFACEMETHOD(put_HologramComposition)(
                _In_ boolean value);

            IFACEMETHOD(get_VideoStabilization)(
                _Out_ boolean *value);

            IFACEMETHOD(put_VideoStabilization)(
                _In_ boolean value);

            IFACEMETHOD(get_VideoStabilizationBufferLength)(
                _Out_ UINT32 *value);

            IFACEMETHOD(put_VideoStabilizationBufferLength)(
                _In_ UINT32 value);

            IFACEMETHOD(get_VideoStabilizationMaximumBufferLength)(
                _Out_ UINT32 *value);

            IFACEMETHOD(put_GlobalOpacityCoefficient)(
                _In_ float value);

            IFACEMETHOD(get_GlobalOpacityCoefficient)(
                _Out_ float *value);

            IFACEMETHOD(put_RecordingIndicatorEnabled)(
                _In_ boolean value);

            IFACEMETHOD(get_RecordingIndicatorEnabled)(
                _Out_ boolean *value);

        private:
            Wrappers::CriticalSection _lock;

            Wrappers::HString _activatableClassId;

            ComPtr<ABI::Windows::Foundation::IPropertyValueStatics> _spFactory;
            ComPtr<ABI::Windows::Foundation::Collections::IPropertySet> _spPropertySet;

            ABI::Windows::Media::Capture::MediaStreamType _streamType;
            boolean _hologramCompositionEnabled;
            boolean _videoStabilizationEnabled;
            boolean _recordingIndicatorEnabled;
            UINT32  _videoStabilizationBufferLength;
            float   _globalOpacityCoefficient;
            UINT32  _maxSupportedVsBuffer;
        };

    }
}