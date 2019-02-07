// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

namespace RealtimeStreaming
{
    namespace Network
    {

        class DataBufferImpl
            : public RuntimeClass
            < RuntimeClassFlags<RuntimeClassType::WinRtClassicComMix>
            , ABI::RealtimeStreaming::Network::IDataBuffer
            , ABI::Windows::Storage::Streams::IBuffer
            , Windows::Storage::Streams::IBufferByteAccess
            , FtmBase >
        {
            InspectableClass(RuntimeClass_RealtimeStreaming_Network_DataBuffer, BaseTrust);

        public:
            DataBufferImpl();
            ~DataBufferImpl();

            STDMETHODIMP RuntimeClassInitialize(
                _In_ DWORD dwMaxLength);
            STDMETHODIMP RuntimeClassInitialize(
                _In_ IMFMediaBuffer* pMediaBuffer);

            // IBuffer
            IFACEMETHOD(get_Capacity)(
                _Out_ UINT32 *value);
            IFACEMETHOD(get_Length)(
                _Out_ UINT32 *value);
            IFACEMETHOD(put_Length)(
                _In_ UINT32 value);

            // IBufferByteAccess
            IFACEMETHOD(Buffer)(
                _Outptr_ BYTE** ppBuffer);

            // IDataBuffer
            IFACEMETHOD(get_Offset)(
                _Out_ ULONG* offset) override;
            IFACEMETHOD(put_Offset)(
                _In_ ULONG offset) override;

            IFACEMETHOD(get_CurrentLength)(
                _Out_ ULONG *currentLength) override;
            IFACEMETHOD(put_CurrentLength)(
                _In_ ULONG currentLength) override;

            IFACEMETHOD(TrimLeft)(
                _In_ ULONG cbSize) override;
            IFACEMETHOD(TrimRight)(
                _In_ ULONG cbSize,
                _COM_Outptr_ IDataBuffer** buffer) override;

            IFACEMETHOD(Reset)(void) override;

            // DataBufferImpl
            STDMETHODIMP get_MediaBuffer(
                _COM_Outptr_ IMFMediaBuffer** mfBuffer);

            STDMETHODIMP get_Texture(
                _Outptr_ ID3D11Texture2D** ppTexture,
                _Out_ UINT *uiViewIndex);

            IFACEMETHOD_(IMFMediaBuffer*, GetMediaBuffer) () const { return _mfMediaBuffer.Get(); }
            IFACEMETHOD_(BYTE*, GetBuffer) () const { return _byteBuffer + _bufferOffset; }
            IFACEMETHOD_(DWORD, GetOffset) () const { return _bufferOffset; }

            STDMETHODIMP get_Buffer(
                _Outptr_ BYTE** buffer);

        private:
            ComPtr<IMFMediaBuffer>  _mfMediaBuffer;
            ComPtr<IMF2DBuffer>     _mf2DBuffer;
            ComPtr<IMFDXGIBuffer>   _mfDXGIBuffer;

            BYTE* _byteBuffer;
            DWORD _bufferOffset;
        };

    }
}