#pragma once

#include "Network.DataBundleArgs.g.h"

namespace winrt::RealtimeStreaming::Network::implementation
{
    struct DataBundleArgs : DataBundleArgsT<DataBundleArgs>
    {
        DataBundleArgs() = delete;
        DataBundleArgs(RealtimeStreaming::Common::PayloadType const& type, RealtimeStreaming::Network::Connection const& connection, RealtimeStreaming::Network::DataBundle const& dataBundle);

        RealtimeStreaming::Common::PayloadType PayloadType();
        RealtimeStreaming::Network::Connection DataConnection();
        RealtimeStreaming::Network::DataBundle Bundle();
    };
}

namespace winrt::RealtimeStreaming::Network::factory_implementation
{
    struct DataBundleArgs : DataBundleArgsT<DataBundleArgs, implementation::DataBundleArgs>
    {
    };
}
