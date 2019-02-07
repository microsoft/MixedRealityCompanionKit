// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

namespace RealtimeStreaming
{
    namespace Network
    {
        using namespace ABI::RealtimeStreaming;

        class ListenerImpl
            : public RuntimeClass
            < RuntimeClassFlags<RuntimeClassType::WinRtClassicComMix>
            , Plugin::IModule
            , ABI::RealtimeStreaming::Network::IListener
            , IAsyncOperation<ABI::RealtimeStreaming::Network::Connection*>
            , AsyncBase<IAsyncOperationCompletedHandler<ABI::RealtimeStreaming::Network::Connection*>>
            , FtmBase >
        {
            InspectableClass(RuntimeClass_RealtimeStreaming_Network_Listener, BaseTrust);

        public:
            ListenerImpl();
            ~ListenerImpl();

            STDMETHODIMP RuntimeClassInitialize(
                _In_ UINT16 port);

            // IModule
            IFACEMETHOD(get_IsInitialized)(
                _Out_ boolean* pIsInitialzed);
            IFACEMETHOD(Uninitialize)(void);

            // IListener
            IFACEMETHOD(add_Closed)(
                _In_ Plugin::IClosedEventHandler *eventHandler,
                _In_ EventRegistrationToken *token);
            IFACEMETHOD(remove_Closed)(
                _In_ EventRegistrationToken token);

            IFACEMETHOD(ListenAsync)(
                _Out_ IAsyncOperation<ABI::RealtimeStreaming::Network::Connection*>** operation);

            // IAsyncOperation<IConnection*>
            IFACEMETHOD(put_Completed)(
                _In_ IAsyncOperationCompletedHandler<ABI::RealtimeStreaming::Network::Connection*>* operation);
            IFACEMETHOD(get_Completed)(
                _Out_ IAsyncOperationCompletedHandler<ABI::RealtimeStreaming::Network::Connection*>** operation);
            IFACEMETHOD(GetResults)(
                _Out_ ABI::RealtimeStreaming::Network::IConnection** result);
            virtual HRESULT OnStart(void);
            virtual void OnClose(void);
            virtual void OnCancel(void);

            // ITypedEventHandler
            IFACEMETHOD(OnConnectionReceived)(
                _In_ IStreamSocketListener* sender,
                _In_ IStreamSocketListenerConnectionReceivedEventArgs* args);

        private:
            STDMETHODIMP_(void) CloseInternal();

            inline HRESULT CheckClosed() const
            {
                return _isInitialized ? S_OK : MF_E_SHUTDOWN;
            }

        private:
            Wrappers::CriticalSection   _lock;

            boolean                     _isInitialized;

            UINT16                      _port;
            EventSource<Plugin::IClosedEventHandler>    _evtClosed;

            ComPtr<IStreamSocketListener>   _socketListener;
            EventRegistrationToken          _connectionReceivedEventToken;

            ComPtr<IStreamSocket>           _streamSocketResult;
        };

        class ListenerStaticsImpl
            : public ActivationFactory
            < ABI::RealtimeStreaming::Network::IListenerStatics
            , FtmBase >
        {
            InspectableClassStatic(RuntimeClass_RealtimeStreaming_Network_Listener, BaseTrust);

        public:
            // IListenerStatics
            STDMETHODIMP Create(
                _In_ UINT16 port,
                _COM_Outptr_result_maybenull_ ABI::RealtimeStreaming::Network::IListener** ppListener) override
            {
                ComPtr<ListenerImpl> spListener;
                IFR(Microsoft::WRL::MakeAndInitialize<ListenerImpl>(&spListener, port));
                
                NULL_CHK_HR(spListener, E_OUTOFMEMORY);

                return spListener.CopyTo(ppListener);
            }
        };
    }
}
