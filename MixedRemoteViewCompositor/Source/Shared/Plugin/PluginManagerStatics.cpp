// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "PluginManagerStatics.h"

ActivatableStaticOnlyFactory(PluginManagerStaticsImpl);

ComPtr<PluginManagerImpl>   PluginManagerStaticsImpl::s_spInstance = nullptr;
INIT_ONCE                   PluginManagerStaticsImpl::s_initOnce = INIT_ONCE_STATIC_INIT;
DWORD                       PluginManagerStaticsImpl::s_threadId = 0;
BOOL                        PluginManagerStaticsImpl::s_isInitialized = FALSE;

PluginManagerStaticsImpl::~PluginManagerStaticsImpl()
{
    s_isInitialized = FALSE;
}

_Use_decl_annotations_
HRESULT PluginManagerStaticsImpl::get_Instance(
    IPluginManager** ppPluginManager)
{
    return GetInstance().CopyTo(ppPluginManager);
}

_Use_decl_annotations_
ComPtr<PluginManagerImpl> PluginManagerStaticsImpl::GetInstance()
{
    HRESULT hr = CheckThread();
    //if (FAILED(hr))
    //{
    //    return nullptr;
    //}
    LOG_RESULT(hr);

    return s_spInstance;
}

_Use_decl_annotations_
ComPtr<IThreadPoolStatics> PluginManagerStaticsImpl::GetThreadPool()
{
    HRESULT hr = CheckThread();

    ComPtr<IThreadPoolStatics> spThreadPool;
    if (FAILED(hr))
    {
        goto done;
    }

    hr = s_spInstance->get_ThreadPool(&spThreadPool);
    if (FAILED(hr))
    {
        goto done;
    }

done:
    return spThreadPool;
}

_Use_decl_annotations_
BOOL PluginManagerStaticsImpl::IsInitialized()
{
    return s_isInitialized;
}

_Use_decl_annotations_
HRESULT PluginManagerStaticsImpl::Uninitialize()
{
    IFR(InitOnceExecuteOnce(&s_initOnce, OneTimeInitializerProc, nullptr, nullptr) ? S_OK : HRESULT_FROM_WIN32(GetLastError()));

    if (!IsOnThread())
    {
        IFR(RPC_E_WRONG_THREAD);
    }

    s_spInstance = nullptr;

    InitOnceInitialize(&s_initOnce);

    return HRESULT_FROM_WIN32(GetLastError());
}

_Use_decl_annotations_
BOOL PluginManagerStaticsImpl::IsOnThread()
{
    return (s_threadId == GetCurrentThreadId());
}

_Use_decl_annotations_
HRESULT PluginManagerStaticsImpl::CheckThread()
{
    IFR(InitOnceExecuteOnce(&s_initOnce, OneTimeInitializerProc, nullptr, nullptr) ? S_OK : HRESULT_FROM_WIN32(GetLastError()));

    if (!IsOnThread())
    {
        IFR(RPC_E_WRONG_THREAD);
    }

    if (nullptr == s_spInstance)
    {
        ENSURE_HR(MakeAndInitialize<PluginManagerImpl>(&s_spInstance));

        s_isInitialized = true;
    }

    return S_OK;
}


_Use_decl_annotations_
void PluginManagerStaticsImpl::OnGraphicsDeviceEvent(UnityGfxDeviceEventType deviceEventType)
{
    return s_spInstance->OnDeviceEvent(deviceEventType);
}

_Use_decl_annotations_
void PluginManagerStaticsImpl::OnPluginEvent(int pluginEventType)
{
    return s_spInstance->OnPlugInEvent(static_cast<PluginEventType>(pluginEventType));
}

// https://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx
_Use_decl_annotations_
void PluginManagerStaticsImpl::SetThreadName(LPCSTR threadName)
{
#pragma pack(push, 8)
    struct THREADNAME_INFO
    {
        DWORD dwType;
        LPCSTR szName;
        DWORD dwThreadID;
        DWORD dwFlags;
    };
#pragma pack(pop)

    THREADNAME_INFO info{ 0x1000, threadName, 0xFFFFFFFF, 0 };

    __try
    {
        RaiseException(0x406D1388, 0, sizeof(info) / sizeof(ULONG_PTR), reinterpret_cast<ULONG_PTR*>(&info));
    }
    __except (EXCEPTION_CONTINUE_EXECUTION) { }
}


_Use_decl_annotations_
BOOL CALLBACK
PluginManagerStaticsImpl::OneTimeInitializerProc(
    INIT_ONCE* pInitOnce,
    VOID* pParameter,
    VOID** ppvContext)
{
    UNREFERENCED_PARAMETER(pInitOnce);
    UNREFERENCED_PARAMETER(pParameter);
    UNREFERENCED_PARAMETER(ppvContext);

    s_threadId = GetCurrentThreadId();

    return TRUE;
}

