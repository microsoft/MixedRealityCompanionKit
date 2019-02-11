// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

namespace RealtimeStreaming
{
    namespace Network
    {
        class ConnectorImpl
            : public RuntimeClass
            < RuntimeClassFlags<RuntimeClassType::WinRtClassicComMix>
            , Plugin::IModule
            , ABI::RealtimeStreaming::Network::IConnector
            , IConnectionCreatedOperation
            , AsyncBase<IAsyncOperationCompletedHandler<Connection*>>
            , FtmBase >
        {
            InspectableClass(RuntimeClass_RealtimeStreaming_Network_Connector, BaseTrust);

        public:
            ConnectorImpl();
            ~ConnectorImpl();

            STDMETHODIMP RuntimeClassInitialize(
                _In_ IHostName* hostName,
                _In_ UINT16 port);

            // IModule
            IFACEMETHOD(get_IsInitialized)(
                _Out_ boolean* initialize);
            IFACEMETHOD(Uninitialize)();

            // IConnector
            IFACEMETHOD(add_Closed)(
                _In_ IClosedEventHandler *eventHandler,
                _In_ EventRegistrationToken *token) override;
            IFACEMETHOD(remove_Closed)(
                _In_ EventRegistrationToken token) override;

            IFACEMETHOD(ConnectAsync)(
                _Out_ IConnectionCreatedOperation** operation) override;

            // IAsyncOperation<IConnection*>
            IFACEMETHOD(put_Completed)(
                _In_ IAsyncOperationCompletedHandler<Connection*> *operation) override;
            IFACEMETHOD(get_Completed)(
                _Out_ IAsyncOperationCompletedHandler<Connection*>** operation) override;
            IFACEMETHOD(GetResults)(
                _Out_ IConnection** result) override;

            // AsyncBase<IAsyncOperationCompletedHandler<Connection*>>
            virtual HRESULT OnStart(void) override;
            virtual void OnClose(void) override;
            virtual void OnCancel(void) override;

        private:
            void STDAPICALLTYPE CloseInternal();

        private:
            Wrappers::CriticalSection _lock;
            boolean _isInitialized;

            com_ptr<IHostName> _hostName;
            UINT16 _port;
            EventSource<IClosedEventHandler> _evtClosed;

            com_ptr<IStreamSocket> _streamSocketResult;
        };

        class ConnectorStaticsImpl
            : public ActivationFactory
            < ABI::RealtimeStreaming::Network::IConnectorStatics
            , FtmBase >
        {
            InspectableClassStatic(RuntimeClass_RealtimeStreaming_Network_Connector, BaseTrust);

        public:
            // IConnectorStatics
            STDMETHODIMP Create(
                _In_ HSTRING hostAddress,
                _In_ UINT16 port,
                _COM_Outptr_result_maybenull_ ABI::RealtimeStreaming::Network::IConnector** ppConnector) override
            {
                // Get IHostNameFactory instance
                com_ptr<ABI::Windows::Networking::IHostNameFactory> spHostNameFactory;
                check_hresult(Windows::Foundation::GetActivationFactory(
                    Wrappers::HStringReference(RuntimeClass_Windows_Networking_HostName).get(),
                    &spHostNameFactory));

                // create hostname
                com_ptr<ABI::Windows::Networking::IHostName> spHostName;
                check_hresult(spHostNameFactory->CreateHostName(hostAddress, &spHostName));

                com_ptr<ConnectorImpl> spConnector;
                check_hresult(MakeAndInitialize<ConnectorImpl>(&spConnector, spHostName.get(), port));

                NULL_CHK_HR(spConnector, E_OUTOFMEMORY);

                return spConnector.CopyTo(ppConnector);
            }
        };
    }
}
