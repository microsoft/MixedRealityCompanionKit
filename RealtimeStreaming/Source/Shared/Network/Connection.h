// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

namespace winrt::RealtimeStreaming::Network::implementation
{
    /*
    typedef IAsyncOperation<Connection> IConnectionCreatedOperation;
    typedef IAsyncOperationCompletedHandler<Connection> IConnectionCreatedCompletedEventHandler;

    typedef IAsyncOperationWithProgress<IBuffer, UINT32> IStreamReadOperation;
    typedef IAsyncOperationWithProgressCompletedHandler<IBuffer, UINT32> IStreamReadCompletedEventHandler;

    typedef IAsyncOperationWithProgress<UINT32, UINT32> IStreamWriteOperation;
    typedef IAsyncOperationWithProgressCompletedHandler<UINT32, UINT32> IStreamWriteCompletedEventHandler;
    */

    struct Connection : ConnectionT<Connection, Module>
    {

    public:
        Connection(_In_ Windows::Networking::Sockets::StreamSocket socket);
        ~Connection();

        // IClosable
        void Close();

        //IConnection
        bool IsConnected();
        Windows.Networking.Sockets.StreamSocketInformation ConnectionInfo();

        Windows::Foundation::IAsyncAction SendPayloadTypeAsync(
            _In_ RealtimeStreaming::PayloadType payloadType);

        Windows::Foundation::IAsyncAction SendBundleAsync(
            _In_ RealtimeStreaming::Network::DataBundle dataBundle);

    protected:
        // IConnectionInternal
        inline IFACEMETHOD(CheckClosed)()
        {
            return (nullptr != m_streamSocket) ? S_OK : MF_E_SHUTDOWN;
        }

        Windows::Foundation::IAsyncAction WaitForHeader();
        Windows::Foundation::IAsyncAction WaitForPayload();
        Windows::Foundation::IAsyncAction OnHeaderReceived(); // TODO: save header locally versus passing by parameter?
        Windows::Foundation::IAsyncAction OnPayloadReceived(_In_ Windows::Storage::IBuffer payloadBuffer);

        STDMETHODIMP NotifyBundleComplete(
            _In_ RealtimeStreaming::PayloadType operation,
            _In_ RealtimeStreaming::Network::DataBundle dataBundle);

        void ResetBundle();

    private:
        HRESULT ProcessHeaderBuffer(
            _In_ PayloadHeader* header,
            _In_ RealtimeStreaming::Network::DataBuffer dataBuffer);

    private:
        Wrappers::CriticalSection _lock;

        UINT16      m_concurrentFailedBuffers;
        UINT16      m_concurrentFailedBundles;

        Windows::Networking::Sockets::StreamSocket    m_streamSocket{ nullptr };

        RealtimeStreaming::Network::DataBuffer  m_spHeaderBuffer{ nullptr };

        // currently bundle that is incoming
        PayloadHeader m_receivedHeader;
        RealtimeStreaming::Network::DataBundle    m_receivedBundle{ nullptr };
            
        winrt::event<Windows::Foundation::EventHandler<IInspectable>> m_evtDisconnected;
        winrt::event<Windows::Foundation::EventHandler<BundleReceivedArgs>> m_evtBundleReceived;

    };
}