// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "Network.DataBuffer.g.h"

#include <mfapi.h>
#include <mfidl.h>
#include <mferror.h>

DECLARE_INTERFACE_IID_(IDataBufferPriv, ::IUnknown, "297dd8ca-2fbe-11e9-b210-d663bd873d93")
{
    STDMETHOD(GetMediaBuffer)(
        _COM_Outptr_ IMFMediaBuffer** mfBuffer) PURE;

    //STDMETHOD(GetBufferPointer)(_Out_ BYTE** pBuffer) PURE;

    STDMETHOD(GetTexture)(
        _Outptr_ ID3D11Texture2D** ppTexture,
        _Out_ UINT *uiViewIndex) PURE;
};

namespace winrt::RealtimeStreaming::Network::implementation
{
    struct DataBuffer : DataBufferT<DataBuffer, IDataBufferPriv>
    {
        public:
            DataBuffer() = default;
            DataBuffer(_In_ DWORD dwMaxLength);
            DataBuffer(_In_ IMFMediaBuffer* pMediaBuffer);
            //DataBuffer(_In_ Windows::Storage::Streams::IBuffer dwMaxLength);
            ~DataBuffer();

            // IBuffer
            UINT32 Capacity();
            UINT32 Length();
            void Length(UINT32);

            // IBufferByteAccess
            IFACEMETHOD(Buffer)(
                _Outptr_ BYTE** ppBuffer);

            // IDataBuffer
            ULONG Offset();
            void Offset(ULONG value);

            ULONG CurrentLength();
            void CurrentLength(ULONG len);

            void TrimLeft(_In_ ULONG cbSize);
            Network::DataBuffer TrimRight(_In_ ULONG cbSize);

            void Reset();

            // IDataBufferPriv
            STDOVERRIDEMETHODIMP  GetMediaBuffer(
                _COM_Outptr_ IMFMediaBuffer** mfBuffer);

            STDOVERRIDEMETHODIMP  GetTexture(
                _Outptr_ ID3D11Texture2D** ppTexture,
                _Out_ UINT *uiViewIndex);

            // TODO: Troy consider removing?
            /*
            IFACEMETHOD_(IMFMediaBuffer*, GetMediaBuffer) () const { return _mfMediaBuffer.get(); }
            IFACEMETHOD_(BYTE*, GetBuffer) () const { return _byteBuffer + _bufferOffset; }
            IFACEMETHOD_(DWORD, GetOffset) () const { return _bufferOffset; }
            */
        private:
            void Initialize(_In_ IMFMediaBuffer* pMediaBuffer);

            com_ptr<IMFMediaBuffer>  _mfMediaBuffer;
            com_ptr<IMF2DBuffer>     _mf2DBuffer;
            //com_ptr<IMFDXGIBuffer>   _mfDXGIBuffer;

            BYTE* _byteBuffer;
            DWORD _bufferOffset;
    };
}

namespace winrt::RealtimeStreaming::Network::factory_implementation
{
    struct DataBuffer : DataBufferT<DataBuffer, implementation::DataBuffer>
    {
    };
}
