#pragma once

#include "Plugin/DirectXManager.g.h"
#include "Plugin.Module.h"

namespace winrt::RealtimeStreaming::Plugin::implementation
{
    struct DirectXManager : DirectXManagerT<DirectXManager, RealtimeStreaming::Plugin::implementation::Module>
    {
        DirectXManager() = delete;

        void Lost();
        void Reset();
    };
}
