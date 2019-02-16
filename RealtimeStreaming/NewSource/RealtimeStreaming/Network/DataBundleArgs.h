// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "Network.DataBundleArgs.g.h"

namespace winrt::RealtimeStreaming::Network::implementation
{
    struct DataBundleArgs : DataBundleArgsT<DataBundleArgs>
    {
        DataBundleArgs(_In_ Common::PayloadType operation,
            _In_ RealtimeStreaming::Network::Connection connection,
            _In_ RealtimeStreaming::Network::DataBundle bundle);
        ~DataBundleArgs();

        //IDataBundleArgs
        RealtimeStreaming::Common::PayloadType PayloadType();
        RealtimeStreaming::Network::Connection DataConnection();
        RealtimeStreaming::Network::DataBundle Bundle();

    private:
        Common::PayloadType   m_payloadType;
        Connection    m_connection;
        DataBundle    m_bundle;
    };
}

namespace winrt::RealtimeStreaming::Network::factory_implementation
{
    struct DataBundleArgs : DataBundleArgsT<DataBundleArgs, implementation::DataBundleArgs>
    {
    };
}