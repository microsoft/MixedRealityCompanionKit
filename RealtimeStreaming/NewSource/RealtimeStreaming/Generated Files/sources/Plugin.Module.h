#pragma once

#include "Plugin/Module.g.h"

namespace winrt::RealtimeStreaming::Plugin::implementation
{
    struct Module : ModuleT<Module>
    {
        Module() = delete;

        void Shutdown();
    };
}
