// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "Network.DataBundle.g.h"

struct __declspec(uuid("bb9ac868-2fbd-11e9-b210-d663bd873d93")) IDataBundlePriv : ::IUnknown
{
    STDMETHOD(CopyTo)(_In_ DWORD nOffset,
        _In_ DWORD cbSize,
        _In_reads_bytes_(cbSize) void* pDest,
        _Out_ UINT32* pCBCopied) PURE;

    STDMETHOD(MoveLeft)(_In_ DWORD cbSize,
        _Out_writes_bytes_(cbSize) void* pDest) PURE;

    STDMETHOD(TrimLeft)(_In_ DWORD cbSize) PURE;

    STDMETHOD(ToMFSample)(_COM_Outptr_result_maybenull_ IMFSample** ppSample) PURE;
};

namespace winrt::RealtimeStreaming::Network::implementation
{
    struct DataBundle : DataBundleT<DataBundle, IDataBundlePriv>
    {
    public:
        DataBundle();
        DataBundle(_In_ Network::DataBuffer dataBuffer);
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

        // IDataBundlePriv
        STDOVERRIDEMETHODIMP CopyTo(_In_ DWORD nOffset,
            _In_ DWORD cbSize,
            _In_reads_bytes_(cbSize) void* pDest,
            _Out_ UINT32* pCBCopied);

        STDOVERRIDEMETHODIMP  MoveLeft(_In_ DWORD cbSize,
            _Out_writes_bytes_(cbSize) void* pDest);

        STDOVERRIDEMETHODIMP  TrimLeft(_In_ DWORD cbSize);

        STDOVERRIDEMETHODIMP ToMFSample(_COM_Outptr_result_maybenull_ IMFSample** ppSample);
        
        std::vector<winrt::RealtimeStreaming::Network::DataBuffer> GetBuffers();

    private:
        std::vector<winrt::RealtimeStreaming::Network::DataBuffer> m_buffers;
    };
}