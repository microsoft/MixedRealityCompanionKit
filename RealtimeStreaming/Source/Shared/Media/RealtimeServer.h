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
                _In_ IConnection* pConnection,
                _In_ GUID inputMediaType,
                _In_ IMediaEncodingProfile* pMediaEncodingProperties);

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

            ComPtr<IMediaEncodingProfile> _spMediaEncodingProfile;
            ComPtr<NetworkMediaSinkImpl> _spNetworkMediaSink;
            ComPtr<IMFSinkWriter> _spSinkWriter;
            DWORD m_sinkWriterStream;

            GUID m_mediaInputFormat;

            LONGLONG rtStart = 0;

            const UINT32 VIDEO_FPS = 30;
            const UINT64 VIDEO_FRAME_DURATION = 10 * 1000 * 1000 / VIDEO_FPS;
            //const UINT32 VIDEO_BIT_RATE = 800000;
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
                _In_ GUID inputMediaType,
                _In_ ABI::Windows::Media::MediaProperties::IMediaEncodingProfile *pMediaEncodingProfile,
                _COM_Outptr_result_maybenull_ ABI::MixedRemoteViewCompositor::Media::IRealtimeServer** ppRealtimeServer) override
            {
                ComPtr<RealtimeServerImpl> spRealtimeServer;
                IFR(Microsoft::WRL::MakeAndInitialize<RealtimeServerImpl>(&spRealtimeServer, 
                    pConnection,
                    inputMediaType,
                    pMediaEncodingProfile));

                NULL_CHK_HR(spRealtimeServer, E_OUTOFMEMORY);

                return spRealtimeServer.CopyTo(ppRealtimeServer);
            }
		};
    }
}
