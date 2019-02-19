// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "DataBundleArgs.h"
#include "Connection.h"
#include "DataBundle.h"

using namespace winrt;
using namespace RealtimeStreaming::Network::implementation;

DataBundleArgs::DataBundleArgs(RealtimeStreaming::Common::PayloadType const& type,
    RealtimeStreaming::Network::Connection const& connection,
    RealtimeStreaming::Network::DataBundle const& dataBundle)
    : m_payloadType(type),
    m_connection(connection),
    m_bundle(dataBundle)
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