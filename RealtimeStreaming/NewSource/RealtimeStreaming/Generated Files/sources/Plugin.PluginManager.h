#pragma once

#include "Plugin.PluginManager.g.h"

namespace winrt::RealtimeStreaming::Plugin::implementation
{
    struct PluginManager : PluginManagerT<PluginManager>
    {
        PluginManager() = default;

        RealtimeStreaming::Plugin::ModuleManager ModuleManager();
    };
}

namespace winrt::RealtimeStreaming::Plugin::factory_implementation
{
    struct PluginManager : PluginManagerT<PluginManager, implementation::PluginManager>
    {
    };
}
