// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

namespace MixedRemoteViewCompositor
{
    namespace Plugin
    {
        using namespace ABI::MixedRemoteViewCompositor;

        class PluginManagerStaticsImpl
            : public ActivationFactory
            < Plugin::IPluginManagerStatics
            , FtmBase >
        {
            InspectableClassStatic(RuntimeClass_MixedRemoteViewCompositor_Plugin_PluginManager, BaseTrust);

        public:
            ~PluginManagerStaticsImpl();

            //// ActivationFactory
            //IFACEMETHOD(ActivateInstance)(
            //    _COM_Outptr_opt_result_maybenull_ IInspectable** ppvObject);

            // IPluginManagerstatics
            IFACEMETHOD(get_Instance)(
                _COM_Outptr_opt_result_maybenull_ IPluginManager** ppPluginManager);

            // PluginManagerImpl publics
            static STDMETHODIMP_(ComPtr<PluginManagerImpl>) GetInstance();
            static STDMETHODIMP_(ComPtr<IThreadPoolStatics>) GetThreadPool();
            static STDMETHODIMP_(BOOL) IsInitialized();
            static STDMETHODIMP Uninitialize();

            static STDMETHODIMP_(BOOL) IsOnThread();
            static STDMETHODIMP CheckThread();

            static STDMETHODIMP_(void) OnGraphicsDeviceEvent(
                _In_ UnityGfxDeviceEventType deviceEventType);
            static STDMETHODIMP_(void) OnPluginEvent(
                _In_ int pluginEventType);

            static STDMETHODIMP_(void) SetThreadName(LPCSTR szThreadName);


        private:
            static STDMETHODIMP_(BOOL) OneTimeInitializerProc(
                _Inout_ INIT_ONCE* pInitOnce,
                _Inout_opt_ VOID* pParameter,
                _Outptr_opt_result_maybenull_ VOID** ppContext);

        private:
            static INIT_ONCE                    s_initOnce;
            static DWORD                        s_threadId;
            static ComPtr<PluginManagerImpl>    s_spInstance;
            static BOOL                         s_isInitialized;
        };

    }
}