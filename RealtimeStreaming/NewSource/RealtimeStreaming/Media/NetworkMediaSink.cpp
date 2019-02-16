// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "NetworkMediaSink.h"
#include "MediaUtils.h"
#include "NetworkMediaSinkStream.h"
#include "Network/Connection.h"
#include "Network/DataBuffer.h"

using namespace winrt;
using namespace RealtimeStreaming::Media::implementation;
using namespace RealtimeStreaming::Network;
using namespace RealtimeStreaming::Common;

using namespace Windows::Foundation;
using namespace Windows::Media::MediaProperties;
using namespace Windows::Media::Capture;

class ShutdownFunc
{
public:
    HRESULT operator()(_In_ IMFStreamSink* pStream) const
    {
        IFT(reinterpret_cast<winrt::IInspectable*>(request)->QueryInterface(spRequest.ReleaseAndGetAddressOf()));

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
    _In_ IPropertyValue propValue, 
    _In_ IMFAttributes *mfAttributes)
{
    NULL_CHK(mfAttributes);

    // store as a smart pointer
    com_ptr<IMFAttributes> spAttrib; 
    spAttrib.attach(mfAttributes);

    switch (propValue.Type)
    {
    case PropertyType::UInt8Array:
    {
        com_array<byte> buffer;
        propValue.GetUInt8Array(buffer);
        
        IFT(spAttrib->SetBlob(guidKey, buffer.data(), buffer.size()));
    }
    break;

    case PropertyType::Double:
    {
        IFT(spAttrib->SetDouble(guidKey, propValue.GetDouble()));
    }
    break;

    case PropertyType::Guid:
    {
        GUID value = propValue.GetGuid();
        IFT(spAttrib->SetGUID(guidKey, value));
    }
    break;

    case PropertyType::String:
    {
        winrt::hstring value = propValue.GetString();

        IFT(spAttrib->SetString(guidKey, value.c_str()));
    }
    break;

    case PropertyType::UInt32:
    {
        IFT(spAttrib->SetUINT32(guidKey, propValue.GetUInt32()));
    }
    break;

    case PropertyType::UInt64:
    {
        IFT(spAttrib->SetUINT64(guidKey, propValue.GetUInt64()));
    }
    break;

    default:
        //ignore unknown values
        break;
    };

    return S_OK;
}

HRESULT ConvertPropertiesToMediaType(
    _In_ IMediaEncodingProperties mediaEncodingProp,
    _Out_ IMFMediaType** ppMediaType)
{
    NULL_CHK(ppMediaType);

   *ppMediaType = nullptr;

    // get Iterator
   MediaPropertySet propSet = mediaEncodingProp.Properties;

   com_ptr<IMFMediaType> spMediaType;
   IFR(MFCreateMediaType(spMediaType.put()));

   for (auto& keyValuePair : propSet)
   {
       IFR(AddAttribute(keyValuePair.Key, keyValuePair.Value, spMediaType.get()));
   }
   
    IPropertyValue propValue = propSet.Lookup(MF_MT_MAJOR_TYPE).as<IPropertyValue>();
    GUID guiMajorType = propValue.GetGuid();

    if (guiMajorType != MFMediaType_Video && guiMajorType != MFMediaType_Audio)
    {
        IFR(E_UNEXPECTED);
    }

    NULL_CHK_HR(spMediaType, E_NOT_SET);

    spMediaType.copy_to(ppMediaType);
    
    return S_OK;
}

HRESULT GetStreamId(
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

NetworkMediaSink::NetworkMediaSink(IAudioEncodingProperties audioEncodingProperties,
    IVideoEncodingProperties videoEncodingProperties,
    Connection connection)
    : m_llStartTime(0)
    , _cStreamsEnded(0)
    , m_presentationClock(nullptr)
    , m_connection(connection)
{

    // Set up media streams
    if (nullptr != audioEncodingProperties)
    {
        IFT(SetMediaStreamProperties(MediaStreamType::Audio, audioEncodingProperties));
    }

    IFT(SetMediaStreamProperties(MediaStreamType::VideoRecord, 
        videoEncodingProperties));

    // subscribe to connection data
    // define callback
    auto bundleReceivedCallback = [&](_In_ Connection sender, _In_ DataBundleArgs args) -> HRESULT
    {
        Log(Log_Level_Info, L"NetworkSinkImpl::OnBundleReceived");

        HRESULT hr = S_OK;

        switch (args.PayloadType)
        {
        case PayloadType::RequestMediaDescription:
            IFC(SendDescription());
            break;
        case PayloadType::RequestMediaStart:
            // triggers the _connected state
            if (nullptr != m_presentationClock)
            {
                LOG_RESULT_MSG(m_presentationClock->GetTime(&m_llStartTime), L"NetworkSinkImpl - MediaStartRequested, Not able to set start time from presentation clock");
            }
            IFC(ForEach(m_streams, ConnectedFunc(true, m_llStartTime)));
            break;
        case PayloadType::RequestMediaStop:
            IFC(ForEach(m_streams, ConnectedFunc(false, m_llStartTime)));
            break;
        };

    done:
        return S_OK;
    });

    m_bundleReceivedEventToken = m_connection.Received(bundleReceivedCallback);

    IFT(SendCaptureReady());
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
    slim_lock_guard guard(m_lock);

    NULL_CHK(pMediaType);
    NULL_CHK(ppStreamSink);

    IFR(CheckShutdown());

    com_ptr<IMFStreamSink> spMFStream;
    if (SUCCEEDED(GetStreamSinkById(dwStreamSinkIdentifier, spMFStream.put())))
    {
        IFT(MF_E_STREAMSINK_EXISTS);
    }

    auto networkSinkStream = winrt::make<NetworkMediaSinkStream>(dwStreamSinkIdentifier, m_connection, *this);

    spMFStream = nullptr; //unseat just in case
    spMFStream = networkSinkStream.as<IMFStreamSink>();

    IFR(networkSinkStream.as<IMFMediaTypeHandler>()->SetCurrentMediaType(pMediaType));

    // Insert in proper position
    StreamContainer::POSITION pos = m_streams.FrontPosition();
    StreamContainer::POSITION posEnd = m_streams.EndPosition();
    for (; pos != posEnd; pos = m_streams.Next(pos))
    {
        com_ptr<IMFStreamSink> spCurr;
        IFR(m_streams.GetItemPos(pos, spCurr.put()));

        DWORD dwCurrId;
        IFR(spCurr->GetIdentifier(&dwCurrId));
        if (dwCurrId > dwStreamSinkIdentifier)
        {
            break;
        }
    }

    NULL_CHK_HR(spMFStream, E_NOT_SET);

    IFR(m_streams.InsertPos(pos, spMFStream.get()));

    spMFStream.copy_to(ppStreamSink);
    
    return S_OK;
}

_Use_decl_annotations_
HRESULT NetworkMediaSink::RemoveStreamSink(DWORD dwStreamSinkIdentifier)
{
    slim_lock_guard guard(m_lock);

    IFR(CheckShutdown());

    com_ptr<IMFStreamSink> spStream;

    StreamContainer::POSITION pos = m_streams.FrontPosition();
    StreamContainer::POSITION endPos = m_streams.EndPosition();
    for (; pos != endPos; pos = m_streams.Next(pos))
    {
        IFR(m_streams.GetItemPos(pos, spStream.put()));

        DWORD dwId;
        IFR(spStream->GetIdentifier(&dwId));
        if (dwId == dwStreamSinkIdentifier)
        {
            break;
        }
    }

    if (pos == endPos)
    {
        return MF_E_INVALIDSTREAMNUMBER;
    }

    IFR(m_streams.Remove(pos, nullptr));

    return static_cast<NetworkMediaSinkStream*>(spStream.get())->Shutdown();
}

_Use_decl_annotations_
HRESULT NetworkMediaSink::GetStreamSinkCount(DWORD* pcStreamSinkCount)
{
    NULL_CHK(pcStreamSinkCount);

    slim_shared_lock_guard const guard(m_lock);

    IFR(CheckShutdown());

   *pcStreamSinkCount = m_streams.GetCount();

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
    DWORD const cStreams = m_streams.GetCount();
    if (cStreams <= dwIndex)
    {
        IFR(MF_E_INVALIDINDEX);
    }

    IFR(CheckShutdown());

    DWORD dwCurrent = 0;
    StreamContainer::POSITION pos = m_streams.FrontPosition();
    StreamContainer::POSITION endPos = m_streams.EndPosition();
    for (; pos != endPos && dwCurrent < dwIndex; pos = m_streams.Next(pos), ++dwCurrent)
    {
        // Just move to proper position
    }

    if (pos == endPos)
    {
        IFR(MF_E_UNEXPECTED);
    }

    IFR(m_streams.GetItemPos(pos, spStream.put()));
    
    spStream.copy_to(ppStreamSink);

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

    StreamContainer::POSITION pos = m_streams.FrontPosition();
    StreamContainer::POSITION endPos = m_streams.EndPosition();
    for (; pos != endPos; pos = m_streams.Next(pos))
    {
        com_ptr<IMFStreamSink> spStream;
        IFR(m_streams.GetItemPos(pos, spStream.put()));

        DWORD dwId;
        IFR(spStream->GetIdentifier(&dwId));
        
        if (dwId == dwStreamSinkIdentifier)
        {
            spResult = spStream;
            break;
        }
    }

    if (pos == endPos)
    {
        return MF_E_INVALIDSTREAMNUMBER;
    }

    NULL_CHK_HR(spResult, E_NOT_SET);

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
    m_presentationClock.attach(pPresentationClock);

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
    ForEach(m_streams, ShutdownFunc());
    m_streams.Clear();

    // notify any connections that sink is shutdown
    SendCaptureStopped();

    m_presentationClock = nullptr;

    _cStreamsEnded = 0;

    m_evtClosed(*this);
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
    return ForEach(m_streams, StartFunc(llClockStartOffset));
}

_Use_decl_annotations_
HRESULT NetworkMediaSink::OnClockStop(MFTIME hnsSystemTime)
{
    slim_lock_guard guard(m_lock);

    IFR(CheckShutdown());

    // Stop each stream
    return ForEach(m_streams, StopFunc());
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
HRESULT NetworkMediaSink::SendCaptureReady()
{
    NULL_CHK(m_connection);

    m_connection.SendPayloadTypeAsync(PayloadType::State_CaptureReady).get();

    return S_OK;
}

_Use_decl_annotations_
HRESULT NetworkMediaSink::SendDescription(void)
{
    Log(Log_Level_Info, L"NetworkSinkImpl::SendDescription() begin...\n");

    // Size of the constant buffer header
    const DWORD c_cStreams = m_streams.GetCount();
    const DWORD c_cbPayloadHeaderSize = sizeof(PayloadHeader);
    const DWORD c_cbMediaDescriptionSize = sizeof(MediaDescription);
    const DWORD c_cbStraemTypeDescription = sizeof(MediaTypeDescription);
    const DWORD c_cbStreamTypeHeaderSize = c_cStreams * c_cbStraemTypeDescription;
    const DWORD c_cbBufferSize = c_cbPayloadHeaderSize + c_cbMediaDescriptionSize + c_cbStreamTypeHeaderSize;

    // Create send buffer
    DataBuffer dataBuffer = DataBuffer(c_cbBufferSize);

    // get the buffer pointer
    auto dataBufferImpl = dataBuffer.as<Network::implementation::DataBuffer>();
    BYTE* pBuf;
    dataBufferImpl->Buffer(&pBuf);

    // Prepare payload header 8 bytes - type and size of payload to follow
    PayloadHeader* pOpHeader = reinterpret_cast<PayloadHeader*>(pBuf);
    NULL_CHK(pOpHeader);
    ZeroMemory(pOpHeader, c_cbPayloadHeaderSize);
    pOpHeader->ePayloadType = PayloadType::SendMediaDescription;
    pOpHeader->cbPayloadSize = c_cbMediaDescriptionSize + c_cbStreamTypeHeaderSize; // still need to add the size of MediaAttributes for each stream

    // Media Description - how many streams and the total bytes for each streams attributes
    MediaDescription* pMediaDescription = reinterpret_cast<MediaDescription*>(pBuf + c_cbPayloadHeaderSize);
    NULL_CHK(pMediaDescription);
    ZeroMemory(pMediaDescription, c_cbMediaDescriptionSize);
    pMediaDescription->StreamCount = c_cStreams;
    pMediaDescription->StreamTypeHeaderSize = c_cbStreamTypeHeaderSize; // populates later

    // create array to hold the stream's IMFAttributes property
    std::vector<DataBuffer> svStreamMFAttributes(c_cStreams);

    // Prepare the MediaTypeDescription
    MediaTypeDescription* streamTypeHeaderPtr = reinterpret_cast<MediaTypeDescription* >(pBuf + c_cbPayloadHeaderSize + c_cbMediaDescriptionSize);
    NULL_CHK(streamTypeHeaderPtr);

    // get each streams ppMediaType info and its attrbutes as a DataBuffer
    StreamContainer::POSITION pos = m_streams.FrontPosition();
    StreamContainer::POSITION endPos = m_streams.EndPosition();
    DWORD nStream = 0;
    for (; pos != endPos; pos = m_streams.Next(pos), ++nStream)
    {
        // zero out the memory block for stream type description
        ZeroMemory(&streamTypeHeaderPtr[nStream], c_cbStraemTypeDescription);

        com_ptr<IMFStreamSink> spStream;
        IFR(m_streams.GetItemPos(pos, spStream.put()));

        com_ptr <NetworkMediaSinkStream> spMediaTypeHandler = spStream.as<NetworkMediaSinkStream>();
        NULL_CHK(spMediaTypeHandler.get());

        // Get the MediaType info from stream
        svStreamMFAttributes[nStream] = spMediaTypeHandler->FillStreamDescription(&streamTypeHeaderPtr[nStream]);

        // Add size of ppMediaType blob to the payload size.
        pOpHeader->cbPayloadSize += streamTypeHeaderPtr[nStream].AttributesBlobSize;
    }

    // update total payload size
    dataBuffer.CurrentLength(c_cbBufferSize);

    // Prepare the bundle to send
    DataBundle dataBundle = DataBundle(dataBuffer);

    // Add ppMediaType data
    for (DWORD nStream = 0; nStream < c_cStreams; ++nStream)
    {
        dataBundle.AddBuffer(svStreamMFAttributes[nStream]);
    }

    // Send the data, set callback
    Log(Log_Level_Info, L"NetworkMediaSink::SendDescription()\n");

    // Block thread until async call finishes
    m_connection.SendBundleAsync(dataBundle).get();
    
    return S_OK;
 }

_Use_decl_annotations_
HRESULT NetworkMediaSink::SetMediaStreamProperties(
    Windows::Media::Capture::MediaStreamType mediaStreamType,
    Windows::Media::MediaProperties::IMediaEncodingProperties mediaEncodingProperties)
{
    if (mediaStreamType != MediaStreamType::VideoRecord && mediaStreamType != MediaStreamType::Audio)
    {
        IFR(E_INVALIDARG);
    }

    DWORD streamId;
    IFR(GetStreamId(mediaStreamType, &streamId));

    RemoveStreamSink(streamId);

    if (nullptr != mediaEncodingProperties)
    {
        com_ptr<IMFStreamSink> spStreamSink;
        com_ptr<IMFMediaType> spMediaType;
        ConvertPropertiesToMediaType(mediaEncodingProperties, spMediaType.put());
        IFR(AddStreamSink(streamId, spMediaType.get(), spStreamSink.put()));
    }

    return S_OK;
}
