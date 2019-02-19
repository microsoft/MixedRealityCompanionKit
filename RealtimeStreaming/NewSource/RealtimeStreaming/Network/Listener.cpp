// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "Listener.h"
#include "Connection.h"

using namespace winrt;
using namespace RealtimeStreaming::Network::implementation;
using namespace Windows::Foundation;
using namespace Windows::Networking::Sockets;

Listener::Listener(UINT16 port)
    : m_port(port)
    , m_socketListener(nullptr)
    , m_streamSocketResult(nullptr)
{
    Log(Log_Level_Info, L"Listener::ListenerImpl()\n");
}

Listener::~Listener()
{
    Log(Log_Level_Info, L"Listener::~ListenerImpl()\n");

    // TODO: Close like original?
    // Close();
}

// IListener
_Use_decl_annotations_
IAsyncOperation<RealtimeStreaming::Network::Connection> Listener::ListenAsync()
{
    Log(Log_Level_Info, L"ListenerImpl::ListenAsync()\n");

    // create a listener
    m_connectionReceivedEventToken = m_socketListener.ConnectionReceived({ this, &Listener::OnConnectionReceived });
    m_signal = handle(::CreateEvent(nullptr, true, false, nullptr));

    // Convert our port to a string
    std::string port = std::to_string(m_port);

    co_await m_socketListener.BindServiceNameAsync(winrt::to_hstring(port));

    co_await winrt::resume_on_signal(m_signal.get());

    co_return m_connection;
}

/* Event Handlers */
_Use_decl_annotations_
event_token Listener::Closed(Windows::Foundation::EventHandler<bool> const& handler)
{
    Log(Log_Level_All, L"Listener::add_Closed() - Tid: %d \n", GetCurrentThreadId());

    //auto lock = _lock.Lock();
    slim_lock_guard guard(m_lock);

    return m_evtClosed.add(handler);
}

_Use_decl_annotations_
void Listener::Closed(winrt::event_token const& token)
{
    Log(Log_Level_Info, L"Listener::remove_Closed()\n");

    //auto lock = _lock.Lock();
    slim_lock_guard guard(m_lock);

    m_evtClosed.remove(token);
}

_Use_decl_annotations_
void  Listener::OnConnectionReceived(StreamSocketListener /* sender */,
    StreamSocketListenerConnectionReceivedEventArgs args)
{
    Log(Log_Level_Info, L"ListenerImpl::OnConnectionReceived()\n");

    //slim_lock_guard guard(m_lock);

    m_connection = winrt::make<implementation::Connection>(args.Socket());

    // Signal completion of ListenAsync()
    SetEvent(m_signal.get());
};

_Use_decl_annotations_
void Listener::Close()
{
    // TODO: Revisit by Troy...seemed to be running in old version
    /*
    com_ptr<Windows::Foundation::IClosable> closeable;
    if (nullptr != m_socketListener)
    {
        // cleanup socket
        LOG_RESULT(m_socketListener->remove_ConnectionReceived(_connectionReceivedEventToken));
        if SUCCEEDED(m_socketListener.As(&closeable))
        {
            LOG_RESULT(closeable->Close());
        }

        com_ptr<IListener> spThis(this);
        LOG_RESULT(_evtClosed.InvokeAll(spThis.get()));
        // m_evtClosed(*this, ture);
    }
    
    m_socketListener = nullptr;

    if (nullptr != m_streamSocketResult)
    {
        if SUCCEEDED(m_streamSocketResult.As(&closeable))
        {
            LOG_RESULT(closeable->Close());
        }
    }
    */
    m_socketListener = nullptr;
    m_streamSocketResult = nullptr;
}
