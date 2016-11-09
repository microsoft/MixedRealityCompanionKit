// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "NetworkMediaSink.h"
#include "Media.h"

class ShutdownFunc
{
public:
    HRESULT operator()(_In_ IMFStreamSink* pStream) const
    {
        return static_cast<NetworkMediaSinkStreamImpl*>(pStream)->Shutdown();
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
        return static_cast<NetworkMediaSinkStreamImpl*>(pStream)->ConnectedFunc(_fConnected, _llStartTime);
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
        return static_cast<NetworkMediaSinkStreamImpl*>(pStream)->Start(_llStartTime);
    }

    LONGLONG _llStartTime;
};

class StopFunc
{
public:
    HRESULT operator()(_In_ IMFStreamSink* pStream) const
    {
        return static_cast<NetworkMediaSinkStreamImpl*>(pStream)->Stop();
    }
};

static HRESULT AddAttribute(
    _In_ GUID guidKey, 
    _In_ IPropertyValue* propValue, 
    _In_ IMFAttributes *mfAttributes)
{
    NULL_CHK(propValue);
    NULL_CHK(mfAttributes);

    // store as a smart pointer
    ComPtr<IPropertyValue> spPropVal(propValue);
    ComPtr<IMFAttributes> spAttrib(mfAttributes);
    PropertyType type;
    IFR(spPropVal->get_Type(&type));

    switch (type)
    {
    case PropertyType::PropertyType_UInt8Array:
    {
        UINT32 size = 0;
        BYTE* buffer = nullptr;
        IFR(spPropVal->GetUInt8Array(&size, &buffer));
        IFR(spAttrib->SetBlob(guidKey, buffer, size));
    }
    break;

    case PropertyType::PropertyType_Double:
    {
        double value = 0.0;
        IFR(spPropVal->GetDouble(&value));
        IFR(spAttrib->SetDouble(guidKey, value));
    }
    break;

    case PropertyType::PropertyType_Guid:
    {
        GUID value(GUID_NULL);
        IFR(spPropVal->GetGuid(&value));
        IFR(spAttrib->SetGUID(guidKey, value));
    }
    break;

    case PropertyType::PropertyType_String:
    {
        Wrappers::HString value;
        IFR(spPropVal->GetString(value.GetAddressOf()));

        IFR(spAttrib->SetString(guidKey, value.GetRawBuffer(nullptr)));
    }
    break;

    case PropertyType::PropertyType_UInt32:
    {
        UINT32 value = 0;
        IFR(spPropVal->GetUInt32(&value));
        IFR(spAttrib->SetUINT32(guidKey, value));
    }
    break;

    case PropertyType::PropertyType_UInt64:
    {
        UINT64 value = 0;
        IFR(spPropVal->GetUInt64(&value));
        IFR(spAttrib->SetUINT64(guidKey, value));
    }
    break;

    default:
        //ignore unknown values
        break;
    };

    return S_OK;
}

