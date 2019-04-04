// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "NetworkMediaSink.h"
#include "MediaUtils.h"
#include "NetworkMediaSinkStream.h"
#include "Connection.h"
#include "DataBuffer.h"
#include "DataBundle.h"

using namespace winrt;
using namespace RealtimeStreaming::Media::implementation;
using namespace RealtimeStreaming::Network;
using namespace RealtimeStreaming::Common;

using namespace Windows::Foundation;
using namespace Windows::Media::MediaProperties;
using namespace Windows::Media::Capture;

using implDataBuffer = winrt::RealtimeStreaming::Network::implementation::DataBuffer;

class ShutdownFunc
{
public:
    HRESULT operator()(_In_ IMFStreamSink* pStream) const
    {
        return static_cast<NetworkMediaSinkStream*>(pStream)->Shutdown();
    }
};

class ConnectedFunc
{
public:
    ConnectedFunc(bool fConnected, LONGLONG llStartTime)
        : _fConnected(fConnected)
        , m_llStartTime(llStartTime)
    {
    }

    HRESULT operator()(_In_ IMFStreamSink* pStream) const
    {
        return static_cast<NetworkMediaSinkStream*>(pStream)->ConnectedFunc(_fConnected, m_llStartTime);
    }

    bool _fConnected;
    LONGLONG m_llStartTime;
};

class StartFunc
{
public:
    StartFunc(LONGLONG llStartTime)
        : m_llStartTime(llStartTime)
    {
    }

    HRESULT operator()(_In_ IMFStreamSink* pStream) const
    {
        return static_cast<NetworkMediaSinkStream*>(pStream)->Start(m_llStartTime);
    }

    LONGLONG m_llStartTime;
};

class StopFunc
{
public:
    HRESULT operator()(_In_ IMFStreamSink* pStream) const
    {
        return static_cast<NetworkMediaSinkStream*>(pStream)->Stop();
    }
};

static HRESULT AddAttribute(
    _In_ GUID guidKey, 
    _In_ IPropertyValue const& propValue, 
    _In_ IMFAttributes *mfAttributes)
{
    NULL_CHK(mfAttributes);

    switch (propValue.Type())
    {
    case PropertyType::UInt8Array:
    {
        com_array<byte> buffer;
        propValue.GetUInt8Array(buffer);
        
        IFT(mfAttributes->SetBlob(guidKey, buffer.data(), buffer.size()));
    }
    break;

    case PropertyType::Double:
    {
        IFT(mfAttributes->SetDouble(guidKey, propValue.GetDouble()));
    }
    break;

    case PropertyType::Guid:
    {
        GUID value = propValue.GetGuid();
        IFT(mfAttributes->SetGUID(guidKey, value));
    }
    break;

    case PropertyType::String:
    {
        winrt::hstring value = propValue.GetString();

        IFT(mfAttributes->SetString(guidKey, value.c_str()));
    }
    break;

    case PropertyType::UInt32:
    {
        IFT(mfAttributes->SetUINT32(guidKey, propValue.GetUInt32()));
    }
    break;

    case PropertyType::UInt64:
    {
        IFT(mfAttributes->SetUINT64(guidKey, propValue.GetUInt64()));
    }
    break;

    default:
        //ignore unknown values
        break;
    };

    return S_OK;
}

inline HRESULT GetStreamId(
    _In_ Windows::Media::Capture::MediaStreamType mediaStreamType,
    _Out_ DWORD* streamId)
{
    NULL_CHK(streamId);

    *streamId = -1;

    switch (mediaStreamType)
    {
    case Windows::Media::Capture::MediaStreamType::VideoRecord:
        *streamId = 0;
        break;
    case Windows::Media::Capture::MediaStreamType::Audio:
        *streamId = 1;
        break;
    default:
        return E_INVALIDARG;
    };

    return S_OK;
}

template <class TFunc>
inline HRESULT ExecutePerStream(std::vector<com_ptr<IMFStreamSink>> &list, TFunc fn)
{
    HRESULT hr = S_OK;

    for (auto const& stream : list)
    {
        hr = fn(stream.get());
        if (FAILED(hr))
        {
            return hr;
        }
    }

    return hr;
}

NetworkMediaSink::NetworkMediaSink(Connection connection)
    : m_llStartTime(0)
    , m_presentationClock(nullptr)
    , m_connection(connection)
{
    // Subscribe to connection data received event
    m_bundleReceivedEventToken = m_connection.Received({ this, &NetworkMediaSink::OnDataReceived });
}

NetworkMediaSink::~NetworkMediaSink()
{
    Shutdown();

    // disconnect from events
    m_connection.Received(m_bundleReceivedEventToken);
    m_connection = nullptr;
}

