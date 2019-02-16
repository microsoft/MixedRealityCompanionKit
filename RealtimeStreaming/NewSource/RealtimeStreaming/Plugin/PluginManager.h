// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "Plugin.PluginManager.g.h"

#include "Unity\IUnityInterface.h"
#include "Unity\IUnityGraphics.h"

namespace winrt::RealtimeStreaming::Plugin::implementation
{
    typedef UINT32 ModuleHandle;

    // UNITY_INTERFACE_API instead of __stdcall
    extern "C" typedef void(__stdcall *PluginCallback)(
        _In_ ModuleHandle handle,
        _In_ void* pCallbackObject,
        _In_ HRESULT result);
    //_In_ LPCWSTR pszMessage); // TODO: issue with instantiating data in c++ and marshalling back to C#

    extern "C" typedef void(__stdcall *PlayerCreatedCallback)(
        _In_ void* pCallbackObject,
        _In_ HRESULT result,
        _In_ UINT32 width,
        _In_ UINT32 height);

    extern "C" typedef void(__stdcall *DataReceivedHandler)(
        _In_ ModuleHandle handle,
        _In_ void* pCallbackObject,
        _In_ UINT16 opertion,
        _In_ UINT32 bufferSize,
        _In_ const byte* buffer);

    extern "C" struct MediaSampleArgs
    {
        ::IUnknown* videoTexture;
        UINT32 width;
        UINT32 height;
        UINT64 timestamp;
    };

    struct PluginManager : PluginManagerT<PluginManager>
    {
        public:
            PluginManager();
            ~PluginManager();

            static PluginManager Instance();

            winrt::RealtimeStreaming::Plugin::implementation::ModuleManager ModuleManager();

            void Load(_In_ IUnityInterfaces* unityInterfaces);
            void UnLoad();
            void OnDeviceEvent(_In_ UnityGfxDeviceEventType eventType);
            BOOL IsOnThread();

            // Plugin
            STDMETHODIMP ListenerCreateAndStart(
                _In_ UINT16 port,
                _Inout_ ModuleHandle* listenerHandle,
                _In_ PluginCallback callback,
                _In_ void* pCallbackObject);
            STDMETHODIMP ListenerStopAndClose(
                _In_ ModuleHandle listenerHandle);

            STDMETHODIMP ConnectorCreateAndStart(
                _In_ LPCWSTR address,
                _Inout_ ModuleHandle* connectorHandle,
                _In_ PluginCallback callback,
                _In_ void* pCallbackObject);
            STDMETHODIMP ConnectorStopAndClose(
                _In_ ModuleHandle connectorHandle);

            STDMETHODIMP ConnectionAddDisconnected(
                _In_ ModuleHandle connectionHandle,
                _In_ PluginCallback callback,
                _In_ void* pCallbackObject,
                _Out_ INT64* tokenValue);
            STDMETHODIMP ConnectionRemoveDisconnected(
                _In_ ModuleHandle connectionHandle,
                _In_ INT64 token);
            STDMETHODIMP ConnectionAddReceived(
                _In_ ModuleHandle connectionHandle,
                _In_ DataReceivedHandler callback,
                _In_ void* pCallbackObject,
                _Out_ INT64* tokenValue);
            STDMETHODIMP ConnectionRemoveReceived(
                _In_ ModuleHandle connectionHandle,
                _In_ INT64 token);
            STDMETHODIMP ConnectionSendRawData(
                _In_ ModuleHandle connectionHandle,
                _In_ winrt::RealtimeStreaming::Common::PayloadType payloadType,
                __in_ecount(bufferSize) byte* pBuffer,
                _In_ UINT32 bufferSize);
            STDMETHODIMP ConnectionClose(
                _In_ ModuleHandle connectionHandle);

            STDMETHODIMP RTServerCreate(
                _In_ ModuleHandle connectionHandle,
                _Inout_ ModuleHandle* serverHandle);
            STDMETHODIMP RTServerShutdown(
                _In_ ModuleHandle serverHandle);
            STDMETHODIMP RTServerWriteFrame(
                _In_ ModuleHandle serverHandle,
                __in_ecount(bufferSize) BYTE const* pBuffer,
                _In_ UINT32 bufferSize);

            STDMETHODIMP RTPlayerCreate(
                _In_ ModuleHandle connectionHandle,
                //_In_ StateChangedCallback fnCallback,
                _In_ PlayerCreatedCallback createdCallback,
                _In_ void* pCallbackObject);

            STDMETHODIMP RTPlayerRelease();

            STDMETHODIMP RTPlayerCreateTexture(
                _In_ UINT32 width,
                _In_ UINT32 height,
                _COM_Outptr_ void** ppvTexture);

            STDMETHODIMP RTPlayerStart();
            STDMETHODIMP RTPlayerStop();
            STDMETHODIMP RTPlayerPause();

        private:
            void Uninitialize();

            STDMETHODIMP_(void) CompletePluginCallback(
                _In_ PluginCallback callback,
                _In_ void* pCallbackObject,
                _In_ ModuleHandle handle,
                _In_ HRESULT hr);

            template <class T>
            T GetModule(_In_ ModuleHandle handle);

            void StoreToken(
                _In_ ModuleHandle handle,
                _In_ winrt::event_token const& newToken);
                
            winrt::event_token RemoveToken(
                _In_ ModuleHandle handle,
                _In_ INT64 tokenValue);

        private:
            static DWORD                        s_threadId;

            //Wrappers::CriticalSection _lock;
            slim_mutex m_lock;

            ModuleManager               m_moduleManager;

            IUnityInterfaces*                   m_unityInterfaces;
            IUnityGraphics*                     m_unityGraphics;
            IUnityGraphicsDeviceEventCallback   m_unityCallback;

            std::wstring    m_streamingAssetsPath;

            std::map<ModuleHandle, std::vector<winrt::event_token>> m_eventTokens;

            std::vector<byte> _bundleData;
    };
}


namespace winrt::RealtimeStreaming::Plugin::factory_implementation
{
    struct PluginManager : PluginManagerT<PluginManager, implementation::PluginManager>
    {
    };
}