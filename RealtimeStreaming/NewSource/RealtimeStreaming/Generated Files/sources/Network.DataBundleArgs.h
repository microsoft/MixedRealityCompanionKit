#pragma once

#include "Network.DataBundleArgs.g.h"

namespace winrt::RealtimeStreaming::Network::implementation
{
    struct DataBundleArgs : DataBundleArgsT<DataBundleArgs>
    {
        DataBundleArgs() = delete;

        RealtimeStreaming::Common::PayloadType PayloadType();
        RealtimeStreaming::Network::Connection DataConnection();
        RealtimeStreaming::Network::DataBundle Bundle();
    };
}
