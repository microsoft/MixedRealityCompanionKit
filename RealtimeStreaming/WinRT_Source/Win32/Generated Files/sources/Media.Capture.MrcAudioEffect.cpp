#include "pch.h"
#include "Media.Capture.MrcAudioEffect.h"

namespace winrt::CameraCapture::Media::Capture::implementation
{
    CameraCapture::Media::Capture::AudioMixerMode MrcAudioEffect::MixerMode()
    {
        throw hresult_not_implemented();
    }

    void MrcAudioEffect::MixerMode(CameraCapture::Media::Capture::AudioMixerMode const& value)
    {
        throw hresult_not_implemented();
    }

    hstring MrcAudioEffect::ActivatableClassId()
    {
        throw hresult_not_implemented();
    }

    Windows::Foundation::Collections::IPropertySet MrcAudioEffect::Properties()
    {
        throw hresult_not_implemented();
    }
}
