// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

namespace MixedRemoteViewCompositor 
{
    namespace Media 
    {
		DECLARE_INTERFACE_IID_(IRealtimeServerWriter, IUnknown, "a43cdc39-c524-45b4-9aba-f79b1b897dfd")
		{
			STDMETHOD(WriteFrame)(_In_ IMFMediaBuffer* pBuffer) PURE;
			STDMETHOD(GetCurrentResolution)(_Out_ UINT32* pWidth, _Out_ UINT32* pHeight) PURE;
		};

        class RealtimeServerImpl
            : public RuntimeClass
            < RuntimeClassFlags<RuntimeClassType::WinRtClassicComMix>
            , ABI::MixedRemoteViewCompositor::Plugin::IModule
            , ABI::MixedRemoteViewCompositor::Media::IRealtimeServer
			, IRealtimeServerWriter
            , FtmBase >
        {
            InspectableClass(RuntimeClass_MixedRemoteViewCompositor_Media_RealtimeServer, BaseTrust)

        public:
            RealtimeServerImpl();
            ~RealtimeServerImpl();

            HRESULT RuntimeClassInitialize(
					 _In_ IConnection* pConnection);

            // IModule
            IFACEMETHOD(get_IsInitialized)(
                _Out_ boolean* pIsInitialzed);
            IFACEMETHOD(Uninitialize)(void);

            // IRealtimeServer
            IFACEMETHOD(Shutdown)();

			// IRealtimeServerWriter
			IFACEMETHOD(WriteFrame)(
				_In_ IMFMediaBuffer* pMediaBuffer);

			IFACEMETHOD(GetCurrentResolution)(
				_Out_ UINT32* pWidth,
				_Out_ UINT32* pHeight);

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

		class RealtimeServerStaticsImpl
			: public ActivationFactory
			< ABI::MixedRemoteViewCompositor::Media::IRealtimeServerStatics
			, FtmBase >
		{
			InspectableClassStatic(RuntimeClass_MixedRemoteViewCompositor_Media_RealtimeServer, BaseTrust);

		public:
			// IRealtimeServerStatics
			STDMETHODIMP Create(
				 _In_ ABI::MixedRemoteViewCompositor::Network::IConnection* pConnection,
				_COM_Outptr_result_maybenull_ ABI::MixedRemoteViewCompositor::Media::IRealtimeServer** ppRealtimeServer) override
			{
				ComPtr<RealtimeServerImpl> spRealtimeServer;
				IFR(Microsoft::WRL::MakeAndInitialize<RealtimeServerImpl>(&spRealtimeServer, pConnection));

				NULL_CHK_HR(spRealtimeServer, E_OUTOFMEMORY);

				return spRealtimeServer.CopyTo(ppRealtimeServer);
			}
		};
    }
}
