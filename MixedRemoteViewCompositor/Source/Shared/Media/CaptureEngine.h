// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

namespace MixedRemoteViewCompositor 
{
    namespace Media 
    {
        class CaptureEngineImpl
            : public RuntimeClass
            < RuntimeClassFlags<RuntimeClassType::WinRtClassicComMix>
            , ABI::MixedRemoteViewCompositor::Plugin::IModule
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

            // ICaptureEngine
            IFACEMETHOD(Init)(
                _In_ boolean enableAudio,
				_In_ IConnection *connection);

            IFACEMETHOD(Shutdown)();

			IFACEMETHOD(WriteFrame)();
				// TODO: add in framebuffer pointer*
				//_In_ const LONGLONG& rtStart);
        private:
            Wrappers::CriticalSection _lock;

            boolean _isInitialized;
            boolean _enableAudio;
            boolean _captureStarted;
			
			ComPtr<IMediaEncodingProfile> _spMediaEncodingProfile;
            ComPtr<NetworkMediaSinkImpl> _spNetworkMediaSink;
			ComPtr<IMFSinkWriter> _spSinkWriter;
			DWORD _sinkWriterStream;

			LONGLONG rtStart = 0;

			// Buffer to hold the video frame data.
			const UINT32 VIDEO_PELS = 921600; // 1280 x 720
			DWORD _videoFrameBuffer[921600]; // TODO: remove hardcode
			//const UINT32 VIDEO_WIDTH = 640;
			//const UINT32 VIDEO_HEIGHT = 480;
			const UINT32 VIDEO_FPS = 30;
			const UINT64 VIDEO_FRAME_DURATION = 10 * 1000 * 1000 / VIDEO_FPS; // TODO: Needed???
			const UINT32 VIDEO_BIT_RATE = 800000; // TODO: How to modulate?
			//const GUID   VIDEO_ENCODING_FORMAT = MFVideoFormat_H265; //MFVideoFormat_H264
			const GUID   VIDEO_INPUT_FORMAT = MFVideoFormat_RGB32;
			//const UINT32 VIDEO_PELS = VIDEO_WIDTH * VIDEO_HEIGHT;
        };

		/*
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
		*/

		class CaptureEngineStaticsImpl
			: public ActivationFactory
			< ABI::MixedRemoteViewCompositor::Media::ICaptureEngineStatics
			, FtmBase >
		{
			InspectableClassStatic(RuntimeClass_MixedRemoteViewCompositor_Media_CaptureEngine, BaseTrust);

		public:
			// ICaptureEngineStatics
			STDMETHODIMP Create(
				_COM_Outptr_result_maybenull_ ABI::MixedRemoteViewCompositor::Media::ICaptureEngine** ppCaptureEngine) override
			{
				ComPtr<CaptureEngineImpl> spCaptureEngine;
				IFR(Microsoft::WRL::MakeAndInitialize<CaptureEngineImpl>(&spCaptureEngine));

				NULL_CHK_HR(spCaptureEngine, E_OUTOFMEMORY);

				return spCaptureEngine.CopyTo(ppCaptureEngine);
			}
		};
    }
}
