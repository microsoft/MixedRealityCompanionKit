// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

namespace RealtimeStreaming
{
    namespace Network
    {
        class DataBundleImpl
            : public RuntimeClass
            < RuntimeClassFlags<RuntimeClassType::WinRtClassicComMix>
            , IDataBundle
            , FtmBase >
        {
            InspectableClass(RuntimeClass_RealtimeStreaming_Network_DataBundle, BaseTrust);

        public:
            typedef std::list<com_ptr<IDataBuffer> > Container;
            typedef std::list<com_ptr<IDataBuffer> >::iterator Iterator;

            ~DataBundleImpl();

            STDMETHODIMP RuntimeClassInitialize();
            STDMETHODIMP RuntimeClassInitialize(
                _In_ IDataBuffer* dataBuffer);
            STDMETHODIMP RuntimeClassInitialize(
                _In_ IMFSample* pSample);

            // IDataBundle
            IFACEMETHOD(get_BufferCount)(
                _Out_ UINT32 *count);

            IFACEMETHOD(get_TotalSize)(
                _Out_ ULONG *size);

            IFACEMETHOD(AddBuffer)(
                _In_ IDataBuffer *buffer);

            IFACEMETHOD(InsertBuffer)(
                _In_ UINT32 index,
                _In_ IDataBuffer *buffer);

            IFACEMETHOD(RemoveBuffer)(
                _In_ IDataBuffer *buffer);

            IFACEMETHOD(RemoveBufferByIndex)(
                _In_ UINT32 index,
                _COM_Outptr_opt_result_maybenull_ IDataBuffer** buffer);

            IFACEMETHOD(Reset)(void) override;

            // DataBundleImpl
            IFACEMETHOD(CopyTo)(DWORD nOffset, DWORD cbSize, _In_reads_bytes_(cbSize) void* pDest, _Out_ DWORD* pcbCopied);
            IFACEMETHOD(MoveLeft)(DWORD cbSize, _Out_writes_bytes_(cbSize) void* pDest);
            IFACEMETHOD(TrimLeft)(DWORD cbSize);
            IFACEMETHOD(ToMFSample)(_COM_Outptr_result_maybenull_ IMFSample** ppSample);

            STDMETHODIMP get_Buffers(
                _Out_ Container* buffers)
            {
                *buffers = _buffers;

                return S_OK;
            }

        private:
            Container _buffers;
        };

    }
}