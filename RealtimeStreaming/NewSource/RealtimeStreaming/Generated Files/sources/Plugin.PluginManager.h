#pragma once

#include "Plugin/PluginManager.g.h"

namespace winrt::RealtimeStreaming::Plugin::implementation
{
    struct PluginManager : PluginManagerT<PluginManager>
    {
        PluginManager() = delete;

        RealtimeStreaming::Plugin::ModuleManager ModuleManager();
        RealtimeStreaming::Plugin::DirectXManager DirectXManager();
    };
}
