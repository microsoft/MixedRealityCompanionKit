#pragma once

#include "Media/Capture/MrcVideoEffect.g.h"

namespace winrt::CameraCapture::Media::Capture::implementation
{
    struct MrcVideoEffect : MrcVideoEffectT<MrcVideoEffect>
    {
        MrcVideoEffect() = default;

        float GlobalOpacityCoefficient();
        void GlobalOpacityCoefficient(float value);
        bool HologramCompositionEnabled();
        void HologramCompositionEnabled(bool value);
        bool RecordingIndicatorEnabled();
        void RecordingIndicatorEnabled(bool value);
        Windows::Media::Capture::MediaStreamType StreamType();
        void StreamType(Windows::Media::Capture::MediaStreamType const& value);
        bool VideoStabilizationEnabled();
        void VideoStabilizationEnabled(bool value);
        uint32_t VideoStabilizationBufferLength();
        void VideoStabilizationBufferLength(uint32_t value);
        uint32_t VideoStabilizationMaximumBufferLength();
        hstring ActivatableClassId();
        Windows::Foundation::Collections::IPropertySet Properties();
    };
}

namespace winrt::CameraCapture::Media::Capture::factory_implementation
{
    struct MrcVideoEffect : MrcVideoEffectT<MrcVideoEffect, implementation::MrcVideoEffect>
    {
    };
}
