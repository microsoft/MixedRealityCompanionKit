// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

namespace RealtimeStreaming
{
    namespace Media
    {

        class NetworkMediaSinkStreamImpl
            : public RuntimeClass<RuntimeClassFlags<RuntimeClassType::ClassicCom>
            , IMFMediaEventGenerator
            , IMFStreamSink
            , IMFMediaTypeHandler>
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
            NetworkMediaSinkStreamImpl();
            virtual ~NetworkMediaSinkStreamImpl();

            HRESULT RuntimeClassInitialize(
                _In_ DWORD id, 
                _In_ ABI::RealtimeStreaming::Network::IConnection* pConnection,
                _In_ ABI::RealtimeStreaming::Media::INetworkMediaSink* parentMediaSink);

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

            HRESULT Start(_In_ MFTIME start);
            HRESULT Restart();
            HRESULT Stop();
            HRESULT Pause();
            HRESULT Shutdown();

            HRESULT ConnectedFunc(_In_ bool fConnected, _In_ LONGLONG llCurrentTime);
            HRESULT CheckShutdown() const
            {
                if (_state == SinkStreamState_Stopped)
                {
                    return MF_E_SHUTDOWN;
                }
                else
                {
                    return S_OK;
                }
            }

            bool IsVideo() const { return _fIsVideo; }

            HRESULT FillStreamDescription(
                _Inout_ MediaTypeDescription* pStreamDescription,
                _Out_ IDataBuffer** ppDataBuffer);

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

            HRESULT PrepareSample(
                _In_ IMFSample* pSample, 
                _In_ bool fForce,
                _Out_ IDataBundle** ppDataBundle);
            HRESULT PrepareStreamTick(
                _In_ IMFAttributes* pAtrributes, 
                _Out_ IDataBundle** ppDataBundle);
            HRESULT PrepareFormatChange(
                _In_ IMFMediaType* pMediaType, 
                _Out_ IDataBundle** ppDataBundle);
            HRESULT ProcessCameraData(
                _In_ IMFSample* pSample,
                _Inout_ MediaSampleHeader* pSampleHeader);

            HRESULT HandleError(HRESULT hr)
            {
                if (!_isShutdown && FAILED(hr))
                {
                    QueueEvent(MEError, GUID_NULL, hr, nullptr);
                }

                return hr;
            }

        private:
            Wrappers::CriticalSection _lock;

            DWORD _dwStreamId;          // streamId
            bool _fIsVideo;             // for video type streams, we have special data to send
            bool _fGetFirstSampleTime;  // wait for the first keyframe

            SinkStreamState _state;         // current state of the sink
            bool _isShutdown;           // Flag to indicate if Shutdown() method was called.
            bool _isPlayerConnected;    // Flag to indicate we have a connected player and should send data

            LONGLONG _adjustedStartTime;    // Presentation time when the clock started.

            ComPtr<ABI::RealtimeStreaming::Network::IConnection> _spConnection;
            ComPtr<ABI::RealtimeStreaming::Media::INetworkMediaSink>  _spParentMediaSink;

            ComPtr<IMFMediaType> _currentType;
            GUID _currentSubtype;

            DWORD _workQueueId;     // ID of the work queue for asynchronous operations.
            AsyncCallback<NetworkMediaSinkStreamImpl> _workQueueCB;     // Callback for the work queue.
            ComPtr<IMFMediaEventQueue>  _eventQueue;    // Event queue
            ComPtrList<IUnknown>        _sampleQueue;   // Queue to hold samples and markers.
                                                        // Applies to: ProcessSample, PlaceMarker

            // ValidStateMatrix: Defines a look-up table that says which operations
            // are valid from which states.
            static BOOL ValidStateMatrix[SinkStreamState_Count][SinkStreamOperation_Count];
        };
    }
}