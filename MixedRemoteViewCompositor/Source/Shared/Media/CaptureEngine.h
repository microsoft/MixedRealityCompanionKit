// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

namespace MixedRemoteViewCompositor 
{
    namespace Media 
    {

        typedef std::unordered_set<UINT64> ResolutionList;

        class CaptureEngineImpl
            : public RuntimeClass
            < RuntimeClassFlags<RuntimeClassType::WinRtClassicComMix>
            , ABI::MixedRemoteViewCompositor::Plugin::IModule
            , ABI::Windows::Foundation::IClosable
            , ABI::MixedRemoteViewCompositor::Media::ICaptureEngine
            , FtmBase >
        {
            InspectableClass(RuntimeClass_MixedRemoteViewCompositor_Media_CaptureEngine, BaseTrust)

        public:
            CaptureEngineImpl();
            ~CaptureEngineImpl();

            HRESULT RuntimeClassInitialize();

            // IModule
            IFACEMETHOD(get_IsInitialized)(
                _Out_ boolean* pIsInitialzed);
            IFACEMETHOD(Uninitialize)(void);

            // IClosable
            IFACEMETHOD(Close)(void);

            // ICaptureEngine
            IFACEMETHOD(add_Closed)(
                _In_ Plugin::IClosedEventHandler *eventHandler,
                _In_ EventRegistrationToken* token);
            IFACEMETHOD(remove_Closed)(
                _In_ EventRegistrationToken token);

            IFACEMETHOD(put_SpatialCoordinateSystem)(
                _In_ ABI::Windows::Perception::Spatial::ISpatialCoordinateSystem *coordinateSystem);

            IFACEMETHOD(get_SpatialCoordinateSystem)(
                _Outptr_opt_result_maybenull_ ABI::Windows::Perception::Spatial::ISpatialCoordinateSystem** coordinateSystem);

            IFACEMETHOD(InitAsync)(
                _In_ boolean enableAudio,
                _Out_ ABI::Windows::Foundation::IAsyncAction** action);

            IFACEMETHOD(StartAsync)(
                _In_ boolean enableMrc,
                _In_ IConnection *connection,
                _Out_ ABI::Windows::Foundation::IAsyncAction** action);

            IFACEMETHOD(StopAsync)(
                _Out_ ABI::Windows::Foundation::IAsyncAction** action);

        protected:
            // media capture callbacks
            HRESULT OnMediaCaptureFailed(
                _In_ ABI::Windows::Media::Capture::IMediaCapture *sender,
                _In_ ABI::Windows::Media::Capture::IMediaCaptureFailedEventArgs *failedArgs);
            HRESULT OnRecordLimitationExceeded(
                _In_ ABI::Windows::Media::Capture::IMediaCapture *sender);

        private:
            Wrappers::CriticalSection _lock;

            boolean _isInitialized;
            boolean _enableAudio;
            boolean _enableMrc;
            boolean _videoEffectAdded;
            boolean _audioEffectAdded;
            boolean _captureStarted;

            ResolutionList _videoResolutions;

            ComPtr<ABI::Windows::Media::Capture::IMediaCapture> _mediaCapture;
            EventRegistrationToken _failedEventToken;
            EventRegistrationToken _recordLimitExceededEventToken;

            EventSource<ABI::MixedRemoteViewCompositor::Plugin::IClosedEventHandler> _evtClosed;

            ComPtr<NetworkMediaSinkImpl> _networkMediaSink;
            ComPtr<ABI::Windows::Perception::Spatial::ISpatialCoordinateSystem> _spSpatialCoordinateSystem;
        };

        class CreateCaptureEngineAsync
            : public RuntimeClass<RuntimeClassFlags<WinRtClassicComMix>
            , IAsyncOperation<ABI::MixedRemoteViewCompositor::Media::CaptureEngine*>
            , AsyncBase<IAsyncOperationCompletedHandler<ABI::MixedRemoteViewCompositor::Media::CaptureEngine*>>
            , FtmBase >
        {
            InspectableClass(L"MixedRemoteViewCompositor.Media.CreateCaptureEngineAsync", BaseTrust)

        public:
            CreateCaptureEngineAsync() : _captureEngineResult(nullptr) { Start(); }
            ~CreateCaptureEngineAsync() { _captureEngineResult.Reset(); }

            // call this when ready to report the capture engine is complete
            HRESULT SetCaptureEngineComplete(HRESULT hr, ABI::MixedRemoteViewCompositor::Media::ICaptureEngine *captureEngine)
            {
                ComPtr<ABI::MixedRemoteViewCompositor::Media::ICaptureEngine> spCaptureEngine(captureEngine);

                IFC(hr);

                IFC(spCaptureEngine.As(&_captureEngineResult));

            done:
                if (FAILED(hr))
                {
                    TryTransitionToError(hr);
                }

                return FireCompletion();
            }

            // Inherited via IAsyncOperation
            IFACEMETHOD(put_Completed)(
                _In_ IAsyncOperationCompletedHandler<ABI::MixedRemoteViewCompositor::Media::CaptureEngine*> *handler)
            {
                return PutOnComplete(handler);
            }

            IFACEMETHOD(get_Completed)(
                _Out_ IAsyncOperationCompletedHandler<ABI::MixedRemoteViewCompositor::Media::CaptureEngine*>** handler)
            {
                return GetOnComplete(handler);
            }

            IFACEMETHOD(GetResults)(
                _Out_ ABI::MixedRemoteViewCompositor::Media::ICaptureEngine** captureEngine)
            {
                IFR(AsyncBase::CheckValidStateForResultsCall());

                NULL_CHK_HR(_captureEngineResult, E_NOT_SET);

                return _captureEngineResult.CopyTo(captureEngine);
            }

            virtual HRESULT OnStart(void) override { return S_OK; }
            virtual void OnClose(void) override { }
            virtual void OnCancel(void) override { }

        private:
            ComPtr<ABI::MixedRemoteViewCompositor::Media::ICaptureEngine> _captureEngineResult;
        };

        class CaptureEngineStaticsImpl
            : public ActivationFactory
            < ABI::MixedRemoteViewCompositor::Media::ICaptureEngineStatics
            , FtmBase >
        {
            InspectableClassStatic(RuntimeClass_MixedRemoteViewCompositor_Media_CaptureEngine, BaseTrust);

        public:
            IFACEMETHOD(CreateAsync)(
                _In_ boolean enableAudio,
                _Out_ IAsyncOperation<ABI::MixedRemoteViewCompositor::Media::CaptureEngine*>** asyncOperation);
        };
    }
}
