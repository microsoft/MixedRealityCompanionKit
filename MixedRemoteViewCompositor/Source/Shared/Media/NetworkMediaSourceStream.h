// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

namespace MixedRemoteViewCompositor
{
    namespace Media
    {
        class NetworkMediaSourceImpl;

        class NetworkMediaSourceStreamImpl 
            : public RuntimeClass<RuntimeClassFlags<ClassicCom>
            , IMFMediaStream
            , IMFGetService
            , IMFQualityAdvise2
            , IMFMediaEventGenerator
            , FtmBase>
        {
        public:
            NetworkMediaSourceStreamImpl() { NetworkMediaSourceStreamImpl(nullptr); }
            NetworkMediaSourceStreamImpl(_In_ NetworkMediaSourceImpl* pSource);
            ~NetworkMediaSourceStreamImpl();

            static HRESULT CreateInstance(
                _In_ MediaTypeDescription* pStreamDescription,
                _In_ IDataBundle* pAttributesBuffer,
                _In_ NetworkMediaSourceImpl* pSource,
                _Outptr_opt_ NetworkMediaSourceStreamImpl** ppStream);

            HRESULT RuntimeClassInitialize(
                _In_ MediaTypeDescription* pStreamDescription,
                _In_ IDataBundle* pAttributesBuffer, 
                _In_ IMFMediaSource* pParent);

            // IMFMediaEventGenerator
            IFACEMETHOD(BeginGetEvent) (_In_ IMFAsyncCallback* pCallback, _In_ IUnknown* punkState);
            IFACEMETHOD(EndGetEvent) (_In_ IMFAsyncResult* pResult, _Out_ IMFMediaEvent** ppEvent);
            IFACEMETHOD(GetEvent) (_In_ DWORD dwFlags, _Out_ IMFMediaEvent** ppEvent);
            IFACEMETHOD(QueueEvent) (_In_ MediaEventType met, _In_ REFGUID guidExtendedType, HRESULT hrStatus, _In_ const PROPVARIANT* pvValue);

            // IMFMediaStream
            IFACEMETHOD(GetMediaSource) (_Outptr_opt_ IMFMediaSource** ppMediaSource);
            IFACEMETHOD(GetStreamDescriptor) (_Outptr_opt_ IMFStreamDescriptor** ppStreamDescriptor);
            IFACEMETHOD(RequestSample) (_In_ IUnknown* pToken);

            // IMFQualityAdvise
            IFACEMETHOD(SetDropMode) (_In_ MF_QUALITY_DROP_MODE eDropMode);
            IFACEMETHOD(SetQualityLevel) (_In_ MF_QUALITY_LEVEL eQualityLevel);
            IFACEMETHOD(GetDropMode) (_Out_ MF_QUALITY_DROP_MODE* peDropMode);
            IFACEMETHOD(GetQualityLevel)(_Out_ MF_QUALITY_LEVEL* peQualityLevel);
            IFACEMETHOD(DropTime) (_In_ LONGLONG hnsAmountToDrop);

            // IMFQualityAdvise2
            IFACEMETHOD(NotifyQualityEvent) (_In_opt_ IMFMediaEvent* pEvent, _Out_ DWORD* pdwFlags);

            // IMFGetService
            IFACEMETHOD(GetService) (_In_ REFGUID guidService,
                _In_ REFIID riid,
                _Out_ LPVOID* ppvObject);

            // Other public methods
            HRESULT Start();
            HRESULT Stop();
            HRESULT SetRate(float flRate);
            HRESULT Flush();
            HRESULT Shutdown();
            HRESULT ProcessSample(
                _In_ Network::MediaSampleHeader* pSampleHeader, 
                _In_ MediaSampleTransforms* pSampleTransforms,
                _In_ IMFSample* pSample);
            HRESULT ProcessTick(__in Network::MediaStreamTick* pTickHeader, __in IMFAttributes* pAttributes);
            HRESULT ProcessFormatChange(__in IMFMediaType* pMediaType);
            HRESULT SetActive(bool fActive);
            bool IsActive() const { return _fActive; }
            SourceStreamState GetState() const { return _eSourceState; }

            DWORD get_StreamId() { return _dwId; }

        private:
            class MediaSourceLock;

        private:
            HRESULT Initialize(
                _In_ MediaTypeDescription* pMeidaTypeDescription, 
                _In_ IDataBundle* pAttributesBuffer);
            HRESULT DeliverSamples();
            HRESULT SetSampleAttributes(
                _In_ MediaSampleHeader* pSampleHeader, 
                _In_opt_ MediaSampleTransforms* pSampleTransforms,
                _In_ IMFSample* pSample);
            void HandleError(
                HRESULT hErrorCode);

            inline HRESULT CheckShutdown() const
            {
                if(_eSourceState == SourceStreamState_Shutdown)
                {
                    return MF_E_SHUTDOWN;
                }
                else
                {
                    return S_OK;
                }
            }

            bool ShouldDropSample(IMFSample* pSample);
            void CleanSampleQueue();
            void ResetDropTime();

        private:
            SourceStreamState  _eSourceState;              // Flag to indicate if Shutdown() method was called.
            Microsoft::WRL::ComPtr<IMFMediaSource>          _spSource;                  // Media source
            Microsoft::WRL::ComPtr<IMFMediaEventQueue>      _spEventQueue;              // Event queue
            Microsoft::WRL::ComPtr<IMFStreamDescriptor>     _spStreamDescriptor;        // Stream descriptor

            ComPtrList<IUnknown>        _samples;
            ComPtrList<IUnknown, true>  _tokens;
			Microsoft::WRL::ComPtr<IUnknown> tokenTest;

            DWORD                       _dwId;
            bool                        _fActive;
            bool                        _fVideo;
            float                       _flRate;

            MF_QUALITY_DROP_MODE        _eDropMode;
            bool                        _fDiscontinuity;
            bool                        _fDropTime;
            bool                        _fInitDropTime;
            bool                        _fWaitingForCleanPoint;
            LONGLONG                    _hnsStartDroppingAt;
            LONGLONG                    _hnsAmountToDrop;
        };
    }
}
