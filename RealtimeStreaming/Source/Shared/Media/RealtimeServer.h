// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

namespace RealtimeStreaming 
{
    namespace Media 
    {
        class RealtimeServerImpl
            : public RuntimeClass
            < RuntimeClassFlags<RuntimeClassType::WinRtClassicComMix>
            , ABI::RealtimeStreaming::Plugin::IModule
            , ABI::RealtimeStreaming::Media::IRealtimeServer
            , FtmBase >
        {
            InspectableClass(RuntimeClass_RealtimeStreaming_Media_RealtimeServer, BaseTrust)

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
            
            IFACEMETHOD(WriteFrame)(
                _In_ UINT32 bufferSize,
                __in_ecount(bufferSize) BYTE* pBuffer);

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
        };

        class RealtimeServerStaticsImpl
	        : public ActivationFactory
	        < ABI::RealtimeStreaming::Media::IRealtimeServerStatics
	        , FtmBase >
        {
            InspectableClassStatic(RuntimeClass_RealtimeStreaming_Media_RealtimeServer, BaseTrust);

        public:
            // IRealtimeServerStatics
            STDMETHODIMP Create(
                _In_ ABI::RealtimeStreaming::Network::IConnection* pConnection,
                _In_ GUID inputMediaType,
                _In_ ABI::Windows::Media::MediaProperties::IMediaEncodingProfile *pMediaEncodingProfile,
                _COM_Outptr_result_maybenull_ ABI::RealtimeStreaming::Media::IRealtimeServer** ppRealtimeServer) override
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
