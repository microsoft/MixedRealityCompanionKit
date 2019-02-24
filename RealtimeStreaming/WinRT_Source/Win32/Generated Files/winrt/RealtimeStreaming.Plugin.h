﻿// WARNING: Please don't edit this file. It was generated by C++/WinRT v1.0.180821.2

#pragma once

#include "winrt/base.h"


static_assert(winrt::check_version(CPPWINRT_VERSION, "1.0.180821.2"), "Mismatched component and base headers.");
#include "winrt/Windows.Foundation.h"
#include "winrt/Windows.Foundation.Collections.h"
#include "winrt/impl/RealtimeStreaming.Plugin.2.h"

namespace winrt::impl {

template <typename D> uint32_t consume_RealtimeStreaming_Plugin_IModuleManager<D>::AddModule(RealtimeStreaming::Plugin::IRTModule const& pluginModule) const
{
    uint32_t result{};
    check_hresult(WINRT_SHIM(RealtimeStreaming::Plugin::IModuleManager)->AddModule(get_abi(pluginModule), &result));
    return result;
}

template <typename D> RealtimeStreaming::Plugin::IRTModule consume_RealtimeStreaming_Plugin_IModuleManager<D>::GetModule(uint32_t moduleHandle) const
{
    RealtimeStreaming::Plugin::IRTModule result{ nullptr };
    check_hresult(WINRT_SHIM(RealtimeStreaming::Plugin::IModuleManager)->GetModule(moduleHandle, put_abi(result)));
    return result;
}

template <typename D> void consume_RealtimeStreaming_Plugin_IModuleManager<D>::ReleaseModule(uint32_t moduleHandle) const
{
    check_hresult(WINRT_SHIM(RealtimeStreaming::Plugin::IModuleManager)->ReleaseModule(moduleHandle));
}

template <typename D> bool consume_RealtimeStreaming_Plugin_IPluginManager<D>::IsOnThread() const
{
    bool result{};
    check_hresult(WINRT_SHIM(RealtimeStreaming::Plugin::IPluginManager)->IsOnThread(&result));
    return result;
}

template <typename D> void consume_RealtimeStreaming_Plugin_IRTModule<D>::Shutdown() const
{
    check_hresult(WINRT_SHIM(RealtimeStreaming::Plugin::IRTModule)->Shutdown());
}

template <typename D>
struct produce<D, RealtimeStreaming::Plugin::IModuleManager> : produce_base<D, RealtimeStreaming::Plugin::IModuleManager>
{
    int32_t WINRT_CALL AddModule(void* pluginModule, uint32_t* result) noexcept final
    {
        try
        {
            typename D::abi_guard guard(this->shim());
            WINRT_ASSERT_DECLARATION(AddModule, WINRT_WRAP(uint32_t), RealtimeStreaming::Plugin::IRTModule const&);
            *result = detach_from<uint32_t>(this->shim().AddModule(*reinterpret_cast<RealtimeStreaming::Plugin::IRTModule const*>(&pluginModule)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    }

    int32_t WINRT_CALL GetModule(uint32_t moduleHandle, void** result) noexcept final
    {
        try
        {
            *result = nullptr;
            typename D::abi_guard guard(this->shim());
            WINRT_ASSERT_DECLARATION(GetModule, WINRT_WRAP(RealtimeStreaming::Plugin::IRTModule), uint32_t);
            *result = detach_from<RealtimeStreaming::Plugin::IRTModule>(this->shim().GetModule(moduleHandle));
            return 0;
        }
        catch (...) { return to_hresult(); }
    }

    int32_t WINRT_CALL ReleaseModule(uint32_t moduleHandle) noexcept final
    {
        try
        {
            typename D::abi_guard guard(this->shim());
            WINRT_ASSERT_DECLARATION(ReleaseModule, WINRT_WRAP(void), uint32_t);
            this->shim().ReleaseModule(moduleHandle);
            return 0;
        }
        catch (...) { return to_hresult(); }
    }
};

template <typename D>
struct produce<D, RealtimeStreaming::Plugin::IPluginManager> : produce_base<D, RealtimeStreaming::Plugin::IPluginManager>
{
    int32_t WINRT_CALL IsOnThread(bool* result) noexcept final
    {
        try
        {
            typename D::abi_guard guard(this->shim());
            WINRT_ASSERT_DECLARATION(IsOnThread, WINRT_WRAP(bool));
            *result = detach_from<bool>(this->shim().IsOnThread());
            return 0;
        }
        catch (...) { return to_hresult(); }
    }
};

template <typename D>
struct produce<D, RealtimeStreaming::Plugin::IRTModule> : produce_base<D, RealtimeStreaming::Plugin::IRTModule>
{
    int32_t WINRT_CALL Shutdown() noexcept final
    {
        try
        {
            typename D::abi_guard guard(this->shim());
            WINRT_ASSERT_DECLARATION(Shutdown, WINRT_WRAP(void));
            this->shim().Shutdown();
            return 0;
        }
        catch (...) { return to_hresult(); }
    }
};

}

WINRT_EXPORT namespace winrt::RealtimeStreaming::Plugin {

inline ModuleManager::ModuleManager() :
    ModuleManager(impl::call_factory<ModuleManager>([](auto&& f) { return f.template ActivateInstance<ModuleManager>(); }))
{}

inline PluginManager::PluginManager() :
    PluginManager(impl::call_factory<PluginManager>([](auto&& f) { return f.template ActivateInstance<PluginManager>(); }))
{}

}

namespace winrt::impl {

}

WINRT_EXPORT namespace winrt::experimental::reflect {
template <> struct get_enumerator_names<RealtimeStreaming::Plugin::DeviceType>
{
    static constexpr std::array<std::wstring_view, 3> value{{ 
        {L"Null", 4},
        {L"DX11", 4},
        {L"Warp", 4}, }};
};
template <> struct get_enumerator_values<RealtimeStreaming::Plugin::DeviceType>
{
    static constexpr std::array<RealtimeStreaming::Plugin::DeviceType, 3> value{{ 
        RealtimeStreaming::Plugin::DeviceType::Null,
        RealtimeStreaming::Plugin::DeviceType::DX11,
        RealtimeStreaming::Plugin::DeviceType::Warp, }};
};

}

WINRT_EXPORT namespace std {

template<> struct hash<winrt::RealtimeStreaming::Plugin::IModuleManager> : winrt::impl::hash_base<winrt::RealtimeStreaming::Plugin::IModuleManager> {};
template<> struct hash<winrt::RealtimeStreaming::Plugin::IPluginManager> : winrt::impl::hash_base<winrt::RealtimeStreaming::Plugin::IPluginManager> {};
template<> struct hash<winrt::RealtimeStreaming::Plugin::IRTModule> : winrt::impl::hash_base<winrt::RealtimeStreaming::Plugin::IRTModule> {};
template<> struct hash<winrt::RealtimeStreaming::Plugin::ModuleManager> : winrt::impl::hash_base<winrt::RealtimeStreaming::Plugin::ModuleManager> {};
template<> struct hash<winrt::RealtimeStreaming::Plugin::PluginManager> : winrt::impl::hash_base<winrt::RealtimeStreaming::Plugin::PluginManager> {};

}