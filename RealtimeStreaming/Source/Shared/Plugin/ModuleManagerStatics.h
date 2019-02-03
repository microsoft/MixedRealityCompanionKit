// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

namespace MixedRemoteViewCompositor
{
    namespace Plugin
    {
        using namespace ABI::MixedRemoteViewCompositor;
        using namespace ABI::MixedRemoteViewCompositor::Plugin;

        class ModuleManagerStaticsImpl
            : public ActivationFactory
            < IModuleManagerStatics
            , FtmBase >
        {
            InspectableClassStatic(RuntimeClass_MixedRemoteViewCompositor_Plugin_ModuleManager, BaseTrust);

        public:
            // IModuleManagerStatics
            IFACEMETHOD(get_InvalidModuleHandle)(
                _Out_ ModuleHandle* moduleHandle);
        };

    }
}