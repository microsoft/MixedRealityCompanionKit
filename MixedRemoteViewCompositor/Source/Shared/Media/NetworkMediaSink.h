// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

namespace MixedRemoteViewCompositor
{
    namespace Media
    {
        typedef ComPtrList<IMFStreamSink> StreamContainer;

        class NetworkMediaSinkImpl
            : public RuntimeClass
            < RuntimeClassFlags<RuntimeClassType::WinRtClassicComMix>
            , ABI::Windows::Media::IMediaExtension
            , ABI::MixedRemoteViewCompositor::Media::INetworkMediaSink
            , IMFMediaSink
            , IMFClockStateSink
            , FtmBase >
        {
            InspectableClass(RuntimeClass_MixedRemoteViewCompositor_Media_NetworkMediaSink, BaseTrust)

        public:
            NetworkMediaSinkImpl();
            virtual ~NetworkMediaSinkImpl();

            HRESULT RuntimeClassInitialize(
                _In_ ABI::Windows::Media::MediaProperties::IAudioEncodingProperties* audioEncodingProperties,
                _In_ ABI::Windows::Media::MediaProperties::IVideoEncodingProperties* videoEncodingProperties,
                _In_ ABI::MixedRemoteViewCompositor::Network::IConnection* connection);

            // IMediaExtension
            IFACEMETHOD(SetProperties)(
                _In_ ABI::Windows::Foundation::Collections::IPropertySet* configuration);

            // IMFMediaSink methods
            IFACEMETHOD(GetCharacteristics)(
                _Out_ DWORD* pdwCharacteristics);
            IFACEMETHOD(AddStreamSink)(
                _In_ DWORD dwStreamSinkIdentifier,
                _In_opt_ IMFMediaType* pMediaType,
                _COM_Outptr_opt_ IMFStreamSink** ppStreamSink);
            IFACEMETHOD(RemoveStreamSink)(
                _In_ DWORD dwStreamSinkIdentifier);
            IFACEMETHOD(GetStreamSinkCount)(
                _Out_ DWORD* pcStreamSinkCount);
            IFACEMETHOD(GetStreamSinkByIndex)(
                _In_ DWORD dwIndex, 
                _COM_Outptr_opt_ IMFStreamSink** ppStreamSink);
            IFACEMETHOD(GetStreamSinkById)(
                _In_ DWORD dwIdentifier, 
                _COM_Outptr_opt_ IMFStreamSink** ppStreamSink);
            IFACEMETHOD(SetPresentationClock)(
                _In_opt_ IMFPresentationClock* pPresentationClock);
            IFACEMETHOD(GetPresentationClock)(
                _COM_Outptr_opt_ IMFPresentationClock** ppPresentationClock);
            IFACEMETHOD(Shutdown)();

            // IMFClockStateSink methods
            IFACEMETHOD(OnClockStart)(
                _In_ MFTIME hnsSystemTime, 
                _In_ LONGLONG llClockStartOffset);
            IFACEMETHOD(OnClockStop)(
                _In_ MFTIME hnsSystemTime);
            IFACEMETHOD(OnClockPause)(
                _In_ MFTIME hnsSystemTime);
            IFACEMETHOD(OnClockRestart)(
                _In_ MFTIME hnsSystemTime);
            IFACEMETHOD(OnClockSetRate)(
                _In_ MFTIME hnsSystemTime, 
                _In_ float flRate);

            // INetworkMediaSink
            IFACEMETHOD(get_SpatialCoordinateSystem)(
                _COM_Outptr_opt_ ABI::Windows::Perception::Spatial::ISpatialCoordinateSystem** coordinateSystem);
            IFACEMETHOD(put_SpatialCoordinateSystem)(
                _In_ ABI::Windows::Perception::Spatial::ISpatialCoordinateSystem *coordinateSystem);

            IFACEMETHOD(add_Closed)(
                _In_ Plugin::IClosedEventHandler *eventHandler,
                _In_ EventRegistrationToken *token);
            IFACEMETHOD(remove_Closed)(
                _In_ EventRegistrationToken token);

            IFACEMETHOD(add_FormatChanged)(
                _In_ ABI::MixedRemoteViewCompositor::Media::IFormatChangedEventHandler *eventHandler,
                _Out_ EventRegistrationToken *token);
            IFACEMETHOD(remove_FormatChanged)(
                _In_ EventRegistrationToken token);

            IFACEMETHOD(add_SampleUpdated)(
                _In_ ABI::MixedRemoteViewCompositor::Media::ISampleUpdatedEventHandler *eventhandler,
                _Out_ EventRegistrationToken *token);
            IFACEMETHOD(remove_SampleUpdated)(
                _In_ EventRegistrationToken token);

            // INetworkMediaSinkInternal
            IFACEMETHOD(SendCaptureReady)(void);
            IFACEMETHOD(SendCaptureStarted)(void);
            IFACEMETHOD(SendCaptureStopped)(void);
            IFACEMETHOD(SendDescription)(void);
            IFACEMETHOD(HandleError)(
                _In_ HRESULT hr);
            IFACEMETHOD(SetMediaStreamProperties)(
                _In_ ABI::Windows::Media::Capture::MediaStreamType MediaStreamType,
                _In_ ABI::Windows::Media::MediaProperties::IMediaEncodingProperties *mediaEncodingProperties);

            IFACEMETHOD(CheckShutdown)()
            {
                NULL_CHK_HR(_spConnection, MF_E_SHUTDOWN);
                
                IFR(_cStreamsEnded == 0 ? S_OK : MF_E_SHUTDOWN);

                return S_OK;
            }

            IFACEMETHOD(OnEndOfStream) (
                _In_ DWORD dwStreamId)
            {
                auto lock = _lock.Lock();

                _cStreamsEnded++;

                return S_OK;
            }

        private:
            HRESULT FormatChanged(_In_ IMFMediaType* pMediaType);
            HRESULT SampleUpdated(_In_ IMFSample* pSample);

        private:
            Wrappers::CriticalSection _lock;

            LONGLONG _llStartTime;
            Microsoft::WRL::ComPtr<IMFPresentationClock> _presentationClock;  // Presentation clock.

            StreamContainer _streams;

            long _cStreamsEnded;

            ComPtr<ABI::MixedRemoteViewCompositor::Network::IConnection> _spConnection;
            EventRegistrationToken _bundleReceivedEventToken;

            ComPtr<ABI::Windows::Perception::Spatial::ISpatialCoordinateSystem> _spUnitySpatialCoordinateSystem;

            EventSource<Plugin::IClosedEventHandler> _evtClosed;
            EventSource<ABI::MixedRemoteViewCompositor::Media::IFormatChangedEventHandler> _evtFormatChanged;
            EventSource<ABI::MixedRemoteViewCompositor::Media::ISampleUpdatedEventHandler> _evtSampleUpdated;
        };
    }
}
