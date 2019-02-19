#pragma once

#include "Plugin/Module.g.h"

namespace winrt::CameraCapture::Plugin::implementation
{
    struct Module : ModuleT<Module>
    {
        Module() = delete;

        void Shutdown();
        void OnRenderEvent(uint16_t frameNumber);
    };
}
