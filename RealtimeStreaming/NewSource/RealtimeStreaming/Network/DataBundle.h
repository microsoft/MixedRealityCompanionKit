// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "Generated Files\Network\DataBundle.g.h"

//#include "DataBuffer.h"

namespace winrt::RealtimeStreaming::Network::implementation
{
    struct DataBundle : DataBundleT<DataBundle>
    {
    public:
        DataBundle();
        //DataBundle(_In_ DataBuffer dataBuffer);
        DataBundle(_In_ IMFSample* pSample);
        ~DataBundle();

        // IDataBundle
        UINT32 BufferCount();
        UINT64 TotalSize();

        void AddBuffer(_In_ RealtimeStreaming::Network::DataBuffer const& dataBuffer);

        bool InsertBuffer(_In_ uint32_t index, 
            RealtimeStreaming::Network::DataBuffer const& dataBuffer);

        bool RemoveBuffer(_In_ RealtimeStreaming::Network::DataBuffer const& dataBuffer);

        void Reset();

        // DataBundleImpl
        UINT32 CopyTo(_In_ DWORD nOffset,
            _In_ DWORD cbSize,
            _In_reads_bytes_(cbSize) void* pDest);

        void MoveLeft(_In_ DWORD cbSize,
            _Out_writes_bytes_(cbSize) void* pDest);

        void TrimLeft(_In_ DWORD cbSize);

        STDMETHOD(ToMFSample)(_COM_Outptr_result_maybenull_ IMFSample** ppSample);
        
        std::vector<winrt::RealtimeStreaming::Network::DataBuffer> GetBuffers();

    private:
        std::vector<winrt::RealtimeStreaming::Network::DataBuffer> m_buffers;
    };
}