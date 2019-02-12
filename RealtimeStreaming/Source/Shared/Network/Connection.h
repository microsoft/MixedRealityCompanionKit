// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

namespace winrt::RealtimeStreaming::Network::implementation
{
    
        typedef IAsyncOperation<Connection> IConnectionCreatedOperation;
        typedef IAsyncOperationCompletedHandler<Connection> IConnectionCreatedCompletedEventHandler;

        typedef IAsyncOperationWithProgress<IBuffer, UINT32> IStreamReadOperation;
        typedef IAsyncOperationWithProgressCompletedHandler<IBuffer, UINT32> IStreamReadCompletedEventHandler;

        typedef IAsyncOperationWithProgress<UINT32, UINT32> IStreamWriteOperation;
        typedef IAsyncOperationWithProgressCompletedHandler<UINT32, UINT32> IStreamWriteCompletedEventHandler;

        struct Connection : ConnectionT<Connection, Module>
        {

        public:
            Connection(_In_ Windows::Networking::Sockets::IStreamSocket socket);
            ~Connection();

            // IClosable
            void Close();

            //IConnection
            bool IsConnected();

            // IConnection
            IFACEMETHOD(SendPayloadType)(
                _In_ PayloadType payloadType);

            Windows::Foundation::IAsyncAction SendBundleAsync(
                _In_ RealtimeStreaming::Network::DataBundle dataBundle);

        protected:
            // IConnectionInternal
            inline IFACEMETHOD(CheckClosed)()
            {
                return (nullptr != m_streamSocket) ? S_OK : MF_E_SHUTDOWN;
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
                _In_ ABI::RealtimeStreaming::Network::IDataBundle *dataBundle);

            IFACEMETHOD(ResetBundle)();

        private:
            HRESULT ProcessHeaderBuffer(
                _In_ PayloadHeader* header,
                _In_ ABI::RealtimeStreaming::Network::IDataBuffer *dataBuffer);

        private:
            Wrappers::CriticalSection _lock;

            boolean     _isInitialized;
            UINT16      m_concurrentFailedBuffers;
            UINT16      _concurrentFailedBundles;

            Windows::Networking::Sockets::StreamSocket    m_streamSocket{ nullptr };

            RealtimeStreaming::Network::DataBufferImpl  m_spHeaderBuffer{ nullptr };

            // currently bundle that is incoming
            PayloadHeader m_receivedHeader;
            RealtimeStreaming::Network::DataBundle    m_receivedBundle{ nullptr };
            
            winrt::event<Windows::Foundation::EventHandler<IInspectable>> m_evtDisconnected;
            winrt::event<Windows::Foundation::EventHandler<BundleReceivedArgs>> m_evtBundleReceived;

        };
    }
}