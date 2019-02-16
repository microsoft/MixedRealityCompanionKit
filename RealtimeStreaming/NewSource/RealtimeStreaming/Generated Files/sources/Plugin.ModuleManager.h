#pragma once

#include "Plugin.ModuleManager.g.h"

namespace winrt::RealtimeStreaming::Plugin::implementation
{
    struct ModuleManager : ModuleManagerT<ModuleManager>
    {
        ModuleManager() = delete;

        uint32_t AddModule(RealtimeStreaming::Plugin::Module const& pluginModule);
        RealtimeStreaming::Plugin::Module GetModule(uint32_t moduleHandle);
        void ReleaseModule(uint32_t moduleHandle);
    };
}
