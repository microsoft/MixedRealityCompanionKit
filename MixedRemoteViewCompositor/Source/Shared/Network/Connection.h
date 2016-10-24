// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

namespace MixedRemoteViewCompositor
{
    namespace Network
    {
        typedef IAsyncOperation<Connection*> IConnectionCreatedOperation;
        typedef IAsyncOperationCompletedHandler<Connection*> IConnectionCreatedCompletedEventHandler;

        typedef IAsyncOperationWithProgress<IBuffer*, UINT32> IStreamReadOperation;
        typedef IAsyncOperationWithProgressCompletedHandler<IBuffer*, UINT32> IStreamReadCompletedEventHandler;

        typedef IAsyncOperationWithProgress<UINT32, UINT32> IStreamWriteOperation;
        typedef IAsyncOperationWithProgressCompletedHandler<UINT32, UINT32> IStreamWriteCompletedEventHandler;

        MIDL_INTERFACE("edb95f27-f221-4c5e-b869-516e15cc6c2c")
            IWriteCompleted : IUnknown
        {
            IFACEMETHOD_(void, SignalCompleted)(HRESULT hr) = 0;
        };

        class WriteCompleteImpl
            : public RuntimeClass
            < RuntimeClassFlags<WinRtClassicComMix>
            , CloakedIid<IWriteCompleted>
            , IAsyncAction
            , AsyncBase<IAsyncActionCompletedHandler>
            , Microsoft::WRL::FtmBase >
        {
            InspectableClass(L"Windows.Foundation.IAsyncAction", BaseTrust);

        public:
            WriteCompleteImpl()
                : _bufferCount(0)
                , _completed(0)
            {
            }

            ~WriteCompleteImpl()
            {
                if (_completed < _bufferCount)
                {
                    SignalCompleted(E_UNEXPECTED);
                }
            }

            HRESULT RuntimeClassInitialize(UINT32 bufferCount)
            {
                _completed = 0;
                _bufferCount = bufferCount;

                return AsyncBase::Start();
            }

            // IAsyncAction
            IFACEMETHOD(put_Completed)(
                _In_ ABI::Windows::Foundation::IAsyncActionCompletedHandler *handler) override
            {
                return PutOnComplete(handler);
            }

            IFACEMETHOD(get_Completed)(
                _Out_ ABI::Windows::Foundation::IAsyncActionCompletedHandler** handler) override
            {
                return GetOnComplete(handler);
            }

            IFACEMETHOD(GetResults)(void) override
            {
                return AsyncBase::CheckValidStateForResultsCall();
            }

            // AsyncBase
            virtual HRESULT OnStart(void) { return S_OK; }
            virtual void OnClose(void) {};
            virtual void OnCancel(void) {};

            // WriteCompleteImpl
            IFACEMETHOD_(void, SignalCompleted)(HRESULT hr) override
            {
                Log(Log_Level_Info, L"WriteCompleteImpl::SignalCompleted()\n");

                if (SUCCEEDED(hr))
                {
                    _completed++;
                }

                if (_completed < _bufferCount && hr == S_OK)
                {
                    return;
                }

                if (FAILED(hr))
                {
                    AsyncBase::TryTransitionToError(hr);
                }

                AsyncBase::FireCompletion();
            }

        private:
            UINT32 _bufferCount;
            UINT32 _completed;
        };

        class ConnectionImpl
            : public RuntimeClass
            < RuntimeClassFlags<RuntimeClassType::WinRtClassicComMix>
            , ABI::MixedRemoteViewCompositor::Network::IConnection
            , ABI::MixedRemoteViewCompositor::Network::IConnectionInternal
            , Plugin::IModule
            , ABI::Windows::Foundation::IClosable
            , FtmBase >
        {
            InspectableClass(RuntimeClass_MixedRemoteViewCompositor_Network_Connection, BaseTrust);

        public:
            ConnectionImpl();
            ~ConnectionImpl();

            // RuntimeClass
            STDMETHODIMP RuntimeClassInitialize(
                _In_ ABI::Windows::Networking::Sockets::IStreamSocket *socket);

            // IModule
            IFACEMETHOD(get_IsInitialized)(
                _Out_ boolean* pIsInitialzed);
            IFACEMETHOD(Uninitialize)(void);

            // IClosable
            IFACEMETHOD(Close)(void);

            // IConnection
            IFACEMETHOD(get_IsConnected)(
                _Out_ boolean *connected);
            IFACEMETHOD(add_Disconnected)(
                _In_ ABI::MixedRemoteViewCompositor::Network::IDisconnectedEventHandler *eventHandler,
                _Out_ EventRegistrationToken *token);
            IFACEMETHOD(remove_Disconnected)(
                _In_ EventRegistrationToken token);
            IFACEMETHOD(add_Received)(
                _In_ ABI::MixedRemoteViewCompositor::Network::IBundleReceivedEventHandler *eventHandler,
                _Out_ EventRegistrationToken *token);
            IFACEMETHOD(remove_Received)(
                _In_ EventRegistrationToken token);
            IFACEMETHOD(SendPayloadType)(
                _In_ PayloadType payloadType);
            IFACEMETHOD(SendBundle)(
                _In_ ABI::MixedRemoteViewCompositor::Network::IDataBundle *dataBundle);
            IFACEMETHOD(SendBundleAsync)(
                _In_ ABI::MixedRemoteViewCompositor::Network::IDataBundle *dataBundle,
                _Out_ ABI::Windows::Foundation::IAsyncAction **sendAction);

        protected:
            // IConnectionInternal
            inline IFACEMETHOD(CheckClosed)()
            {
                return (nullptr != _streamSocket) ? S_OK : MF_E_SHUTDOWN;
            }
            IFACEMETHOD(WaitForHeader)();
            IFACEMETHOD(WaitForPayload)();

            HRESULT OnHeaderReceived(
                _In_ IAsyncOperationWithProgress<IBuffer*, UINT32> *asyncResult, 
                _In_ AsyncStatus asyncStatus);
            HRESULT OnPayloadReceived(
                _In_ IAsyncOperationWithProgress<IBuffer*, UINT32> *asyncResult, 
                _In_ AsyncStatus asyncStatus);
            STDMETHODIMP NotifyBundleComplete(
                _In_ PayloadType operation,
                _In_ ABI::MixedRemoteViewCompositor::Network::IDataBundle *dataBundle);

            IFACEMETHOD(ResetBundle)();

        private:
            HRESULT ProcessHeaderBuffer(
                _In_ PayloadHeader* header,
                _In_ ABI::MixedRemoteViewCompositor::Network::IDataBuffer *dataBuffer);

        private:
            Wrappers::CriticalSection _lock;

            boolean     _isInitialized;
            UINT16      _concurrentFailedBuffers;
            UINT16      _concurrentFailedBundles;

            ComPtr<IThreadPoolStatics> _threadPoolStatics;
            ComPtr<ABI::Windows::Networking::Sockets::IStreamSocket>    _streamSocket;

            ComPtr<MixedRemoteViewCompositor::Network::DataBufferImpl>  _spHeaderBuffer;

            // currently bundle that is incoming
            PayloadHeader _receivedHeader;
            ComPtr<ABI::MixedRemoteViewCompositor::Network::IDataBundle>    _receivedBundle;
            EventSource<ABI::MixedRemoteViewCompositor::Network::IDisconnectedEventHandler>    _evtDisconnected;
            EventSource<ABI::MixedRemoteViewCompositor::Network::IBundleReceivedEventHandler>    _evtBundleReceived;
        };
    }
}