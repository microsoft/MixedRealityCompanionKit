// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "NetworkMediaSink.h"
#include "MediaUtils.h"

using namespace winrt;
using namespace RealtimeStreaming::Media::implementation;

class ShutdownFunc
{
public:
    HRESULT operator()(_In_ IMFStreamSink* pStream) const
    {
        // TODO: cast with winrt::iunknown?
        CHECK_HR(hr = reinterpret_cast<IInspectable*>(request)->QueryInterface(spRequest.ReleaseAndGetAddressOf()), "Failed to get MF interface for media sample request.\n");

        return static_cast<NetworkMediaSinkStream*>(pStream)->Shutdown();
    }
};

class ConnectedFunc
{
public:
    ConnectedFunc(bool fConnected, LONGLONG llStartTime)
        : _fConnected(fConnected)
        , _llStartTime(llStartTime)
    {
    }

    HRESULT operator()(_In_ IMFStreamSink* pStream) const
    {
        return static_cast<NetworkMediaSinkStream*>(pStream)->ConnectedFunc(_fConnected, _llStartTime);
    }

    bool _fConnected;
    LONGLONG _llStartTime;
};

class StartFunc
{
public:
    StartFunc(LONGLONG llStartTime)
        : _llStartTime(llStartTime)
    {
    }

    HRESULT operator()(_In_ IMFStreamSink* pStream) const
    {
        return static_cast<NetworkMediaSinkStream*>(pStream)->Start(_llStartTime);
    }

    LONGLONG _llStartTime;
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
    NULL_CHK(propValue);
    NULL_CHK(mfAttributes);

    // store as a smart pointer
    com_ptr<IPropertyValue> spPropVal(propValue);
    com_ptr<IMFAttributes> spAttrib(mfAttributes);
    PropertyType type;
    check_hresult(spPropVal->get_Type(&type));

