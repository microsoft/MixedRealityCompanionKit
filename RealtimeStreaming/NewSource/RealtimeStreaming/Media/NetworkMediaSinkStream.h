// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "Media.NetworkMediaSinkStream.g.h"

#include "Common/LinkList.h"

#include <mfapi.h>
#include <mfidl.h>
#include <mferror.h>

namespace winrt::RealtimeStreaming::Media::implementation
{
        struct NetworkMediaSinkStream : NetworkMediaSinkStreamT<NetworkMediaSinkStream, IMFStreamSink, IMFMediaEventGenerator, IMFMediaTypeHandler>
        {
            
            // AsyncOperation:
            // Used to queue asynchronous operations. When we call MFPutWorkItem, we use this
            // object for the callback state (pState). Then, when the callback is invoked,
            // we can use the object to determine which asynchronous operation to perform.
            class AsyncOperation 
                : public IUnknown
            {
            public:
                AsyncOperation(SinkStreamOperation op);

                SinkStreamOperation _op;   // The operation to perform.

                // IUnknown methods.
                STDMETHODIMP QueryInterface(_In_ REFIID iid, _COM_Outptr_ void** ppv);
                STDMETHODIMP_(ULONG) AddRef();
                STDMETHODIMP_(ULONG) Release();

            private:
                long    _cRef;
                virtual ~AsyncOperation();
            };
            

        public:
            NetworkMediaSinkStream(_In_ DWORD streamId,
                _In_ RealtimeStreaming::Network::Connection connection,
                _In_ RealtimeStreaming::Media::NetworkMediaSink parentMediaSink);
            virtual ~NetworkMediaSinkStream();

            // IMFMediaEventGenerator
            IFACEMETHOD(GetEvent)(
                _In_ DWORD dwFlags, 
                _COM_Outptr_opt_ IMFMediaEvent** ppEvent);
            IFACEMETHOD(BeginGetEvent)(
                _In_ IMFAsyncCallback* pCallback, 
                _In_ IUnknown* pUnkState);
            IFACEMETHOD(EndGetEvent) (
                _In_ IMFAsyncResult* pResult, 
                _Outptr_ IMFMediaEvent** ppEvent);
            IFACEMETHOD(QueueEvent) (
                _In_ MediaEventType mediaEventType, 
                _In_ REFGUID guidExtendedType, 
                _In_ HRESULT hrStatus, 
                _In_opt_ PROPVARIANT const* pvValue);

            // IMFStreamSink
            IFACEMETHOD(GetMediaSink) (
                _COM_Outptr_opt_ IMFMediaSink** ppMediaSink);
            IFACEMETHOD(GetIdentifier) (
                _Out_ DWORD* pdwIdentifier);
            IFACEMETHOD(GetMediaTypeHandler) (
                _COM_Outptr_opt_ IMFMediaTypeHandler** ppTypeHandler);
            IFACEMETHOD(ProcessSample) (
                _In_opt_ IMFSample* pSample);
            IFACEMETHOD(PlaceMarker) (
                _In_ MFSTREAMSINK_MARKER_TYPE eMarkerType,
                _In_ PROPVARIANT const* pvarMarkerValue,
                _In_ PROPVARIANT const* pvarContextValue);
            IFACEMETHOD(Flush)();

            // IMFMediaTypeHandler
            IFACEMETHOD(IsMediaTypeSupported) (
                _In_ IMFMediaType* pMediaType, 
                _COM_Outptr_opt_result_maybenull_ IMFMediaType** ppMediaType);
            IFACEMETHOD(GetMediaTypeCount) (
                _Out_ DWORD* pdwTypeCount);
            IFACEMETHOD(GetMediaTypeByIndex) (
                _In_ DWORD dwIndex, _COM_Outptr_ IMFMediaType** ppMediaType);
            IFACEMETHOD(SetCurrentMediaType) (
                _In_ IMFMediaType* pMediaType);
            IFACEMETHOD(GetCurrentMediaType) (
                _COM_Outptr_ IMFMediaType** ppMediaType);
            IFACEMETHOD(GetMajorType) (
                _Out_ GUID* pguidMajorType);

