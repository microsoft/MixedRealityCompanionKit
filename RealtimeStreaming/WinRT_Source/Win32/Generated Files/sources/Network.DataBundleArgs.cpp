#include "pch.h"
#include "Network.DataBundleArgs.h"

namespace winrt::RealtimeStreaming::Network::implementation
{
    DataBundleArgs::DataBundleArgs(RealtimeStreaming::Common::PayloadType const& type, RealtimeStreaming::Network::Connection const& connection, RealtimeStreaming::Network::DataBundle const& dataBundle)
    {
        throw hresult_not_implemented();
    }

    RealtimeStreaming::Common::PayloadType DataBundleArgs::PayloadType()
    {
        throw hresult_not_implemented();
    }

    RealtimeStreaming::Network::Connection DataBundleArgs::DataConnection()
    {
        throw hresult_not_implemented();
    }

    RealtimeStreaming::Network::DataBundle DataBundleArgs::Bundle()
    {
        throw hresult_not_implemented();
    }
}
