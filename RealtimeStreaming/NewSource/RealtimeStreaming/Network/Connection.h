// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "Generated Files/Network/Connection.g.h"
#include "Plugin/Module.h"

//using namespace winrt::RealtimeStreaming::Plugin::implementation;

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
    
    struct Connection : ConnectionT<Connection, Plugin::Module>
    {

    public:
        Connection(_In_ Windows::Networking::Sockets::StreamSocket socket);
        ~Connection();

        // IClosable
        void Close();

        //IConnection
        bool IsConnected();
        Windows::Networking::Sockets::StreamSocketInformation ConnectionInfo();

        Windows::Foundation::IAsyncAction SendPayloadTypeAsync(
            _In_ RealtimeStreaming::Common::PayloadType payloadType);

        Windows::Foundation::IAsyncAction SendBundleAsync(
            _In_ RealtimeStreaming::Network::DataBundle dataBundle);

        event_token Disconnected(RealtimeStreaming::Network::DisconnectedDelegate const& handler);
        void Disconnected(event_token const& token);

        event_token Received(Windows::Foundation::EventHandler<RealtimeStreaming::Network::BundleReceivedArgs> const& handler);
        void Received(event_token const& token);
    protected:
        // IConnectionInternal
        inline IFACEMETHOD(CheckClosed)()
        {
            return (nullptr != m_streamSocket) ? S_OK : MF_E_SHUTDOWN;
        }

        Windows::Foundation::IAsyncAction WaitForHeader();
        Windows::Foundation::IAsyncAction WaitForPayload();
        Windows::Foundation::IAsyncAction OnHeaderReceived(); // TODO: save header locally versus passing by parameter?
        Windows::Foundation::IAsyncAction OnPayloadReceived(_In_ Windows::Storage::Streams::IBuffer payloadBuffer);

        STDMETHODIMP NotifyBundleComplete(
            _In_ RealtimeStreaming::Common::PayloadType operation,
            _In_ RealtimeStreaming::Network::DataBundle dataBundle);

        void ResetBundle();

    private:
        HRESULT ProcessHeaderBuffer(
            _In_ Common::PayloadHeader* header,
            _In_ RealtimeStreaming::Network::DataBuffer dataBuffer);

    private:
        //Wrappers::CriticalSection _lock;
        slim_mutex m_lock;

        UINT16      m_concurrentFailedBuffers;
        UINT16      m_concurrentFailedBundles;

        Windows::Networking::Sockets::StreamSocket    m_streamSocket{ nullptr };

        RealtimeStreaming::Network::DataBuffer  m_spHeaderBuffer{ nullptr };

        // currently bundle that is incoming
        Common::PayloadHeader m_receivedHeader;
        RealtimeStreaming::Network::DataBundle    m_receivedBundle{ nullptr };
            
        winrt::event<Windows::Foundation::EventHandler<IInspectable>> m_evtDisconnected;
        winrt::event<Windows::Foundation::EventHandler<BundleReceivedArgs>> m_evtBundleReceived;

    };
}