            void SetProperties(Windows::Foundation::Collections::IPropertySet const& configuration) {};

            HRESULT Start(_In_ MFTIME start);
            HRESULT Restart();
            HRESULT Stop();
            HRESULT Pause();
            HRESULT Shutdown();

            HRESULT ConnectedFunc(_In_ bool fConnected, _In_ LONGLONG llCurrentTime);
            HRESULT CheckShutdown() const
            {
                if (m_state == SinkStreamState::Stopped)
                {
                    return MF_E_SHUTDOWN;
                }
                else
                {
                    return S_OK;
                }
            }

            bool IsVideo() const { return m_fIsVideo; }

            Network::DataBuffer FillStreamDescription(_Inout_ Common::MediaTypeDescription* pStreamDescription);

        private:
            HRESULT ValidateOperation(
                _In_ SinkStreamOperation op);

            HRESULT QueueAsyncOperation(
                _In_ SinkStreamOperation op);

            HRESULT OnDispatchWorkItem(
                _In_ IMFAsyncResult* pAsyncResult);

            HRESULT DispatchProcessedSample(
                _In_ AsyncOperation* pOp);

            HRESULT DropSamplesFromQueue(
                _Out_ boolean* fRequestNewSample);
            HRESULT SendSampleFromQueue(
                _Out_ boolean* fRequestNewSample);
            HRESULT ProcessSamplesFromQueue(
                _In_ boolean fFlush,
                _Out_ boolean* fRequestNewSample);
            HRESULT ProcessFormatChange(_In_ IMFMediaType* pMediaType);

            Network::DataBundle PrepareSample(
                _In_ IMFSample* pSample,
                _In_ bool fForce);
            Network::DataBundle PrepareStreamTick(
                _In_ IMFAttributes* pAtrributes);
            Network::DataBundle PrepareFormatChange(
                _In_ IMFMediaType* pMediaType);
            HRESULT ProcessCameraData(
                _In_ IMFSample* pSample,
                _Inout_ MediaSampleHeader* pSampleHeader);

            HRESULT HandleError(HRESULT hr)
            {
                if (!m_isShutdown && FAILED(hr))
                {
                    QueueEvent(MEError, GUID_NULL, hr, nullptr);
                }

                return hr;
            }

        private:
            //Wrappers::CriticalSection _lock;
            slim_mutex m_lock;

            DWORD m_dwStreamId;          // streamId
            bool m_fIsVideo;             // for video type streams, we have special data to send
            bool m_fGetFirstSampleTime;  // wait for the first keyframe

            SinkStreamState m_state;         // current state of the sink
            bool m_isShutdown;           // Flag to indicate if Shutdown() method was called.
            bool m_isPlayerConnected;    // Flag to indicate we have a connected player and should send data

            LONGLONG m_adjustedStartTime;    // Presentation time when the clock started.

            RealtimeStreaming::Network::Connection m_connection{ nullptr };
            RealtimeStreaming::Media::NetworkMediaSink  m_parentMediaSink{ nullptr };

            com_ptr<IMFMediaType> m_currentType;
            GUID m_currentSubtype;

            DWORD m_workQueueId;     // ID of the work queue for asynchronous operations.
            AsyncCallback<NetworkMediaSinkStream> m_workQueueCB;     // Callback for the work queue.

            com_ptr<IMFMediaEventQueue>  m_eventQueue;    // Event queue
            
            // array<com_ptr>?
            ComPtrList<IUnknown>        m_sampleQueue;   // Queue to hold samples and markers.
                                                        // Applies to: ProcessSample, PlaceMarker

            // ValidStateMatrix: Defines a look-up table that says which operations
            // are valid from which states.
            static bool ValidStateMatrix[(int)SinkStreamState::Count][(int)SinkStreamOperation::Count];
        };
}

namespace winrt::RealtimeStreaming::Media::factory_implementation
{
    struct NetworkMediaSinkStream : NetworkMediaSinkStreamT<NetworkMediaSinkStream, implementation::NetworkMediaSinkStream>
    {
    };
}