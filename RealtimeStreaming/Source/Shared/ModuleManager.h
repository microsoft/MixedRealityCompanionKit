// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "Plugin.ModuleManager.g.h"

namespace winrt::RealtimeStreaming::Plugin::implementation
{
    typedef uint32_t ModuleHandle;

    struct ModuleManager : ModuleManagerT<ModuleManager>
    {

    public:
        ModuleManager() = default;
        ~ModuleManager();

        UINT32 AddModule(_In_ IRTModule newModule);

        IRTModule GetModule(_In_ ModuleHandle moduleHandle);

        void ReleaseModule(_In_ ModuleHandle moduleHandle);

        void ClearModules();

        void StoreToken(
            _In_ ModuleHandle handle,
            _In_ winrt::event_token const& newToken);

        winrt::event_token RemoveToken(
            _In_ ModuleHandle handle,
            _In_ INT64 tokenValue);

        std::vector<winrt::event_token> RemoveTokens(_In_ ModuleHandle handle);

    private:
        slim_mutex m_lock;

        ModuleHandle m_lastModuleHandleIndex = MODULE_HANDLE_START;

        std::map<ModuleHandle, IRTModule> m_moduleHandleMap;

        std::map<ModuleHandle, std::vector<winrt::event_token>> m_eventTokens;
    };
}

namespace winrt::RealtimeStreaming::Plugin::factory_implementation
{
    struct ModuleManager : ModuleManagerT<ModuleManager, implementation::ModuleManager>
    {
    };
}
