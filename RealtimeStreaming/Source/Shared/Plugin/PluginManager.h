// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

namespace RealtimeStreaming
{
    namespace Plugin
    {
        using namespace ABI::RealtimeStreaming;
        using namespace ABI::RealtimeStreaming::Media;
        using namespace ABI::RealtimeStreaming::Plugin;

        using namespace ABI::Windows::Foundation;
        using namespace ABI::Windows::System::Threading;

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
            Matrix4x4 cameraViewTransform;
            Matrix4x4 cameraProjection;
            Matrix4x4 cameraCoordinate;
            Matrix4x4 cameraAffine;
        };

        typedef std::function<void(_In_ float deltaTime, _In_ float relativeTime)> UpdateAction;
        typedef std::function<void()> RenderAction;

        class PluginManagerImpl
            : public RuntimeClass
            < RuntimeClassFlags<RuntimeClassType::WinRtClassicComMix>
            , IPluginManager
            , FtmBase >
        {
        public:
            ~PluginManagerImpl();

            // RuntimeClass
            STDMETHODIMP RuntimeClassInitialize();

            // IPluginManager
            IFACEMETHOD(get_ModuleManager)(
                _COM_Outptr_opt_result_maybenull_ IModuleManager** moduleManager);
            IFACEMETHOD(get_ThreadPool)(
                _COM_Outptr_opt_result_maybenull_ IThreadPoolStatics** threadPool);
            IFACEMETHOD(get_DirectXManager)(
                _COM_Outptr_opt_result_maybenull_ IDirectXManager** dxManager);

            // PluginManagerImpl
            STDMETHODIMP_(void) Load(
                _In_ IUnityInterfaces* unityInterfaces,
                _In_ IUnityGraphicsDeviceEventCallback callback);
            STDMETHODIMP_(void) UnLoad();
            STDMETHODIMP_(void) OnDeviceEvent(
                _In_ UnityGfxDeviceEventType eventType);
            STDMETHODIMP_(void) SetTime(
                _In_ float t);
            STDMETHODIMP_(void) OnPlugInEvent(
                _In_ Plugin::PluginEventType eventType);

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

            STDMETHODIMP GetConnection(
                _In_ ModuleHandle handle,
                _Out_ IConnection** ppPlaybackEngine);

            STDMETHODIMP GetRealtimeServer(
                _In_ ModuleHandle handle,
                _Out_ IRealtimeServer** ppPlaybackEngine);

            STDMETHODIMP_(void) StoreToken(
                _In_ ModuleHandle handle,
                _In_ EventRegistrationToken& newToken);
            STDMETHODIMP_(void) RemoveToken(
                _In_ ModuleHandle handle,
                _In_ INT64 tokenValue,
                _Out_ EventRegistrationToken* token);

        private:
            Wrappers::CriticalSection _lock;

            ComPtr<IThreadPoolStatics>          _threadPoolStatics;
            ComPtr<IModuleManager>              _moduleManager;

            ComPtr<DirectXManagerImpl>          _dxManager;

            IUnityInterfaces*                   _unityInterfaces;
            IUnityGraphics*                     _unityGraphics;
            IUnityGraphicsDeviceEventCallback   _unityCallback;

            std::wstring    _streamingAssetsPath;

            float   _timeElasped;
            float   _deltaTime;

            concurrency::concurrent_queue<UpdateAction>     _updateQueue;
            concurrency::concurrent_queue<RenderAction>     _renderQueue;

            std::map<ModuleHandle, std::vector<EventRegistrationToken>> _eventTokens;

            std::vector<byte> _bundleData;
        };

    }
}