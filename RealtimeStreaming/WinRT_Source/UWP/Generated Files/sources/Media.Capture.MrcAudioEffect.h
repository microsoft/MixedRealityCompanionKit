#pragma once

#include "Media/Capture/MrcAudioEffect.g.h"

namespace winrt::CameraCapture::Media::Capture::implementation
{
    struct MrcAudioEffect : MrcAudioEffectT<MrcAudioEffect>
    {
        MrcAudioEffect() = default;

        CameraCapture::Media::Capture::AudioMixerMode MixerMode();
        void MixerMode(CameraCapture::Media::Capture::AudioMixerMode const& value);
        hstring ActivatableClassId();
        Windows::Foundation::Collections::IPropertySet Properties();
    };
}

namespace winrt::CameraCapture::Media::Capture::factory_implementation
{
    struct MrcAudioEffect : MrcAudioEffectT<MrcAudioEffect, implementation::MrcAudioEffect>
    {
    };
}
