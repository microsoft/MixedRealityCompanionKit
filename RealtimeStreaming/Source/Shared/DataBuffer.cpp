// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "DataBuffer.h"

using namespace winrt;
using namespace winrt::RealtimeStreaming::Network::implementation;
using namespace winrt::Windows::Storage::Streams;

/* static */
BYTE* DataBuffer::GetBufferPointer(
    _In_ RealtimeStreaming::Network::DataBuffer const& dataBuffer)
{
    BYTE* pBuffer;
    auto implBuffer = dataBuffer.try_as<RealtimeStreaming::Network::implementation::DataBuffer>();
    if (implBuffer != nullptr)
    {
        implBuffer->Buffer(&pBuffer);
        return pBuffer;
    }

    return nullptr;
}

_Use_decl_annotations_
DataBuffer::DataBuffer(
    DWORD maxLength)
    : _bufferOffset(0)
    , _byteBuffer(nullptr)
{
    Log(Log_Level_Verbose, L"DataBufferImpl::DataBuffer(DWORD)\n");

    com_ptr<IMFMediaBuffer> spMediaBuffer;
    IFT(MFCreateMemoryBuffer(maxLength, spMediaBuffer.put()));

    Initialize(spMediaBuffer.get());
}

_Use_decl_annotations_
DataBuffer::DataBuffer(
    IMFMediaBuffer* pMediaBuffer)
    : _bufferOffset(0)
    , _byteBuffer(nullptr)
{
    Log(Log_Level_Verbose, L"DataBufferImpl::DataBuffer(IMFMediaBuffer)\n");

    Initialize(pMediaBuffer);
}

_Use_decl_annotations_
DataBuffer::DataBuffer(
    IBuffer const& copyFromBuffer)
    : _bufferOffset(0)
    , _byteBuffer(nullptr)
{
    Log(Log_Level_Verbose, L"DataBufferImpl::DataBuffer(IBuffer)\n");

    UINT32 bufferLen = copyFromBuffer.Length();

    com_ptr<IMFMediaBuffer> spMediaBuffer;
    IFT(MFCreateMemoryBuffer(bufferLen, spMediaBuffer.put()));

    Initialize(spMediaBuffer.get());

    BYTE* pInData;
    auto bufferReader = copyFromBuffer.as<IBufferByteAccess>();
    bufferReader->Buffer(&pInData);

    memcpy(_byteBuffer, pInData, bufferLen);

    CurrentLength(bufferLen);
}

DataBuffer::~DataBuffer()
{
    Log(Log_Level_Verbose, L"DataBufferImpl::~DataBufferImpl()\n");

    if (nullptr != _mfMediaBuffer)
    {
        if (nullptr != _mf2DBuffer)
        {
            _mf2DBuffer->Unlock2D();
            _mf2DBuffer = nullptr;
        }
        else
        {
            _mfMediaBuffer->Unlock();
        }

        _mfMediaBuffer = nullptr;
    }
}

_Use_decl_annotations_
void DataBuffer::Initialize(IMFMediaBuffer* pMediaBuffer)
{
    NULL_THROW(pMediaBuffer);

    _mfMediaBuffer.copy_from(pMediaBuffer);

    auto imf2dBuffer = _mfMediaBuffer.try_as<IMF2DBuffer>();
    if (imf2dBuffer != nullptr)
    {
        _mf2DBuffer = imf2dBuffer;
        LONG lPitch;

        // Get us access to the memory in the buffer, for reading or writing.
        IFT(_mf2DBuffer->Lock2D(&_byteBuffer, &lPitch));
    }
    else
    {
        DWORD cbMaxLength;
        DWORD cbCurrentLength;
        
        // Get us access to the memory in the buffer, for reading or writing.
        IFT(_mfMediaBuffer->Lock(&_byteBuffer, &cbMaxLength, &cbCurrentLength));
    }
}

// IBuffer
_Use_decl_annotations_
UINT32 DataBuffer::Capacity()
{
    DWORD maxLength = 0;
    IFT(_mfMediaBuffer->GetMaxLength(&maxLength));

    return maxLength;
}

_Use_decl_annotations_
UINT32 DataBuffer::Length()
{
    return CurrentLength();
}

_Use_decl_annotations_
void DataBuffer::Length(UINT32 value)
{
    CurrentLength(value);
}

// IBufferByteAccess methods
_Use_decl_annotations_
HRESULT DataBuffer::Buffer(
    BYTE** ppBuffer)
{
    *ppBuffer = _byteBuffer + _bufferOffset;
    return S_OK;
}

// IDataBuffer
ULONG DataBuffer::Offset()
{
    return _bufferOffset;
}

void DataBuffer::Offset(ULONG value)
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

ULONG DataBuffer::CurrentLength()
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

void DataBuffer::CurrentLength(ULONG len)
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
RealtimeStreaming::Network::DataBuffer DataBuffer::TrimRight(ULONG cbSize)
{
    ULONG cbCurrentLen = CurrentLength();

    // Offset cannot be larger than current length.
    if (cbCurrentLen <= cbSize)
    {
        IFT(E_INVALIDARG);
    }

    // Create result buffer representing at the end of the current one
    com_ptr<IMFMediaBuffer> existingMediaBuffer;
    GetMediaBuffer(existingMediaBuffer.put());

    com_ptr<IMFMediaBuffer> spMediaBuffer;
    IFT(MFCreateMediaBufferWrapper(existingMediaBuffer.get(),
        Offset() + cbCurrentLen - cbSize, 
        cbSize, 
        spMediaBuffer.put()));

    auto dataBuffer = winrt::make<Network::implementation::DataBuffer>(spMediaBuffer.get());

    // Change the size of the length of the buffer
    CurrentLength(cbCurrentLen - cbSize);

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
HRESULT DataBuffer::GetMediaBuffer(
    IMFMediaBuffer** ppMediaBuffer)
{
    NULL_CHK_HR(_mfMediaBuffer, E_NOT_SET);

    _mfMediaBuffer.copy_to(ppMediaBuffer);
}

_Use_decl_annotations_
HRESULT DataBuffer::GetTexture(
    ID3D11Texture2D** ppTexture,
    UINT *subIndex)
{
    com_ptr<IMFDXGIBuffer> spDXGIBuffer = _mfMediaBuffer.as<IMFDXGIBuffer>();

    // get resourceView
    IFR(spDXGIBuffer->GetResource(__uuidof(ID3D11Texture2D),
        reinterpret_cast<LPVOID*>(ppTexture)));

    // if requested
    if (nullptr != subIndex)
    {
        IFR(spDXGIBuffer->GetSubresourceIndex(subIndex));
    }

    return S_OK;
}
