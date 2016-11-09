// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "OpQueue.h"
#include "LinkList.h"
#include "BaseAttributes.h"

namespace MixedRemoteViewCompositor
{
    namespace Media
    {
        using namespace Microsoft::WRL;

        class NetworkMediaSourceStreamImpl;

        // Base class representing asyncronous source operation
        class SourceOperation : public IUnknown
        {
        public:
            enum Type
            {
                // request media describe
                Operation_Init,
                // Start the source
                Operation_Start,
                // Stop the source
                Operation_Stop,
                // Set rate
                Operation_SetRate,
            };

        public:
            SourceOperation(Type opType);
            virtual ~SourceOperation();

            // IUnknown
            IFACEMETHOD(QueryInterface) (REFIID riid, _COM_Outptr_ void** ppv);
            IFACEMETHOD_(ULONG, AddRef) ();
            IFACEMETHOD_(ULONG, Release) ();

            Type GetOperationType() const { return _opType; }
            const PROPVARIANT &GetData() const { return _data; }
            HRESULT SetData(const PROPVARIANT &varData);

        private:
            long _cRef; // reference count
            Type _opType;
            PROPVARIANT _data;
        };

        // Start operation
        class StartOperation : public SourceOperation
        {
        public:
            StartOperation(_In_ IMFPresentationDescriptor* pPD);
            ~StartOperation();

            IMFPresentationDescriptor* GetPresentationDescriptor() { return _spPD.Get(); }

        private:
            Microsoft::WRL::ComPtr<IMFPresentationDescriptor> _spPD; // Presentation descriptor
        };

        // SetRate operation
        class SetRateOperation : public SourceOperation
        {
        public:
            SetRateOperation(_In_ BOOL fThin, _In_ float flRate);
            ~SetRateOperation();

            BOOL IsThin() const { return _fThin; }
            float GetRate() const { return _flRate; }

        private:
            BOOL _fThin;
            float _flRate;
        };

