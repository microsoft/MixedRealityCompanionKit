#pragma once

#include "Network/BundleReceivedArgs.g.h"

namespace winrt::RealtimeStreaming::Network::implementation
{
    struct BundleReceivedArgs : BundleReceivedArgsT<BundleReceivedArgs>
    {
        BundleReceivedArgs() = delete;

        RealtimeStreaming::PayloadType PayloadType();
        RealtimeStreaming::Network::Connection DataConnection();
        RealtimeStreaming::Network::DataBundle Bundle();
    };
}
