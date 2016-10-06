// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

namespace MixedRemoteViewCompositor
{
    namespace Media
    {
        const USHORT MaxRetryAmount = 15;

        class ProcessMediaSample
            : public Microsoft::WRL::RuntimeClass
            < Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::ClassicCom>
            , IUnknown >
        {
        public:
            ProcessMediaSample()
                : StreamFlags(-1)
                , Timestamp(0)
                , Sample(nullptr)
            {
            }

            ProcessMediaSample(DWORD dwStreamFlags, LONGLONG llTimestamp, IMFSample* pSample)
                : StreamFlags(dwStreamFlags)
                , Timestamp(llTimestamp)
                , Sample(pSample)
            {
            }

            DWORD StreamFlags;
            LONGLONG Timestamp;
            ComPtr<IMFSample> Sample;
        };

        class FormatChangedEventArgsImpl
            : public RuntimeClass<RuntimeClassFlags<WinRtClassicComMix>
            , ABI::MixedRemoteViewCompositor::Media::IFormatChangedEventArgs
            , FtmBase >
        {
            InspectableClass(RuntimeClass_MixedRemoteViewCompositor_Media_FormatChangedEventArgs, BaseTrust)
        public:
            FormatChangedEventArgsImpl(UINT32 width, UINT32 height)
                : Width(width)
                , Height(height)
            {
            }

            IFACEMETHOD(get_Width)(
                _Out_ UINT32 *width)
            {
                NULL_CHK(width);

                *width = Width;

                return S_OK;
            }

            IFACEMETHOD(get_Height)(
                _Out_ UINT32 *height)
            {
                NULL_CHK(height);

                *height = Height;

                return S_OK;
            }

            UINT32 Width;
            UINT32 Height;
        };

        class SampleUpdatedArgsImpl
            : public RuntimeClass<RuntimeClassFlags<WinRtClassicComMix>
            , ABI::MixedRemoteViewCompositor::Media::ISampleUpdatedEventArgs
            , FtmBase >
        {
            InspectableClass(RuntimeClass_MixedRemoteViewCompositor_Media_SampleUpdatedEventArgs, BaseTrust)

        public:

            SampleUpdatedArgsImpl(
                UINT32 width, UINT32 height, UINT64 timestamp,
                ABI::Windows::Foundation::Numerics::Matrix4x4 cameraView,
                ABI::Windows::Foundation::Numerics::Matrix4x4 cameraProj,
                ID3D11Texture2D* pTexture)
                : Width(width)
                , Height(height)
                , Timestamp(timestamp)
                , CameraView(cameraView)
                , CameraProj(cameraProj)
                , _videoTexture(pTexture)
            { }

            ~SampleUpdatedArgsImpl() {}

            IFACEMETHOD(get_Width)(
                _Out_ UINT32 *width) 
            {
                NULL_CHK(width);

                *width = Width;

                return S_OK;
            }

            IFACEMETHOD(get_Height)(
                _Out_ UINT32 *height)
            {
                NULL_CHK(height);

                *height = Height;

                return S_OK;
            }


            IFACEMETHOD(get_Timestamp)(
                _Out_ UINT64 *timestamp)
            {
                NULL_CHK(timestamp);

                *timestamp = Timestamp;

                return S_OK;
            }

            IFACEMETHOD(get_Transforms)(
                _Out_ ABI::MixedRemoteViewCompositor::MediaSampleTransforms *transform)
            {
                NULL_CHK(transform);

                (*transform).cameraProjectionTransform = CameraProj;
                (*transform).cameraViewTransform = CameraView;

                return S_OK;
            }

            IFACEMETHOD(get_VideoTexture)(
                _Out_ IInspectable** ppvTexture)
            {
                NULL_CHK(ppvTexture);

                NULL_CHK_HR(_videoTexture, E_NOT_SET);

                return _videoTexture.CopyTo(ppvTexture);
            }

            UINT32 Width;
            UINT32 Height;
            UINT64 Timestamp;
            ABI::Windows::Foundation::Numerics::Matrix4x4 CameraView;
            ABI::Windows::Foundation::Numerics::Matrix4x4 CameraProj;
            ComPtr<IUnknown> _videoTexture;
        };

