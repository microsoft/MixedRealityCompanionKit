// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "ModuleManager.h"

ActivatableStaticOnlyFactory(ModuleManagerStaticsImpl);

_Use_decl_annotations_
ModuleManagerImpl::~ModuleManagerImpl() 
{
    Uninitialize();
}

_Use_decl_annotations_
HRESULT ModuleManagerImpl::RuntimeClassInitialize()
{
    _lastModuleHandleIndex = MODULE_HANDLE_START;
    _moduleHandleMap.clear();

    return S_OK;
}

_Use_decl_annotations_
HRESULT ModuleManagerImpl::AddModule(
    IModule *module, 
    ModuleHandle *moduleHandle)
{
    NULL_CHK(module);
    NULL_CHK(moduleHandle);

    auto lock = _lock.Lock();

    ComPtr<IModule> spModule(module);

    // get the last index value;
    ModuleHandle handle = _lastModuleHandleIndex;

    // create a pair for the insert success
    std::pair<std::map<ModuleHandle, ComPtr<IModule>>::iterator, bool> pairReturn;
    pairReturn = _moduleHandleMap.insert(std::make_pair(handle, spModule.Get()));

    // make sure pair was added
    if (pairReturn.second)
    {
        // assign and increment the value
        *moduleHandle = handle++;

        // assign the new value
        _lastModuleHandleIndex = handle;
    }

    return (pairReturn.second ? S_OK : E_UNEXPECTED);
}

_Use_decl_annotations_
HRESULT ModuleManagerImpl::GetModule(
    ModuleHandle moduleHandle, 
    IModule** ppModule)
{
    NULL_CHK(ppModule);

    if (moduleHandle <= MODULE_HANDLE_INVALID)
    {
        IFR(E_INVALIDARG);
    }

    auto lock = _lock.Lock();

    auto iter = _moduleHandleMap.find(moduleHandle);
    if (iter == _moduleHandleMap.end())
    {
        IFR(HRESULT_FROM_WIN32(ERROR_NOT_FOUND));
    }

    return iter->second.CopyTo(ppModule);
}

_Use_decl_annotations_
HRESULT ModuleManagerImpl::ReleaseModule(
    ModuleHandle moduleHandle)
{
    if (moduleHandle <= MODULE_HANDLE_INVALID)
    {
        IFR(E_INVALIDARG);
    }

    auto lock = _lock.Lock();

    auto iter = _moduleHandleMap.find(moduleHandle);
    if (iter == _moduleHandleMap.end())
    {
        IFR(HRESULT_FROM_WIN32(ERROR_NOT_FOUND));
    }

    ComPtr<IModule> module;
    IFR(iter->second.As(&module));

    LOG_RESULT(module->Uninitialize());
    LOG_RESULT(module.Reset());
    module = nullptr;

    LOG_RESULT(iter->second.Reset());
    iter->second = nullptr;

    _moduleHandleMap.erase(iter);

    return S_OK;
}

_Use_decl_annotations_
IFACEMETHODIMP 
ModuleManagerImpl::Uninitialize(void)
{
    auto lock = _lock.Lock();

    for each (auto map in _moduleHandleMap)
    {
        LOG_RESULT(map.second->Uninitialize());
        LOG_RESULT(map.second.Reset());
        map.second = nullptr;
    }
    _moduleHandleMap.clear();

    _lastModuleHandleIndex = MODULE_HANDLE_INVALID;

    return S_OK;
}
