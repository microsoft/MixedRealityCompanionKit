// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "ModuleManager.h"

using winrt;
using RealtimeStreaming::Plugin::implementation;

_Use_decl_annotations_
ModuleManager::ModuleManager()
{
    _lastModuleHandleIndex = MODULE_HANDLE_START;
    _moduleHandleMap.clear();

    return S_OK;
}

_Use_decl_annotations_
ModuleManager::~ModuleManager() 
{
    auto lock = _lock.Lock();

    _moduleHandleMap.clear();

    _lastModuleHandleIndex = MODULE_HANDLE_INVALID;
}

_Use_decl_annotations_
ModuleHandle ModuleManager::AddModule(Module newModule)
{
    auto lock = _lock.Lock();

    // get the last index value;
    ModuleHandle handle = _lastModuleHandleIndex;
    ModuleHandle returnHandle = MODULE_HANDLE_INVALID;

    // create a pair for the insert success
    std::pair<std::map<ModuleHandle, IModule>>::iterator, bool> pairReturn;
    pairReturn = _moduleHandleMap.insert(std::make_pair(handle, newModule));

    // make sure pair was added
    if (pairReturn.second)
    {
        // assign and increment the value
        returnHandle = handle++;

        // assign the new value
        _lastModuleHandleIndex = handle;
    }

    return returnHandle;
}

_Use_decl_annotations_
Module ModuleManager::GetModule(ModuleHandle moduleHandle)
{
    NULL_CHK(ppModule);

    if (moduleHandle <= MODULE_HANDLE_INVALID)
    {
        IFT(E_INVALIDARG);
    }

    auto lock = _lock.Lock();

    auto iter = _moduleHandleMap.find(moduleHandle);
    if (iter == _moduleHandleMap.end())
    {
        IFT(HRESULT_FROM_WIN32(ERROR_NOT_FOUND));
    }

    return iter->second;
}

_Use_decl_annotations_
void ModuleManager::ReleaseModule(
    ModuleHandle moduleHandle)
{
    if (moduleHandle <= MODULE_HANDLE_INVALID)
    {
        IFT(E_INVALIDARG);
    }

    auto lock = _lock.Lock();

    auto iter = _moduleHandleMap.find(moduleHandle);
    if (iter == _moduleHandleMap.end())
    {
        IFT(HRESULT_FROM_WIN32(ERROR_NOT_FOUND));
    }

    _moduleHandleMap.erase(iter);
}
