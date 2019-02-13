// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

namespace winrt::RealtimeStreaming::Network::implementation
{
    struct DataBundle : DataBundleT<DataBundle>
    {
        public:
            DataBundle(_In_ IDataBuffer dataBuffer);
            DataBundle(_In_ IMFSample* pSample);
            ~DataBundle();

            // IDataBundle
            UINT32 BufferCount();
            UINT64 TotalSize();

            void AddBuffer(_In_DataBuffer dataBuffer);

            bool InsertBuffer(
                _In_ UINT32 index,
                _In_ DataBuffer dataBuffer);

            bool RemoveBuffer(DataBuffer dataBuffer);

            void Reset();

            // DataBundleImpl
            UINT32 CopyTo(_In_ DWORD nOffset,
                _In_ DWORD cbSize,
                _In_reads_bytes_(cbSize) void* pDest);
            
            void MoveLeft(_In_ DWORD cbSize, 
                _Out_writes_bytes_(cbSize) void* pDest);
            
            void TrimLeft(_In_ DWORD cbSize);

            IFACEMETHOD(ToMFSample)(_COM_Outptr_result_maybenull_ IMFSample** ppSample);

            std::vector<IDataBuffer> Buffers() 
            {
                return m_buffers;
            }

        private:
            std::vector< IDataBuffer > m_buffers;
    };
}