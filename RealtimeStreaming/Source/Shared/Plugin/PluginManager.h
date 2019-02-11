// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "PluginManager.g.h"

namespace winrt::RealtimeStreaming::Plugin::implementation
{
    extern "C" typedef void(UNITY_INTERFACE_API *PluginCallback)(
        _In_ ModuleHandle handle,
        _In_ void* pCallbackObject,
        _In_ HRESULT result);
    //_In_ LPCWSTR pszMessage); // TODO: issue with instantiating data in c++ and marshalling back to C#

    extern "C" typedef void(UNITY_INTERFACE_API *PlayerCreatedCallback)(
        _In_ void* pCallbackObject,
        _In_ HRESULT result,
        _In_ UINT32 width,
        _In_ UINT32 height);

    extern "C" typedef void(UNITY_INTERFACE_API *DataReceivedHandler)(
        _In_ ModuleHandle handle,
        _In_ void* pCallbackObject,
        _In_ UINT16 opertion,
        _In_ UINT32 bufferSize,
        _In_ const byte* buffer);

    extern "C" struct MediaSampleArgs
    {
        IUnknown* videoTexture;
        UINT32 width;
        UINT32 height;
        UINT64 timestamp;
    };

    typedef std::function<void(_In_ float deltaTime, _In_ float relativeTime)> UpdateAction;
    typedef std::function<void()> RenderAction;

    struct PluginManager : PluginManagerT<PluginManager>
    {
        public:
            PluginManager();
            ~PluginManager();

            RealtimeStreaming.Plugin.ModuleManager ModuleManager();

            RealtimeStreaming.Plugin.DirectXManager DirectXManager();

            IFACEMETHOD(get_ThreadPool)(
                _COM_Outptr_opt_result_maybenull_ IThreadPoolStatics** threadPool);

            void Load(
                _In_ IUnityInterfaces* unityInterfaces,
                _In_ IUnityGraphicsDeviceEventCallback callback);
            void UnLoad();
            void OnDeviceEvent(_In_ UnityGfxDeviceEventType eventType);

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
                _In_ PayloadType payloadType,
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
                __in_ecount(bufferSize) BYTE* pBuffer,
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
            STDMETHODIMP_(void) Uninitialize();

            STDMETHODIMP_(void) CompletePluginCallback(
                _In_ PluginCallback callback,
                _In_ void* pCallbackObject,
                _In_ ModuleHandle handle,
                _In_ HRESULT hr);

            Connection GetConnection(_In_ ModuleHandle handle);

            RealtimeServer GetRealtimeServer(_In_ ModuleHandle handle);

            STDMETHODIMP_(void) StoreToken(
                _In_ ModuleHandle handle,
                _In_ EventRegistrationToken& newToken);
            STDMETHODIMP_(void) RemoveToken(
                _In_ ModuleHandle handle,
                _In_ INT64 tokenValue,
                _Out_ EventRegistrationToken* token);

        private:
            Wrappers::CriticalSection _lock;

            com_ptr<IThreadPoolStatics>          m_threadPoolStatics;
            ModuleManager              m_moduleManager;

            DirectXManager          m_dxManager;

            IUnityInterfaces*                   m_unityInterfaces;
            IUnityGraphics*                     m_unityGraphics;
            IUnityGraphicsDeviceEventCallback   m_unityCallback;

            std::wstring    m_streamingAssetsPath;

            std::map<ModuleHandle, std::vector<EventRegistrationToken>> m_eventTokens;

            std::vector<byte> _bundleData;
        };

    }
}