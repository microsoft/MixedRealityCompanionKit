// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "DataBundleArgs.h"
#include "Connection.h"
#include "DataBundle.h"

using namespace winrt;
using namespace RealtimeStreaming::Network::implementation;

_Use_decl_annotations_
DataBundleArgs::DataBundleArgs(
    RealtimeStreaming::Common::PayloadType operation,
    RealtimeStreaming::Network::Connection connection,
    RealtimeStreaming::Network::DataBundle bundle)
    : m_payloadType(operation),
    m_connection(connection),
    m_bundle(bundle)
{
    Log(Log_Level_Info, L"DataBundleArgsImpl::DataBundleArgsImpl()\n");
}

DataBundleArgs::~DataBundleArgs()
{
    Log(Log_Level_Info, L"DataBundleArgsImpl::~DataBundleArgsImpl()\n");
}

RealtimeStreaming::Common::PayloadType DataBundleArgs::PayloadType()
{
    return m_payloadType;
}

RealtimeStreaming::Network::Connection DataBundleArgs::DataConnection()
{
    return m_connection;
}

RealtimeStreaming::Network::DataBundle DataBundleArgs::Bundle()
{
    return m_bundle;
}
