#include "pch.h"
#include "Network.DataBundle.h"

namespace winrt::RealtimeStreaming::Network::implementation
{
    uint32_t DataBundle::BufferCount()
    {
        throw hresult_not_implemented();
    }

    uint64_t DataBundle::TotalSize()
    {
        throw hresult_not_implemented();
    }

    void DataBundle::AddBuffer(RealtimeStreaming::Network::DataBuffer const& dataBuffer)
    {
        throw hresult_not_implemented();
    }

    bool DataBundle::InsertBuffer(uint32_t index, RealtimeStreaming::Network::DataBuffer const& dataBuffer)
    {
        throw hresult_not_implemented();
    }

    bool DataBundle::RemoveBuffer(RealtimeStreaming::Network::DataBuffer const& dataBuffer)
    {
        throw hresult_not_implemented();
    }

    void DataBundle::Reset()
    {
        throw hresult_not_implemented();
    }
}
