// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

typedef uint32_t ModuleHandle;

struct __declspec(uuid("7948cd23-83dd-4f2f-bee8-624565f303d2")) IModuleManager : ::IUnknown
{
    virtual  UINT32 __stdcall AddModule(winrt::Windows::Foundation::IInspectable pluginModule) = 0;

    virtual winrt::Windows::Foundation::IInspectable __stdcall GetModule(UINT32 moduleHandle) = 0;

    virtual void __stdcall ReleaseModule(UINT32 moduleHandle) = 0;

    virtual void __stdcall ClearModules() = 0;

    virtual void __stdcall StoreToken(UINT32 handle, winrt::event_token const& newToken) = 0;

    virtual winrt::event_token __stdcall RemoveToken(
        UINT32 handle,
        INT64 tokenValue) = 0;

    virtual std::vector<winrt::event_token> __stdcall RemoveTokens(ModuleHandle handle) = 0;
};

namespace winrt::RealtimeStreaming::Plugin
{
    struct ModuleManager : winrt::implements<ModuleManager, IModuleManager>
    {

    public:
        ModuleManager() = default;
        virtual ~ModuleManager();

        STDOVERRIDEMETHODIMP_(UINT32) AddModule(
            _In_ Windows::Foundation::IInspectable newModule);

        STDOVERRIDEMETHODIMP_(Windows::Foundation::IInspectable) GetModule(
            _In_ ModuleHandle moduleHandle);

        STDOVERRIDEMETHODIMP_(void) ReleaseModule(
            _In_ ModuleHandle moduleHandle);

        STDOVERRIDEMETHODIMP_(void) ClearModules();

        STDOVERRIDEMETHODIMP_(void) StoreToken(
            _In_ ModuleHandle handle,
            _In_ winrt::event_token const& newToken);

        STDOVERRIDEMETHODIMP_(winrt::event_token) RemoveToken(
            _In_ ModuleHandle handle,
            _In_ INT64 tokenValue);

        STDOVERRIDEMETHODIMP_(std::vector<winrt::event_token>) RemoveTokens(
            _In_ ModuleHandle handle);

    private:
        slim_mutex m_lock;

        ModuleHandle m_lastModuleHandleIndex = MODULE_HANDLE_START;

        std::map<ModuleHandle, Windows::Foundation::IInspectable> m_moduleHandleMap;

        std::map<ModuleHandle, std::vector<winrt::event_token>> m_eventTokens;
    };
}