#include "pch.h"
#include "Network.DataBuffer.h"

namespace winrt::RealtimeStreaming::Network::implementation
{
    DataBuffer::DataBuffer(uint64_t size)
    {
        throw hresult_not_implemented();
    }

    uint64_t DataBuffer::CurrentLength()
    {
        throw hresult_not_implemented();
    }

    void DataBuffer::CurrentLength(uint64_t value)
    {
        throw hresult_not_implemented();
    }

    uint64_t DataBuffer::Offset()
    {
        throw hresult_not_implemented();
    }

    void DataBuffer::Offset(uint64_t value)
    {
        throw hresult_not_implemented();
    }

    winrt::hresult DataBuffer::TrimLeft(uint64_t cbSize)
    {
        throw hresult_not_implemented();
    }

    RealtimeStreaming::Network::DataBuffer DataBuffer::TrimRight(uint64_t cbSize)
    {
        throw hresult_not_implemented();
    }

    winrt::hresult DataBuffer::Reset()
    {
        throw hresult_not_implemented();
    }

    uint32_t DataBuffer::Capacity()
    {
        throw hresult_not_implemented();
    }

    uint32_t DataBuffer::Length()
    {
        throw hresult_not_implemented();
    }

    void DataBuffer::Length(uint32_t value)
    {
        throw hresult_not_implemented();
    }
}
