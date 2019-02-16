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
        ModuleManager();
        ~ModuleManager();

        // IModuleManager
        UINT32 AddModule(_In_ Module newModule);

        Module GetModule(_In_ ModuleHandle moduleHandle);

        void ReleaseModule(_In_ ModuleHandle moduleHandle);

    private:
        //Wrappers::CriticalSection _lock;
        slim_mutex m_lock;

        ModuleHandle _lastModuleHandleIndex;

        std::map<ModuleHandle, IModule> _moduleHandleMap;
    };
}