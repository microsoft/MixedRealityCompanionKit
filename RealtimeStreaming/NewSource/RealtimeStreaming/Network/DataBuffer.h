// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "Generated Files\Network\DataBuffer.g.h"

#include <mfapi.h>
#include <mfidl.h>
#include <mferror.h>

namespace winrt::RealtimeStreaming::Network::implementation
{
    struct DataBuffer : DataBufferT<DataBuffer, Windows::Storage::Streams::IBuffer, Windows::Storage::Streams::IBufferByteAccess>
    {
        public:
            DataBuffer(_In_ DWORD dwMaxLength);
            DataBuffer(_In_ IMFMediaBuffer* pMediaBuffer);
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
            DataBuffer TrimRight(_In_ ULONG cbSize);

            void Reset();

            // DataBufferImpl
            STDMETHODIMP GetMediaBuffer(
                _COM_Outptr_ IMFMediaBuffer** mfBuffer);

            BYTE* GetBufferPointer();

            STDMETHODIMP get_Texture(
                _Outptr_ ID3D11Texture2D** ppTexture,
                _Out_ UINT *uiViewIndex);

            // TODO: Troy consider removing?
            /*
            IFACEMETHOD_(IMFMediaBuffer*, GetMediaBuffer) () const { return _mfMediaBuffer.get(); }
            IFACEMETHOD_(BYTE*, GetBuffer) () const { return _byteBuffer + _bufferOffset; }
            IFACEMETHOD_(DWORD, GetOffset) () const { return _bufferOffset; }
            */
        private:
            com_ptr<IMFMediaBuffer>  _mfMediaBuffer;
            com_ptr<IMF2DBuffer>     _mf2DBuffer;
            //com_ptr<IMFDXGIBuffer>   _mfDXGIBuffer;

            BYTE* _byteBuffer;
            DWORD _bufferOffset;
        };

    }
}