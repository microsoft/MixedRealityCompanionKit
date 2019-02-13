#include "pch.h"
#include "Network.BundleReceivedArgs.h"

namespace winrt::RealtimeStreaming::Network::implementation
{
    RealtimeStreaming::PayloadType BundleReceivedArgs::PayloadType()
    {
        throw hresult_not_implemented();
    }

    RealtimeStreaming::Network::Connection BundleReceivedArgs::DataConnection()
    {
        throw hresult_not_implemented();
    }

    RealtimeStreaming::Network::DataBundle BundleReceivedArgs::Bundle()
    {
        throw hresult_not_implemented();
    }
}
