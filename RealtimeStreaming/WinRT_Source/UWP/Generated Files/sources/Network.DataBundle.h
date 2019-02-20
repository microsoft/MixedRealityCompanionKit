#pragma once

#include "Network.DataBundle.g.h"

namespace winrt::RealtimeStreaming::Network::implementation
{
    struct DataBundle : DataBundleT<DataBundle>
    {
        DataBundle() = delete;

        uint32_t BufferCount();
        uint64_t TotalSize();
        void AddBuffer(RealtimeStreaming::Network::DataBuffer const& dataBuffer);
        bool InsertBuffer(uint32_t index, RealtimeStreaming::Network::DataBuffer const& dataBuffer);
        bool RemoveBuffer(RealtimeStreaming::Network::DataBuffer const& dataBuffer);
        void Reset();
    };
}
