// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "ModuleManager.h"

using namespace winrt;
using namespace RealtimeStreaming::Plugin::implementation;

_Use_decl_annotations_
ModuleManager::~ModuleManager() 
{
    ClearModules();
}

_Use_decl_annotations_
void ModuleManager::ClearModules()
{
    slim_lock_guard guard(m_lock);

    _moduleHandleMap.clear();

    _lastModuleHandleIndex = MODULE_HANDLE_INVALID;
}

_Use_decl_annotations_
ModuleHandle ModuleManager::AddModule(
    RealtimeStreaming::Plugin::IRTModule newModule)
{
    slim_lock_guard guard(m_lock);

    // get the last index value;
    ModuleHandle handle = _lastModuleHandleIndex;
    ModuleHandle returnHandle = MODULE_HANDLE_INVALID;

    // create a pair for the insert success
    //std::pair<std::map<ModuleHandle, IRTModule> >::iterator, bool> pairReturn;
    auto pairReturn = _moduleHandleMap.insert(std::make_pair(handle, newModule));

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
RealtimeStreaming::Plugin::IRTModule ModuleManager::GetModule(ModuleHandle moduleHandle)
{
    if (moduleHandle <= MODULE_HANDLE_INVALID)
    {
        IFT(E_INVALIDARG);
    }

    slim_lock_guard guard(m_lock);

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

    slim_lock_guard guard(m_lock);

    auto iter = _moduleHandleMap.find(moduleHandle);
    if (iter == _moduleHandleMap.end())
    {
        IFT(HRESULT_FROM_WIN32(ERROR_NOT_FOUND));
    }

    _moduleHandleMap.erase(iter);
}
