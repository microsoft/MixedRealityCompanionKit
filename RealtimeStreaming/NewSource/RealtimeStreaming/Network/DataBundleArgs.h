// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "Generated Files\Network\BundleReceivedArgs.g.h"

namespace winrt::RealtimeStreaming::Network::implementation
{
    struct DataBundleArgs : BundleReceivedArgsT<DataBundleArgs>
    {
        DataBundleArgs(_In_ PayloadType operation,
            _In_ Connection connection,
            _In_ DataBundle bundle);
        ~DataBundleArgs();

        //IDataBundleArgs
        RealtimeStreaming::PayloadType PayloadType();
        Connection DataConnection();
        DataBundle Bundle();

    private:
        RealtimeStreaming::PayloadType   m_payloadType;
        RealtimeStreaming::Network::Connection    m_connection;
        RealtimeStreaming::Network::DataBundle    m_bundle;
    };
}
