// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "DataBundle.h"

DataBundleImpl::~DataBundleImpl()
{
    Log(Log_Level_All, L"DataBundleImpl::~DataBundleImpl()\n");

    Reset();
}

_Use_decl_annotations_
HRESULT DataBundleImpl::RuntimeClassInitialize()
{
    Log(Log_Level_All, L"DataBundleImpl::~RuntimeClassInitialize()\n");

    _buffers.clear();

    return S_OK;
}

_Use_decl_annotations_
HRESULT DataBundleImpl::RuntimeClassInitialize(
    IMFSample* mediaSample)
{
    Log(Log_Level_All, L"DataBundleImpl::~RuntimeClassInitialize(imfsample)\n");

    NULL_CHK(mediaSample);

    _buffers.clear();

    DWORD bufferCount = 0;
    IFR(mediaSample->GetBufferCount(&bufferCount));

    // for every buffer, create a DataBuffer
    for (DWORD index = 0; index < bufferCount; ++index)
    {
        // get the media buffer
        ComPtr<IMFMediaBuffer> spMediaBuffer;
        IFR(mediaSample->GetBufferByIndex(index, &spMediaBuffer));

        // create the dataBuffer
        ComPtr<IDataBuffer> spDataBuffer;
        IFR(MakeAndInitialize<DataBufferImpl>(&spDataBuffer, spMediaBuffer.Get()));

        // add the buffer to bundle
        IFR(AddBuffer(spDataBuffer.Get()));
    }

    return S_OK;
}

// IDataBundle
_Use_decl_annotations_
HRESULT DataBundleImpl::get_BufferCount(
    UINT32 *count)
{
    NULL_CHK(count);

    *count = _buffers.size();

    return S_OK;
}

_Use_decl_annotations_
HRESULT DataBundleImpl::get_TotalSize(
    ULONG *totalLength)
{
    NULL_CHK(totalLength);

    Iterator it = _buffers.begin();
    Iterator itEnd = _buffers.end();
    DWORD cbSize = 0;
    for (; it != itEnd; ++it)
    {
        DWORD cbLen;
        IFR((*it)->get_CurrentLength(&cbLen));

        cbSize += cbLen;
    }

    *totalLength = cbSize;

    return S_OK;
}

_Use_decl_annotations_
HRESULT DataBundleImpl::AddBuffer(
    IDataBuffer *dataBuffer)
{
    NULL_CHK(dataBuffer);

    _buffers.push_back(dataBuffer);

    return S_OK;
}

_Use_decl_annotations_
HRESULT DataBundleImpl::InsertBuffer(
    UINT32 index, 
    IDataBuffer *dataBuffer)
{
    NULL_CHK(dataBuffer);

    if (_buffers.size() < index)
    {
        IFR(E_INVALIDARG);
    }

    Iterator it = _buffers.begin();
    Iterator itEnd = _buffers.end();

    for (DWORD nCurrent = 1; nCurrent < index && it != itEnd; ++nCurrent, ++it);

    _buffers.insert(it, dataBuffer);

    return S_OK;
}

_Use_decl_annotations_
HRESULT DataBundleImpl::RemoveBuffer(
    IDataBuffer *dataBuffer)
{
    NULL_CHK(dataBuffer);

    if (_buffers.size() == 0)
    {
        IFR(E_INVALIDARG);
    }

    Iterator it = _buffers.begin();
    Iterator itEnd = _buffers.end();

    for (; ((*it).Get() != dataBuffer) && it != itEnd; ++it);

    _buffers.erase(it);

    return S_OK;
}

_Use_decl_annotations_
HRESULT DataBundleImpl::RemoveBufferByIndex(
    UINT32 index, 
    IDataBuffer** ppDataBuffer)
{
    NULL_CHK(ppDataBuffer);
    if (_buffers.size() < index)
    {
        IFR(E_INVALIDARG);
    }

    Iterator it = _buffers.begin();
    Iterator itEnd = _buffers.end();

    for (DWORD nCurrent = 1; nCurrent < index && it != itEnd; ++nCurrent, ++it);

    NULL_CHK_HR(*it, E_NOT_SET);

    IFR((*it).CopyTo(ppDataBuffer));

    _buffers.erase(it);

    return S_OK;
}

_Use_decl_annotations_
HRESULT DataBundleImpl::Reset(void)
{
    Iterator it = _buffers.begin();
    Iterator itEnd = _buffers.end();

    for (; it != itEnd; ++it)
    {
        (*it).Reset();
        (*it) = nullptr;
    }
    _buffers.clear();

    return S_OK;
}

