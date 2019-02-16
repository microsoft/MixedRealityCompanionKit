#pragma once

#include "Network.DataBuffer.g.h"

namespace winrt::RealtimeStreaming::Network::implementation
{
    struct DataBuffer : DataBufferT<DataBuffer>
    {
        DataBuffer() = delete;
        DataBuffer(uint64_t size);

        uint64_t CurrentLength();
        void CurrentLength(uint64_t value);
        uint64_t Offset();
        void Offset(uint64_t value);
        winrt::hresult TrimLeft(uint64_t cbSize);
        RealtimeStreaming::Network::DataBuffer TrimRight(uint64_t cbSize);
        winrt::hresult Reset();
        uint32_t Capacity();
        uint32_t Length();
        void Length(uint32_t value);
    };
}

namespace winrt::RealtimeStreaming::Network::factory_implementation
{
    struct DataBuffer : DataBufferT<DataBuffer, implementation::DataBuffer>
    {
    };
}
