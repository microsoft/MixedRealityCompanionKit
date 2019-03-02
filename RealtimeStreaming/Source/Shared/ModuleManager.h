// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "Plugin.ModuleManager.g.h"

namespace winrt::RealtimeStreaming::Plugin::implementation
{
    typedef UINT32 ModuleHandle;

    struct ModuleManager : ModuleManagerT<ModuleManager>
    {

    public:
        ModuleManager() = default;
        ~ModuleManager();

        UINT32 AddModule(_In_ IRTModule newModule);

        IRTModule GetModule(_In_ ModuleHandle moduleHandle);

        void ReleaseModule(_In_ ModuleHandle moduleHandle);

        void ClearModules();

    private:
        slim_mutex m_lock;

        ModuleHandle _lastModuleHandleIndex = MODULE_HANDLE_START;

        std::map<ModuleHandle, IRTModule> _moduleHandleMap;
    };
}

namespace winrt::RealtimeStreaming::Plugin::factory_implementation
{
    struct ModuleManager : ModuleManagerT<ModuleManager, implementation::ModuleManager>
    {
    };
}