        class PlaybackEngineImpl
            : public RuntimeClass
            < RuntimeClassFlags<RuntimeClassType::WinRtClassicComMix>
            , ABI::MixedRemoteViewCompositor::Media::IPlaybackEngine
            , ABI::MixedRemoteViewCompositor::Plugin::IModule
            , ABI::Windows::Foundation::IClosable
            , IMFSourceReaderCallback
            , ABI::Windows::Foundation::IAsyncAction
            , AsyncBase<ABI::Windows::Foundation::IAsyncActionCompletedHandler>
            , FtmBase >
        {
            InspectableClass(RuntimeClass_MixedRemoteViewCompositor_Media_PlaybackEngine, BaseTrust)

        public:
            PlaybackEngineImpl();
            ~PlaybackEngineImpl();

            HRESULT RuntimeClassInitialize(
                _In_ ABI::MixedRemoteViewCompositor::Network::IConnection *connection);

            // IModule
            IFACEMETHOD(get_IsInitialized)(
                _Out_ boolean* pIsInitialzed);
            IFACEMETHOD(Uninitialize)(void);

            // IClosable
            IFACEMETHOD(Close)(void);

            // IAsyncAction
            IFACEMETHOD(put_Completed)(
                _In_ ABI::Windows::Foundation::IAsyncActionCompletedHandler *handler) 
            {
                return PutOnComplete(handler);
            }
            IFACEMETHOD(get_Completed)(
                _Out_ ABI::Windows::Foundation::IAsyncActionCompletedHandler** handler) 
            {
                return GetOnComplete(handler);
            }
            IFACEMETHOD(GetResults)(void) 
            { 
                return S_OK; 
            }

            // AsyncBase
            virtual HRESULT OnStart(void) { return S_OK; }
            virtual void OnClose(void) {};
            virtual void OnCancel(void) {};

            // Inherited via IMFSourceReaderCallback
            IFACEMETHOD(OnReadSample)(
                _In_ HRESULT hrStatus, 
                _In_ DWORD dwStreamIndex, 
                _In_ DWORD dwStreamFlags, 
                _In_ LONGLONG llTimestamp, 
                _In_ IMFSample* pSample);
            IFACEMETHOD(OnFlush)(
                _In_ DWORD dwStreamIndex);
            IFACEMETHOD(OnEvent)(
                _In_ DWORD dwStreamIndex, 
                _In_ IMFMediaEvent* pEvent);

            // IPlaybackEgnine 
            IFACEMETHOD(add_Closed)(
                _In_ Plugin::IClosedEventHandler *eventHandler,
                _In_ EventRegistrationToken* token);
            IFACEMETHOD(remove_Closed)(
                _In_ EventRegistrationToken token);

            IFACEMETHOD(add_FormatChanged)(
                _In_ ABI::MixedRemoteViewCompositor::Media::IFormatChangedEventHandler *eventHandler,
                _Out_ EventRegistrationToken* token);
            IFACEMETHOD(remove_FormatChanged)(
                _In_ EventRegistrationToken token);

            IFACEMETHOD(add_SampleUpdated)(
                _In_ ABI::MixedRemoteViewCompositor::Media::ISampleUpdatedEventHandler *eventhandler,
                _Out_ EventRegistrationToken* token);
            IFACEMETHOD(remove_SampleUpdated)(
                _In_ EventRegistrationToken token);

            IFACEMETHOD(InitializeAsync)(
                _Out_ ABI::Windows::Foundation::IAsyncAction** ppOperation);

            IFACEMETHOD(StartPlayback)();

            IFACEMETHOD(StopPlayback)();

            // PlaybackEngineImpl
            HRESULT GetFrameData(
                _In_ MixedRemoteViewCompositor::Plugin::MediaSampleArgs* args);

        protected:
            HRESULT CompleteAsyncAction(
                _In_ HRESULT hResult);
            HRESULT RequestNextSampleAsync(
                _In_ DWORD streamId = MF_SOURCE_READER_FIRST_VIDEO_STREAM);

        private:
            Wrappers::CriticalSection _lock;

            bool _isInitialized;
            bool _playbackStarted;
            bool _waitForFirstVideoSample;

            EventSource<ABI::MixedRemoteViewCompositor::Plugin::IClosedEventHandler> _evtClosed;
            EventSource<ABI::MixedRemoteViewCompositor::Media::IFormatChangedEventHandler> _evtFormatChanged;
            EventSource<ABI::MixedRemoteViewCompositor::Media::ISampleUpdatedEventHandler> _evtSampleUpdated;

            ComPtr<IMFSourceReader> _sourceReader;
            UINT32 _videoWidth, _videoHeight;

            ComPtr<ABI::MixedRemoteViewCompositor::Plugin::IDirectXManager> _dxManager;

            // DX11 texture
            ComPtr<IMFDXGIDeviceManager> _dxgiManager;
            ComPtr<ID3D11VideoDevice> _videoDevice;
            ComPtr<ID3D11VideoContext> _videoContext;

            ProcessMediaSample _latestMediaSample[2];
        };

        class PlaybackEngineStaticsImpl
            : public ActivationFactory
            < ABI::MixedRemoteViewCompositor::Media::IPlaybackEngineStatics
            , FtmBase >
        {
            InspectableClassStatic(RuntimeClass_MixedRemoteViewCompositor_Media_PlaybackEngine, BaseTrust);
        public:
            IFACEMETHOD(Create)(
                _In_ ABI::MixedRemoteViewCompositor::Network::IConnection *connection,
                _Out_ ABI::MixedRemoteViewCompositor::Media::IPlaybackEngine** playbackEngine);
        };
    }
}
