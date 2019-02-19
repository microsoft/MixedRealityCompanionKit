#include "pch.h"
#include "Plugin.Module.h"

namespace winrt::CameraCapture::Plugin::implementation
{
    void Module::Shutdown()
    {
        throw hresult_not_implemented();
    }

    void Module::OnRenderEvent(uint16_t frameNumber)
    {
        throw hresult_not_implemented();
    }
}
