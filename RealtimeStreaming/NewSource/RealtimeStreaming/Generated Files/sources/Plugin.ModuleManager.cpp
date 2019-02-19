#include "pch.h"
#include "Plugin.ModuleManager.h"

namespace winrt::RealtimeStreaming::Plugin::implementation
{
    uint32_t ModuleManager::AddModule(RealtimeStreaming::Plugin::IRTModule const& pluginModule)
    {
        throw hresult_not_implemented();
    }

    RealtimeStreaming::Plugin::IRTModule ModuleManager::GetModule(uint32_t moduleHandle)
    {
        throw hresult_not_implemented();
    }

    void ModuleManager::ReleaseModule(uint32_t moduleHandle)
    {
        throw hresult_not_implemented();
    }
}
