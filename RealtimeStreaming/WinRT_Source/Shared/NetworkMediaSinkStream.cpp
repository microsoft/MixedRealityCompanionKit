// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "NetworkMediaSinkStream.h"
#include "MediaUtils.h"
#include "Marker.h"

#include "DataBuffer.h"
#include "DataBundle.h"

using namespace winrt;
using namespace RealtimeStreaming::Media::implementation;
using namespace RealtimeStreaming::Network;
using namespace RealtimeStreaming::Common;

using namespace Windows::Foundation;
using namespace Windows::Storage::Streams;

using SinkStreamOp = RealtimeStreaming::Media::SinkStreamOperation;

#define SET_SAMPLE_FLAG(dest, destMask, pSample, flagName) \
{ \
    UINT32 unValue; \
    if (SUCCEEDED(pSample->GetUINT32(MFSampleExtension_##flagName, &unValue))) \
    { \
        dest |= (unValue != FALSE) ? static_cast<DWORD>(SampleFlags::SampleFlag_##flagName) : 0; \
        destMask |= static_cast<DWORD>(SampleFlags::SampleFlag_##flagName); \
    } \
}

// If an entry is TRUE, the operation is valid from that state.
bool NetworkMediaSinkStream::ValidStateMatrix[(int)winrt::RealtimeStreaming::Media::SinkStreamState::Count][(int)SinkStreamOp::Count] =
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
NetworkMediaSinkStream::AsyncOperation::AsyncOperation(winrt::RealtimeStreaming::Media::SinkStreamOperation op)
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
NetworkMediaSinkStream::NetworkMediaSinkStream(uint32_t streamId,
    RealtimeStreaming::Network::Connection connection,
    RealtimeStreaming::Media::NetworkMediaSink parentMediaSink)
    : m_dwStreamId(streamId)
    , m_state(SinkStreamState::NotSet)
    , m_isShutdown(false)
    , m_isPlayerConnected(false)
    , m_fIsVideo(false)
    , m_fGetFirstSampleTime(false)
    , m_adjustedStartTime(0)
    , m_workQueueId(0)
    , m_workQueueCB(this, &NetworkMediaSinkStream::OnDispatchWorkItem)
    , m_connection(connection)
    ,m_parentMediaSink(parentMediaSink)
{
    NULL_THROW(parentMediaSink);

    // Create the event queue helper.
    IFT(MFCreateEventQueue(m_eventQueue.put()));
    IFT(MFAllocateSerialWorkQueue(MFASYNC_CALLBACK_QUEUE_STANDARD, &m_workQueueId));

    ZeroMemory(&m_currentSubtype, sizeof(m_currentSubtype));
}

_Use_decl_annotations_
NetworkMediaSinkStream::~NetworkMediaSinkStream()
{
    assert(m_isShutdown);
}

// IMFMediaEventGenerator methods.
// Note: These methods call through to the event queue helper object.
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::BeginGetEvent(
    IMFAsyncCallback* pCallback, 
    IUnknown* punkState)
{
    slim_shared_lock_guard const guard(m_lock);

    IFR(CheckShutdown());

    return m_eventQueue->BeginGetEvent(pCallback, punkState);
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::EndGetEvent(
    IMFAsyncResult* pResult, 
    IMFMediaEvent** ppEvent)
{
    NULL_CHK(ppEvent);

    slim_shared_lock_guard const guard(m_lock);

    IFR(CheckShutdown());

    return m_eventQueue->EndGetEvent(pResult, ppEvent);
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
        slim_shared_lock_guard const guard(m_lock);

        // Check shutdown
        IFR(CheckShutdown());

        // Get the pointer to the event queue.
        spQueue = m_eventQueue;
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
    slim_shared_lock_guard const guard(m_lock);

    // Call internal method wrapped with lock
    return _QueueEvent(met, guidExtendedType, hrStatus, pvValue);
}

// IMFStreamSink methods
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::GetMediaSink(
    IMFMediaSink** ppMediaSink)
{
    NULL_CHK(ppMediaSink);

    slim_lock_guard guard(m_lock);

    IFR(CheckShutdown());

    NULL_CHK_HR(m_parentMediaSink, E_NOT_SET);

    com_ptr<IMFMediaSink> spMediaSink = m_parentMediaSink.as<IMFMediaSink>();

    spMediaSink.copy_to(ppMediaSink);
    
    return S_OK;
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::GetIdentifier(
    DWORD* pdwIdentifier)
{
    NULL_CHK(pdwIdentifier);

    slim_lock_guard guard(m_lock);

    IFR(CheckShutdown());

   * pdwIdentifier = m_dwStreamId;

    return S_OK;
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::GetMediaTypeHandler(
    IMFMediaTypeHandler** ppHandler)
{
    NULL_CHK(ppHandler);

    slim_shared_lock_guard const guard(m_lock);

    IFR(CheckShutdown());

    // This stream object acts as its own type handler, so we QI ourselves.
    return QueryInterface(IID_IMFMediaTypeHandler, reinterpret_cast<void**>(ppHandler));
}

// We received a sample from an upstream component
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::ProcessSample(
    IMFSample* pSample)
{
    NULL_CHK(pSample);

    Log(Log_Level_All, L"NetworkMediaSinkStream::ProcessSample() begin...\n");

    slim_lock_guard guard(m_lock);

    HRESULT hr = S_OK;

    IFC(CheckShutdown());

    // Validate the operation.
    IFC(ValidateOperation(SinkStreamOp::ProcessSample));

    if(!m_isPlayerConnected)
    {
        // we do not have a connected player yet
        Log(Log_Level_Info, L"Waiting for a connected player, ignoring sample...\n");

        if (nullptr != m_connection)
        {
            // TODO: Switch back to sync
            m_connection.SendPayloadTypeAsync(PayloadType::State_CaptureReady);//.get();
        }

        IFC(_QueueEvent(MEStreamSinkRequestSample, GUID_NULL, hr, nullptr));
    }
    else
    {
        // is this our first sample since connecting?
        if (m_isPlayerConnected && m_fGetFirstSampleTime)
        {
            // pulls the sample time to rebase timestamps
            IFC(pSample->GetSampleTime(&m_adjustedStartTime));
            Log(Log_Level_Info, L"first sample timeStamp: %I64d\n", m_adjustedStartTime);
            m_fGetFirstSampleTime = false;
        }

        // Add the sample to the sample queue.
        com_ptr<IMFSample> spSample;
        spSample.copy_from(pSample);

        m_sampleQueue.push_back(spSample);
        //IFC(m_sampleQueue.InsertBack(pSample));

        // Unless we are paused, start an async operation to dispatch the next sample.
        if (SinkStreamState::Paused != m_state)
        {
            // Queue the operation.
            IFC(QueueAsyncOperation(SinkStreamOp::ProcessSample));
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
    slim_lock_guard guard(m_lock);

    IFR(ValidateOperation(SinkStreamOp::PlaceMarker));

    com_ptr<IMarker> spMarker;
    IFR(MarkerImpl::Create(eMarkerType, pvarMarkerValue, pvarContextValue, spMarker.put()));

    m_sampleQueue.push_back(spMarker);
    //m_sampleQueue.InsertBack(spMarker.get());

    if (SinkStreamState::Paused == m_state)
    {
        return S_OK;
    }

    // Queue the operation.
    return QueueAsyncOperation(SinkStreamOp::PlaceMarker); // Increments ref count on pOp.
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::Flush()
{
    slim_lock_guard guard(m_lock);

    IFR(CheckShutdown());

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

    slim_shared_lock_guard const guard(m_lock);

    IFR(CheckShutdown());

    GUID majorType = GUID_NULL;
    IFR(pMediaType->GetGUID(MF_MT_MAJOR_TYPE, &majorType));

    IFR((majorType == MFMediaType_Video || majorType == MFMediaType_Audio) ? S_OK : MF_E_INVALIDTYPE);

    if (nullptr != m_currentType)
    {
        GUID guiNewSubtype = GUID_NULL;
        IFR(pMediaType->GetGUID(MF_MT_SUBTYPE, &guiNewSubtype));
        
        if(guiNewSubtype != m_currentSubtype)
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

    slim_shared_lock_guard const guard(m_lock);

    IFR(CheckShutdown());

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

    slim_shared_lock_guard const guard(m_lock);

    IFR(CheckShutdown());

    if (dwIndex > 0)
    {
        IFR(MF_E_NO_MORE_TYPES);
    }

    NULL_CHK_HR(m_currentType, E_NOT_SET);

    com_ptr<IMFMediaType> currType = m_currentType.as<IMFMediaType>();

    currType.copy_to(ppType);
}

// Set the current media type.
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::SetCurrentMediaType(
    IMFMediaType* pMediaType)
{
    NULL_CHK(pMediaType);

    {
        slim_lock_guard guard(m_lock);

        IFR(CheckShutdown());

        // don't allow format changes after streaming starts.
        IFR(ValidateOperation(SinkStreamOp::SetMediaType));
    }

    // set media type already
    if (SinkStreamState::Ready <= m_state)
    {
        IFR(IsMediaTypeSupported(pMediaType, nullptr));
    }

    slim_lock_guard guard(m_lock);

    GUID guiMajorType = GUID_NULL;
    IFR(pMediaType->GetMajorType(&guiMajorType));
    m_fIsVideo = (guiMajorType == MFMediaType_Video);

    m_currentType = nullptr; // reset comptr for new setting
    IFR(MFCreateMediaType(m_currentType.put()));
    IFR(pMediaType->CopyAllItems(m_currentType.get()));
    IFR(m_currentType->GetGUID(MF_MT_SUBTYPE, &m_currentSubtype));
    if (m_state < SinkStreamState::Ready)
    {
        m_state = SinkStreamState::Ready;
    }
    else if (m_state > SinkStreamState::Ready)
    {
        com_ptr<IMFMediaType> spType;
        IFR(MFCreateMediaType(spType.put()));
        IFR(pMediaType->CopyAllItems(spType.get()));
        IFR(ProcessFormatChange(spType.get()));
    }

    return S_OK;
}

// Return the current media type, if any.
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::GetCurrentMediaType(
    IMFMediaType** ppMediaType)
{
    NULL_CHK(ppMediaType);

    slim_lock_guard guard(m_lock);

    IFR(CheckShutdown());

    if (nullptr == m_currentType)
    {
        IFR(MF_E_NOT_INITIALIZED);
    }

    NULL_CHK_HR(m_currentType, E_NOT_SET);

    m_currentType.copy_to(ppMediaType);
    return S_OK;
}

// Return the major type GUID.
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::GetMajorType(
    GUID* pguidMajorType)
{
    NULL_CHK(pguidMajorType);

    slim_shared_lock_guard const guard(m_lock);

    IFR(CheckShutdown());

    if (nullptr == m_currentType)
    {
        IFR(MF_E_NOT_INITIALIZED);
    }

    *pguidMajorType = (m_fIsVideo) ? MFMediaType_Video : MFMediaType_Audio;

    return S_OK;
}

// Called when the presentation clock starts.
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::Start(MFTIME start)
{
    slim_lock_guard guard(m_lock);

    IFR(ValidateOperation(SinkStreamOp::Start));

    if (start != PRESENTATION_CURRENT_POSITION)
    {
        m_adjustedStartTime = start; // Cache the start time
    }
    m_state = SinkStreamState::Started;

    return QueueAsyncOperation(SinkStreamOp::Start);
}

// Called when the presentation clock stops.
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::Stop()
{
    IFR(ValidateOperation(SinkStreamOp::Stop));

    m_state = SinkStreamState::Stopped;

    return QueueAsyncOperation(SinkStreamOp::Stop);
}

// Called when the presentation clock pauses.
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::Pause()
{
    IFR(ValidateOperation(SinkStreamOp::Pause));
    
    m_state = SinkStreamState::Paused;
    
    return QueueAsyncOperation(SinkStreamOp::Pause);
}

// Called when the presentation clock restarts.
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::Restart()
{
    IFR(ValidateOperation(SinkStreamOp::Restart));

    m_state = SinkStreamState::Started;

    return QueueAsyncOperation(SinkStreamOp::Restart);
}

// Shuts down the stream sink.
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::Shutdown()
{
    m_isPlayerConnected = false;

    if (!m_isShutdown)
    {
        if (m_eventQueue)
        {
            m_eventQueue->Shutdown();
        }

        MFUnlockWorkQueue(m_workQueueId);

        m_sampleQueue.clear();

        m_eventQueue = nullptr;
        m_currentType = nullptr;

        m_isShutdown = true;

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
    slim_lock_guard guard(m_lock);

    HRESULT hr = S_OK;

    if (m_currentType == nullptr)
    {
        IFC(MF_E_NOT_INITIALIZED);
    }

    m_isPlayerConnected = fConnected;

    m_adjustedStartTime = llCurrentTime;

    if (m_isPlayerConnected)
    {
        m_fGetFirstSampleTime = true;
    }

    if (fConnected)
    {
        Log(Log_Level_Info, L"SetConnected start=%I64d\n", m_adjustedStartTime);
    }

done:
    return HandleError(hr);
}

// Puts an async operation on the work queue.
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::QueueAsyncOperation(
    RealtimeStreaming::Media::SinkStreamOperation op)
{
    com_ptr<IUnknown> spOp;
    spOp.attach(new (std::nothrow) AsyncOperation(op)); // Created with ref count = 1
    NULL_CHK_HR(spOp.get(), E_OUTOFMEMORY);

    /*
    IMFAsyncCallback* owning{ nullptr };
    winrt::copy_to_abi(m_workQueueCB, *reinterpret_cast<void**>(&owning));

    return MFPutWorkItem2(m_workQueueId, 0, owning, spOp.get());
    */
    return MFPutWorkItem2(m_workQueueId, 0, &m_workQueueCB, spOp.get());
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::OnDispatchWorkItem(
    IMFAsyncResult* pAsyncResult)
{
    // Called by work queue thread. Need to hold the critical section.
    slim_lock_guard guard(m_lock);

    Log(Log_Level_All, L"NetworkMediaSinkStream::OnDispatchWorkItem() begin...\n");

    HRESULT hr = S_OK;

    // The state object is a AsncOperation object.
    com_ptr<IUnknown> spState;
    IFR(pAsyncResult->GetState(spState.put()));

    AsyncOperation* pOp = static_cast<AsyncOperation *>(spState.get());
    switch (pOp->_op)
    {
    case SinkStreamOp::Start:
    case SinkStreamOp::Restart:
        // Send MEStreamSinkStarted.
        IFC(_QueueEvent(MEStreamSinkStarted, GUID_NULL, S_OK, nullptr));

        // There might be samples queue from earlier (ie, while paused).
        boolean fRequestMoreSamples;
        if (!m_isPlayerConnected)
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
            IFC(_QueueEvent(MEStreamSinkRequestSample, GUID_NULL, S_OK, nullptr));
        }
        break;

    case SinkStreamOp::Stop:
        // Drop samples from queue.
        IFC(DropSamplesFromQueue(nullptr));

        // Send the event even if the previous call failed.
        IFC(_QueueEvent(MEStreamSinkStopped, GUID_NULL, S_OK, nullptr));
        break;

    case SinkStreamOp::Pause:
        IFC(_QueueEvent(MEStreamSinkPaused, GUID_NULL, S_OK, nullptr));
        break;

    case SinkStreamOp::ProcessSample:
    case SinkStreamOp::PlaceMarker:
    case SinkStreamOp::SetMediaType:
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
    if (!m_isPlayerConnected)
    {
        IFR(DropSamplesFromQueue(&fRequestMoreSamples));
    }
    else
    {
        IFR(SendSampleFromQueue(&fRequestMoreSamples));
    }

    // Ask for another sample
    if (fRequestMoreSamples)
    {
        if (pOp->_op == SinkStreamOp::ProcessSample)
        {
            IFR(_QueueEvent(MEStreamSinkRequestSample, GUID_NULL, S_OK, nullptr));
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
HRESULT NetworkMediaSinkStream::ValidateOperation(
    RealtimeStreaming::Media::SinkStreamOperation op)
{
    assert(!m_isShutdown);

    HRESULT hr = S_OK;

    if (NetworkMediaSinkStream::ValidStateMatrix[static_cast<int>(m_state)][static_cast<int>(op)])
    {
        return S_OK;
    }
    else if (m_state == SinkStreamState::NotSet)
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
    Log(Log_Level_Info, L"NetworkMediaSinkStream::ProcessSamplesFromQueue()\n");

    if (FAILED(CheckShutdown()))
    {
        fFlush = true;
    }

    boolean fRequestSamples = false;
    boolean fSendSamples = true;
    boolean fSendEOS = false;
    com_ptr<IUnknown> spUnknown;

    if (m_sampleQueue.empty())
    {
        fRequestSamples = true;
        fSendSamples = false;
    }
    else
    {
        spUnknown = m_sampleQueue.front();
        m_sampleQueue.erase(m_sampleQueue.begin()); // Remove front element
    }

    /*
    TODO: necessary?
    if (FAILED(m_sampleQueue.RemoveFront(spUnknown.put())))
    {
        fRequestSamples = true;
        fSendSamples = false;
    }*/

    while (fSendSamples)
    {
        DataBundle dataBundle = nullptr;

        assert(spUnknown);

        bool fProcessingSample = false;

        // Determine if this is a marker or a sample.
        com_ptr<IMFSample> spMediaSample = spUnknown.as<IMFSample>();
        if (spMediaSample != nullptr)
        {
            if (!fFlush)
            {
                dataBundle = PrepareSample(spMediaSample.get(), false);
                fProcessingSample = true;
            }
        }
        else
        {
            // Check for marker
            com_ptr<IMarker> spMarker = spUnknown.as<IMarker>();
            if (spMarker != nullptr)
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
                        hr = _QueueEvent(MEStreamSinkMarker, GUID_NULL, S_OK, &var);
                    }
                }

                PropVariantClear(&var);

                IFR(hr);

                switch (markerType)
                {
                case MFSTREAMSINK_MARKER_ENDOFSEGMENT:
                    fSendEOS = true;
                    break;
                case MFSTREAMSINK_MARKER_TICK:
                    if (!fFlush || (m_isPlayerConnected && !m_fGetFirstSampleTime))
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

                        IFR(hr)

                        com_ptr<IMFSample> spSample;
                        IFR(MFCreateSample(spSample.put()));
                        IFR(spSample->SetSampleTime(timeStamp.QuadPart));

                        dataBundle = PrepareStreamTick(spSample.get());
                    }
                    break;
                }
            }
            else
            {
                com_ptr<IMFMediaType> spMediaType = spUnknown.as<IMFMediaType>();
                if (!fFlush && !m_fGetFirstSampleTime)
                {
                    dataBundle = PrepareFormatChange(spMediaType.get());
                }
            }
        }

        if (nullptr != dataBundle)
        {
            try
            {
                // Block thread until bundle is sent
                 m_connection.SendBundleAsync(dataBundle).get();
            }
            catch (hresult_error const & e)
            {
                fProcessingSample = false;
                LOG_RESULT(e.code());
            }

            if (m_state == SinkStreamState::Started && fProcessingSample)
            {
                // If we are still in started state request another sample
                IFR(_QueueEvent(MEStreamSinkRequestSample, GUID_NULL, S_OK, nullptr));
            }

            // We stop if we processed a sample otherwise keep looking
            fSendSamples = !fProcessingSample;
        }

        if (fSendSamples)
        {

            if (m_sampleQueue.empty())
            //if (FAILED(m_sampleQueue.RemoveFront(spUnknown.put())))
            {
                fRequestSamples = true;
                fSendSamples = false;
            }
            else
            {
                spUnknown = m_sampleQueue.front();
                m_sampleQueue.erase(m_sampleQueue.begin()); // Remove front element
            }
        }
    }

    if (fSendEOS)
    {
        /* TODO: Look at this if acting up
        com_ptr<implementation::NetworkMediaSinkStream> spThis(*this);
        com_ptr<NetworkMediaSink> spParent(m_parentMediaSink.get());
        concurrency::create_task([this, spThis, spParent]()
        {
            spParent->OnEndOfStream(m_dwStreamId);
        });*/

        concurrency::create_task([this]()
        {
            m_parentMediaSink.OnEndOfStream(m_dwStreamId);
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
    Log(Log_Level_Info, L"NetworkMediaSinkStream::ProcessFormatChange()\n");

    NULL_CHK(pMediaType)

    // Add the media type to the sample queue.
    com_ptr<IMFMediaType> spMediaType;
    spMediaType.copy_from(pMediaType);
    m_sampleQueue.push_back(spMediaType);

    // Unless we are paused, start an async operation to dispatch the next sample.
    // Queue the operation.
    return QueueAsyncOperation(SinkStreamOp::SetMediaType);
}

// Prepare bundle to send with frame sample data
_Use_decl_annotations_
DataBundle NetworkMediaSinkStream::PrepareSample(
    IMFSample* pSample, 
    bool fForce)
{
    Log(Log_Level_Info, L"NetworkMediaSinkStream::PrepareSample()\n");

    NULL_THROW(pSample);
    HRESULT hr = S_OK;

    const size_t c_cPayloadHeader = sizeof(PayloadHeader);
    const size_t c_cMediaSampleHeader = sizeof(MediaSampleHeader);
    const size_t c_cbSampleHeaderSize = c_cPayloadHeader + c_cMediaSampleHeader;

    LONGLONG llSampleTime;
    IFT(pSample->GetSampleTime(&llSampleTime));

    LONGLONG llDuration;
    IFT(pSample->GetSampleDuration(&llDuration));

    llSampleTime -= m_adjustedStartTime;

    if (llSampleTime < 0 && !fForce)
    {
        IFT(MF_E_LATE_SAMPLE);
    }

    if (llSampleTime < 0)
    {
        llSampleTime = 0;
    }

    DWORD cbTotalSampleLength = 0;
    IFT(pSample->GetTotalLength(&cbTotalSampleLength));

    // Create a bundle and initialize it with the sample
    DataBundle dataBundle = make<Network::implementation::DataBundle>(pSample);

    // create a buffer for the media sample header
    DataBuffer sampleInfoBuffer = winrt::make<Network::implementation::DataBuffer>(c_cbSampleHeaderSize);

    // Prepare the buffer
    BYTE* pBuf = Network::implementation::DataBuffer::GetBufferPointer(sampleInfoBuffer);

    // populate the PayloadType header
    PayloadHeader* pOpHeader = reinterpret_cast<PayloadHeader*>(pBuf);
    ZeroMemory(pOpHeader, c_cPayloadHeader);
    pOpHeader->ePayloadType = PayloadType::SendMediaSample;
    pOpHeader->cbPayloadSize = c_cMediaSampleHeader + cbTotalSampleLength;

    // fill in the media sample header info
    MediaSampleHeader* pSampleHeader = reinterpret_cast<MediaSampleHeader *>(pBuf + c_cPayloadHeader);
    ZeroMemory(pSampleHeader, c_cMediaSampleHeader);
    pSampleHeader->dwStreamId = m_dwStreamId;
    pSampleHeader->hnsTimestamp = llSampleTime;
    pSampleHeader->hnsDuration = llDuration;

    // dwFlags and masks
    if (IsVideo())
    {
        Log(Log_Level_All, L"NetworkMediaSinkStream::PrepareSample: TS: %I64d, DUR: %I64d\n", pSampleHeader->hnsTimestamp, pSampleHeader->hnsDuration);
        SET_SAMPLE_FLAG(pSampleHeader->dwFlags, pSampleHeader->dwFlagMasks, pSample, BottomFieldFirst);
        SET_SAMPLE_FLAG(pSampleHeader->dwFlags, pSampleHeader->dwFlagMasks, pSample, CleanPoint);
        SET_SAMPLE_FLAG(pSampleHeader->dwFlags, pSampleHeader->dwFlagMasks, pSample, DerivedFromTopField);
        SET_SAMPLE_FLAG(pSampleHeader->dwFlags, pSampleHeader->dwFlagMasks, pSample, Discontinuity);
        SET_SAMPLE_FLAG(pSampleHeader->dwFlags, pSampleHeader->dwFlagMasks, pSample, Interlaced);
        SET_SAMPLE_FLAG(pSampleHeader->dwFlags, pSampleHeader->dwFlagMasks, pSample, RepeatFirstField);
        SET_SAMPLE_FLAG(pSampleHeader->dwFlags, pSampleHeader->dwFlagMasks, pSample, SingleField);
    }

    // set the size of the header buffer
    sampleInfoBuffer.CurrentLength(c_cbSampleHeaderSize);

    // Put headers before the mediasample and camera data
    dataBundle.InsertBuffer(0, sampleInfoBuffer);

    return dataBundle;
}

// Prepare bundle to notify missing sample for particular timestamp
_Use_decl_annotations_
RealtimeStreaming::Network::DataBundle NetworkMediaSinkStream::PrepareStreamTick(
    IMFAttributes* pAttributes)
{
    Log(Log_Level_Info, L"NetworkMediaSinkStream::PrepareStreamTick()\n");

    NULL_THROW(pAttributes);
    
    HRESULT hr = S_OK;
    const DWORD c_cbHeaderSize = sizeof(PayloadHeader) + sizeof(MediaStreamTick);

    DataBuffer dataBuffer_Header = winrt::make<Network::implementation::DataBuffer>(c_cbHeaderSize);

    // Prepare PayloadType header
    auto dataBufferImpl = dataBuffer_Header.as<Network::implementation::DataBuffer>();
    BYTE* pHeaderBuffer;
    dataBufferImpl->Buffer(&pHeaderBuffer);

    PayloadHeader* pOpHeader = reinterpret_cast<PayloadHeader *>(pHeaderBuffer);
    pOpHeader->ePayloadType = PayloadType::SendMediaStreamTick;
    pOpHeader->cbPayloadSize = sizeof(MediaStreamTick);

    // Prepare description
    MediaStreamTick* pSampleTick = reinterpret_cast<MediaStreamTick *>(pHeaderBuffer + sizeof(PayloadHeader));
    pSampleTick->dwStreamId = m_dwStreamId;

    LONGLONG llSampleTime;
    com_ptr<IMFSample> spSample;
    IFT(pAttributes->QueryInterface(__uuidof(IMFSample), spSample.put_void()));
    //IFC(pAttributes->QueryInterface(__uuidof(IMFSample), static_cast<LPVOID*>(&spSample)));
    IFT(spSample->GetSampleTime(&llSampleTime));

    pSampleTick->hnsTimestamp = llSampleTime - m_adjustedStartTime;
    if (pSampleTick->hnsTimestamp < 0)
    {
        pSampleTick->hnsTimestamp = 0;
    }

    IFT(pAttributes->SetUINT64(MFSampleExtension_Timestamp, llSampleTime));

    // copy attribute blob to a buffer
    // Get size of attributes blob
    IFT(MFGetAttributesAsBlobSize(pAttributes, &pSampleTick->cbAttributesSize));

    // Create a buffer for attribute blob
    DataBuffer attributeBuffer = winrt::make<Network::implementation::DataBuffer>(pSampleTick->cbAttributesSize);

    // Prepare the IBuffer
    auto attribBufferImpl = attributeBuffer.as<Network::implementation::DataBuffer>();
    BYTE* pBuf;
    attribBufferImpl->Buffer(&pBuf);

    // Copy attributes to the buffer
    IFT(MFGetAttributesAsBlob(pAttributes, pBuf, pSampleTick->cbAttributesSize));

    attributeBuffer.CurrentLength(pSampleTick->cbAttributesSize);

    // Add size of variable size attribute blob to size of the package.
    pOpHeader->cbPayloadSize += pSampleTick->cbAttributesSize;

    // Set length of the buffer
    dataBuffer_Header.CurrentLength(c_cbHeaderSize);

    // Add fixed size header and description to the bundle
    DataBundle dataBundle = make<Network::implementation::DataBundle>(dataBuffer_Header);

    // Add attributes to message
    dataBundle.AddBuffer(attributeBuffer);

    return dataBundle;
}

// Prepare bundle with format change information to be sent over the wire
_Use_decl_annotations_
DataBundle NetworkMediaSinkStream::PrepareFormatChange(
    IMFMediaType* pMediaType)
{
    Log(Log_Level_Info, L"NetworkMediaSinkStream::PrepareFormatChange()\n");

    const DWORD c_cbPayloadSize = sizeof(PayloadHeader) + sizeof(MediaTypeDescription);

    // TODO: FIX THIS!!!!
    /*
    auto dataBuffer = make<Network::implementation::DataBuffer>(c_cbPayloadSize);

    // Prepare PayloadType header
    auto dataBufferImpl = dataBuffer.as<Network::implementation::DataBuffer>();
    BYTE* pBuf;
    dataBufferImpl->Buffer(&pBuf);

    PayloadHeader* pOpHeader = reinterpret_cast<PayloadHeader *>(pBuf);
    pOpHeader->cbPayloadSize = sizeof(MediaTypeDescription);
    pOpHeader->ePayloadType = PayloadType::SendFormatChange;

    // Prepare description
    MediaTypeDescription* pStreamDescription = reinterpret_cast<MediaTypeDescription *>(pBuf + sizeof(PayloadHeader));
    DataBuffer spAttr = FillStreamDescription(pStreamDescription);

    // Add size of attribute blob to size of the package.
    pOpHeader->cbPayloadSize += pStreamDescription->AttributesBlobSize;

    // Set length of the buffer
    dataBuffer.CurrentLength(c_cbPayloadSize);

    // Add fixed size header and description to the bundle
    DataBundle dataBundle = make<Network::implementation::DataBundle>(dataBuffer);

    // Add attributes
    dataBundle.AddBuffer(spAttr);
    
    return dataBundle;
    */
    return make<Network::implementation::DataBundle>();
}


HRESULT NetworkMediaSinkStream::GetAttributesBlobSize(UINT32* ppAttributeSize)
{
    // Get the media type for the stream
    com_ptr<IMFMediaType> spMediaType;
    IFR(GetCurrentMediaType(spMediaType.put()));

    /*
    // filter types to those deemed needed
    com_ptr<IMFMediaType> spFilteredMediaType;
    IFC(MFCreateMediaType(&spFilteredMediaType));
    IFC(FilterOutputMediaType(spMediaType.get(), spFilteredMediaType.get()));
    */

    // Set size of attributes blob
    //IFC(MFGetAttributesAsBlobSize(spFilteredMediaType.get(), &attributesSize));
    IFR(MFGetAttributesAsBlobSize(spMediaType.get(), ppAttributeSize));

    return S_OK;
}

// Fill stream description and prepare attributes blob.
_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::FillStreamDescription(
    RealtimeStreaming::Common::MediaTypeDescription* pStreamDescription,
    BYTE* pAttributesBlob)
{
    Log(Log_Level_Info, L"NetworkMediaSinkStream::FillStreamDescription()\n");

    NULL_CHK(pStreamDescription);
    NULL_CHK(pAttributesBlob);

    // Clear the stream descriptor memory
    ZeroMemory(pStreamDescription, sizeof(MediaTypeDescription));

    // Get the media type for the stream
    com_ptr<IMFMediaType> spMediaType;
    IFR(GetCurrentMediaType(spMediaType.put()));

    // Get major type (Audio, Video and so on) and subtype (format of the stream)
    GUID majorType, subType;
    IFR(GetMajorType(&majorType));
    IFR(spMediaType->GetGUID(MF_MT_SUBTYPE, &subType));

    // Copy Attributes out to blob
    UINT32 attributeBlobSize;
    GetAttributesBlobSize(&attributeBlobSize);

    IFR(MFGetAttributesAsBlob(spMediaType.get(), pAttributesBlob, attributeBlobSize));
    //IFR(MFGetAttributesAsBlob(spFilteredMediaType.get(), pBuffer, attributesSize));

    // fill in streamDescription
    pStreamDescription->guiMajorType = majorType;
    pStreamDescription->dwStreamId = m_dwStreamId;
    pStreamDescription->guiSubType = subType;
    pStreamDescription->AttributesBlobSize = attributeBlobSize;

    return S_OK;
}

_Use_decl_annotations_
HRESULT NetworkMediaSinkStream::_QueueEvent(
    MediaEventType met,
    REFGUID guidExtendedType,
    HRESULT hrStatus,
    PROPVARIANT const* pvValue)
{
    // Underlying method for interface IMFMediaEventGenerator::QueueEvent() so class methods with lock can call internally
    IFR(CheckShutdown());

    return m_eventQueue->QueueEventParamVar(met, guidExtendedType, hrStatus, pvValue);
}