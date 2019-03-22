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

    m_moduleHandleMap.clear();

    m_lastModuleHandleIndex = MODULE_HANDLE_INVALID;
}

_Use_decl_annotations_
ModuleHandle ModuleManager::AddModule(
    RealtimeStreaming::Plugin::IRTModule newModule)
{
    slim_lock_guard guard(m_lock);

    // get the last index value;
    ModuleHandle handle = m_lastModuleHandleIndex;
    ModuleHandle returnHandle = MODULE_HANDLE_INVALID;

    // create a pair for the insert success
    auto pairReturn = m_moduleHandleMap.insert(std::make_pair(handle, newModule));

    // make sure pair was added
    if (pairReturn.second)
    {
        // assign and increment the value
        returnHandle = handle++;

        // assign the new value
        m_lastModuleHandleIndex = handle;
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

    auto iter = m_moduleHandleMap.find(moduleHandle);
    if (iter == m_moduleHandleMap.end())
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

    auto iter = m_moduleHandleMap.find(moduleHandle);
    if (iter == m_moduleHandleMap.end())
    {
        IFT(HRESULT_FROM_WIN32(ERROR_NOT_FOUND));
    }

    m_moduleHandleMap.erase(iter);
}

void ModuleManager::StoreToken(
    ModuleHandle handle,
    winrt::event_token const& newToken)
{
    // get the list of tokens for connection
    auto iter = m_eventTokens.find(handle);
    if (iter == m_eventTokens.end())
    {
        // if none exist, create then list
        m_eventTokens.insert(std::move(std::make_pair(handle, std::vector<winrt::event_token>())));

        // it should be in the list now
        iter = m_eventTokens.find(handle);
    }

    // set eventList
    std::vector<winrt::event_token> &eventList = iter->second;

    // register callback and track token
    eventList.emplace_back(std::move(newToken));
}

winrt::event_token ModuleManager::RemoveToken(
    ModuleHandle handle,
    INT64 tokenValue)
{
    winrt::event_token returnToken;

    // get the list of tokens for connection
    auto mapIt = m_eventTokens.find(handle);
    if (mapIt != m_eventTokens.end())
    {
        // iterate tokens to match value
        std::vector<winrt::event_token> &eventList = mapIt->second;
        auto vectorIt = std::find_if(
            eventList.begin(),
            eventList.end(),
            [&tv = tokenValue](const winrt::event_token& t) -> bool
        {
            return (tv == t.value);
        });

        if (vectorIt != eventList.end())
        {
            returnToken = *vectorIt;

            eventList.erase(vectorIt);
        }
    }

    return returnToken;
}

std::vector<winrt::event_token> ModuleManager::RemoveTokens(
    _In_ ModuleHandle handle)
{
    std::vector<winrt::event_token> eventList;
    auto mapIt = m_eventTokens.find(handle);
    if (mapIt != m_eventTokens.end())
    {
        eventList = mapIt->second;

        m_eventTokens.erase(handle);
    }

    return eventList;
}
