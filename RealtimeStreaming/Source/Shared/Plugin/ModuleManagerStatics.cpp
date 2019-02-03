// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "MixedRemoteViewCompositor.h"
#include "ModuleManagerStatics.h"

_Use_decl_annotations_
HRESULT ModuleManagerStaticsImpl::get_InvalidModuleHandle(ModuleHandle* moduleHandle)
{
    NULL_CHK(moduleHandle);

    *moduleHandle = MODULE_HANDLE_INVALID;

    return S_OK;
}