    switch (type)
    {
    case PropertyType::PropertyType_UInt8Array:
    {
        UINT32 size = 0;
        BYTE* buffer = nullptr;
        check_hresult(spPropVal->GetUInt8Array(&size, &buffer));
        check_hresult(spAttrib->SetBlob(guidKey, buffer, size));
    }
    break;

    case PropertyType::PropertyType_Double:
    {
        double value = 0.0;
        check_hresult(spPropVal->GetDouble(&value));
        check_hresult(spAttrib->SetDouble(guidKey, value));
    }
    break;

    case PropertyType::PropertyType_Guid:
    {
        GUID value(GUID_NULL);
        check_hresult(spPropVal->GetGuid(&value));
        check_hresult(spAttrib->SetGUID(guidKey, value));
    }
    break;

    case PropertyType::PropertyType_String:
    {
        Wrappers::HString value;
        check_hresult(spPropVal->GetString(value.GetAddressOf()));

        check_hresult(spAttrib->SetString(guidKey, value.GetRawBuffer(nullptr)));
    }
    break;

    case PropertyType::PropertyType_UInt32:
    {
        UINT32 value = 0;
        check_hresult(spPropVal->GetUInt32(&value));
        check_hresult(spAttrib->SetUINT32(guidKey, value));
    }
    break;

    case PropertyType::PropertyType_UInt64:
    {
        UINT64 value = 0;
        check_hresult(spPropVal->GetUInt64(&value));
        check_hresult(spAttrib->SetUINT64(guidKey, value));
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
   IFR(MFCreateMediaType(&spMediaType));

   for (auto& key : propSet.Keys)
   {
       IPropertyValue propValue = spMap[key];

       IFR(AddAttribute(guidKey, spPropValue, spMediaType.get()));
   }

    IPropertyValue propValue = spMap.Lookup(MF_MT_MAJOR_TYPE);
    GUID guiMajorType = spPropValue.GetGuid();

    if (guiMajorType != MFMediaType_Video && guiMajorType != MFMediaType_Audio)
    {
        IFR(E_UNEXPECTED);
    }

    NULL_CHK_HR(spMediaType, E_NOT_SET);

    return spMediaType.CopyTo(ppMediaType);
}

HRESULT GetStreamId(
    _In_ ABI::Windows::Media::Capture::MediaStreamType mediaStreamType,
    _Out_ DWORD* streamId)
{
    NULL_CHK(streamId);

    *streamId = -1;

    switch (mediaStreamType)
    {
    case ABI::Windows::Media::Capture::MediaStreamType::MediaStreamType_VideoRecord:
        *streamId = 0;
        break;
    case ABI::Windows::Media::Capture::MediaStreamType::MediaStreamType_Audio:
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
    : m_connection(nullptr)
    , _llStartTime(0)
    , _cStreamsEnded(0)
    , _presentationClock(nullptr)
    , m_connection(connection)
{

    // Set up media streams
    if (nullptr != audioEncodingProperties)
    {
        check_hresult(SetMediaStreamProperties(MediaStreamType::MediaStreamType_Audio, 
            audioEncodingProperties));
    }

    check_hresult(SetMediaStreamProperties(MediaStreamType::MediaStreamType_VideoRecord, 
        videoEncodingProperties));

    // subscribe to connection data
    // define callback
    auto bundleReceivedCallback = [&](_In_ Connection sender, _In_ BundleReceivedArgs args) -> HRESULT
    {
        Log(Log_Level_Info, L"NetworkSinkImpl::OnBundleReceived");

        HRESULT hr = S_OK;

        switch (args.PayloadType)
        {
        case PayloadType_RequestMediaDescription:
            IFC(SendDescription());
            break;
        case PayloadType_RequestMediaStart:
            // triggers the _connected state
            if (nullptr != _presentationClock)
            {
                LOG_RESULT_MSG(_presentationClock->GetTime(&_llStartTime), L"NetworkSinkImpl - MediaStartRequested, Not able to set start time from presentation clock");
            }
            IFC(ForEach(_streams, ConnectedFunc(true, _llStartTime)));
            break;
        case PayloadType_RequestMediaStop:
            IFC(ForEach(_streams, ConnectedFunc(false, _llStartTime)));
            break;
        };

    done:
        return S_OK;
    });

    m_bundleReceivedEventToken = m_connection.Received(bundleReceivedCallback);

    check_hresult(SendCaptureReady());
}

NetworkMediaSink::~NetworkMediaSink()
{
    Shutdown();

    // disconnect from events
    m_connection.Received(m_bundleReceivedEventToken);
    m_connection = nullptr;
}

// IMediaExtension
HRESULT NetworkMediaSink::SetProperties(
    ABI::Windows::Foundation::Collections::IPropertySet* configuration)
{
    return S_OK;
}

// IMFMediaSink
_Use_decl_annotations_
HRESULT NetworkMediaSink::GetCharacteristics(
    DWORD* pdwCharacteristics)
{
    NULL_CHK(pdwCharacteristics);

    // TODO: Troy to convert all locks in this class to the slim?
    auto lock = _lock.Lock();

    check_hresult(CheckShutdown());

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
    auto lock = _lock.Lock();

    NULL_CHK(pMediaType);
    NULL_CHK(ppStreamSink);

    IFR(CheckShutdown());

    com_ptr<IMFStreamSink> spMFStream;
    if (SUCCEEDED(GetStreamSinkById(dwStreamSinkIdentifier, &spMFStream)))
    {
        check_hresult(MF_E_STREAMSINK_EXISTS);
    }

    com_ptr<NetworkMediaSinkStream> spNetSink;
    IFR(MakeAndInitialize<NetworkMediaSinkStream>(&spNetSink, dwStreamSinkIdentifier, m_connection.get(), this));
    IFR(spNetSink.As(&spMFStream));

    IFR(spNetSink->SetCurrentMediaType(pMediaType));

    // Insert in proper position
    StreamContainer::POSITION pos = _streams.FrontPosition();
    StreamContainer::POSITION posEnd = _streams.EndPosition();
    for (; pos != posEnd; pos = _streams.Next(pos))
    {
        com_ptr<IMFStreamSink> spCurr;
        IFR(_streams.GetItemPos(pos, &spCurr));

        DWORD dwCurrId;
        IFR(spCurr->GetIdentifier(&dwCurrId));
        if (dwCurrId > dwStreamSinkIdentifier)
        {
            break;
        }
    }

    NULL_CHK_HR(spMFStream, E_NOT_SET);

    IFR(_streams.InsertPos(pos, spMFStream.get()));

    return spMFStream.CopyTo(ppStreamSink);
}

_Use_decl_annotations_
HRESULT NetworkMediaSink::RemoveStreamSink(DWORD dwStreamSinkIdentifier)
{
    auto lock = _lock.Lock();

    IFR(CheckShutdown());

    com_ptr<IMFStreamSink> spStream;

    StreamContainer::POSITION pos = _streams.FrontPosition();
    StreamContainer::POSITION endPos = _streams.EndPosition();
    for (; pos != endPos; pos = _streams.Next(pos))
    {
        IFR(_streams.GetItemPos(pos, &spStream));

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

    IFR(_streams.Remove(pos, nullptr));

    return static_cast<NetworkMediaSinkStream*>(spStream.get())->Shutdown();
}

_Use_decl_annotations_
HRESULT NetworkMediaSink::GetStreamSinkCount(DWORD* pcStreamSinkCount)
{
    NULL_CHK(pcStreamSinkCount);

    auto lock = _lock.Lock();

    IFR(CheckShutdown());

   *pcStreamSinkCount = _streams.GetCount();

    return S_OK;
}

_Use_decl_annotations_
HRESULT NetworkMediaSink::GetStreamSinkByIndex(
    DWORD dwIndex, 
    IMFStreamSink** ppStreamSink)
{
    NULL_CHK(ppStreamSink);

    auto lock = _lock.Lock();
    
    com_ptr<IMFStreamSink> spStream;
    DWORD const cStreams = _streams.GetCount();
    if (cStreams <= dwIndex)
    {
        IFR(MF_E_INVALIDINDEX);
    }

    IFR(CheckShutdown());

    DWORD dwCurrent = 0;
    StreamContainer::POSITION pos = _streams.FrontPosition();
    StreamContainer::POSITION endPos = _streams.EndPosition();
    for (; pos != endPos && dwCurrent < dwIndex; pos = _streams.Next(pos), ++dwCurrent)
    {
        // Just move to proper position
    }

    if (pos == endPos)
    {
        IFR(MF_E_UNEXPECTED);
    }

    IFR(_streams.GetItemPos(pos, &spStream));
    
    return spStream.CopyTo(ppStreamSink);
}

_Use_decl_annotations_
HRESULT NetworkMediaSink::GetStreamSinkById(
    DWORD dwStreamSinkIdentifier,
    IMFStreamSink** ppStreamSink)
{
    NULL_CHK(ppStreamSink);

    auto lock = _lock.Lock();

    IFR(CheckShutdown());

    com_ptr<IMFStreamSink> spResult;

    StreamContainer::POSITION pos = _streams.FrontPosition();
    StreamContainer::POSITION endPos = _streams.EndPosition();
    for (; pos != endPos; pos = _streams.Next(pos))
    {
        com_ptr<IMFStreamSink> spStream;
        IFR(_streams.GetItemPos(pos, &spStream));

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

    return spResult.CopyTo(ppStreamSink);
}

_Use_decl_annotations_
HRESULT NetworkMediaSink::SetPresentationClock(IMFPresentationClock* pPresentationClock)
{
    NULL_CHK(pPresentationClock);

    auto lock = _lock.Lock();

    IFR(CheckShutdown());

    // If we already have a clock, remove ourselves from that clock's
    // state notifications.
    if (nullptr != _presentationClock)
    {
        IFR(_presentationClock->RemoveClockStateSink(this));
    }

    // Register ourselves to get state notifications from the new clock.
    if (pPresentationClock)
    {
        IFR(pPresentationClock->AddClockStateSink(this));
    }

    // Release the pointer to the old clock.
    // Store the pointer to the new clock.
    _presentationClock = pPresentationClock;

    return S_OK;
}

_Use_decl_annotations_
HRESULT NetworkMediaSink::GetPresentationClock(IMFPresentationClock** ppPresentationClock)
{
    NULL_CHK(ppPresentationClock)

    auto lock = _lock.Lock();

    IFR(CheckShutdown());

    if (nullptr == _presentationClock)
    {
        IFR(MF_E_NO_CLOCK); // There is no presentation clock.
    }

    NULL_CHK_HR(_presentationClock, E_NOT_SET);

    // Return the pointer to the caller.
    return _presentationClock.CopyTo(ppPresentationClock);
}

_Use_decl_annotations_
HRESULT NetworkMediaSink::Shutdown()
{
    if (nullptr == m_connection)
    {
        return MF_E_SHUTDOWN;
    }

    // clear all streams
    ForEach(_streams, ShutdownFunc());
    _streams.Clear();

    // notify any connections that sink is shutdown
    SendCaptureStopped();

    _presentationClock.Reset();
    _presentationClock = nullptr;

    _cStreamsEnded = 0;

    m_evtClosed(*this);
}

// IMFClockStateSink
_Use_decl_annotations_
HRESULT NetworkMediaSink::OnClockStart(MFTIME hnsSystemTime, LONGLONG llClockStartOffset)
{
    auto lock = _lock.Lock();

    IFR(CheckShutdown());

    Log(Log_Level_Info, L"NetworkSinkImpl::OnClockStart: ts=%I64d\n", llClockStartOffset);
        
    // Start each stream.
    _llStartTime = llClockStartOffset;
    return ForEach(_streams, StartFunc(llClockStartOffset));
}

_Use_decl_annotations_
HRESULT NetworkMediaSink::OnClockStop(MFTIME hnsSystemTime)
{
    auto lock = _lock.Lock();

    IFR(CheckShutdown());

    // Stop each stream
    return ForEach(_streams, StopFunc());
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
winrt::event_token NetworkMediaSink::Closed(Windows::Foundation::EventHandler const& handler)
{
    auto lock = _lock.Lock();

    return m_evtClosed.add(handler);
}

_Use_decl_annotations_
void NetworkMediaSink::Closed(winrt::event_token const& token)
{
    auto lock = _lock.Lock();

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

    return m_connection.SendPayloadType(PayloadType_State_CaptureReady);
}

_Use_decl_annotations_
HRESULT NetworkMediaSink::SendDescription(void)
{
    Log(Log_Level_Info, L"NetworkSinkImpl::SendDescription() begin...\n");

    // Size of the constant buffer header
    const DWORD c_cStreams = _streams.GetCount();
    const DWORD c_cbPayloadHeaderSize = sizeof(PayloadHeader);
    const DWORD c_cbMediaDescriptionSize = sizeof(MediaDescription);
    const DWORD c_cbStraemTypeDescription = sizeof(MediaTypeDescription);
    const DWORD c_cbStreamTypeHeaderSize = c_cStreams * c_cbStraemTypeDescription;
    const DWORD c_cbBufferSize = c_cbPayloadHeaderSize + c_cbMediaDescriptionSize + c_cbStreamTypeHeaderSize;

    // Create send buffer
    DataBuffer dataBuffer = DataBuffer(c_cbBufferSize);

    // Prepare the buffer
    com_ptr<IBuffer> spHeaderBuffer;
    check_hresult(spDataBuffer.As(&spHeaderBuffer));

    // get the buffer pointer
    BYTE* pBuf = GetDataType<BYTE*>(spHeaderBuffer.get());

    // Prepare payload header 8 bytes - type and size of payload to follow
    PayloadHeader* pOpHeader = reinterpret_cast<PayloadHeader*>(pBuf);
    NULL_CHK(pOpHeader);
    ZeroMemory(pOpHeader, c_cbPayloadHeaderSize);
    pOpHeader->ePayloadType = PayloadType_SendMediaDescription;
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
    StreamContainer::POSITION pos = _streams.FrontPosition();
    StreamContainer::POSITION endPos = _streams.EndPosition();
    DWORD nStream = 0;
    for (; pos != endPos; pos = _streams.Next(pos), ++nStream)
    {
        // zero out the memory block for stream type description
        ZeroMemory(&streamTypeHeaderPtr[nStream], c_cbStraemTypeDescription);

        com_ptr<IMFStreamSink> spStream;
        IFR(_streams.GetItemPos(pos, &spStream));

        com_ptr<NetworkMediaSinkStream> spMediaTypeHandler(static_cast<NetworkMediaSinkStream*>(spStream.get()));
        NULL_CHK(spMediaTypeHandler.get());

        // Get the MediaType info from stream
        IFR(spMediaTypeHandler->FillStreamDescription(
            &streamTypeHeaderPtr[nStream],
            &svStreamMFAttributes[nStream]));

        // Add size of ppMediaType blob to the payload size.
        pOpHeader->cbPayloadSize += streamTypeHeaderPtr[nStream].AttributesBlobSize;
    }

    // update total payload size
    IFR(spDataBuffer->put_CurrentLength(c_cbBufferSize));

    // Prepare the bundle to send
    DataBundle dataBundle = DataBundle();

    // Add fixed sized data to bundle
    IFR(dataBundle.AddBuffer(dataBuffer));

    // Add ppMediaType data
    for (DWORD nStream = 0; nStream < c_cStreams; ++nStream)
    {
        IFR(dataBundle.AddBuffer(svStreamMFAttributes[nStream]));
    }

    // Send the data, set callback
    Log(Log_Level_Info, L"NetworkMediaSink::SendDescription()\n");

    return m_connection.SendBundle(dataBundle);
 }

_Use_decl_annotations_
HRESULT NetworkMediaSink::SetMediaStreamProperties(
    Windows::Media::Capture::MediaStreamType mediaStreamType,
    Windows::Media::MediaProperties::IMediaEncodingProperties mediaEncodingProperties)
{
    if (mediaStreamType != MediaStreamType::MediaStreamType_VideoRecord && mediaStreamType != MediaStreamType::MediaStreamType_Audio)
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