// IMFMediaSink
_Use_decl_annotations_
HRESULT NetworkMediaSink::GetCharacteristics(
    DWORD* pdwCharacteristics)
{
    NULL_CHK(pdwCharacteristics);

    slim_shared_lock_guard const guard(m_lock);

    IFT(CheckShutdown());

    // Rateless sink.
   *pdwCharacteristics = MEDIASINK_RATELESS;

    return S_OK;
}

_Use_decl_annotations_
HRESULT NetworkMediaSink::AddStreamSink(
    DWORD dwStreamSinkIdentifier, 
    IMFMediaType* pMediaType, 
    IMFStreamSink** ppStreamSink)
{
    NULL_CHK(pMediaType);
    NULL_CHK(ppStreamSink);

    IFR(CheckShutdown());

    com_ptr<IMFStreamSink> spMFStream;
    if (SUCCEEDED(GetStreamSinkById(dwStreamSinkIdentifier, spMFStream.put())))
    {
        IFT(MF_E_STREAMSINK_EXISTS);
    }

    auto networkSinkStream = winrt::make<NetworkMediaSinkStream>(dwStreamSinkIdentifier, m_connection, *this);
    
    spMFStream = networkSinkStream.as<IMFStreamSink>();

    IFR(networkSinkStream.as<IMFMediaTypeHandler>()->SetCurrentMediaType(pMediaType));

    NULL_CHK_HR(spMFStream, E_NOT_SET);

    {
        slim_lock_guard guard(m_lock);

        // Insert in proper position
        auto it = m_streams.begin();
        for (; it != m_streams.end(); ++it)
        {
            DWORD dwCurrId;
            IFR((*it)->GetIdentifier(&dwCurrId));
            if (dwCurrId > dwStreamSinkIdentifier)
            {
                break;
            }
        }

        m_streams.insert(it, spMFStream);
    }

    spMFStream.copy_to(ppStreamSink);
    
    return S_OK;
}

_Use_decl_annotations_
HRESULT NetworkMediaSink::RemoveStreamSink(DWORD dwStreamSinkIdentifier)
{
    slim_lock_guard guard(m_lock);

    IFR(CheckShutdown());

    com_ptr<IMFStreamSink> spStream;

    // Erase from proper position
    auto it = m_streams.begin();
    for (; it != m_streams.end(); ++it)
    {
        DWORD dwCurrId;
        IFR((*it)->GetIdentifier(&dwCurrId));
        if (dwCurrId == dwStreamSinkIdentifier)
        {
            break;
        }
    }

    if (it == m_streams.end())
    {
        return MF_E_INVALIDSTREAMNUMBER;
    }

    m_streams.erase(it);

    return static_cast<NetworkMediaSinkStream*>(spStream.get())->Shutdown();
}

_Use_decl_annotations_
HRESULT NetworkMediaSink::GetStreamSinkCount(DWORD* pcStreamSinkCount)
{
    NULL_CHK(pcStreamSinkCount);

    slim_shared_lock_guard const guard(m_lock);

    IFR(CheckShutdown());

   *pcStreamSinkCount = m_streams.size();

    return S_OK;
}

_Use_decl_annotations_
HRESULT NetworkMediaSink::GetStreamSinkByIndex(
    DWORD dwIndex, 
    IMFStreamSink** ppStreamSink)
{
    NULL_CHK(ppStreamSink);

    slim_shared_lock_guard const guard(m_lock);
    
    com_ptr<IMFStreamSink> spStream;
    DWORD const cStreams = m_streams.size();
    if (cStreams <= dwIndex)
    {
        IFR(MF_E_INVALIDINDEX);
    }

    IFR(CheckShutdown());

    auto it = m_streams.begin() + dwIndex;

    if (it == m_streams.end())
    {
        IFR(MF_E_UNEXPECTED);
    }

    (*it).copy_to(ppStreamSink);

    return S_OK;
}

_Use_decl_annotations_
HRESULT NetworkMediaSink::GetStreamSinkById(
    DWORD dwStreamSinkIdentifier,
    IMFStreamSink** ppStreamSink)
{
    NULL_CHK(ppStreamSink);

    slim_shared_lock_guard const guard(m_lock);

    IFR(CheckShutdown());

    com_ptr<IMFStreamSink> spResult;

    for (auto const& stream : m_streams)
    {
        DWORD dwCurrId;
        IFR(stream->GetIdentifier(&dwCurrId));
        if (dwCurrId == dwStreamSinkIdentifier)
        {
            spResult = stream;
            break;
        }
    }

    NULL_CHK_HR(spResult, MF_E_INVALIDSTREAMNUMBER);

    spResult.copy_to(ppStreamSink);
    
    return S_OK;
}

