// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

namespace MixedRemoteViewCompositor
{
    namespace Plugin
    {
        using namespace ABI::MixedRemoteViewCompositor;
        using namespace ABI::MixedRemoteViewCompositor::Plugin;

        using namespace ABI::Windows::Foundation;
        using namespace ABI::Windows::System::Threading;

        extern "C" typedef void(UNITY_INTERFACE_API *PluginCallback)(
            _In_ ModuleHandle handle, 
            _In_ HRESULT result, 
            _In_ LPCWSTR pszMessage);

        extern "C" typedef void(UNITY_INTERFACE_API *DataReceivedHandler)(
            _In_ ModuleHandle handle, 
            _In_ UINT16 opertion,
            _In_ UINT32 bufferSize, 
            _In_ const byte* buffer);

        extern "C" typedef void(UNITY_INTERFACE_API *FrameSizeChanged)(
            _In_ UINT32 width, 
            _In_ UINT32 height);

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

        extern "C" typedef void(UNITY_INTERFACE_API *SampleUpdated)(
            _In_ MediaSampleArgs *args);

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
            STDMETHODIMP_(void) SetStreamingAssetsPath(
                _In_ LPCWSTR path);
            STDMETHODIMP_(void) SetTime(
                _In_ float t);
            STDMETHODIMP_(void) OnPlugInEvent(
                _In_ Plugin::PluginEventType eventType);

            // Plugin
            STDMETHODIMP ListenerCreateAndStart(
                _In_ UINT16 port, 
                _Inout_ ModuleHandle* listenerHandle,
                _In_ PluginCallback callback);
            STDMETHODIMP ListenerStopAndClose(
                _In_ ModuleHandle listenerHandle);

            STDMETHODIMP ConnectorCreateAndStart(
                _In_ LPCWSTR address,
                _Inout_ ModuleHandle* connectorHandle,
                _In_ PluginCallback callback);
            STDMETHODIMP ConnectorStopAndClose(
                _In_ ModuleHandle connectorHandle);

            STDMETHODIMP ConnectionAddDisconnected(
                _In_ ModuleHandle connectionHandle,
                _In_ PluginCallback callback,
                _Out_ INT64* tokenValue);
            STDMETHODIMP ConnectionRemoveDisconnected(
                _In_ ModuleHandle connectionHandle,
                _In_ INT64 token);
            STDMETHODIMP ConnectionAddReceived(
                _In_ ModuleHandle connectionHandle,
                _In_ DataReceivedHandler callback,
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
            
            STDMETHODIMP CaptureCreateAsync( 
                _In_ bool enableAudio, 
                _In_ PluginCallback callback);
            STDMETHODIMP CaptureAddClosed(
                _In_ ModuleHandle playbackEngineHandle,
                _In_ PluginCallback callback,
                _Out_ INT64* tokenValue);
            STDMETHODIMP CaptureRemoveClosed(
                _In_ ModuleHandle playbackEngineHandle,
                _Out_ INT64 tokenValue);
            STDMETHODIMP CaptureStartAsync(
                _In_ ModuleHandle captureHandle,
                _In_ ModuleHandle connectionHandle,
                _In_ bool enableMrc,
                _In_ IUnknown* pUnkSpatial,
                _In_ PluginCallback callback);
            STDMETHODIMP CaptureStopAsync(
                _In_ ModuleHandle captureHandle, 
                _In_ PluginCallback callback);
            STDMETHODIMP CaptureClose(
                _In_ ModuleHandle captureHandle);
            STDMETHODIMP SetSpatialCoordinateSystem(
                _In_ ModuleHandle captureHandle, 
                _In_ IUnknown* pUnkSpatial);

            STDMETHODIMP PlaybackCreate(
                _In_ ModuleHandle connectionHandle,
                _In_ PluginCallback createdCallback);
            STDMETHODIMP PlaybackAddClosed(
                _In_ ModuleHandle playbackEngineHandle,
                _In_ PluginCallback callback,
                _Out_ INT64* tokenValue);
            STDMETHODIMP PlaybackRemoveClosed(
                _In_ ModuleHandle playbackEngineHandle,
                _Out_ INT64 tokenValue);
            STDMETHODIMP PlaybackAddSizeChanged(
                _In_ ModuleHandle playbackEngineHandle,
                _In_ FrameSizeChanged callback,
                _Out_ INT64* tokenValue);
            STDMETHODIMP PlaybackRemoveSizeChanged(
                _In_ ModuleHandle playbackEngineHandle,
                _Out_ INT64 tokenValue);
            STDMETHODIMP PlaybackAddSampleUpdated(
                _In_ ModuleHandle playbackEngineHandle,
                _In_ SampleUpdated callback,
                _Out_ INT64* tokenValue);
            STDMETHODIMP PlaybackRemoveSampleUpdated(
                _In_ ModuleHandle playbackEngineHandle,
                _Out_ INT64 tokenValue);
            STDMETHODIMP PlaybackStart(
                _In_ ModuleHandle playbackEngineHandle);
            STDMETHODIMP PlaybackStop(
                _In_ ModuleHandle playbackEngineHandle);
            STDMETHODIMP PlaybackClose(
                _In_ ModuleHandle handle);
            STDMETHODIMP PlaybackGetFrameData(
                _In_ ModuleHandle handle,
                _Inout_ MediaSampleArgs* pSampleArgs);

        private:
            STDMETHODIMP_(void) Uninitialize();

            STDMETHODIMP_(void) CompletePluginCallback(
                _In_ PluginCallback callback, 
                _In_ ModuleHandle handle,
                _In_ HRESULT hr);

            STDMETHODIMP GetConnection(
                _In_ ModuleHandle handle,
                _Out_ IConnection** ppPlaybackEngine);

            STDMETHODIMP GetCaptureEngine(
                _In_ ModuleHandle handle,
                _Out_ ICaptureEngine** ppPlaybackEngine);

            STDMETHODIMP GetPlaybackEngine(
                _In_ ModuleHandle handle,
                _Out_ IPlaybackEngine** ppPlaybackEngine);

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