#include "pch.h"
#include "Plugin.CaptureEngine.h"

namespace winrt::CameraCapture::Plugin::implementation
{
    HRESULT CaptureEngine::StartPreview(uint32_t width, uint32_t height, bool enableAudio, bool enableMrc)
    {
        throw hresult_not_implemented();
    }

    HRESULT CaptureEngine::StopPreview()
    {
        throw hresult_not_implemented();
    }

    Windows::Perception::Spatial::SpatialCoordinateSystem CaptureEngine::AppCoordinateSystem()
    {
        throw hresult_not_implemented();
    }

    void CaptureEngine::AppCoordinateSystem(Windows::Perception::Spatial::SpatialCoordinateSystem const& value)
    {
        throw hresult_not_implemented();
    }
}
