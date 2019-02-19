#pragma once

#include "Plugin/CaptureEngine.g.h"
#include "Plugin.Module.h"

namespace winrt::CameraCapture::Plugin::implementation
{
    struct CaptureEngine : CaptureEngineT<CaptureEngine, CameraCapture::Plugin::implementation::Module>
    {
        CaptureEngine() = default;

        HRESULT StartPreview(uint32_t width, uint32_t height, bool enableAudio, bool enableMrc);
        HRESULT StopPreview();
        Windows::Perception::Spatial::SpatialCoordinateSystem AppCoordinateSystem();
        void AppCoordinateSystem(Windows::Perception::Spatial::SpatialCoordinateSystem const& value);
    };
}

namespace winrt::CameraCapture::Plugin::factory_implementation
{
    struct CaptureEngine : CaptureEngineT<CaptureEngine, implementation::CaptureEngine>
    {
    };
}
