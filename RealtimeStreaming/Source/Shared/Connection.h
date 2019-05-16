// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "Network.Connection.g.h"

namespace winrt::RealtimeStreaming::Network::implementation
{
    struct Connection : ConnectionT<Connection>
    {
    public:
        static RealtimeStreaming::Network::DataBuffer CreatePayloadHeaderBuffer(
            _In_ RealtimeStreaming::Common::PayloadType payloadType,
            _In_ UINT32 cbPayloadSize);

        Connection() = default; // TODO: Clean this up design. Need ability to break waitforheader loop and re-bind connection?
        Connection(_In_ Windows::Networking::Sockets::StreamSocket const socket);
        ~Connection();

        // IClosable
        void Close();

        bool IsConnected();
        Windows::Networking::Sockets::StreamSocketInformation ConnectionInfo();

        Windows::Foundation::IAsyncAction SendPayloadTypeAsync(
            _In_ RealtimeStreaming::Common::PayloadType payloadType);

        Windows::Foundation::IAsyncAction SendBundleAsync(
            _In_ RealtimeStreaming::Network::DataBundle const dataBundle);

        winrt::event_token Disconnected(RealtimeStreaming::Network::DisconnectedDelegate const& handler);
        void Disconnected(winrt::event_token const& token) noexcept;

        event_token Received(Windows::Foundation::EventHandler<RealtimeStreaming::Network::DataBundleArgs> const& handler);
        void Received(event_token const& token) noexcept;

        void Shutdown() {};
    protected:
        // IConnectionInternal
        inline IFACEMETHOD(CheckClosed)()
        {
            return (nullptr != m_streamSocket) ? S_OK : MF_E_SHUTDOWN;
        }

        winrt::fire_and_forget RunSocketLoop();

        HRESULT OnHeaderReceived(_In_ Windows::Storage::Streams::IBuffer const&  headerBuffer);
        HRESULT OnPayloadReceived(_In_ Windows::Storage::Streams::IBuffer const&  payloadBuffer);

        void NotifyBundleComplete(
            _In_ RealtimeStreaming::Common::PayloadType operation,
            _In_ RealtimeStreaming::Network::DataBundle dataBundle);

    private:
        slim_mutex m_lock;
        slim_mutex m_eventLock;

        UINT16      m_concurrentFailedBuffers;
        UINT16      m_concurrentFailedBundles;

        void CloseOnDisconnectedSocketError(HRESULT hResult);

        Windows::Networking::Sockets::StreamSocket    m_streamSocket{ nullptr };
        Common::PayloadHeader m_receivedHeader;
        winrt::event<RealtimeStreaming::Network::DisconnectedDelegate> m_evtDisconnected;
        winrt::event<Windows::Foundation::EventHandler<RealtimeStreaming::Network::DataBundleArgs>> m_evtBundleReceived;
    };
}


namespace winrt::RealtimeStreaming::Network::factory_implementation
{
    struct Connection : ConnectionT<Connection, implementation::Connection>
    {
    };
}