        class NetworkMediaSourceImpl
            : public RuntimeClass<RuntimeClassFlags<WinRtClassicComMix>
            , ABI::MixedRemoteViewCompositor::Media::INetworkMediaSource
            , ABI::Windows::Foundation::IAsyncAction
            , AsyncBase<ABI::Windows::Foundation::IAsyncActionCompletedHandler>
            , IMediaExtension
            , Microsoft::WRL::ChainInterfaces<IMFMediaSource, IMFMediaEventGenerator>
            , IMFGetService
            , IMFRateControl
            , ILockable
            , FtmBase>
            , public OpQueue<NetworkMediaSourceImpl, SourceOperation>
            , public CBaseAttributes<>
        {
            InspectableClass(L"RuntimeClass_MixedRemoteViewCompositor_Media_NetworkMediaSource", BaseTrust)

        public:
            NetworkMediaSourceImpl(void);
            ~NetworkMediaSourceImpl(void);

            STDMETHODIMP RuntimeClassInitialize(
                _In_ ABI::MixedRemoteViewCompositor::Network::IConnection* connection);

            // IMediaExtension
            IFACEMETHOD(SetProperties) (
                _In_opt_ ABI::Windows::Foundation::Collections::IPropertySet *configuration) { return S_OK; }

            // IMFMediaEventGenerator
            IFACEMETHOD(GetEvent) (_In_ DWORD dwFlags, _Outptr_opt_ IMFMediaEvent** ppEvent);
            IFACEMETHOD(BeginGetEvent) (_In_ IMFAsyncCallback* pCallback, _In_ IUnknown* punkState);
            IFACEMETHOD(EndGetEvent) (_In_ IMFAsyncResult* pResult, _Out_ IMFMediaEvent** ppEvent);
            IFACEMETHOD(QueueEvent) (_In_ MediaEventType met, _In_ REFGUID guidExtendedType, HRESULT hrStatus, _In_ const PROPVARIANT* pvValue);

            // IMFMediaSource
            IFACEMETHOD(GetCharacteristics) (_Out_ DWORD* pdwCharacteristics);
            IFACEMETHOD(CreatePresentationDescriptor) (_Outptr_ IMFPresentationDescriptor** ppPresentationDescriptor);
            IFACEMETHOD(Start) (
                _In_opt_ IMFPresentationDescriptor* pPresentationDescriptor,
                _In_opt_ const GUID* pguidTimeFormat,
                _In_opt_ const PROPVARIANT* pvarStartPosition);
            IFACEMETHOD(Stop)();
            IFACEMETHOD(Pause) ();
            IFACEMETHOD(Shutdown) ();

            // IMFGetService
            IFACEMETHOD(GetService) (_In_ REFGUID guidService, _In_ REFIID riid, _Out_opt_ void** ppvObject);

            // IMFRateControl
            IFACEMETHOD(SetRate) (_In_ BOOL fThin, _In_ float flRate);
            IFACEMETHOD(GetRate) (_Inout_opt_ BOOL* pfThin, _Inout_opt_ float* pflRate);

            // ILockable
            _Acquires_lock_(_lock)
                Wrappers::CriticalSection::SyncLock Lock() { return _lock.Lock(); }

            // OpQueue
            __override HRESULT DispatchOperation(_In_ SourceOperation* pOp);
            __override HRESULT ValidateOperation(_In_ SourceOperation* pOp);


            // IAsyncAction
            IFACEMETHOD(put_Completed)(
                _In_ ABI::Windows::Foundation::IAsyncActionCompletedHandler *handler) 
            {
                return PutOnComplete(handler);
            }
            IFACEMETHOD(get_Completed)(
                _Out_ ABI::Windows::Foundation::IAsyncActionCompletedHandler** handler) 
            {
                return GetOnComplete(handler);
            }
            IFACEMETHOD(GetResults)(void) 
            {
                return AsyncBase::CheckValidStateForResultsCall();
            }

            // AsyncBase
            virtual HRESULT OnStart(void);
            virtual void OnClose(void)
            {
                HandleError(MF_E_SHUTDOWN);
            }
            virtual void OnCancel(void)
            {
                HandleError(E_ABORT);
            }

            // INetworkMediaSouce

        protected:
            HRESULT OnDataReceived(
                _In_ IConnection *sender,
                _In_ IBundleReceivedArgs *args);

            typedef ComPtrList<IMFMediaStream> StreamContainer;

        private:
            HRESULT HandleError(HRESULT hResult);
            HRESULT GetStreamById(
                _In_ DWORD dwId, 
                _Outptr_ IMFMediaStream** ppStream);

            HRESULT CompleteOpen(HRESULT hResult);
            HRESULT SendDescribeRequest();
            HRESULT SendStartRequest();
            HRESULT SendStopRequest();

            HRESULT ProcessCaptureReady();
            HRESULT ProcessMediaDescription(_In_ IDataBundle* pBundle);
            HRESULT ProcessMediaSample(_In_ IDataBundle* pBundle);
            HRESULT ProcessMediaTick(_In_ IDataBundle* pBundle);
            HRESULT ProcessMediaFormatChange(_In_ IDataBundle* pBundle);

            //HRESULT AddStream(_In_ MediaTypeDescription* pStreamDesc);
            HRESULT InitPresentationDescription();
            HRESULT ValidatePresentationDescriptor(_In_ IMFPresentationDescriptor* pPD);
            HRESULT SelectStreams(_In_ IMFPresentationDescriptor* pPD);

            HRESULT DoInit(_In_ SourceOperation* pOp);
            HRESULT DoStart(_In_ StartOperation* pOp);
            HRESULT DoStop(_In_ SourceOperation* pOp);
            HRESULT DoSetRate(_In_ SetRateOperation* pOp);

            bool IsRateSupported(float flRate, _Out_ float* pflAdjustedRate);

            inline HRESULT CheckShutdown() const
            {
                if (_eSourceState == SourceStreamState_Shutdown)
                {
                    return MF_E_SHUTDOWN;
                }
                else
                {
                    return S_OK;
                }
            }

        private:
            Wrappers::CriticalSection _lock;

            ComPtr<IConnection> _spConnection; // Network sender
            EventRegistrationToken _evtReceivedToken;

            SourceStreamState _eSourceState; // Flag to indicate if Shutdown() method was called.
            Microsoft::WRL::ComPtr<IMFMediaEventQueue> _spEventQueue; // Event queue

            Microsoft::WRL::ComPtr<IMFPresentationDescriptor> _spPresentationDescriptor;
            StreamContainer _streams; // Collection of streams associated with the source

            float _flRate;
        };

        class NetworkMediaSourceStaticsImpl
            : public ActivationFactory
            < ABI::MixedRemoteViewCompositor::Media::INetworkMediaSourceStatics
            , FtmBase >
        {
            InspectableClassStatic(RuntimeClass_MixedRemoteViewCompositor_Media_NetworkMediaSource, BaseTrust);

        public:
            IFACEMETHOD(Create)(
                _In_ ABI::MixedRemoteViewCompositor::Network::IConnection *connection,
                _Out_ ABI::Windows::Media::Core::IMediaSource** mediaSource);
        };
    }
}
