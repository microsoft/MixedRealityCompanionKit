// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "DataBuffer.h"

_Use_decl_annotations_
DataBuffer::DataBuffer(
    DWORD maxLength)
{
    Log(Log_Level_All, L"DataBufferImpl::DataBuffer(DWORD)\n");

    com_ptr<IMFMediaBuffer> spMediaBuffer;
    IFT(MFCreateMemoryBuffer(maxLength, &spMediaBuffer));

    DataBuffer(spMediaBuffer.get());
}

_Use_decl_annotations_
DataBuffer::DataBuffer(
    IMFMediaBuffer* pMediaBuffer)
    , _bufferOffset(0)
{
    Log(Log_Level_All, L"DataBufferImpl::DataBuffer(IMFMediaBuffer)\n");

    NULL_CHK(pMediaBuffer);

    com_ptr<IMFMediaBuffer> spMediaBuffer(pMediaBuffer);
    HRESULT hr = spMediaBuffer.As(&_mf2DBuffer);

    if (SUCCEEDED(hr))
    {
        LONG lPitch;
        IFT(_mf2DBuffer->Lock2D(&_byteBuffer, &lPitch));
    }
    else
    {
        DWORD cbMaxLength;
        DWORD cbCurrentLength;
        IFT(spMediaBuffer->Lock(&_byteBuffer, &cbMaxLength, &cbCurrentLength));
    }

    IFT(spMediaBuffer.As(&_mfMediaBuffer));
}

DataBuffer::~DataBuffer()
{
    Log(Log_Level_All, L"DataBufferImpl::~DataBufferImpl()\n");

    if (nullptr != _mfMediaBuffer)
    {
        if (nullptr != _mf2DBuffer)
        {
            _mf2DBuffer->Unlock2D();
            _mf2DBuffer.Reset();
            _mf2DBuffer = nullptr;
        }
        else
        {
            _mfMediaBuffer->Unlock();
        }
        _mfMediaBuffer.Reset();
        _mfMediaBuffer = nullptr;
    }
}



// IBuffer
UINT32 DataBuffer::Capacity()
{
    DWORD maxLength = 0;
    IFT(_mfMediaBuffer->GetMaxLength(&maxLength));

    return maxLength;
}
UINT32 DataBuffer::Length()
{
    return CurrentLength();
}

void DataBuffer::Length(UINT32 value)
{
    CurrentLength(value);
}

// IBufferByteAccess methods
_Use_decl_annotations_
HRESULT DataBuffer::Buffer(
    BYTE** ppBuffer)
{
    *ppBuffer = GetBufferPointer();
    return S_OK;
}

// IDataBuffer
ULONG Offset()
{
    return _bufferOffset;
}

void Offset(ULONG value)
{
    // validate the offset is within bounds
    DWORD cbCurrentLength;
    IFT(_mfMediaBuffer->GetCurrentLength(&cbCurrentLength));
    if (cbCurrentLength < value)
    {
        IFT(E_INVALIDARG);
    }

    _bufferOffset = value;
}

ULONG CurrentLength()
{
    ULONG cbCurrentLength;
    IFT(_mfMediaBuffer->GetCurrentLength(&cbCurrentLength));

    if (cbCurrentLength <= _bufferOffset)
    {
        cbCurrentLength = 0;
    }
    else
    {
        cbCurrentLength -= _bufferOffset;
    }

    return cbCurrentLength;
}

void CurrentLength(ULONG len)
{
    DWORD cbMaxLen = 0;
    IFT(_mfMediaBuffer->GetMaxLength(&cbMaxLen));
    if (_bufferOffset > cbMaxLen)
    {
        _bufferOffset = cbMaxLen;
    }

    if (len > (cbMaxLen - _bufferOffset))
    {
        IFT(E_INVALIDARG);
    }

    IFT(_mfMediaBuffer->SetCurrentLength(len + _bufferOffset));
}


_Use_decl_annotations_
DataBuffer DataBuffer::TrimRight(ULONG cbSize)
{
    ULONG cbCurrentLen = CurrentLength();

    // Offset cannot be larger than current length.
    if (cbCurrentLen <= cbSize)
    {
        IFT(E_INVALIDARG);
    }

    // Create result buffer representing at the end of the current one
    com_ptr<IMFMediaBuffer> spMediaBuffer;
    IFT(MFCreateMediaBufferWrapper(GetMediaBuffer(), 
        GetOffset() + cbCurrentLen - cbSize, 
        cbSize, 
        &spMediaBuffer));

    DataBuffer dataBuffer = DataBuffer(spMediaBuffer.get());

    // Change the size of the length of the buffer
    CurrentLength(cbCurrentLen - cbSize)

    return dataBuffer;
}

_Use_decl_annotations_
void DataBuffer::TrimLeft(ULONG cbSize)
{
    ULONG cbCurrentLen = CurrentLength();

    if (cbCurrentLen >= cbSize)
    {
        _bufferOffset += cbSize;
    }
    else
    {
        IFT(E_INVALIDARG);
    }
}

_Use_decl_annotations_
void DataBuffer::Reset()
{
    _bufferOffset = 0;

    IFT(_mfMediaBuffer->SetCurrentLength(0));
}

// DataBufferImpl methods

_Use_decl_annotations_
HRESULT DataBuffer::get_MediaBuffer(
    IMFMediaBuffer** ppMediaBuffer)
{
    NULL_CHK_HR(_mfMediaBuffer, E_NOT_SET);

    return _mfMediaBuffer.CopyTo(ppMediaBuffer);
}

_Use_decl_annotations_
BYTE* DataBuffer::GetBufferPointer()
{
    return _byteBuffer + _bufferOffset;
}

_Use_decl_annotations_
HRESULT DataBuffer::get_Texture(
    ID3D11Texture2D** ppTexture,
    UINT *subIndex)
{
    com_ptr<IMFDXGIBuffer> spDXGIBuffer;
    IFT(_mfMediaBuffer.As(&spDXGIBuffer));

    // get resourceView
    IFT(spDXGIBuffer->GetResource(__uuidof(ID3D11Texture2D), 
        reinterpret_cast<LPVOID*>(ppTexture)));

    // if requested
    if (nullptr != subIndex)
    {
        IFT(pDXGIBuffer->GetSubresourceIndex(subIndex));
    }

    return S_OK;
}
