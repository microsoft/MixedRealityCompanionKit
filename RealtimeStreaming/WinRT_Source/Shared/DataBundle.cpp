// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "DataBundle.h"
#include "DataBuffer.h"

using namespace winrt;
using namespace winrt::RealtimeStreaming::Network::implementation;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Networking::Sockets;

_Use_decl_annotations_
DataBundle::DataBundle()
{
    Log(Log_Level_All, L"DataBundleImpl::DataBundle()\n");

    m_buffers.clear();
}

DataBundle::DataBundle(
    RealtimeStreaming::Network::DataBuffer dataBuffer)
{
    Log(Log_Level_All, L"DataBundleImpl::DataBundle()\n");

    m_buffers.clear();

    AddBuffer(dataBuffer);
}

_Use_decl_annotations_
DataBundle::DataBundle(
    IMFSample* mediaSample)
{
    Log(Log_Level_All, L"DataBundleImpl::DataBundle(imfsample)\n");

    NULL_THROW(mediaSample);

    m_buffers.clear();

    DWORD bufferCount = 0;
    IFT(mediaSample->GetBufferCount(&bufferCount));

    // for every buffer, create a DataBuffer
    for (DWORD index = 0; index < bufferCount; ++index)
    {
        // get the media buffer
        com_ptr<IMFMediaBuffer> spMediaBuffer;
        IFT(mediaSample->GetBufferByIndex(index, spMediaBuffer.put()));

        // create the dataBuffer
        DataBuffer databuffer = DataBuffer(spMediaBuffer.get());

        // add the buffer to bundle
        AddBuffer(databuffer);
    }
}

DataBundle::~DataBundle()
{
    Log(Log_Level_All, L"DataBundleImpl::~DataBundleImpl()\n");

    Reset();
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
    RealtimeStreaming::Network::DataBuffer const& dataBuffer)
{
    m_buffers.push_back(dataBuffer);
}

_Use_decl_annotations_
bool DataBundle::InsertBuffer(uint32_t index,
    RealtimeStreaming::Network::DataBuffer const& dataBuffer)
{
    if (m_buffers.size() < index)
    {
        return false;
    }

    m_buffers.insert(m_buffers.begin() + index, dataBuffer);

    return true;
}

_Use_decl_annotations_
bool DataBundle::RemoveBuffer(
    RealtimeStreaming::Network::DataBuffer const& dataBuffer)
{
    if (m_buffers.size() == 0)
    {
        return false;
    }

    auto it = m_buffers.begin();
    auto itEnd = m_buffers.end();

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
HRESULT  DataBundle::CopyTo(
    DWORD nOffset, 
    DWORD cbSize, 
    void* pDest,
    UINT32* pCBCopied)
{
    NULL_CHK(pDest);

    auto it = m_buffers.begin();
    auto itEnd = m_buffers.end();

    DWORD cbSkipped = 0;
    DWORD cbCopied = 0;

    // Skip to the offset
    for (; cbSkipped < nOffset && it != itEnd; ++it)
    {
        auto buffer = (*it).as<implementation::DataBuffer>();
        DWORD cbLen = buffer->CurrentLength();
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
        buffer->Buffer(&pBuffer);
        NULL_CHK(buffer);

        CopyMemory(pDest, pBuffer + nStart, cbCopy);

        cbCopied = cbCopy;
        cbSkipped += nStart;
    }

    for (; it != itEnd && cbCopied < cbSize; ++it)
    {
        auto dataBuffer = (*it).as<implementation::DataBuffer>();

        DWORD cbLen = dataBuffer->CurrentLength();
        DWORD cbCopy = min(cbLen, cbSize - cbCopied);

        // get the raw byte pointer
        BYTE *pBuffer = nullptr;
        dataBuffer->Buffer(&pBuffer);
        NULL_THROW(pBuffer);

        CopyMemory(static_cast<BYTE *>(pDest) + cbCopied, pBuffer, cbCopy);

        cbCopied += cbCopy;
    }

   return cbCopied;
}

_Use_decl_annotations_
HRESULT  DataBundle::MoveLeft(
    DWORD cbSize, 
    void* pDest)
{
    UINT32 cbCopied;
    DataBundle::CopyTo(0, cbSize, pDest, &cbCopied);

    if (cbCopied != cbSize)
    {
        IFT(E_INVALIDARG);
    }

    TrimLeft(cbSize);
}

_Use_decl_annotations_
HRESULT  DataBundle::TrimLeft(
    DWORD cbSize)
{
    DWORD cbTotalLength = TotalSize();

    if (cbSize > cbTotalLength)
    {
        IFT(E_INVALIDARG);
    }

    auto it = m_buffers.begin();
    auto itEnd = m_buffers.end();
    ULONG cbSkipped = 0;

    for (; cbSkipped < cbSize; it = m_buffers.begin())
    {
        auto dataBuffer = (*it).as<implementation::DataBuffer>();
        ULONG cbLen = dataBuffer->CurrentLength();
        ULONG nStart = 0, cbCopy = 0;
        
        if (cbSkipped + cbLen <= cbSize)
        {
            m_buffers.erase(m_buffers.begin());
            cbSkipped += cbLen;
        }
        else
        {
            // skip the rest
            dataBuffer->TrimLeft(cbSize - cbSkipped);
            break;
        }
    }
}

_Use_decl_annotations_
HRESULT DataBundle::ToMFSample(
    IMFSample** ppSample)
{
    com_ptr<IMFSample> spSample;

    IFR(MFCreateSample(spSample.put()));

    auto it = m_buffers.begin();
    auto endIt = m_buffers.end();

    for (; it != endIt; ++it)
    {
        auto dataBuffer = (*it).as<Network::implementation::DataBuffer>();
        com_ptr<IMFMediaBuffer> spMediaBuffer;

        DWORD offset = dataBuffer->Offset();
        if (offset == 0)
        {
            // If offset is zero we can just use embedded media buffer
            dataBuffer->GetMediaBuffer(spMediaBuffer.put());
        }
        else
        {
            // We have to create MF media buffer wrapper to include offset
            DWORD cbLen = dataBuffer->CurrentLength();

            com_ptr<IMFMediaBuffer> internalMediaBuffer;
            IFR(dataBuffer->GetMediaBuffer(internalMediaBuffer.put()));

            IFR(MFCreateMediaBufferWrapper(internalMediaBuffer.get(),
                dataBuffer->Offset(),
                cbLen,
                spMediaBuffer.put()));
        }

        // Add media buffer to the sample
        IFR(spSample->AddBuffer(spMediaBuffer.get()));
    }

    *ppSample = spSample.detach();

    return S_OK;
}

std::vector<winrt::RealtimeStreaming::Network::DataBuffer> DataBundle::GetBuffers()
{
    return m_buffers;
}