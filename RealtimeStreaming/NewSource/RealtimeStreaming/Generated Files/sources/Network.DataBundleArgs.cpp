#include "pch.h"
#include "Network.DataBundleArgs.h"

namespace winrt::RealtimeStreaming::Network::implementation
{
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