// DataBundleImpl
_Use_decl_annotations_
HRESULT DataBundleImpl::CopyTo(
    DWORD nOffset, 
    DWORD cbSize, 
    void* pDest, 
    DWORD* pcbCopied)
{
    NULL_CHK(pDest);
    NULL_CHK(pcbCopied);

    Iterator it = _buffers.begin();
    Iterator itEnd = _buffers.end();

    DWORD cbSkipped = 0;
    DWORD cbCopied = 0;

    // Skip to the offset
    for (; cbSkipped < nOffset && it != itEnd; ++it)
    {
        DWORD cbLen;
        DWORD nStart = 0;
        DWORD cbCopy = 0;
        IFR((*it)->get_CurrentLength(&cbLen));

        if (cbSkipped + cbLen <= nOffset)
        {
            cbSkipped += cbLen;
            if (cbSkipped == nOffset)
            {
                break;
            }
            continue;
        }
        else
        {
            nStart = nOffset - cbSkipped;
            cbCopy = min(cbSize, cbLen - nStart);
        }

        // cast to IBufferByteAccess
        ComPtr<Windows::Storage::Streams::IBufferByteAccess> spByteAccess;
        IFR((*it).As(&spByteAccess));

        // get the raw byte pointer
        BYTE *buffer = nullptr;
        IFR(spByteAccess->Buffer(&buffer));
        NULL_CHK(buffer);

        CopyMemory(pDest, buffer + nStart, cbCopy);

        cbCopied = cbCopy;
        cbSkipped += nStart;
    }

    for (; it != itEnd && cbCopied < cbSize; ++it)
    {
        DWORD cbLen;
        IFR((*it)->get_CurrentLength(&cbLen));

        DWORD cbCopy = min(cbLen, cbSize - cbCopied);

        // cast to IBufferByteAccess
        ComPtr<Windows::Storage::Streams::IBufferByteAccess> spByteAccess;
        IFR((*it).As(&spByteAccess));

        // get the raw byte pointer
        BYTE *buffer = nullptr;
        IFR(spByteAccess->Buffer(&buffer));
        NULL_CHK(buffer);

        CopyMemory(static_cast<BYTE *>(pDest) + cbCopied, buffer, cbCopy);

        cbCopied += cbCopy;
    }

   * pcbCopied = cbCopied;

    return S_OK;
}

_Use_decl_annotations_
HRESULT DataBundleImpl::MoveLeft(
    DWORD cbSize, 
    void* pDest)
{
    ULONG cbCopied = 0;
    IFR(DataBundleImpl::CopyTo(0, cbSize, pDest, &cbCopied));

    if (cbCopied != cbSize)
    {
        IFR(E_INVALIDARG);
    }

    return TrimLeft(cbSize);
}

_Use_decl_annotations_
HRESULT DataBundleImpl::TrimLeft(
    DWORD cbSize)
{
    DWORD cbTotalLength = 0;
    IFR(get_TotalSize(&cbTotalLength));

    if (cbSize > cbTotalLength)
    {
        IFR(E_INVALIDARG);
    }

    Iterator it = _buffers.begin();
    Iterator itEnd = _buffers.end();
    ULONG cbSkipped = 0;

    for (; cbSkipped < cbSize; it = _buffers.begin())
    {
        ULONG cbLen;
        ULONG nStart = 0;
        ULONG cbCopy = 0;
        IFR((*it)->get_CurrentLength(&cbLen));

        if (cbSkipped + cbLen <= cbSize)
        {
            _buffers.pop_front();
            cbSkipped += cbLen;
        }
        else
        {
            // skip the rest
            IFR((*it)->TrimLeft(cbSize - cbSkipped));
            break;
        }
    }

    return S_OK;
}

_Use_decl_annotations_
HRESULT DataBundleImpl::ToMFSample(
    IMFSample** ppSample)
{
    ComPtr<IMFSample> spSample;

    HRESULT hr = MFCreateSample(&spSample);

    if (SUCCEEDED(hr))
    {
        Container::iterator it = _buffers.begin();
        Container::iterator endIt = _buffers.end();

        for (; it != endIt; ++it)
        {
            DataBufferImpl* pBuffer = static_cast<DataBufferImpl*>((*it).Get());
            ComPtr<IMFMediaBuffer> spMediaBuffer;

            DWORD offset;
            hr = pBuffer->get_Offset(&offset);
            if (SUCCEEDED(hr) && offset == 0)
            {
                // If offset is zero we can just use embedded media buffer
                spMediaBuffer = pBuffer->GetMediaBuffer();
            }
            else
            {
                // We have to create MF media buffer wrapper to include offset
                DWORD cbLen;
                hr = pBuffer->get_CurrentLength(&cbLen);
                if (FAILED(hr))
                {
                    break;
                }
                hr = MFCreateMediaBufferWrapper(pBuffer->GetMediaBuffer(), pBuffer->GetOffset(), cbLen, &spMediaBuffer);
                if (FAILED(hr))
                {
                    break;
                }
            }

            // Add media buffer to the sample
            hr = spSample->AddBuffer(spMediaBuffer.Get());
            if (FAILED(hr))
            {
                break;
            }
        }
    }

    if (SUCCEEDED(hr))
    {
       * ppSample = spSample.Detach();
    }

    return hr;
}
