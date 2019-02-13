// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "DataBundle.h"

DataBundle::~DataBundle()
{
    Log(Log_Level_All, L"DataBundleImpl::~DataBundleImpl()\n");

    Reset();
}

_Use_decl_annotations_
DataBundle::DataBundle()
{
    Log(Log_Level_All, L"DataBundleImpl::DataBundle()\n");

    m_buffers.clear();
}

_Use_decl_annotations_
DataBundle::DataBundle(
    IMFSample* mediaSample)
{
    Log(Log_Level_All, L"DataBundleImpl::DataBundle(imfsample)\n");

    IFT(mediaSample);

    m_buffers.clear();

    DWORD bufferCount = 0;
    IFT(mediaSample->GetBufferCount(&bufferCount));

    // for every buffer, create a DataBuffer
    for (DWORD index = 0; index < bufferCount; ++index)
    {
        // get the media buffer
        com_ptr<IMFMediaBuffer> spMediaBuffer;
        IFT(mediaSample->GetBufferByIndex(index, &spMediaBuffer));

        // create the dataBuffer
        DataBuffer databuffer = DataBuffer(spMediaBuffer.get());

        // add the buffer to bundle
        AddBuffer(databuffer);
    }
}

// IDataBundle
_Use_decl_annotations_
UINT32 DataBundle::BufferCount()
{
    return m_buffers.size();
}

UINT64 DataBundle::TotalSize()
{
    DWORD cbSize = 0;
    for (auto const& currBuffer : m_buffers)
    {
        cbSize += currBuffer.CurrentLength();
    }

    return cbSize;
}

_Use_decl_annotations_
void DataBundle::AddBuffer(
    DataBuffer const& dataBuffer)
{
    m_buffers.push_back(dataBuffer);
}

_Use_decl_annotations_
bool DataBundle::InsertBuffer(
    UINT32 index, 
    DataBuffer dataBuffer)
{
    if (m_buffers.size() < index)
    {
        return false;
    }

    m_buffers.insert(m_buffers.begin() + i, dataBuffer);

    return true;
}

_Use_decl_annotations_
bool DataBundle::RemoveBuffer(
    DataBuffer dataBuffer)
{
    if (m_buffers.size() == 0)
    {
        return false;
    }

    Iterator it = m_buffers.begin();
    Iterator itEnd = m_buffers.end();

    for (; ((*it) != dataBuffer) && it != itEnd; ++it);

    m_buffers.erase(it);

    return true;
}

_Use_decl_annotations_
void DataBundle::Reset(void)
{
    m_buffers.clear();
}

// DataBundleImpl
_Use_decl_annotations_
UINT32 DataBundle::CopyTo(
    DWORD nOffset, 
    DWORD cbSize, 
    void* pDest)
{
    NULL_CHK(pDest);

    Iterator it = m_buffers.begin();
    Iterator itEnd = m_buffers.end();

    DWORD cbSkipped = 0;
    DWORD cbCopied = 0;

    // Skip to the offset
    for (; cbSkipped < nOffset && it != itEnd; ++it)
    {
        DataBuffer buffer = *it;
        DWORD cbLen buffer.CurrentLength();
        DWORD nStart = 0;
        DWORD cbCopy = 0;

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

        // get the raw byte pointer
        BYTE *pBuffer = nullptr;
        buffer.Buffer(&pBuffer);
        NULL_CHK(buffer);

        CopyMemory(pDest, pBuffer + nStart, cbCopy);

        cbCopied = cbCopy;
        cbSkipped += nStart;
    }

    for (; it != itEnd && cbCopied < cbSize; ++it)
    {
        DataBuffer dataBuffer = *it;

        DWORD cbLen = dataBuffer.CurrentLength();
        DWORD cbCopy = min(cbLen, cbSize - cbCopied);

        // get the raw byte pointer
        BYTE *pBuffer = nullptr;
        dataBuffer.Buffer(&pBuffer)
        NULL_CHK(pBuffer);

        CopyMemory(static_cast<BYTE *>(pDest) + cbCopied, pBuffer, cbCopy);

        cbCopied += cbCopy;
    }

   return cbCopied;
}

_Use_decl_annotations_
void DataBundle::MoveLeft(
    DWORD cbSize, 
    void* pDest)
{
    ULONG cbCopied = DataBundle::CopyTo(0, cbSize, pDest, &cbCopied);

    if (cbCopied != cbSize)
    {
        check_hresult(E_INVALIDARG);
    }

    TrimLeft(cbSize);
}

_Use_decl_annotations_
void DataBundle::TrimLeft(
    DWORD cbSize)
{
    DWORD cbTotalLength = TotalSize();

    if (cbSize > cbTotalLength)
    {
        check_hresult(E_INVALIDARG);
    }

    Iterator it = m_buffers.begin();
    Iterator itEnd = m_buffers.end();
    ULONG cbSkipped = 0;

    for (; cbSkipped < cbSize; it = m_buffers.begin())
    {
        DataBuffer dataBuffer = (*it);
        ULONG cbLen = dataBuffer.CurrentLength();
        ULONG nStart = 0, cbCopy = 0;
        
        if (cbSkipped + cbLen <= cbSize)
        {
            m_buffers.erase(m_buffers.begin());
            cbSkipped += cbLen;
        }
        else
        {
            // skip the rest
            dataBuffer.Trimleft(cbSize - cbSkipped);
            break;
        }
    }
}

_Use_decl_annotations_
HRESULT DataBundle::ToMFSample(
    IMFSample** ppSample)
{
    com_ptr<IMFSample> spSample;

    IFR(MFCreateSample(&spSample));

    Container::iterator it = m_buffers.begin();
    Container::iterator endIt = m_buffers.end();

    for (; it != endIt; ++it)
    {
        DataBuffer dataBuffer = *it;
        com_ptr<IMFMediaBuffer> spMediaBuffer;

        DWORD offset = dataBuffer.Offset();
        if (offset == 0)
        {
            // If offset is zero we can just use embedded media buffer
            spMediaBuffer = dataBuffer.GetMediaBuffer();
        }
        else
        {
            // We have to create MF media buffer wrapper to include offset
            DWORD cbLen = dataBuffer.CurrentLength();

            IFR(MFCreateMediaBufferWrapper(dataBuffer.GetMediaBuffer(),
                dataBuffer.Offset(),
                cbLen,
                &spMediaBuffer));
        }

        // Add media buffer to the sample
        IFR(spSample->AddBuffer(spMediaBuffer.get()));
    }

    *ppSample = spSample.detach();

    return S_OK;
}
