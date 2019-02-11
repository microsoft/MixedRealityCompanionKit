// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "DataBuffer.h"

DataBufferImpl::DataBufferImpl()
    : _mfMediaBuffer(nullptr)
    , _mf2DBuffer(nullptr)
    , _byteBuffer(nullptr)
    , _bufferOffset(0)
{
}

DataBufferImpl::~DataBufferImpl()
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


_Use_decl_annotations_
HRESULT DataBufferImpl::RuntimeClassInitialize(
    DWORD maxLength)
{
    Log(Log_Level_All, L"DataBufferImpl::RuntimeClassInitialize(DWORD)\n");

    com_ptr<IMFMediaBuffer> spMediaBuffer;
    check_hresult(MFCreateMemoryBuffer(maxLength, &spMediaBuffer));

    return RuntimeClassInitialize(spMediaBuffer.get());
}

_Use_decl_annotations_
HRESULT DataBufferImpl::RuntimeClassInitialize(
    IMFMediaBuffer* pMediaBuffer)
{
    Log(Log_Level_All, L"DataBufferImpl::RuntimeClassInitialize(IMFMediaBuffer)\n");

    NULL_CHK(pMediaBuffer);

    com_ptr<IMFMediaBuffer> spMediaBuffer(pMediaBuffer);
    HRESULT hr = spMediaBuffer.As(&_mf2DBuffer);
    if (SUCCEEDED(hr))
    {
        LONG lPitch;
        check_hresult(_mf2DBuffer->Lock2D(&_byteBuffer, &lPitch));
    }
    else
    {
        DWORD cbMaxLength;
        DWORD cbCurrentLength;
        check_hresult(spMediaBuffer->Lock(&_byteBuffer, &cbMaxLength, &cbCurrentLength));
    }

    return spMediaBuffer.As(&_mfMediaBuffer);
}


// IBuffer
_Use_decl_annotations_
HRESULT DataBufferImpl::get_Capacity(UINT32 *value)
{
    NULL_CHK(value);

    DWORD maxLength = 0;
    check_hresult(_mfMediaBuffer->GetMaxLength(&maxLength));

    *value = maxLength;

    return S_OK;
}

_Use_decl_annotations_
HRESULT DataBufferImpl::get_Length(UINT32 *value)
{
    NULL_CHK(value);

    DWORD length = 0;
    check_hresult(get_CurrentLength(&length));

    *value = length;

    return S_OK;
}

_Use_decl_annotations_
HRESULT DataBufferImpl::put_Length(
    UINT32 value)
{
    return put_CurrentLength(value);
}


// IBufferByteAccess methods
_Use_decl_annotations_
HRESULT DataBufferImpl::Buffer(
    BYTE** buffer)
{
    return get_Buffer(buffer);
}


// IDataBuffer
_Use_decl_annotations_
HRESULT DataBufferImpl::get_Offset(
    DWORD *offset)
{
    NULL_CHK(offset);

    *offset = _bufferOffset;

    return S_OK;
}

_Use_decl_annotations_
HRESULT DataBufferImpl::put_Offset(
    DWORD offset)
{
    // validate the offset is within bounds
    DWORD cbCurrentLength;
    check_hresult(_mfMediaBuffer->GetCurrentLength(&cbCurrentLength));
    if (cbCurrentLength < offset)
    {
        check_hresult(E_INVALIDARG);
    }

    _bufferOffset = offset;

    return S_OK;
}

_Use_decl_annotations_
HRESULT DataBufferImpl::get_CurrentLength(
    DWORD *currentLength)
{
    NULL_CHK(currentLength);

    DWORD cbCurrentLength;
    check_hresult(_mfMediaBuffer->GetCurrentLength(&cbCurrentLength));

    if (cbCurrentLength <= _bufferOffset)
    {
        cbCurrentLength = 0;
    }
    else
    {
        cbCurrentLength -= _bufferOffset;
    }

    *currentLength = cbCurrentLength;

    return S_OK;
}

