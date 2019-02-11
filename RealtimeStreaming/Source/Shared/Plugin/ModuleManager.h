// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

namespace RealtimeStreaming
{
    namespace Plugin
    {
        using namespace ABI::RealtimeStreaming;
        using namespace ABI::RealtimeStreaming::Plugin;

        class ModuleManagerImpl
            : public RuntimeClass
            < RuntimeClassFlags<RuntimeClassType::WinRtClassicComMix>
            , IModuleManager
            , FtmBase>
        {
            InspectableClass(RuntimeClass_RealtimeStreaming_Plugin_ModuleManager, BaseTrust);

        public:
            ~ModuleManagerImpl();

            // RuntimeClass
            STDMETHODIMP RuntimeClassInitialize();

            // IModuleManager
            IFACEMETHOD(AddModule)(
                _In_ IModule *module,
                _Out_ ModuleHandle* moduleHandle);

            IFACEMETHOD(GetModule)(
                _In_ ModuleHandle moduleHandle,
                _COM_Outptr_result_maybenull_ IModule** module);

            IFACEMETHOD(ReleaseModule)(
                _In_ ModuleHandle moduleHandle);

            IFACEMETHOD(Uninitialize)();

        private:
            Wrappers::CriticalSection _lock;

            ModuleHandle _lastModuleHandleIndex;

            std::map<ModuleHandle, com_ptr<IModule>> _moduleHandleMap;
        };

    }
}