_Use_decl_annotations_
HRESULT NetworkMediaSink::SetPresentationClock(IMFPresentationClock* pPresentationClock)
{
    NULL_CHK(pPresentationClock);

    slim_lock_guard guard(m_lock);

    IFR(CheckShutdown());

    // If we already have a clock, remove ourselves from that clock's
    // state notifications.
    if (nullptr != m_presentationClock)
    {
        IFR(m_presentationClock->RemoveClockStateSink(this));
    }

    // Register ourselves to get state notifications from the new clock.
    if (pPresentationClock)
    {
        IFR(pPresentationClock->AddClockStateSink(this));
    }

    // Release the pointer to the old clock.
    // Store the pointer to the new clock.
    m_presentationClock.copy_from(pPresentationClock);

    return S_OK;
}

_Use_decl_annotations_
HRESULT NetworkMediaSink::GetPresentationClock(IMFPresentationClock** ppPresentationClock)
{
    NULL_CHK(ppPresentationClock)

        slim_shared_lock_guard const guard(m_lock);

    IFR(CheckShutdown());

    if (nullptr == m_presentationClock)
    {
        IFR(MF_E_NO_CLOCK); // There is no presentation clock.
    }

    NULL_CHK_HR(m_presentationClock, E_NOT_SET);

    // Return the pointer to the caller.
    m_presentationClock.copy_to(ppPresentationClock);
    
    return S_OK;
}

_Use_decl_annotations_
HRESULT NetworkMediaSink::Shutdown()
{
    if (nullptr == m_connection)
    {
        return MF_E_SHUTDOWN;
    }

    // clear all streams
    ExecutePerStream(m_streams, ShutdownFunc());
    m_streams.clear();

    // notify any connections that sink is shutdown
    SendStreamStopped();

    m_presentationClock = nullptr;

    m_evtClosed(*this, true);
}

// IMFClockStateSink
_Use_decl_annotations_
HRESULT NetworkMediaSink::OnClockStart(MFTIME hnsSystemTime, LONGLONG llClockStartOffset)
{
    slim_lock_guard guard(m_lock);

    IFR(CheckShutdown());

    Log(Log_Level_Info, L"NetworkSinkImpl::OnClockStart: ts=%I64d\n", llClockStartOffset);
        
    // Start each stream.
    m_llStartTime = llClockStartOffset;

    return ExecutePerStream(m_streams, StartFunc(llClockStartOffset));
}

_Use_decl_annotations_
HRESULT NetworkMediaSink::OnClockStop(MFTIME hnsSystemTime)
{
    slim_lock_guard guard(m_lock);

    IFR(CheckShutdown());

    // Stop each stream
    return ExecutePerStream(m_streams, StopFunc());
}

_Use_decl_annotations_
HRESULT NetworkMediaSink::OnClockPause(MFTIME hnsSystemTime)
{
    return MF_E_INVALID_STATE_TRANSITION;
}

_Use_decl_annotations_
HRESULT NetworkMediaSink::OnClockRestart(MFTIME hnsSystemTime)
{
    return MF_E_INVALID_STATE_TRANSITION;
}

_Use_decl_annotations_
HRESULT NetworkMediaSink::OnClockSetRate(MFTIME hnsSystemTime, float flRate)
{
    return S_OK;
}

_Use_decl_annotations_
winrt::event_token NetworkMediaSink::Closed(Windows::Foundation::EventHandler<bool> const& handler)
{
    slim_lock_guard guard(m_lock);

    return m_evtClosed.add(handler);
}

_Use_decl_annotations_
void NetworkMediaSink::Closed(winrt::event_token const& token)
{
    slim_lock_guard guard(m_lock);

    m_evtClosed.remove(token);
}
_Use_decl_annotations_
void NetworkMediaSink::StartNetwork()
{
    SendStreamReady();
}

void NetworkMediaSink::StopNetwork()
{
    SendStreamStopped();
}

_Use_decl_annotations_
void NetworkMediaSink::OnDataReceived(
    //RealtimeStreaming::Network::Connection const& sender,
    IInspectable const& sender,
    RealtimeStreaming::Network::DataBundleArgs const& args)
{
    Log(Log_Level_Info, L"NetworkMediaSink::OnDataReceived - Tid:%d \n", GetCurrentThreadId());

    HRESULT hr = S_OK;

    switch (args.PayloadType())
    {
    case PayloadType::RequestMediaDescription:
        SendDescription();
        break;
    case PayloadType::RequestMediaStart:
        // triggers the _connected state
        if (nullptr != m_presentationClock)
        {
            LOG_RESULT_MSG(m_presentationClock->GetTime(&m_llStartTime), L"NetworkMediaSink - MediaStartRequested, Not able to set start time from presentation clock");
        }
        IFT(ExecutePerStream(m_streams, ConnectedFunc(true, m_llStartTime)));
        break;
    case PayloadType::RequestMediaStop:
        IFT(ExecutePerStream(m_streams, ConnectedFunc(false, m_llStartTime)));
        break;
    };
}

