// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "RealtimeStreaming.g.h"

#include <mfapi.h>
#include <mfidl.h>
#include <mferror.h>

namespace winrt::RealtimeStreaming::Media::implementation
{
        typedef ComPtrList<IMFStreamSink> StreamContainer;

        struct NetworkMediaSink : NetworkMediaSinkT<NetworkMediaSink, IMFStreamSink, IMFClockStateSink, IMFMediaTypeHandler>
        {
        public:
            NetworkMediaSink(
                _In_ ABI::Windows::Media::MediaProperties::IAudioEncodingProperties* audioEncodingProperties,
                _In_ ABI::Windows::Media::MediaProperties::IVideoEncodingProperties* videoEncodingProperties,
                _In_ ABI::RealtimeStreaming::Network::IConnection* connection);

            virtual ~NetworkMediaSink();

            // IMediaExtension
            void SetProperties(Windows::Foundation::Collections::IPropertySet const& configuration);

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

            // NetworkMediaSink
            IFACEMETHOD(add_Closed)(
                _In_ Plugin::IClosedEventHandler *eventHandler,
                _In_ EventRegistrationToken *token);
            IFACEMETHOD(remove_Closed)(
                _In_ EventRegistrationToken token);

        private:
            HRESULT SendCaptureReady();
            HRESULT SendDescription();
            HRESULT HandleError(_In_ HRESULT hr);

            HRESULT SetMediaStreamProperties(
                _In_ Windows::Media::Capture::MediaStreamType mediaStreamType,
                _In_ Windows::Media::MediaProperties::IMediaEncodingProperties mediaEncodingProperties);

            HRESULT CheckShutdown()
            {
                NULL_CHK_HR(m_connection, MF_E_SHUTDOWN);

                check_hresult(_cStreamsEnded == 0 ? S_OK : MF_E_SHUTDOWN);

                return S_OK;
            }

            HRESULT OnEndOfStream(
                _In_ DWORD dwStreamId)
            {
                auto lock = _lock.Lock();

                _cStreamsEnded++;

                return S_OK;
            }

        private:
            Wrappers::CriticalSection _lock;

            LONGLONG _llStartTime;
            com_ptr<IMFPresentationClock> _presentationClock;  // Presentation clock.

            StreamContainer _streams;

            long _cStreamsEnded;

            RealtimeStreaming::Network::Connection m_connection;
            winrt::event_token m_bundleReceivedEventToken;

            winrt::event<Windows::Foundation::EventHandler> m_evtClosed;
        };
}

namespace winrt::RealtimeStreaming::Media::factory_implementation
{
    struct NetworkMediaSink : NetworkMediaSinkT<NetworkMediaSink, implementation::NetworkMediaSink>
    {
    };
}