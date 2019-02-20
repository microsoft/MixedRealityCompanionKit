// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "Connector.h"

#include "Connection.h"

using namespace winrt;
using namespace RealtimeStreaming::Network::implementation;
using namespace Windows::Foundation;
using namespace Windows::Networking::Sockets;

_Use_decl_annotations_
Connector::Connector(_In_ Windows::Networking::HostName hostName,
    _In_ UINT16 port)
    : m_hostName(hostName)
    , m_port(port)
    , m_streamSocketResult(nullptr)
{
    Log(Log_Level_Info, L"ConnectorImpl::Connector()\n");
}

_Use_decl_annotations_
Connector::~Connector()
{
    Log(Log_Level_Info, L"ConnectorImpl::~Connector()\n");

    //Close();
}

// IConnector
_Use_decl_annotations_
IAsyncOperation<RealtimeStreaming::Network::Connection> Connector::ConnectAsync()
{
    Log(Log_Level_Info, L"ConnectorImpl::ConnectAsync()\n");

    // activate a stream socket
    m_streamSocketResult.Control().KeepAlive(true);
    m_streamSocketResult.Control().NoDelay(true);
    m_streamSocketResult.Control().QualityOfService(SocketQualityOfService::LowLatency);

    std::string port = std::to_string(m_port);

    co_await m_streamSocketResult.ConnectAsync(m_hostName, winrt::to_hstring(port));

    auto connection = winrt::make<Network::implementation::Connection>(m_streamSocketResult);

    co_return connection;
}

/* Event Handlers */
_Use_decl_annotations_
event_token Connector::Closed(Windows::Foundation::EventHandler<bool> const& handler)
{
    Log(Log_Level_All, L"ConnectorImpl::add_Closed() - Tid: %d \n", GetCurrentThreadId());

    //auto lock = _lock.Lock();
    slim_lock_guard guard(m_lock);

    return m_evtClosed.add(handler);
}

_Use_decl_annotations_
void Connector::Closed(winrt::event_token const& token)
{
    Log(Log_Level_Info, L"ConnectorImpl::remove_Closed()\n");

    //auto lock = _lock.Lock();
    slim_lock_guard guard(m_lock);

    m_evtClosed.remove(token);
}

_Use_decl_annotations_
void Connector::Close()
{
    Log(Log_Level_Info, L"ConnectorImpl::Close()\n");

    /*
    com_ptr<Windows::Foundation::IClosable> closeable;
    if (nullptr != m_streamSocketResult)
    {
        if SUCCEEDED(m_streamSocketResult.As(&closeable))
        {
            LOG_RESULT(closeable->Close());
        }

        com_ptr<IConnector> spThis(this);
        LOG_RESULT(_evtClosed.InvokeAll(spThis.get()));
    }
    m_streamSocketResult.Reset();
    m_streamSocketResult = nullptr;
    */
}