_Use_decl_annotations_
HRESULT DataBufferImpl::put_CurrentLength(
    DWORD currentLength)
{
    DWORD cbMaxLen = 0;
    check_hresult(_mfMediaBuffer->GetMaxLength(&cbMaxLen));
    if (_bufferOffset > cbMaxLen)
    {
        _bufferOffset = cbMaxLen;
    }

    if (currentLength > (cbMaxLen - _bufferOffset))
    {
        check_hresult(E_INVALIDARG);
    }

    return _mfMediaBuffer->SetCurrentLength(currentLength + _bufferOffset);
}

_Use_decl_annotations_
HRESULT DataBufferImpl::TrimLeft(
    DWORD cbSize)
{
    DWORD cbCurrentLen;
    check_hresult(get_CurrentLength(&cbCurrentLen));

    if (cbCurrentLen >= cbSize)
    {
        _bufferOffset += cbSize;
    }
    else
    {
        check_hresult(E_INVALIDARG);
    }

    return S_OK;
}

_Use_decl_annotations_
HRESULT DataBufferImpl::Reset()
{
    _bufferOffset = 0;

    return _mfMediaBuffer->SetCurrentLength(0);
}

// DataBufferImpl methods

// Trims cbSize bytes from the right end and returns them as a new buffer
_Use_decl_annotations_
HRESULT DataBufferImpl::TrimRight(
    DWORD cbSize,
    IDataBuffer** dataBuffer)
{
    NULL_CHK(dataBuffer);

    *dataBuffer = nullptr;

    com_ptr<IDataBuffer> spResult;

    DWORD cbCurrentLen;
    HRESULT hr = this->get_CurrentLength(&cbCurrentLen);

    // Offset cannot be larger than current length.
    if (SUCCEEDED(hr) && cbCurrentLen <= cbSize)
    {
        return E_INVALIDARG;
    }

    // Create result buffer representing at the end of the current one
    if (SUCCEEDED(hr))
    {
        com_ptr<IMFMediaBuffer> spMediaBuffer;
        hr = MFCreateMediaBufferWrapper(GetMediaBuffer(), GetOffset() + cbCurrentLen - cbSize, cbSize, &spMediaBuffer);

        if (SUCCEEDED(hr))
        {
            hr = MakeAndInitialize<DataBufferImpl>(&spResult, spMediaBuffer.get());
        }
    }

    // Change the size of the length of the buffer
    if (SUCCEEDED(hr))
    {
        hr = put_CurrentLength(cbCurrentLen - cbSize);
    }

    if (SUCCEEDED(hr))
    {
        *dataBuffer = spResult.detach();
    }

    return hr;
}

_Use_decl_annotations_
HRESULT DataBufferImpl::get_MediaBuffer(
    IMFMediaBuffer** ppMediaBuffer)
{
    NULL_CHK_HR(_mfMediaBuffer, E_NOT_SET);

    return _mfMediaBuffer.CopyTo(ppMediaBuffer);
}

_Use_decl_annotations_
HRESULT DataBufferImpl::get_Buffer(
    BYTE** buffer)
{
    *buffer = _byteBuffer + _bufferOffset;

    return S_OK;
}

_Use_decl_annotations_
HRESULT DataBufferImpl::get_Texture(
    ID3D11Texture2D** ppTexture,
    UINT *subIndex)
{
    com_ptr<IMFDXGIBuffer> spDXGIBuffer;
    check_hresult(_mfMediaBuffer.As(&spDXGIBuffer));

    // get resourceView
    check_hresult(spDXGIBuffer->GetResource(__uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(ppTexture)));

    HRESULT hr = S_OK;

    // if requested
    if (nullptr != subIndex)
    {
        hr = spDXGIBuffer->GetSubresourceIndex(subIndex);
    }

    return hr;
}
