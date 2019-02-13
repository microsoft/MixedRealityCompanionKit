// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "NetworkMediaSinkStream.h"
#include "MediaUtils.h"

#define SET_SAMPLE_FLAG(dest, destMask, pSample, flagName) \
{ \
    UINT32 unValue; \
    if (SUCCEEDED(pSample->GetUINT32(MFSampleExtension_##flagName, &unValue))) \
    { \
        dest |= (unValue != FALSE) ? static_cast<DWORD>(SampleFlags_SampleFlag_##flagName) : 0; \
        destMask |= static_cast<DWORD>(SampleFlags_SampleFlag_##flagName); \
    } \
}

// If an entry is TRUE, the operation is valid from that state.
BOOL NetworkMediaSinkStream::ValidStateMatrix[SinkStreamState_Count][SinkStreamOperation_Count] =
{
    //            Operations:
    // States:    SetType  Start    Restart Pause   Stop    Sample  Marker
    /* NotSet */  TRUE,    FALSE,   FALSE,  FALSE,  FALSE,  FALSE,  FALSE,

    /* Ready */   TRUE,    TRUE,    FALSE,  TRUE,   TRUE,   FALSE,  TRUE,

    /* Start */   TRUE,    TRUE,    FALSE,  TRUE,   TRUE,   TRUE,   TRUE,

    /* Pause */   TRUE,    TRUE,    TRUE,   TRUE,   TRUE,   TRUE,   TRUE,

    /* Stop */    TRUE,    TRUE,    FALSE,  FALSE,  TRUE,   FALSE,  TRUE,
};

_Use_decl_annotations_
NetworkMediaSinkStream::AsyncOperation::AsyncOperation(SinkStreamOperation op)
    : _cRef(1)
    , _op(op)
{
}

_Use_decl_annotations_
NetworkMediaSinkStream::AsyncOperation::~AsyncOperation()
{
    assert(_cRef == 0);
}

_Use_decl_annotations_
ULONG NetworkMediaSinkStream::AsyncOperation::AddRef()
{
    return InterlockedIncrement(&_cRef);
}

_Use_decl_annotations_
ULONG NetworkMediaSinkStream::AsyncOperation::Release()
{
    ULONG cRef = InterlockedDecrement(&_cRef);
    if (cRef == 0)
    {
        delete this;
    }

    return cRef;
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::AsyncOperation::QueryInterface(REFIID iid, void** ppv)
{
    if (!ppv)
    {
        return E_POINTER;
    }
    if (iid == IID_IUnknown)
    {
       * ppv = static_cast<IUnknown*>(this);
    }
    else
    {
       * ppv = nullptr;
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}

_Use_decl_annotations_
NetworkMediaSinkStream::NetworkMediaSinkStream()
    : _dwStreamId(-1)
    , _state(SinkStreamState_NotSet)
    , _isShutdown(false)
    , _isPlayerConnected(false)
    , _fIsVideo(false)
    , _fGetFirstSampleTime(false)
    , _adjustedStartTime(0)
    , m_connection(nullptr)
    , _spParentMediaSink(nullptr)
    , _workQueueId(0)
    , _workQueueCB(this, &NetworkMediaSinkStream::OnDispatchWorkItem)
{
    ZeroMemory(&_currentSubtype, sizeof(_currentSubtype));
}

_Use_decl_annotations_
NetworkMediaSinkStream::~NetworkMediaSinkStream()
{
    assert(_isShutdown);
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::RuntimeClassInitialize(
    DWORD id, 
    IConnection* pConnection,
    INetworkMediaSink* pParentMediaSink)
{
    NULL_CHK(pConnection);
    NULL_CHK(pParentMediaSink);

    // Create the event queue helper.
    check_hresult(MFCreateEventQueue(&_eventQueue));
    check_hresult(MFAllocateSerialWorkQueue(MFASYNC_CALLBACK_QUEUE_STANDARD, &_workQueueId));

    _dwStreamId = id;
    m_connection = pConnection;
    _spParentMediaSink = pParentMediaSink;

    return S_OK;
}

// IMFMediaEventGenerator methods.
// Note: These methods call through to the event queue helper object.
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::BeginGetEvent(
    IMFAsyncCallback* pCallback, 
    IUnknown* punkState)
{
    auto lock = _lock.Lock();

    check_hresult(CheckShutdown());

    return _eventQueue->BeginGetEvent(pCallback, punkState);
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::EndGetEvent(
    IMFAsyncResult* pResult, 
    IMFMediaEvent** ppEvent)
{
    NULL_CHK(ppEvent);

    auto lock = _lock.Lock();

    check_hresult(CheckShutdown());

    return _eventQueue->EndGetEvent(pResult, ppEvent);
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::GetEvent(
    DWORD dwFlags, 
    IMFMediaEvent** ppEvent)
{
    NULL_CHK(ppEvent);

    // NOTE:
    // GetEvent can block indefinitely, so we don't hold the lock.
    // This requires some juggling with the event queue pointer.
    HRESULT hr = S_OK;

    com_ptr<IMFMediaEventQueue> spQueue;
    {
        auto lock = _lock.Lock();

        // Check shutdown
        check_hresult(CheckShutdown());

        // Get the pointer to the event queue.
        spQueue = _eventQueue;
    }

    // Now get the event.
    return spQueue->GetEvent(dwFlags, ppEvent);
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::QueueEvent(
    MediaEventType met, 
    REFGUID guidExtendedType, 
    HRESULT hrStatus, 
    PROPVARIANT const* pvValue)
{
    auto lock = _lock.Lock();

    check_hresult(CheckShutdown());

    return _eventQueue->QueueEventParamVar(met, guidExtendedType, hrStatus, pvValue);
}

// IMFStreamSink methods
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::GetMediaSink(
    IMFMediaSink** ppMediaSink)
{
    NULL_CHK(ppMediaSink);

    auto lock = _lock.Lock();

    check_hresult(CheckShutdown());

    NULL_CHK_HR(_spParentMediaSink, E_NOT_SET);

    return _spParentMediaSink.CopyTo(ppMediaSink);
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::GetIdentifier(
    DWORD* pdwIdentifier)
{
    NULL_CHK(pdwIdentifier);

    auto lock = _lock.Lock();

    check_hresult(CheckShutdown());

   * pdwIdentifier = _dwStreamId;

    return S_OK;
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::GetMediaTypeHandler(
    IMFMediaTypeHandler** ppHandler)
{
    NULL_CHK(ppHandler);

    auto lock = _lock.Lock();

    check_hresult(CheckShutdown());

    // This stream object acts as its own type handler, so we QI ourselves.
    return QueryInterface(IID_IMFMediaTypeHandler, reinterpret_cast<void**>(ppHandler));
}

// We received a sample from an upstream component
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::ProcessSample(
    IMFSample* pSample)
{
    NULL_CHK(pSample);

    Log(Log_Level_All, L"NetworkMediaSinkStreamImpl::ProcessSample() begin...\n");

    auto lock = _lock.Lock();

    HRESULT hr = S_OK;

    IFC(CheckShutdown());

    // Validate the operation.
    IFC(ValidateOperation(SinkStreamOperation_ProcessSample));

    if(!_isPlayerConnected)
    {
        // we do not have a connected player yet
        Log(Log_Level_Info, L"Waiting for a connected player, ignoring sample...\n");

        if (nullptr != m_connection)
        {
            check_hresult(m_connection->SendPayloadType(PayloadType_State_CaptureReady));
        }

        IFC(QueueEvent(MEStreamSinkRequestSample, GUID_NULL, hr, nullptr));
    }
    else
    {
        // is this our first sample since connecting?
        if (_isPlayerConnected && _fGetFirstSampleTime)
        {
            // pulls the sample time to rebase timestamps
            IFC(pSample->GetSampleTime(&_adjustedStartTime));
            Log(Log_Level_Info, L"first sample timeStamp: %I64d\n", _adjustedStartTime);
            _fGetFirstSampleTime = false;
        }

        // Add the sample to the sample queue.
        IFC(_sampleQueue.InsertBack(pSample));

        // Unless we are paused, start an async operation to dispatch the next sample.
        if (SinkStreamState_Paused != _state)
        {
            // Queue the operation.
            IFC(QueueAsyncOperation(SinkStreamOperation_ProcessSample));
        }
    }

    //if (nullptr != _spParentMediaSink) // for local playback
    //{
    //    //_spParentMediaSink->OnSampleUpdated(pSample);
    //}

done:
    return hr;
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::PlaceMarker(
    MFSTREAMSINK_MARKER_TYPE eMarkerType, 
    PROPVARIANT const* pvarMarkerValue, 
    PROPVARIANT const* pvarContextValue)
{
    auto lock = _lock.Lock();

    check_hresult(ValidateOperation(SinkStreamOperation_PlaceMarker));

    com_ptr<IMarker> spMarker;
    check_hresult(MarkerImpl::Create(eMarkerType, pvarMarkerValue, pvarContextValue, &spMarker));

    _sampleQueue.InsertBack(spMarker.get());

    if (SinkStreamState_Paused == _state)
    {
        return S_OK;
    }

    // Queue the operation.
    return QueueAsyncOperation(SinkStreamOperation_PlaceMarker); // Increments ref count on pOp.
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::Flush()
{
    auto lock = _lock.Lock();

    check_hresult(CheckShutdown());

    // Note: Even though we are flushing data, we still need to send
    // any marker events that were queued.
    return HandleError(ProcessSamplesFromQueue(true, nullptr));
}

/// IMFMediaTypeHandler methods
// Check if a media type is supported.
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::IsMediaTypeSupported(
    IMFMediaType* pMediaType, 
    IMFMediaType** ppMediaType)
{
    NULL_CHK(pMediaType);

    auto lock = _lock.Lock();

    check_hresult(CheckShutdown());

    GUID majorType = GUID_NULL;
    check_hresult(pMediaType->GetGUID(MF_MT_MAJOR_TYPE, &majorType));

    check_hresult((majorType == MFMediaType_Video || majorType == MFMediaType_Audio) ? S_OK : MF_E_INVALIDTYPE);

    if (nullptr != _currentType)
    {
        GUID guiNewSubtype = GUID_NULL;
        check_hresult(pMediaType->GetGUID(MF_MT_SUBTYPE, &guiNewSubtype));
        
        if(guiNewSubtype != _currentSubtype)
        {
            return MF_E_INVALIDTYPE;
        }
    }

    if (nullptr != ppMediaType)
    {
       * ppMediaType = nullptr;
    }

    return S_OK;
}

// Return the number of preferred media types.
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::GetMediaTypeCount(
    DWORD* pdwTypeCount)
{
    NULL_CHK(pdwTypeCount);

    auto lock = _lock.Lock();

    check_hresult(CheckShutdown());

    // We've have only one media type
   * pdwTypeCount = 1;

    return S_OK;
}

// Return a preferred media type by index.
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::GetMediaTypeByIndex(
    DWORD dwIndex, 
    IMFMediaType** ppType)
{
    NULL_CHK(ppType);

    auto lock = _lock.Lock();

    check_hresult(CheckShutdown());

    if (dwIndex > 0)
    {
        check_hresult(MF_E_NO_MORE_TYPES);
    }

    NULL_CHK_HR(_currentType, E_NOT_SET);

    return _currentType.CopyTo(ppType);
}

// Set the current media type.
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::SetCurrentMediaType(
    IMFMediaType* pMediaType)
{
    NULL_CHK(pMediaType);

    auto lock = _lock.Lock();

    check_hresult(CheckShutdown());

    // don't allow format changes after streaming starts.
    check_hresult(ValidateOperation(SinkStreamOperation_SetMediaType));

    // set media type already
    if (SinkStreamState_Ready <= _state)
    {
        check_hresult(IsMediaTypeSupported(pMediaType, nullptr));
    }

    GUID guiMajorType = GUID_NULL;
    check_hresult(pMediaType->GetMajorType(&guiMajorType));
    _fIsVideo = (guiMajorType == MFMediaType_Video);

    check_hresult(MFCreateMediaType(&_currentType));
    check_hresult(pMediaType->CopyAllItems(_currentType.get()));
    check_hresult(_currentType->GetGUID(MF_MT_SUBTYPE, &_currentSubtype));
    if (_state < SinkStreamState_Ready)
    {
        _state = SinkStreamState_Ready;
    }
    else if (_state > SinkStreamState_Ready)
    {
        com_ptr<IMFMediaType> spType;
        check_hresult(MFCreateMediaType(&spType));
        check_hresult(pMediaType->CopyAllItems(spType.get()));
        check_hresult(ProcessFormatChange(spType.get()));
    }

    return S_OK;
}

// Return the current media type, if any.
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::GetCurrentMediaType(
    IMFMediaType** ppMediaType)
{
    NULL_CHK(ppMediaType);

    auto lock = _lock.Lock();

    check_hresult(CheckShutdown());

    if (nullptr == _currentType)
    {
        check_hresult(MF_E_NOT_INITIALIZED);
    }

    NULL_CHK_HR(_currentType, E_NOT_SET);

    return _currentType.CopyTo(ppMediaType);
}

// Return the major type GUID.
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::GetMajorType(
    GUID* pguidMajorType)
{
    NULL_CHK(pguidMajorType);

    auto lock = _lock.Lock();

    check_hresult(CheckShutdown());

    if (nullptr == _currentType)
    {
        check_hresult(MF_E_NOT_INITIALIZED);
    }

   * pguidMajorType = (_fIsVideo) ? MFMediaType_Video : MFMediaType_Audio;

    return S_OK;
}

// Called when the presentation clock starts.
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::Start(MFTIME start)
{
    auto lock = _lock.Lock();

    check_hresult(ValidateOperation(SinkStreamOperation_Start));

    if (start != PRESENTATION_CURRENT_POSITION)
    {
        _adjustedStartTime = start; // Cache the start time
    }
    _state = SinkStreamState_Started;

    return QueueAsyncOperation(SinkStreamOperation_Start);
}

// Called when the presentation clock stops.
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::Stop()
{
    check_hresult(ValidateOperation(SinkStreamOperation_Stop));

    _state = SinkStreamState_Stopped;

    return QueueAsyncOperation(SinkStreamOperation_Stop);
}

// Called when the presentation clock pauses.
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::Pause()
{
    check_hresult(ValidateOperation(SinkStreamOperation_Pause));
    
    _state = SinkStreamState_Paused;
    
    return QueueAsyncOperation(SinkStreamOperation_Pause);
}

// Called when the presentation clock restarts.
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::Restart()
{
    check_hresult(ValidateOperation(SinkStreamOperation_Restart));

    _state = SinkStreamState_Started;

    return QueueAsyncOperation(SinkStreamOperation_Restart);
}

// Shuts down the stream sink.
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::Shutdown()
{
    _isPlayerConnected = false;

    if (!_isShutdown)
    {
        if (_eventQueue)
        {
            _eventQueue->Shutdown();
        }

        MFUnlockWorkQueue(_workQueueId);

        _sampleQueue.Clear();

        _eventQueue.Reset();
        _currentType.Reset();

        _isShutdown = true;

        m_connection.Reset();
        m_connection = nullptr;
    }

    return S_OK;
}

// Set the information if we are connected to a client
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::ConnectedFunc(
    bool fConnected, 
    LONGLONG llCurrentTime)
{
    auto lock = _lock.Lock();

    HRESULT hr = S_OK;

    if (_currentType == nullptr)
    {
        IFC(MF_E_NOT_INITIALIZED);
    }

    _isPlayerConnected = fConnected;

    _adjustedStartTime = llCurrentTime;

    if (_isPlayerConnected)
    {
        _fGetFirstSampleTime = true;
    }

    if (fConnected)
    {
        Log(Log_Level_Info, L"SetConnected start=%I64d\n", _adjustedStartTime);
    }

done:
    return HandleError(hr);
}

// Puts an async operation on the work queue.
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::QueueAsyncOperation(
    SinkStreamOperation op)
{
    com_ptr<IUnknown> spOp;
    spOp.attach(new (std::nothrow) AsyncOperation(op)); // Created with ref count = 1
    NULL_CHK_HR(spOp.get(), E_OUTOFMEMORY);

    return MFPutWorkItem2(_workQueueId, 0, &_workQueueCB, spOp.get());
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::OnDispatchWorkItem(
    IMFAsyncResult* pAsyncResult)
{
    // Called by work queue thread. Need to hold the critical section.
    auto lock = _lock.Lock();

    Log(Log_Level_All, L"NetworkMediaSinkStreamImpl::OnDispatchWorkItem() begin...\n");

    HRESULT hr = S_OK;

    // The state object is a AsncOperation object.
    com_ptr<IUnknown> spState;
    check_hresult(pAsyncResult->GetState(&spState));

    AsyncOperation* pOp = static_cast<AsyncOperation *>(spState.get());
    switch (pOp->_op)
    {
    case SinkStreamOperation_Start:
    case SinkStreamOperation_Restart:
        // Send MEStreamSinkStarted.
        IFC(QueueEvent(MEStreamSinkStarted, GUID_NULL, S_OK, nullptr));

        // There might be samples queue from earlier (ie, while paused).
        boolean fRequestMoreSamples;
        if (!_isPlayerConnected)
        {
            // Just drop samples if we are not connected
            IFC(DropSamplesFromQueue(&fRequestMoreSamples));
        }
        else
        {
            IFC(SendSampleFromQueue(&fRequestMoreSamples));
        }

        // If false there is no samples in the queue now so request one
        if (fRequestMoreSamples)
        {
            IFC(QueueEvent(MEStreamSinkRequestSample, GUID_NULL, S_OK, nullptr));
        }
        break;

    case SinkStreamOperation_Stop:
        // Drop samples from queue.
        IFC(DropSamplesFromQueue(nullptr));

        // Send the event even if the previous call failed.
        IFC(QueueEvent(MEStreamSinkStopped, GUID_NULL, S_OK, nullptr));
        break;

    case SinkStreamOperation_Pause:
        IFC(QueueEvent(MEStreamSinkPaused, GUID_NULL, S_OK, nullptr));
        break;

    case SinkStreamOperation_ProcessSample:
    case SinkStreamOperation_PlaceMarker:
    case SinkStreamOperation_SetMediaType:
        IFC(DispatchProcessedSample(pOp));
        break;
    }

done:
    return HandleError(hr);
}

// Complete a ProcessSample or PlaceMarker request.
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::DispatchProcessedSample(
    AsyncOperation* pOp)
{
    NULL_CHK(pOp);

    boolean fRequestMoreSamples = false;
    if (!_isPlayerConnected)
    {
        check_hresult(DropSamplesFromQueue(&fRequestMoreSamples));
    }
    else
    {
        check_hresult(SendSampleFromQueue(&fRequestMoreSamples));
    }

    // Ask for another sample
    if (fRequestMoreSamples)
    {
        if (pOp->_op == SinkStreamOperation_ProcessSample)
        {
            check_hresult(QueueEvent(MEStreamSinkRequestSample, GUID_NULL, S_OK, nullptr));
        }
    }

    return S_OK;
}

// Drop samples in the queue
HRESULT NetworkMediaSinkStream::DropSamplesFromQueue(
    boolean* fRequestMoreSamples)
{
    return ProcessSamplesFromQueue(true, fRequestMoreSamples);
}

// Send sample from the queue
HRESULT NetworkMediaSinkStream::SendSampleFromQueue(
    boolean* fRequestMoreSamples)
{
    return ProcessSamplesFromQueue(false, fRequestMoreSamples);
}

// Checks if an operation is valid in the current state.
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::ValidateOperation(SinkStreamOperation op)
{
    assert(!_isShutdown);

    HRESULT hr = S_OK;

    if (NetworkMediaSinkStream::ValidStateMatrix[static_cast<int>(_state)][static_cast<int>(op)])
    {
        return S_OK;
    }
    else if (_state == SinkStreamState_NotSet)
    {
        return MF_E_NOT_INITIALIZED;
    }
    else
    {
        return MF_E_INVALIDREQUEST;
    }
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::ProcessSamplesFromQueue(
    boolean fFlush,
    boolean *fNeedMoreSamples)
{
    Log(Log_Level_Info, L"NetworkMediaSinkStreamImpl::ProcessSamplesFromQueue()\n");

    if (FAILED(CheckShutdown()))
    {
        fFlush = true;
    }

    boolean fRequestSamples = false;
    boolean fSendSamples = true;
    boolean fSendEOS = false;

    com_ptr<IUnknown> spUnknown;
    if (FAILED(_sampleQueue.RemoveFront(&spUnknown)))
    {
        fRequestSamples = true;
        fSendSamples = false;
    }

    while (fSendSamples)
    {
        com_ptr<IDataBundle> spDataBundle;

        assert(spUnknown);

        bool fProcessingSample = false;

        // Determine if this is a marker or a sample.
        com_ptr<IMFSample> spMediaSample;
        if (SUCCEEDED(spUnknown.As(&spMediaSample)))
        {
            if (!fFlush)
            {
                check_hresult(PrepareSample(spMediaSample.get(), false, &spDataBundle));
                fProcessingSample = true;
            }
        }
        else
        {
            // Check for marker
            com_ptr<IMarker> spMarker;
            if (SUCCEEDED(spUnknown.As(&spMarker)))
            {
                PROPVARIANT var;
                PropVariantInit(&var);

                MFSTREAMSINK_MARKER_TYPE markerType;
                HRESULT hr = spMarker->GetMarkerType(&markerType);
                if (SUCCEEDED(hr))
                {
                    // Get the context data.
                    hr = spMarker->GetContext(&var);
                    if (SUCCEEDED(hr))
                    {
                        hr = QueueEvent(MEStreamSinkMarker, GUID_NULL, S_OK, &var);
                    }
                }

                PropVariantClear(&var);

                check_hresult(hr);

                switch (markerType)
                {
                case MFSTREAMSINK_MARKER_ENDOFSEGMENT:
                    fSendEOS = true;
                    break;
                case MFSTREAMSINK_MARKER_TICK:
                    if (!fFlush || (_isPlayerConnected && !_fGetFirstSampleTime))
                    {
                        PROPVARIANT var;
                        PropVariantInit(&var);

                        LARGE_INTEGER timeStamp;;
                        hr = spMarker->GetMarkerValue(&var);
                        if (SUCCEEDED(hr))
                        {
                            ZeroMemory(&timeStamp, sizeof(timeStamp));
                            if (var.vt == VT_I8)
                            {
                                timeStamp = var.hVal;
                            }
                        }

                        PropVariantClear(&var);

                        check_hresult(hr)

                        com_ptr<IMFSample> spSample;
                        check_hresult(MFCreateSample(&spSample));
                        check_hresult(spSample->SetSampleTime(timeStamp.QuadPart));
                        check_hresult(PrepareStreamTick(spSample.get(), &spDataBundle));
                    }
                    break;
                }
            }
            else
            {
                com_ptr<IMFMediaType> spMediaType;
                check_hresult(spUnknown.As(&spMediaType));
                if (!fFlush && !_fGetFirstSampleTime)
                {
                    check_hresult(PrepareFormatChange(spMediaType.get(), &spDataBundle));
                }
            }
        }

        if (nullptr != spDataBundle.get())
        {
            com_ptr<IAsyncAction> spSendAction;
            if (FAILED(m_connection->SendBundleAsync(spDataBundle.get(), &spSendAction)))
            {
                fProcessingSample = false;
            }
            else
            {
                com_ptr<NetworkMediaSinkStream> spThis(this);
                check_hresult(StartAsyncThen(
                    spSendAction.get(),
                    [this, spThis, fProcessingSample](_In_ HRESULT hr, _In_ IAsyncAction* pResult, _In_ AsyncStatus asyncStatus) -> HRESULT
                {
                    LOG_RESULT(hr);

                    if (_state == SinkStreamState_Started && fProcessingSample)
                    {
                        // If we are still in started state request another sample
                        check_hresult(QueueEvent(MEStreamSinkRequestSample, GUID_NULL, S_OK, nullptr));
                    }

                    return S_OK;
                }));
            }

            // We stop if we processed a sample otherwise keep looking
            fSendSamples = !fProcessingSample;
        }

        if (fSendSamples)
        {
            if (FAILED(_sampleQueue.RemoveFront(&spUnknown)))
            {
                fRequestSamples = true;
                fSendSamples = false;
            }
        }
    }

    if (fSendEOS)
    {
        com_ptr<NetworkMediaSinkStream> spThis(this);
        com_ptr<INetworkMediaSink> spParent(_spParentMediaSink.get());
        concurrency::create_task([this, spThis, spParent]()
        {
            spParent->OnEndOfStream(_dwStreamId);
        });

        fSendSamples = false;
    }

    if (nullptr != fNeedMoreSamples)
    {
        if (FAILED(CheckShutdown()))
        {
            *fNeedMoreSamples = false;
        }
        else
        {
            *fNeedMoreSamples = !fSendSamples;
        }
    }

    return S_OK;
}

HRESULT NetworkMediaSinkStream::ProcessFormatChange(
    IMFMediaType* pMediaType)
{
    Log(Log_Level_Info, L"NetworkMediaSinkStreamImpl::ProcessFormatChange()\n");

    NULL_CHK(pMediaType)

    // Add the media type to the sample queue.
    check_hresult(_sampleQueue.InsertBack(pMediaType));

    // Unless we are paused, start an async operation to dispatch the next sample.
    // Queue the operation.
    return QueueAsyncOperation(SinkStreamOperation_SetMediaType);
}

// Prepare bundle to send with frame sample data
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::PrepareSample(
    IMFSample* pSample, 
    bool fForce,
    IDataBundle** ppDataBundle)
{
    Log(Log_Level_Info, L"NetworkMediaSinkStreamImpl::PrepareSample()\n");

    NULL_CHK(pSample);

    const size_t c_cPayloadHeader = sizeof(PayloadHeader);
    const size_t c_cMediaSampleHeader = sizeof(MediaSampleHeader);
    const size_t c_cbSampleHeaderSize = c_cPayloadHeader + c_cMediaSampleHeader;

    LONGLONG llSampleTime;
    check_hresult(pSample->GetSampleTime(&llSampleTime));

    LONGLONG llDuration;
    check_hresult(pSample->GetSampleDuration(&llDuration));

    llSampleTime -= _adjustedStartTime;

    if (llSampleTime < 0 && !fForce)
    {
        check_hresult(MF_E_LATE_SAMPLE);
    }

    if (llSampleTime < 0)
    {
        llSampleTime = 0;
    }

    DWORD cbTotalSampleLength = 0;
    check_hresult(pSample->GetTotalLength(&cbTotalSampleLength));

    // Create a bundle and initialize it with the sample
    com_ptr<IDataBundle> spBundle;
    check_hresult(MakeAndInitialize<DataBundle>(&spBundle, pSample));

    // create a buffer for the media sample header
    com_ptr<IDataBuffer> spDataBuffer;
    check_hresult(MakeAndInitialize<DataBuffer>(&spDataBuffer, c_cbSampleHeaderSize));

    // Prepare the buffer
    com_ptr<IBuffer> spHeaderBuffer;
    check_hresult(spDataBuffer.As(&spHeaderBuffer));

    BYTE* pBuf = GetDataType<BYTE*>(spHeaderBuffer.get());

    // populate the PayloadType header
    PayloadHeader* pOpHeader = reinterpret_cast<PayloadHeader*>(pBuf);
    ZeroMemory(pOpHeader, c_cPayloadHeader);

    // fill in the PayloadType header info
    pOpHeader->ePayloadType = PayloadType_SendMediaSample;
    pOpHeader->cbPayloadSize = c_cMediaSampleHeader + cbTotalSampleLength;

    // fill in the media sample header info
    MediaSampleHeader* pSampleHeader = reinterpret_cast<MediaSampleHeader *>(pBuf + c_cPayloadHeader);
    ZeroMemory(pSampleHeader, c_cMediaSampleHeader);
    GetIdentifier(&pSampleHeader->dwStreamId);
    pSampleHeader->hnsTimestamp = llSampleTime;
    pSampleHeader->hnsDuration = llDuration;

    // dwFlags and masks
    if (IsVideo())
    {
        Log(Log_Level_All, L"NetworkMediaSinkStreamImpl::PrepareSample: TS: %I64d, DUR: %I64d\n", pSampleHeader->hnsTimestamp, pSampleHeader->hnsDuration);
        SET_SAMPLE_FLAG(pSampleHeader->dwFlags, pSampleHeader->dwFlagMasks, pSample, BottomFieldFirst);
        SET_SAMPLE_FLAG(pSampleHeader->dwFlags, pSampleHeader->dwFlagMasks, pSample, CleanPoint);
        SET_SAMPLE_FLAG(pSampleHeader->dwFlags, pSampleHeader->dwFlagMasks, pSample, DerivedFromTopField);
        SET_SAMPLE_FLAG(pSampleHeader->dwFlags, pSampleHeader->dwFlagMasks, pSample, Discontinuity);
        SET_SAMPLE_FLAG(pSampleHeader->dwFlags, pSampleHeader->dwFlagMasks, pSample, Interlaced);
        SET_SAMPLE_FLAG(pSampleHeader->dwFlags, pSampleHeader->dwFlagMasks, pSample, RepeatFirstField);
        SET_SAMPLE_FLAG(pSampleHeader->dwFlags, pSampleHeader->dwFlagMasks, pSample, SingleField);

        pSampleHeader->cbCameraDataSize = 0;
		/*
		TODO: Revisit here
        if (FAILED(ProcessCameraData(pSample, pSampleHeader)))
        {
            ZeroMemory(&pSampleHeader->worldToCameraMatrix, sizeof(Matrix4x4));
            ZeroMemory(&pSampleHeader->cameraProjectionTransform, sizeof(Matrix4x4));
            ZeroMemory(&pSampleHeader->cameraViewTransform, sizeof(Matrix4x4));
        }*/

		// We don't care about this now. Zero out now
		ZeroMemory(&pSampleHeader->worldToCameraMatrix, sizeof(Matrix4x4));
		ZeroMemory(&pSampleHeader->cameraProjectionTransform, sizeof(Matrix4x4));
		ZeroMemory(&pSampleHeader->cameraViewTransform, sizeof(Matrix4x4));
    }

    // update the payload size to include additional buffer
    pOpHeader->cbPayloadSize += pSampleHeader->cbCameraDataSize;

    // set the size of the header buffer
    check_hresult(spDataBuffer->put_CurrentLength(c_cbSampleHeaderSize));

    // Put headers before the mediasample and camera data
    check_hresult(spBundle->InsertBuffer(0, spDataBuffer.get()));

    NULL_CHK_HR(spBundle, E_NOT_SET);

    return spBundle.CopyTo(ppDataBundle);
}

// Prepare bundle to notify missing sample for particular timestamp
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::PrepareStreamTick(
    IMFAttributes* pAttributes,
    IDataBundle** ppDataBundle)
{
    Log(Log_Level_Info, L"NetworkMediaSinkStreamImpl::PrepareStreamTick()\n");

    NULL_CHK(pAttributes);
    NULL_CHK(ppDataBundle);

    const DWORD c_cbHeaderSize = sizeof(PayloadHeader) + sizeof(MediaStreamTick);

    com_ptr<IDataBuffer> spHeader;
    check_hresult(MakeAndInitialize<DataBuffer>(&spHeader, c_cbHeaderSize));

    // Prepare the buffer
    com_ptr<IBuffer> spBuffer;
    check_hresult(spHeader.As(&spBuffer));

    // Prepare PayloadType header
    BYTE* pHeaderBuffer = GetDataType<BYTE*>(spBuffer.get());
    PayloadHeader* pOpHeader = reinterpret_cast<PayloadHeader *>(pHeaderBuffer);
    pOpHeader->ePayloadType = PayloadType_SendMediaStreamTick;
    pOpHeader->cbPayloadSize = sizeof(MediaStreamTick);

    // Prepare description
    MediaStreamTick* pSampleTick = reinterpret_cast<MediaStreamTick *>(pHeaderBuffer + sizeof(PayloadHeader));
    pSampleTick->dwStreamId = _dwStreamId;

    LONGLONG llSampleTime;
    com_ptr<IMFSample> spSample;
    check_hresult(pAttributes->QueryInterface(__uuidof(IMFSample), static_cast<LPVOID*>(&spSample)));
    check_hresult(spSample->GetSampleTime(&llSampleTime));

    pSampleTick->hnsTimestamp = llSampleTime - _adjustedStartTime;
    if (pSampleTick->hnsTimestamp < 0)
    {
        pSampleTick->hnsTimestamp = 0;
    }

    check_hresult(pAttributes->SetUINT64(MFSampleExtension_Timestamp, llSampleTime));

    // copy attribute blob to a buffer
    // Get size of attributes blob
    check_hresult(MFGetAttributesAsBlobSize(pAttributes, &pSampleTick->cbAttributesSize));

    // Create a buffer for attribute blob
    com_ptr<IDataBuffer> spDataBuffer;
    check_hresult(MakeAndInitialize<DataBuffer>(&spDataBuffer, pSampleTick->cbAttributesSize));

    // Prepare the IBuffer
    com_ptr<IBuffer> spAttribBuffer;
    check_hresult(spDataBuffer.As(&spAttribBuffer));

    BYTE* pBuf = GetDataType<BYTE*>(spAttribBuffer.get());

    // Copy attributes to the buffer
    check_hresult(MFGetAttributesAsBlob(pAttributes, pBuf, pSampleTick->cbAttributesSize));

    check_hresult(spDataBuffer->put_CurrentLength(pSampleTick->cbAttributesSize));

    // Add size of variable size attribute blob to size of the package.
    pOpHeader->cbPayloadSize += pSampleTick->cbAttributesSize;

    // Set length of the buffer
    check_hresult(spHeader->put_CurrentLength(c_cbHeaderSize));

    // Prepare bundle to send
    com_ptr<IDataBundle> spBundle;
    check_hresult(MakeAndInitialize<DataBundle>(&spBundle));

    // Add fixed size header and description to the bundle
    check_hresult(spBundle->AddBuffer(spHeader.get()));

    // Add attributes
    check_hresult(spBundle->AddBuffer(spDataBuffer.get()));

    NULL_CHK_HR(spBundle, E_NOT_SET);

    return spBundle.CopyTo(ppDataBundle);
}

// Prepare bundle with format change information to be sent over the wire
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::PrepareFormatChange(
    IMFMediaType* pMediaType, 
    IDataBundle** ppDataBundle)
{
    Log(Log_Level_Info, L"NetworkMediaSinkStreamImpl::CompleteOpen()\n");

    const DWORD c_cbPayloadSize = sizeof(PayloadHeader) + sizeof(MediaTypeDescription);

    com_ptr<IDataBuffer> spDataBuffer;
    check_hresult(MakeAndInitialize<DataBuffer>(&spDataBuffer, c_cbPayloadSize));

    // Prepare the buffer
    com_ptr<IBuffer> spBuffer;
    check_hresult(spDataBuffer.As(&spBuffer));

    // Prepare PayloadType header
    BYTE* pBuf = GetDataType<BYTE*>(spBuffer.get());
    PayloadHeader* pOpHeader = reinterpret_cast<PayloadHeader *>(pBuf);
    pOpHeader->cbPayloadSize = sizeof(MediaTypeDescription);
    pOpHeader->ePayloadType = PayloadType_SendFormatChange;

    // Prepare description
    MediaTypeDescription* pStreamDescription = reinterpret_cast<MediaTypeDescription *>(pBuf + sizeof(PayloadHeader));
    com_ptr<IDataBuffer> spAttr;
    check_hresult(FillStreamDescription(pStreamDescription, &spAttr));

    // Add size of attribute blob to size of the package.
    pOpHeader->cbPayloadSize += pStreamDescription->AttributesBlobSize;

    // Set length of the buffer
    check_hresult(spDataBuffer->put_CurrentLength(c_cbPayloadSize));

    // Prepare bundle to send
    com_ptr<IDataBundle> spBundle;
    check_hresult(MakeAndInitialize<DataBundle>(&spBundle));

    // Add fixed size header and description to the bundle
    check_hresult(spBundle->AddBuffer(spDataBuffer.get()));

    // Add attributes
    check_hresult(spBundle->AddBuffer(spAttr.get()));

    return spBundle.CopyTo(ppDataBundle);
}

// Fill stream description and prepare attributes blob.
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::FillStreamDescription(
    MediaTypeDescription* pStreamDescription,
    IDataBuffer** ppDataBuffer)
{
    Log(Log_Level_Info, L"NetworkMediaSinkStreamImpl::CompleteOpen()\n");

    NULL_CHK(ppDataBuffer);

    // Clear the stream descriptor memory
    ZeroMemory(pStreamDescription, sizeof(MediaTypeDescription));

    // Get the media type for the stream
    com_ptr<IMFMediaType> spMediaType;
    check_hresult(GetCurrentMediaType(&spMediaType));

    /*
    // filter types to those deemed needed
    com_ptr<IMFMediaType> spFilteredMediaType;
    check_hresult(MFCreateMediaType(&spFilteredMediaType));
    check_hresult(FilterOutputMediaType(spMediaType.get(), spFilteredMediaType.get()));
    */

    // fill in streamDescription
    pStreamDescription->dwStreamId = _dwStreamId;

    // set major type (Audio, Video and so on)
    check_hresult(GetMajorType(&pStreamDescription->guiMajorType));

    // set subtype (format of the stream)
    check_hresult(spMediaType->GetGUID(MF_MT_SUBTYPE, &pStreamDescription->guiSubType));

    // Set size of attributes blob
    UINT32 attributesSize = 0;
    //check_hresult(MFGetAttributesAsBlobSize(spFilteredMediaType.get(), &attributesSize));
    check_hresult(MFGetAttributesAsBlobSize(spMediaType.get(), &attributesSize));

    // Prepare a buffer for the filtered mediaType
    com_ptr<DataBuffer> spAttributes;
    check_hresult(MakeAndInitialize<DataBuffer>(&spAttributes, attributesSize));

    // Set length of the buffer
    check_hresult(spAttributes->put_CurrentLength(attributesSize));

    // Copy attributes to the buffer
    UINT8* pBuffer = GetDataType<UINT8*>(spAttributes.get());
    NULL_CHK(pBuffer);

    check_hresult(MFGetAttributesAsBlob(spMediaType.get(), pBuffer, attributesSize));
    //check_hresult(MFGetAttributesAsBlob(spFilteredMediaType.get(), pBuffer, attributesSize));

    // were good, save the valus and return
    pStreamDescription->AttributesBlobSize = attributesSize;

    return spAttributes.CopyTo(ppDataBuffer);
}