HRESULT ConvertPropertiesToMediaType(
    _In_ IMediaEncodingProperties* mediaEncodingProp,
    _Out_ IMFMediaType** ppMediaType)
{
    NULL_CHK(mediaEncodingProp);
    NULL_CHK(ppMediaType);

   * ppMediaType = nullptr;

    // get Iterator
    ComPtr<IMap<GUID, IInspectable*>> spMap;
    IFR(mediaEncodingProp->get_Properties(&spMap));

    ComPtr<IIterable<IKeyValuePair<GUID, IInspectable*>*>> spIterable;
    IFR(spMap.As(&spIterable));

    ComPtr<IIterator<IKeyValuePair<GUID, IInspectable*>*>> spIterator;
    IFR(spIterable->First(&spIterator));

    ComPtr<IMFMediaType> spMediaType;
    IFR(MFCreateMediaType(&spMediaType));

    boolean hasCurrent = false;
    IFR(spIterator->get_HasCurrent(&hasCurrent));

    while (hasCurrent)
    {
        ComPtr<IKeyValuePair<GUID, IInspectable*> > spKeyValuePair;
        IFR(spIterator->get_Current(&spKeyValuePair));

        GUID guidKey;
        IFR(spKeyValuePair->get_Key(&guidKey));

        ComPtr<IInspectable> spValue;
        IFR(spKeyValuePair->get_Value(&spValue));

        ComPtr<IPropertyValue> spPropValue;
        IFR(spValue.As(&spPropValue));

        IFR(AddAttribute(guidKey, spPropValue.Get(), spMediaType.Get()));

        IFR(spIterator->MoveNext(&hasCurrent));
    }

    ComPtr<IInspectable> spValue;
    IFR(spMap->Lookup(MF_MT_MAJOR_TYPE, &spValue));

    ComPtr<IPropertyValue> spPropValue;
    IFR(spValue.As(&spPropValue));

    GUID guiMajorType;
    IFR(spPropValue->GetGuid(&guiMajorType));

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

NetworkMediaSinkImpl::NetworkMediaSinkImpl()
    : _spConnection(nullptr)
    , _llStartTime(0)
    , _cStreamsEnded(0)
    , _presentationClock(nullptr)
{
}

NetworkMediaSinkImpl::~NetworkMediaSinkImpl()
{
    Shutdown();

    // disconnect from events
    _spConnection->remove_Received(_bundleReceivedEventToken);

    // remove the connection
    _spConnection.Reset();
    _spConnection = nullptr;
}

HRESULT NetworkMediaSinkImpl::RuntimeClassInitialize(
    IAudioEncodingProperties* audioEncodingProperties, 
    IVideoEncodingProperties* videoEncodingProperties, 
    IConnection* connection)
{
    // audio can be null
    // NULL_CHK(audioEncodingProperties);
    NULL_CHK(videoEncodingProperties);
    NULL_CHK(connection);

    ComPtr<IConnection> spConnection(connection);
    IFR(spConnection.As(&_spConnection));

    // Set up media streams
    if(nullptr != audioEncodingProperties)
    {
        ComPtr<IAudioEncodingProperties> spAudio(audioEncodingProperties);
        ComPtr<IMediaEncodingProperties> spAudoEncProperties;
        IFR(spAudio.As(&spAudoEncProperties));
        IFR(SetMediaStreamProperties(MediaStreamType::MediaStreamType_Audio, spAudoEncProperties.Get()));
    }

    ComPtr<IVideoEncodingProperties> spVideo(videoEncodingProperties);
    ComPtr<IMediaEncodingProperties> spVideoEncProperties;
    IFR(spVideo.As(&spVideoEncProperties));
    IFR(SetMediaStreamProperties(MediaStreamType::MediaStreamType_VideoRecord, spVideoEncProperties.Get()));

    // subscribe to connection data
    // define callback
    ComPtr<NetworkMediaSinkImpl> spThis(this);
    auto bundleReceivedCallback = Callback<IBundleReceivedEventHandler>(
        [this, spThis](
            _In_ IConnection *sender,
            _In_ IBundleReceivedArgs *args) -> HRESULT
    {
        HRESULT hr = S_OK;

        PayloadType type;
        IFC(args->get_PayloadType(&type));

        switch (type)
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

    EventRegistrationToken bundleEventToken;
    IFR(spConnection->add_Received(bundleReceivedCallback.Get(), &bundleEventToken));

    IFR(SendCaptureReady());

    return spConnection.As(&_spConnection);
}

// IMediaExtension
HRESULT NetworkMediaSinkImpl::SetProperties(
    ABI::Windows::Foundation::Collections::IPropertySet* configuration)
{
    return S_OK;
}

// IMFMediaSink
_Use_decl_annotations_
HRESULT NetworkMediaSinkImpl::GetCharacteristics(
    DWORD* pdwCharacteristics)
{
    NULL_CHK(pdwCharacteristics);

    auto lock = _lock.Lock();

    IFR(CheckShutdown());

    // Rateless sink.
   * pdwCharacteristics = MEDIASINK_RATELESS;

    return S_OK;
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkImpl::AddStreamSink(
    DWORD dwStreamSinkIdentifier, 
    IMFMediaType* pMediaType, 
    IMFStreamSink** ppStreamSink)
{
    auto lock = _lock.Lock();

    NULL_CHK(pMediaType);
    NULL_CHK(ppStreamSink);

    IFR(CheckShutdown());

    ComPtr<IMFStreamSink> spMFStream;
    if (SUCCEEDED(GetStreamSinkById(dwStreamSinkIdentifier, &spMFStream)))
    {
        IFR(MF_E_STREAMSINK_EXISTS);
    }

    ComPtr<NetworkMediaSinkStreamImpl> spNetSink;
    IFR(MakeAndInitialize<NetworkMediaSinkStreamImpl>(&spNetSink, dwStreamSinkIdentifier, _spConnection.Get(), this));
    IFR(spNetSink.As(&spMFStream));

    IFR(spNetSink->SetCurrentMediaType(pMediaType));

    // Insert in proper position
    StreamContainer::POSITION pos = _streams.FrontPosition();
    StreamContainer::POSITION posEnd = _streams.EndPosition();
    for (; pos != posEnd; pos = _streams.Next(pos))
    {
        ComPtr<IMFStreamSink> spCurr;
        IFR(_streams.GetItemPos(pos, &spCurr));

        DWORD dwCurrId;
        IFR(spCurr->GetIdentifier(&dwCurrId));
        if (dwCurrId > dwStreamSinkIdentifier)
        {
            break;
        }
    }

    NULL_CHK_HR(spMFStream, E_NOT_SET);

    IFR(_streams.InsertPos(pos, spMFStream.Get()));

    return spMFStream.CopyTo(ppStreamSink);
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkImpl::RemoveStreamSink(DWORD dwStreamSinkIdentifier)
{
    auto lock = _lock.Lock();

    IFR(CheckShutdown());

    ComPtr<IMFStreamSink> spStream;

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
    return static_cast<NetworkMediaSinkStreamImpl*>(spStream.Get())->Shutdown();
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkImpl::GetStreamSinkCount(DWORD* pcStreamSinkCount)
{
    NULL_CHK(pcStreamSinkCount);

    auto lock = _lock.Lock();

    IFR(CheckShutdown());

   * pcStreamSinkCount = _streams.GetCount();

    return S_OK;
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkImpl::GetStreamSinkByIndex(
    DWORD dwIndex, 
    IMFStreamSink** ppStreamSink)
{
    NULL_CHK(ppStreamSink);

    auto lock = _lock.Lock();

    ComPtr<IMFStreamSink> spStream;
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
HRESULT NetworkMediaSinkImpl::GetStreamSinkById(
    DWORD dwStreamSinkIdentifier,
    IMFStreamSink** ppStreamSink)
{
    NULL_CHK(ppStreamSink);

    auto lock = _lock.Lock();

    IFR(CheckShutdown());

    ComPtr<IMFStreamSink> spResult;

    StreamContainer::POSITION pos = _streams.FrontPosition();
    StreamContainer::POSITION endPos = _streams.EndPosition();
    for (; pos != endPos; pos = _streams.Next(pos))
    {
        ComPtr<IMFStreamSink> spStream;
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
HRESULT NetworkMediaSinkImpl::SetPresentationClock(IMFPresentationClock* pPresentationClock)
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
HRESULT NetworkMediaSinkImpl::GetPresentationClock(IMFPresentationClock** ppPresentationClock)
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
HRESULT NetworkMediaSinkImpl::Shutdown()
{
    if (nullptr == _spConnection)
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

    ComPtr<INetworkMediaSink> spThis(this);
    return _evtClosed.InvokeAll(spThis.Get());
}

// IMFClockStateSink
_Use_decl_annotations_
HRESULT NetworkMediaSinkImpl::OnClockStart(MFTIME hnsSystemTime, LONGLONG llClockStartOffset)
{
    auto lock = _lock.Lock();

    IFR(CheckShutdown());

    Log(Log_Level_Info, L"NetworkSinkImpl::OnClockStart: ts=%I64d\n", llClockStartOffset);
        
    // Start each stream.
    _llStartTime = llClockStartOffset;
    return ForEach(_streams, StartFunc(llClockStartOffset));
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkImpl::OnClockStop(MFTIME hnsSystemTime)
{
    auto lock = _lock.Lock();

    IFR(CheckShutdown());

    // Stop each stream
    return ForEach(_streams, StopFunc());
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkImpl::OnClockPause(MFTIME hnsSystemTime)
{
    return MF_E_INVALID_STATE_TRANSITION;
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkImpl::OnClockRestart(MFTIME hnsSystemTime)
{
    return MF_E_INVALID_STATE_TRANSITION;
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkImpl::OnClockSetRate(MFTIME hnsSystemTime, float flRate)
{
    return S_OK;
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkImpl::get_SpatialCoordinateSystem(
    ABI::Windows::Perception::Spatial::ISpatialCoordinateSystem** ppCoordinateSystem)
{
    NULL_CHK(ppCoordinateSystem);

    if (nullptr == _spUnitySpatialCoordinateSystem.Get())
    {
        return E_NOT_SET;
    }

    return _spUnitySpatialCoordinateSystem.CopyTo(ppCoordinateSystem);
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkImpl::put_SpatialCoordinateSystem(
    ABI::Windows::Perception::Spatial::ISpatialCoordinateSystem* coordinateSystem)
{
    _spUnitySpatialCoordinateSystem = coordinateSystem;

    return S_OK;
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkImpl::add_Closed(
    IClosedEventHandler* eventHandler,
    EventRegistrationToken* token)
{
    return _evtClosed.Add(eventHandler, token);
}
_Use_decl_annotations_
HRESULT NetworkMediaSinkImpl::remove_Closed(
    EventRegistrationToken token)
{
    return _evtClosed.Remove(token);
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkImpl::add_FormatChanged(
    IFormatChangedEventHandler *eventHandler,
    EventRegistrationToken* token)
{
    return _evtFormatChanged.Add(eventHandler, token);
}
_Use_decl_annotations_
HRESULT NetworkMediaSinkImpl::remove_FormatChanged(
    EventRegistrationToken token) 
{
    return _evtFormatChanged.Remove(token);
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkImpl::add_SampleUpdated(
    ISampleUpdatedEventHandler *eventHandler,
    EventRegistrationToken* token) 
{
    return _evtSampleUpdated.Add(eventHandler, token);
}
_Use_decl_annotations_
HRESULT NetworkMediaSinkImpl::remove_SampleUpdated(
    EventRegistrationToken token) 
{
    return _evtSampleUpdated.Remove(token);
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkImpl::HandleError(
    HRESULT hr)
{
    if (FAILED(hr))
    {
        return Shutdown();
    }

    return S_OK;
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkImpl::SendCaptureReady()
{
    NULL_CHK(_spConnection);

    return _spConnection->SendPayloadType(PayloadType_State_CaptureReady);
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkImpl::SendCaptureStarted()
{
    NULL_CHK(_spConnection);

    return _spConnection->SendPayloadType(PayloadType_State_CaptureStarted);
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkImpl::SendCaptureStopped()
{
    NULL_CHK(_spConnection);

    return _spConnection->SendPayloadType(PayloadType_State_CaptureStopped);
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkImpl::SendDescription(void)
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
    ComPtr<IDataBuffer> spDataBuffer;
    IFR(MakeAndInitialize<DataBufferImpl>(&spDataBuffer, c_cbBufferSize));

    // Prepare the buffer
    ComPtr<IBuffer> spHeaderBuffer;
    IFR(spDataBuffer.As(&spHeaderBuffer));

    // get the buffer pointer
    BYTE* pBuf = GetDataType<BYTE*>(spHeaderBuffer.Get());

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
    std::vector<ComPtr<IDataBuffer>> svStreamMFAttributes(c_cStreams);

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

        ComPtr<IMFStreamSink> spStream;
        IFR(_streams.GetItemPos(pos, &spStream));

        ComPtr<NetworkMediaSinkStreamImpl> spMediaTypeHandler(static_cast<NetworkMediaSinkStreamImpl*>(spStream.Get()));
        NULL_CHK(spMediaTypeHandler.Get());

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
    ComPtr<IDataBundle> spDataBundle;
    IFR(MakeAndInitialize<DataBundleImpl>(&spDataBundle));

    // Add fixed sized data to bundle
    IFR(spDataBundle->AddBuffer(spDataBuffer.Get()));

    // Add ppMediaType data
    for (DWORD nStream = 0; nStream < c_cStreams; ++nStream)
    {
        IFR(spDataBundle->AddBuffer(svStreamMFAttributes[nStream].Get()));
    }

    // Send the data, set callback
    Log(Log_Level_Info, L"NetworkMediaSink::SendDescription()\n");

    return _spConnection->SendBundle(spDataBundle.Get());
 }

_Use_decl_annotations_
HRESULT NetworkMediaSinkImpl::SetMediaStreamProperties(
    ABI::Windows::Media::Capture::MediaStreamType mediaStreamType,
    ABI::Windows::Media::MediaProperties::IMediaEncodingProperties *mediaEncodingProperties)
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
        ComPtr<IMFStreamSink> spStreamSink;
        ComPtr<IMFMediaType> spMediaType;
        ConvertPropertiesToMediaType(mediaEncodingProperties, &spMediaType);
        IFR(AddStreamSink(streamId, spMediaType.Get(), &spStreamSink));
    }
    return S_OK;
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkImpl::FormatChanged(IMFMediaType* pMediaType)
{
    return S_OK;
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkImpl::SampleUpdated(IMFSample* pSample)
{
    return S_OK;
}