_Use_decl_annotations_
HRESULT NetworkMediaSink::HandleError(
    HRESULT hr)
{
    if (FAILED(hr))
    {
        return Shutdown();
    }

    return S_OK;
}

_Use_decl_annotations_
winrt::fire_and_forget NetworkMediaSink::SendStreamReady()
{
    NULL_THROW(m_connection);

    co_await m_connection.SendPayloadTypeAsync(PayloadType::State_CaptureReady);
}

_Use_decl_annotations_
winrt::fire_and_forget NetworkMediaSink::SendStreamStopped()
{
    NULL_THROW(m_connection);

    co_await m_connection.SendPayloadTypeAsync(PayloadType::State_CaptureStopped);
}

_Use_decl_annotations_
winrt::fire_and_forget NetworkMediaSink::SendDescription()
{
    Log(Log_Level_Info, L"NetworkSinkImpl::SendDescription() begin...\n");

    if (m_streams.empty())
    {
        Log(Log_Level_Warning, L"NetworkSinkImpl::SendDescription() no streams added yet...\n");
        return;
    }

    // Size of the constant buffer header
    const UINT32 c_cStreams = m_streams.size();
    const UINT32 c_cbMediaDescriptionSize = sizeof(MediaDescription);
    const UINT32 c_cbStreamTypeDescription = sizeof(MediaTypeDescription);
    const UINT32 c_cbStreamTypeHeaderSize = c_cStreams * c_cbStreamTypeDescription;

    // Calculate size of attributes for each stream to get total payload size
    UINT32 cbPayloadSize = c_cbMediaDescriptionSize + c_cbStreamTypeHeaderSize;
    for (auto const& stream : m_streams)
    {
        auto spMediaTypeHandler = stream.as<NetworkMediaSinkStream>();

        UINT32 attributeBlobSize;
        spMediaTypeHandler->GetAttributesBlobSize(&attributeBlobSize);

        cbPayloadSize += attributeBlobSize;
    }

    // Create payload buffer
    DataBuffer payloadDataBuffer = winrt::make<implDataBuffer>(cbPayloadSize);
    payloadDataBuffer.CurrentLength(cbPayloadSize);

    BYTE* pPayloadBuffer = implDataBuffer::GetBufferPointer(payloadDataBuffer);

    // Media Description - how many streams and the total bytes for each streams attributes
    MediaDescription* pMediaDescription = reinterpret_cast<MediaDescription*>(pPayloadBuffer);
    NULL_THROW(pMediaDescription);
    ZeroMemory(pMediaDescription, c_cbMediaDescriptionSize);
    pMediaDescription->StreamCount = c_cStreams;
    pMediaDescription->StreamTypeHeaderSize = c_cbStreamTypeHeaderSize; // populates later

    // Prepare the MediaTypeDescription
    MediaTypeDescription* streamTypeHeaderPtr = reinterpret_cast<MediaTypeDescription*>(pPayloadBuffer + c_cbMediaDescriptionSize);
    NULL_THROW(streamTypeHeaderPtr);

    // get each streams MediaTypeDescription info and its attrbutes into the databuffer
    DWORD nStream = 0;
    auto it = m_streams.begin();
    BYTE* pCurrAttributes = pPayloadBuffer + c_cbMediaDescriptionSize + c_cbStreamTypeHeaderSize;
    for (; it != m_streams.end(); ++nStream, ++it)
    {
        // zero out the memory block for stream type description
        ZeroMemory(&streamTypeHeaderPtr[nStream], c_cbStreamTypeDescription);

        auto spMediaTypeHandler = (*it).as<NetworkMediaSinkStream>();
        NULL_THROW(spMediaTypeHandler);

        // Get the MediaType info from stream
        IFT(spMediaTypeHandler->FillStreamDescription(&streamTypeHeaderPtr[nStream], pCurrAttributes));

        // Move our attributes pointer ahead correctly
        UINT32 attributeBlobSize;
        spMediaTypeHandler->GetAttributesBlobSize(&attributeBlobSize);
        pCurrAttributes += attributeBlobSize;
    }

    // Prepare the bundle to send
    auto headerDataBuffer = Network::implementation::Connection::CreatePayloadHeaderBuffer(PayloadType::SendMediaDescription, cbPayloadSize);

    DataBundle dataBundle = winrt::make<Network::implementation::DataBundle>(headerDataBuffer);
    dataBundle.AddBuffer(payloadDataBuffer);

    co_await m_connection.SendBundleAsync(dataBundle);

    Log(Log_Level_Info, L"NetworkMediaSink::SendDescription finished()\n");
}

