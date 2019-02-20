#pragma once

#include "Plugin.ModuleManager.g.h"

namespace winrt::RealtimeStreaming::Plugin::implementation
{
    struct ModuleManager : ModuleManagerT<ModuleManager>
    {
        ModuleManager() = default;

        uint32_t AddModule(RealtimeStreaming::Plugin::IRTModule const& pluginModule);
        RealtimeStreaming::Plugin::IRTModule GetModule(uint32_t moduleHandle);
        void ReleaseModule(uint32_t moduleHandle);
    };
}

namespace winrt::RealtimeStreaming::Plugin::factory_implementation
{
    struct ModuleManager : ModuleManagerT<ModuleManager, implementation::ModuleManager>
    {
    };
}
