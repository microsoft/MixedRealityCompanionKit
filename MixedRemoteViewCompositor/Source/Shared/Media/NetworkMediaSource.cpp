// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "Media.h"
#include "NetworkMediaSource.h"
#include "NetworkMediaSourceStream.h"
#include <IntSafe.h>

_Use_decl_annotations_
SourceOperation::SourceOperation(SourceOperation::Type opType)
    : _cRef(1)
    , _opType(opType)
{
    ZeroMemory(&_data, sizeof(_data));
}

_Use_decl_annotations_
SourceOperation::~SourceOperation()
{
    PropVariantClear(&_data);
}

// IUnknown methods
_Use_decl_annotations_
IFACEMETHODIMP SourceOperation::QueryInterface(REFIID riid, void** ppv)
{
    return E_NOINTERFACE;
}

_Use_decl_annotations_
IFACEMETHODIMP_(ULONG) SourceOperation::AddRef()
{
    return InterlockedIncrement(&_cRef);
}

_Use_decl_annotations_
IFACEMETHODIMP_(ULONG) SourceOperation::Release()
{
    long cRef = InterlockedDecrement(&_cRef);
    if (cRef == 0)
    {
        delete this;
    }
    return cRef;
}

_Use_decl_annotations_
HRESULT SourceOperation::SetData(const PROPVARIANT &varData)
{
    return PropVariantCopy(&_data, &varData);
}

_Use_decl_annotations_
StartOperation::StartOperation(IMFPresentationDescriptor* pPD)
    : SourceOperation(SourceOperation::Operation_Start)
    , _spPD(pPD)
{
}

_Use_decl_annotations_
StartOperation::~StartOperation()
{
}

_Use_decl_annotations_
SetRateOperation::SetRateOperation(BOOL fThin, float flRate)
    : SourceOperation(SourceOperation::Operation_SetRate)
    , _fThin(fThin)
    , _flRate(flRate)
{
}

_Use_decl_annotations_
SetRateOperation::~SetRateOperation()
{
}


_Use_decl_annotations_
NetworkMediaSourceImpl::NetworkMediaSourceImpl()
    : OpQueue<NetworkMediaSourceImpl, SourceOperation>(this)
    , _spConnection(nullptr)
    , _eSourceState(SourceStreamState_Invalid)
    , _flRate(1.0f)
{
}

_Use_decl_annotations_
NetworkMediaSourceImpl::~NetworkMediaSourceImpl()
{
    Shutdown();

    if (nullptr != _spEventQueue)
    {
        _spEventQueue->Shutdown();
    }
    _spEventQueue.Reset();

    _spConnection->remove_Received(_evtReceivedToken);
    _spConnection.Reset();
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::RuntimeClassInitialize(
    IConnection* connection)
{
    NULL_CHK(connection);

    ComPtr<IConnection> spConnection(connection);
    IFR(spConnection.As(&_spConnection));

    // Create the event queue helper.
    if (nullptr == _spEventQueue)
    {
        IFR(MFCreateEventQueue(&_spEventQueue));
    }

    IFR(CBaseAttributes<>::Initialize());

    // register for message events
    auto receivedHandler =
        Callback<IBundleReceivedEventHandler>(this, &NetworkMediaSourceImpl::OnDataReceived);
    IFR(_spConnection->add_Received(receivedHandler.Get(), &_evtReceivedToken));

    // client is connected we need to send the Describe request
    _eSourceState = SourceStreamState_Opening;

    IFR(SendDescribeRequest());

    return AsyncBase::Start();
}

// IMFMediaEventGenerator methods.
// Note: These methods call through to the event queue helper object.
_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::BeginGetEvent(
    IMFAsyncCallback* pCallback, 
    IUnknown* punkState)
{
    HRESULT hr = S_OK;

    auto lock = _lock.Lock();

    hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        if (nullptr == _spEventQueue)
        {
            IFR(MFCreateEventQueue(&_spEventQueue));
        }
        hr = _spEventQueue->BeginGetEvent(pCallback, punkState);
    }

    return hr;
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::EndGetEvent(
    IMFAsyncResult* pResult, 
    IMFMediaEvent** ppEvent)
{
    HRESULT hr = S_OK;

    auto lock = _lock.Lock();

    hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        hr = _spEventQueue->EndGetEvent(pResult, ppEvent);
    }

    return hr;
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::GetEvent(
    DWORD dwFlags, 
    IMFMediaEvent** ppEvent)
{
    // NOTE:
    // GetEvent can block indefinitely, so we don't hold the lock.
    // This requires some juggling with the event queue pointer.
    ComPtr<IMFMediaEventQueue> spQueue;
    {
        auto lock = _lock.Lock();

        // Check shutdown
        IFR(CheckShutdown());

        // Get the pointer to the event queue.
        spQueue = _spEventQueue;
    }

    // Now get the event.
    return spQueue->GetEvent(dwFlags, ppEvent);
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::QueueEvent(
    MediaEventType met, 
    REFGUID guidExtendedType, 
    HRESULT hrStatus, 
    PROPVARIANT const* pvValue)
{
    HRESULT hr = S_OK;

    auto lock = _lock.Lock();

    hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        hr = _spEventQueue->QueueEventParamVar(met, guidExtendedType, hrStatus, pvValue);
    }

    return hr;
}


// IMFMediaSource methods
_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::CreatePresentationDescriptor(
    IMFPresentationDescriptor** ppPresentationDescriptor)
{
    NULL_CHK(ppPresentationDescriptor);

    auto lock = _lock.Lock();

    IFR(CheckShutdown());

    if (_eSourceState == SourceStreamState_Opening 
        || 
        _eSourceState == SourceStreamState_Invalid 
        || 
        !_spPresentationDescriptor)
    {
        IFR(MF_E_NOT_INITIALIZED);
    }

    return _spPresentationDescriptor->Clone(ppPresentationDescriptor);
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::GetCharacteristics(
    DWORD* pdwCharacteristics)
{
    NULL_CHK(pdwCharacteristics);

    auto lock = _lock.Lock();

    IFR(CheckShutdown());

   * pdwCharacteristics = MFMEDIASOURCE_IS_LIVE;

    return S_OK;
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::Pause()
{
    auto lock = _lock.Lock();

    IFR(CheckShutdown());

    return MF_E_INVALID_STATE_TRANSITION;
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::Shutdown()
{
    auto lock = _lock.Lock();

    IFR(CheckShutdown());

    Stop();

    if (SourceStreamState_Shutdown == _eSourceState)
    {
        return S_OK;
    }

    _eSourceState = SourceStreamState_Shutdown;

    CompleteOpen(MF_E_SHUTDOWN);

    StreamContainer::POSITION pos = _streams.FrontPosition();
    while (pos != _streams.EndPosition())
    {
        ComPtr<IMFMediaStream> spStream;
        if (SUCCEEDED(_streams.GetItemPos(pos, &spStream)))
        {
            static_cast<NetworkMediaSourceStreamImpl*>(spStream.Get())->Shutdown();
            spStream.Reset();
        }
        pos = _streams.Next(pos);
    }

    _streams.Clear();

    if (nullptr != _spPresentationDescriptor)
    {
        _spPresentationDescriptor->DeleteAllItems();
        _spPresentationDescriptor.Reset();
    }

    return S_OK;
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::Start(
    IMFPresentationDescriptor* pPresentationDescriptor,
    const GUID* pguidTimeFormat,
    const PROPVARIANT* pvarStartPos)
{
    // Start position and presentation descriptor cannot be NULL.
    NULL_CHK(pvarStartPos);
    NULL_CHK(pPresentationDescriptor);

    // Check the time format.
    if ((pguidTimeFormat != nullptr) && (*pguidTimeFormat != GUID_NULL))
    {
        // Unrecognized time format GUID.
        IFR(MF_E_UNSUPPORTED_TIME_FORMAT);
    }

    // Check the data type of the start position.
    if (pvarStartPos->vt != VT_EMPTY && pvarStartPos->vt != VT_I8)
    {
        IFR(MF_E_UNSUPPORTED_TIME_FORMAT);
    }

    auto lock = _lock.Lock();

    IFR(CheckShutdown());

    HRESULT hr = S_OK;

    ComPtr<StartOperation> spStartOp;
    if (_eSourceState != SourceStreamState_Stopped)
    {
        IFR(MF_E_INVALIDREQUEST);
    }

    // Check if the presentation description is valid.
    IFR(ValidatePresentationDescriptor(pPresentationDescriptor));

    // Prepare asynchronous operation attributes
    spStartOp.Attach(new (std::nothrow) StartOperation(pPresentationDescriptor));
    NULL_CHK_HR(spStartOp, E_OUTOFMEMORY);

    IFR(spStartOp->SetData(*pvarStartPos));

    // Queue asynchronous operation
    return QueueOperation(spStartOp.Get());
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::Stop()
{
    auto lock = _lock.Lock();

    IFR(CheckShutdown());

    ComPtr<SourceOperation> spStopOp;
    spStopOp.Attach(new (std::nothrow) SourceOperation(SourceOperation::Operation_Stop));
    NULL_CHK_HR(spStopOp, E_OUTOFMEMORY);

    // Queue asynchronous stop
    return QueueOperation(spStopOp.Get());
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::GetService(
    REFGUID guidService, 
    REFIID riid, 
    void** ppv)
{
    NULL_CHK(ppv);

    if (MF_RATE_CONTROL_SERVICE == guidService) 
    {
        return QueryInterface(riid, ppv);
    }
    else 
    {
       * ppv = nullptr;

        return MF_E_UNSUPPORTED_SERVICE;
    }
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::SetRate(
    BOOL fThin, 
    float flRate)
{
    if (fThin)
    {
        return MF_E_THINNING_UNSUPPORTED;
    }

    if (!IsRateSupported(flRate, &flRate))
    {
        return MF_E_UNSUPPORTED_RATE;
    }

    auto lock = _lock.Lock();

    HRESULT hr = S_OK;

    if (flRate == _flRate)
    {
        return S_OK;
    }

    ComPtr<SourceOperation> spSetRateOp;
    spSetRateOp.Attach(new (std::nothrow) SetRateOperation(fThin, flRate));
    if (nullptr == spSetRateOp)
    {
        hr = E_OUTOFMEMORY;
    }

    if (SUCCEEDED(hr))
    {
        // Queue asynchronous stop
        hr = QueueOperation(spSetRateOp.Get());
    }

    return hr;
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::GetRate(
    BOOL* pfThin, 
    float* pflRate)
{
    auto lock = _lock.Lock();

    if (nullptr == pfThin || nullptr == pflRate)
    {
        return E_INVALIDARG;
    }

   * pfThin = FALSE;
   * pflRate = _flRate;

    return S_OK;
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::DispatchOperation(
    SourceOperation* pOp)
{
    HRESULT hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        switch (pOp->GetOperationType())
        {
        case SourceOperation::Operation_Init:
            IFC(DoInit(pOp));
            break;
        case SourceOperation::Operation_Start:
            IFC(DoStart(static_cast<StartOperation*>(pOp)));
            break;
        case SourceOperation::Operation_Stop:
            IFC(DoStop(pOp));
            break;
        case SourceOperation::Operation_SetRate:
            IFC(DoSetRate(static_cast<SetRateOperation*>(pOp)));
            break;
        default:
            hr = E_UNEXPECTED;
            break;
        }
    }

done:
    return hr;
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::ValidateOperation(
    SourceOperation* pOp)
{
    return S_OK;
}


_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::OnStart(void)
{
    return S_OK;
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::CompleteOpen(
    HRESULT hResult)
{
    Log(Log_Level_Info, L"NetworkMediaSourceImpl::CompleteOpen()\n");

    // triggers the correct transition for the AsyncAction
    ABI::Windows::Foundation::AsyncStatus status;
    this->get_Status(&status);
    if (AsyncStatus::Completed == status)
    {
        return S_OK;
    }

    HRESULT hr = S_OK;

    IFC(hResult);

    if (AsyncStatus::Started != status)
    {
        this->get_ErrorCode(&hr);

        IFC(hr);
    }

done:
    if (FAILED(hr))
    {
        LOG_RESULT(TryTransitionToError(hr));
    }

    return FireCompletion();
}

// Handle errors
_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::HandleError(
    HRESULT hResult)
{
    IFR(CompleteOpen(hResult));

    if (_eSourceState != SourceStreamState_Shutdown)
    {
        // If we received an error at any other time (except shutdown) send MEError event.
        return QueueEvent(MEError, GUID_NULL, hResult, nullptr);
    }

    return S_OK;
}

// Returns stream object associated with given identifier
_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::GetStreamById(
    _In_ DWORD dwId,
    _Outptr_ IMFMediaStream** ppStream)
{
    NULL_CHK(ppStream);

    HRESULT hr = S_OK;
    StreamContainer::POSITION pos = _streams.FrontPosition();
    StreamContainer::POSITION posEnd = _streams.EndPosition();

    for (; pos != posEnd; pos = _streams.Next(pos))
    {
        ComPtr<IMFMediaStream> spStream;
        if (FAILED(_streams.GetItemPos(pos, &spStream)))
        {
            IFC(MF_E_INVALIDSTREAMNUMBER);
            break;
        }

        NetworkMediaSourceStreamImpl* pStreamImpl
            = static_cast<NetworkMediaSourceStreamImpl*>(spStream.Get());
        IFC(nullptr != pStreamImpl ? S_OK : E_POINTER);

        DWORD id = pStreamImpl->get_StreamId();
        if (id == dwId)
        {
            IFC(spStream.CopyTo(ppStream));
            break;
        }
    }

done:
    return hr;
}


_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::InitPresentationDescription()
{
    ComPtr<IMFPresentationDescriptor> spPresentationDescriptor;
    IMFStreamDescriptor* aStreams[2] = { 0 };
    DWORD streamCount = _streams.GetCount();

    HRESULT hr = S_OK;

    StreamContainer::POSITION pos = _streams.FrontPosition();
    StreamContainer::POSITION posEnd = _streams.EndPosition();

    for (DWORD nStream = 0; pos != posEnd; ++nStream, pos = pos = _streams.Next(pos))
    {
        ComPtr<IMFMediaStream> spStream;
        IFC(_streams.GetItemPos(pos, &spStream));
        IFC(spStream->GetStreamDescriptor(&aStreams[nStream]));
    }

    IFC(MFCreatePresentationDescriptor(streamCount, aStreams, &spPresentationDescriptor));

    for (DWORD nStream = 0; nStream < streamCount; ++nStream)
    {
        IFC(spPresentationDescriptor->SelectStream(nStream));
    }

    IFC(spPresentationDescriptor.CopyTo(&_spPresentationDescriptor));

done:
    for (DWORD nStream = 0; nStream < streamCount; ++nStream)
    {
        if (aStreams[nStream] != nullptr)
        {
            aStreams[nStream]->Release();
        }
    }

    return hr;
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::ValidatePresentationDescriptor(
    IMFPresentationDescriptor* pPD)
{
    HRESULT hr = S_OK;
    BOOL fSelected = FALSE;
    DWORD cStreams = 0;

    if (_streams.GetCount() == 0)
    {
        return E_UNEXPECTED;
    }

    // The caller's PD must have the same number of streams as ours.
    hr = pPD->GetStreamDescriptorCount(&cStreams);

    if (SUCCEEDED(hr))
    {
        if (cStreams != _streams.GetCount())
        {
            hr = E_INVALIDARG;
        }
    }

    // The caller must select at least one stream.
    if (SUCCEEDED(hr))
    {
        for (DWORD i = 0; i < cStreams; i++)
        {
            ComPtr<IMFStreamDescriptor> spSD;
            hr = pPD->GetStreamDescriptorByIndex(i, &fSelected, &spSD);
            if (FAILED(hr))
            {
                break;
            }
        }
    }

    return hr;
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::SelectStreams(
    IMFPresentationDescriptor* pPD)
{
    for (DWORD nStream = 0; nStream < _streams.GetCount(); ++nStream)
    {
        BOOL fSelected;

        // Get next stream descriptor
        ComPtr<IMFStreamDescriptor> spSD;
        IFR(pPD->GetStreamDescriptorByIndex(nStream, &fSelected, &spSD));

        // Get stream id
        DWORD nStreamId;
        IFR(spSD->GetStreamIdentifier(&nStreamId));

        // Get simple net media stream
        ComPtr<IMFMediaStream> spStream;
        IFR(GetStreamById(nStreamId, &spStream));

        NetworkMediaSourceStreamImpl* pStreamImpl =
            static_cast<NetworkMediaSourceStreamImpl*>(spStream.Get());
        NULL_CHK(pStreamImpl);

        // Remember if stream was selected
        bool fWasSelected = pStreamImpl->IsActive();
        IFR(pStreamImpl->SetActive(!!fSelected));

        if (fSelected)
        {
            // Choose event type to send
            MediaEventType met = (fWasSelected == S_OK) ? MEUpdatedStream : MENewStream;

            ComPtr<IUnknown> spStreamUnk;
            IFR(spStream.As(&spStreamUnk));

            IFR(_spEventQueue->QueueEventParamUnk(met, GUID_NULL, S_OK, spStreamUnk.Get()));

            // Start the stream. The stream will send the appropriate event.
            IFR(pStreamImpl->Start());
        }
    }

    return S_OK;
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::DoInit(
    SourceOperation* pOp)
{
    IFR(pOp->GetOperationType() == SourceOperation::Operation_Init ? S_OK : MF_E_INVALID_STATE_TRANSITION);

    return SendDescribeRequest();
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::DoStart(
    StartOperation* pOp)
{
    HRESULT hr = S_OK;

    NULL_CHK(pOp);

    IFC(pOp->GetOperationType() == SourceOperation::Operation_Start ? S_OK : MF_E_INVALID_STATE_TRANSITION);

    {
        ComPtr<IMFPresentationDescriptor> spPD = pOp->GetPresentationDescriptor();

        IFC(SelectStreams(spPD.Get()));

        _eSourceState = SourceStreamState_Starting;

        IFC(SendStartRequest());

        _eSourceState = SourceStreamState_Started;
        IFC(_spEventQueue->QueueEventParamVar(MESourceStarted, GUID_NULL, S_OK, &pOp->GetData()));
    }

done:
    if (FAILED(hr))
    {
        _spEventQueue->QueueEventParamVar(MESourceStarted, GUID_NULL, hr, nullptr);
    }

    return hr;
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::DoStop(
    SourceOperation* pOp)
{
    IFR(pOp->GetOperationType() != SourceOperation::Operation_Stop ? S_OK : MF_E_INVALID_STATE_TRANSITION);

    HRESULT hr = S_OK;


    {
        StreamContainer::POSITION pos = _streams.FrontPosition();
        StreamContainer::POSITION posEnd = _streams.EndPosition();

        for (; pos != posEnd; pos = _streams.Next(pos))
        {
            ComPtr<IMFMediaStream> spStream;
            IFC(_streams.GetItemPos(pos, &spStream));
            
            NetworkMediaSourceStreamImpl* pStream = static_cast<NetworkMediaSourceStreamImpl*>(spStream.Get());
            IFC(nullptr != pStream ? S_OK : E_POINTER);

            if (pStream->IsActive())
            {
                IFC(pStream->Flush());
                IFC(pStream->Stop());
            }
        }

        _streams.Clear();

        IFR(SendStopRequest());
    }

done:

    // Send the "stopped" event. This might include a failure code.
    return _spEventQueue->QueueEventParamVar(MESourceStopped, GUID_NULL, hr, nullptr);
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::DoSetRate(
    SetRateOperation* pOp)
{
    IFR(pOp->GetOperationType() != SourceOperation::Operation_SetRate ? S_OK : MF_E_INVALID_STATE_TRANSITION);

    HRESULT hr = S_OK;

    {
        StreamContainer::POSITION pos = _streams.FrontPosition();
        StreamContainer::POSITION posEnd = _streams.EndPosition();

        for (; pos != posEnd; pos = _streams.Next(pos))
        {
            ComPtr<IMFMediaStream> spStream;
            IFC(_streams.GetItemPos(pos, &spStream));

            NetworkMediaSourceStreamImpl* pStream = static_cast<NetworkMediaSourceStreamImpl*>(spStream.Get());

            if (pStream->IsActive())
            {
                IFC(pStream->Flush());
                IFC(pStream->SetRate(pOp->GetRate()));
            }
        }

        _flRate = pOp->GetRate();
    }
done:
    // Send the "rate changed" event. This might include a failure code.
    return _spEventQueue->QueueEventParamVar(MESourceRateChanged, GUID_NULL, hr, nullptr);
}

_Use_decl_annotations_
bool NetworkMediaSourceImpl::IsRateSupported(
    float flRate, float* pflAdjustedRate)
{
    if (flRate < 0.00001f && flRate > -0.00001f)
    {
       * pflAdjustedRate = 0.0f;
        return true;
    }
    else if (flRate < 1.0001f && flRate > 0.9999f)
    {
       * pflAdjustedRate = 1.0f;
        return true;
    }
    return false;
}


// handle connection state
_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::OnDataReceived(
    IConnection *sender,
    IBundleReceivedArgs *args)
{
    ComPtr<IBundleReceivedArgs> spArgs(args);

    auto lock = _lock.Lock();

    HRESULT hr = S_OK;
    PayloadType type;
    ComPtr<IDataBundle> spDataBundle;

    IFC(CheckShutdown());

    IFC(args->get_PayloadType(&type));

    IFC(args->get_DataBundle(&spDataBundle));

    switch (type)
    {
    case PayloadType_State_CaptureReady:
        IFC(ProcessCaptureReady());
        break;
    case PayloadType_SendMediaDescription:
        IFC(ProcessMediaDescription(spDataBundle.Get()));
        break;
    case PayloadType_SendMediaSample:
        IFC(ProcessMediaSample(spDataBundle.Get()));
        break;
    case PayloadType_SendFormatChange:
        IFC(ProcessMediaFormatChange(spDataBundle.Get()));
        break;
    case PayloadType_SendMediaStreamTick:
        IFC(ProcessMediaTick(spDataBundle.Get()));
        break;
    };

done:
    if (FAILED(hr))
    {
        LOG_RESULT(hr);
    }
    return S_OK;
}


// Sending requests to network
_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::SendDescribeRequest()
{
    Log(Log_Level_Info, L"NetworkMediaSourceImpl::SendDescribeRequest()\n");

    NULL_CHK_HR(_spConnection, E_POINTER);

    return _spConnection->SendPayloadType(PayloadType_RequestMediaDescription);
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::SendStartRequest()
{
    Log(Log_Level_Info, L"NetworkMediaSourceImpl::SendStartRequest()\n");

    NULL_CHK_HR(_spConnection, E_POINTER);

    return _spConnection->SendPayloadType(PayloadType_RequestMediaStart);
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::SendStopRequest()
{
    Log(Log_Level_Info, L"NetworkMediaSourceImpl::SendStopRequest()\n");

    NULL_CHK_HR(_spConnection, E_POINTER);

    return _spConnection->SendPayloadType(PayloadType_RequestMediaStop);
}


// Helper methods to handle received network bundles
_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::ProcessCaptureReady()
{
    if (_eSourceState == SourceStreamState_Stopped
        &&
        _streams.GetCount() > 0)
    {
        return S_OK;
    }

    if (_eSourceState == SourceStreamState_Invalid
        ||
        _streams.GetCount() == 0)
    {
        _eSourceState = SourceStreamState_Opening;

        auto lock = _lock.Lock();

        IFR(CheckShutdown());

        ComPtr<SourceOperation> spInitOp;
        spInitOp.Attach(new (std::nothrow) SourceOperation(SourceOperation::Operation_Init));
        NULL_CHK_HR(spInitOp, E_OUTOFMEMORY);

        // Queue asynchronous stop
        return QueueOperation(spInitOp.Get());
    }

    return HandleError(MF_E_INVALID_STATE_TRANSITION);
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::ProcessMediaDescription(
    IDataBundle* pBundle)
{
    Log(Log_Level_Info, L"NetworkMediaSourceImpl::ProcessMediaDescription()\n");

    if (_eSourceState == SourceStreamState_Started
        &&
        _streams.GetCount() > 0)
    {
        return S_OK;
    }

    // are we ready to start?
    if (_eSourceState == SourceStreamState_Stopped
        &&
        _streams.GetCount() > 0)
    {
        return S_OK;
    }

    DataBundleImpl* pBundleImpl = static_cast<DataBundleImpl*>(pBundle);
    NULL_CHK(pBundleImpl);

    if (_eSourceState != SourceStreamState_Opening)
    {
        if (_eSourceState != SourceStreamState_Stopped)
        {
            Shutdown();
        }
        else
        {
            // Server description should only be sent during opening state
            IFR(MF_E_UNEXPECTED);
        }
    }

    DWORD cbTotalLen = 0;
    DWORD cbHeaderSize = 0;
    DWORD StreamTypeHeaderSize = 0;
    MediaDescription desc;
    const DWORD c_cbMediaDescriptionSize = sizeof(MediaDescription);
    MediaTypeDescription* pMediaTypeDesc = nullptr;
    const DWORD c_cbMediaTypeDescriptionSize = sizeof(MediaTypeDescription);
    BYTE* pPtr = nullptr;

    HRESULT hr = S_OK;

    IFC(pBundleImpl->get_TotalSize(&cbTotalLen));

    // Minimum size of the operation payload is size of Description structure
    // + one mediaType description
    if (cbTotalLen < (c_cbMediaDescriptionSize + c_cbMediaTypeDescriptionSize))
    {
        IFC(MF_E_UNSUPPORTED_FORMAT);
    }

    // Copy description.
    IFC(pBundleImpl->MoveLeft(c_cbMediaDescriptionSize, &desc));

    // Size of the bundle should match size described in the packet (size of Description structure
    // plus size of attribute blob)
    StreamTypeHeaderSize = desc.StreamCount * c_cbMediaTypeDescriptionSize;
    cbHeaderSize = c_cbMediaDescriptionSize + StreamTypeHeaderSize;

    // Check if the input parameters are valid. We only support 2 streams.
    if (cbHeaderSize < c_cbMediaDescriptionSize ||
        StreamTypeHeaderSize != desc.StreamTypeHeaderSize ||
        desc.StreamCount == 0 ||
        desc.StreamCount > 2 ||
        cbTotalLen < cbHeaderSize)
    {
        IFC(MF_E_UNSUPPORTED_FORMAT);
    }

    // make buffer that will hold the streamTypeHeader data
    pPtr = new (std::nothrow) BYTE[StreamTypeHeaderSize];
    if (pPtr == nullptr)
    {
        IFC(E_OUTOFMEMORY);
    }

    // copy the mediaType header from the bundle
    IFC(pBundleImpl->MoveLeft(StreamTypeHeaderSize, pPtr));

    // Data is ready
    pMediaTypeDesc = reinterpret_cast<MediaTypeDescription*>(pPtr);
    IFC(nullptr != pMediaTypeDesc ? S_OK : E_POINTER);

    DWORD cbAttributeSize = 0;
    for (DWORD nStream = 0; nStream < desc.StreamCount; ++nStream)
    {
        if (FAILED(DWordAdd(cbAttributeSize, pMediaTypeDesc[nStream].AttributesBlobSize, &cbAttributeSize)))
        {
            IFC(MF_E_UNSUPPORTED_FORMAT);
        }
    }

    // Validate the parameters. Limit the total size of attributes to 64kB.
    if ((cbTotalLen != (cbHeaderSize + cbAttributeSize)) || (cbAttributeSize > 0x10000))
    {
        IFC(MF_E_UNSUPPORTED_FORMAT);
    }

    // Create stream for every stream description sent by the server.
    for (DWORD nStream = 0; nStream < desc.StreamCount; ++nStream)
    {
        ComPtr<NetworkMediaSourceStreamImpl> spStream;
        IFC(NetworkMediaSourceStreamImpl::CreateInstance(
            &pMediaTypeDesc[nStream],
            pBundle,
            this,
            &spStream));

        IFC(nullptr != spStream ? S_OK : E_OUTOFMEMORY);

        IFC(_streams.InsertBack(spStream.Get()));
    }

    IFC(InitPresentationDescription());

    // Everything succeeded we are in stopped state now
    _eSourceState = SourceStreamState_Stopped;

    SendStopRequest();

done:
    delete[] pPtr;

    return CompleteOpen(hr);
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::ProcessMediaSample(
    IDataBundle* pBundle)
{
    Log(Log_Level_Info, L"NetworkMediaSourceImpl::ProcessMediaSample()\n");

    ComPtr<IDataBundle> spBundle(pBundle);
    DataBundleImpl* pBundleImpl = static_cast<DataBundleImpl*>(pBundle);
    NULL_CHK(pBundleImpl);

    // Only process samples when we are in started state
    if (_eSourceState != SourceStreamState_Started && _eSourceState != SourceStreamState_Shutdown)
    {
        if (FAILED(ProcessCaptureReady()))
        {
            IFR_MSG(MF_E_INVALID_STATE_TRANSITION, L"NetworkMediaSourceImpl::ProcessMediaSample() - not in a state to receive data.");
        }
        else
        {
            return S_OK;
        }
    }

    HRESULT hr = S_OK;

    MediaSampleHeader sampleHead = {};
    MediaSampleTransforms sampleTransforms;
    DWORD cbTotalSize;

    ComPtr<IMFSample> spSample;
    ComPtr<IMFMediaStream> spStream;

    // Copy the header object
    IFC(pBundleImpl->MoveLeft(sizeof(MediaSampleHeader), &sampleHead));
    IFC(pBundleImpl->get_TotalSize(&cbTotalSize));

    if (sampleHead.cbCameraDataSize > 0)
    {
        IFC(pBundleImpl->MoveLeft(sampleHead.cbCameraDataSize, &sampleTransforms));
    }

    // Convert the remaining bundle to MF sample
    IFC(GetStreamById(sampleHead.dwStreamId, &spStream));

    NetworkMediaSourceStreamImpl* pStreamImpl =
        static_cast<NetworkMediaSourceStreamImpl*>(spStream.Get());
    NULL_CHK(pStreamImpl);

    if (pStreamImpl->IsActive())
    {
        IFC(pBundleImpl->ToMFSample(&spSample));

        // Forward sample to a proper stream.
        IFC(pStreamImpl->ProcessSample(&sampleHead, (sampleHead.cbCameraDataSize > 0) ? &sampleTransforms : nullptr, spSample.Get()));
    }

done:
    if (FAILED(hr))
    {
        return HandleError(hr);
    }

    return hr;
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::ProcessMediaTick(
    IDataBundle* pBundle)
{
    DataBundleImpl* pBundleImpl = static_cast<DataBundleImpl*>(pBundle);

    NULL_CHK(pBundleImpl);

    // Only process samples when we are in started state
    if (_eSourceState != SourceStreamState_Started && _eSourceState != SourceStreamState_Shutdown)
    {
        if (FAILED(ProcessCaptureReady()))
        {
            IFR_MSG(MF_E_INVALID_STATE_TRANSITION, L"NetworkMediaSourceImpl::ProcessMediaSample() - not in a state to receive data.");
        }
        else
        {
            return S_OK;
        }
    }

    HRESULT hr = S_OK;

    ComPtr<IMFMediaStream> spStream;

    DWORD cbTotalLen = 0;
    IFC(pBundleImpl->get_TotalSize(&cbTotalLen));

    // Minimum size of the operation payload is size of Description structure
    if (cbTotalLen < sizeof(Network::MediaStreamTick))
    {
        IFC(MF_E_UNSUPPORTED_CHARACTERISTICS);
    }

    // Copy the header object
    Network::MediaStreamTick streamTick;
    IFC(pBundleImpl->MoveLeft(sizeof(streamTick), &streamTick));

    if (cbTotalLen != sizeof(Network::MediaStreamTick) + streamTick.cbAttributesSize || streamTick.cbAttributesSize == 0)
    {
        IFC(MF_E_UNSUPPORTED_CHARACTERISTICS);
    }

    {
        // Prepare buffer where we will copy attributes
        std::vector<byte> attribBlob(streamTick.cbAttributesSize);
        IFC(pBundleImpl->MoveLeft(streamTick.cbAttributesSize, attribBlob.data()));

        // Convert bundle to MFAttribute
        IFC(GetStreamById(streamTick.dwStreamId, &spStream));

        NetworkMediaSourceStreamImpl* pStreamImpl =
            static_cast<NetworkMediaSourceStreamImpl*>(spStream.Get());
        NULL_CHK(pStreamImpl);

        if (pStreamImpl->IsActive())
        {
            // Create attributes
            ComPtr<IMFAttributes> spAttributes;
            IFC(MFCreateAttributes(&spAttributes, 1));

            // Initialize media type's attributes
            IFC(MFInitAttributesFromBlob(spAttributes.Get(), attribBlob.data(), streamTick.cbAttributesSize));

            // Forward tick to a proper stream.
            IFC(pStreamImpl->ProcessTick(&streamTick, spAttributes.Get()));
        }
    }
done:
    if (FAILED(hr))
    {
        return HandleError(hr);
    }

    return hr;
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceImpl::ProcessMediaFormatChange(
    IDataBundle* pBundle)
{
    DataBundleImpl* pBundleImpl = static_cast<DataBundleImpl*>(pBundle);
    NULL_CHK(pBundleImpl);

    if (_eSourceState != SourceStreamState_Started && _eSourceState != SourceStreamState_Shutdown)
    {
        if (FAILED(ProcessCaptureReady()))
        {
            IFR_MSG(MF_E_INVALID_STATE_TRANSITION, L"NetworkMediaSourceImpl::ProcessMediaFormatChange() - not in a state to receive data.");
        }
    }

    HRESULT hr = S_OK;

    ComPtr<IMFMediaType> spMediaType;
    DWORD cbTotalLen = 0;
    MediaTypeDescription streamDesc;
    IFC(pBundleImpl->get_TotalSize(&cbTotalLen));
    DWORD cbTypeDescSize = sizeof(MediaTypeDescription);
    // Minimum size of the operation payload is size of Description structure
    if (cbTotalLen < sizeof(Network::MediaTypeDescription))
    {
        IFC(MF_E_UNSUPPORTED_FORMAT);
    }

    IFC(pBundleImpl->MoveLeft(cbTypeDescSize, &streamDesc));

    if (cbTotalLen != cbTypeDescSize + streamDesc.AttributesBlobSize
        ||
        streamDesc.AttributesBlobSize == 0)
    {
        IFC(MF_E_UNSUPPORTED_FORMAT);
    }

    {
        // Prepare buffer where we will copy attributes to
        std::vector<byte> blob(streamDesc.AttributesBlobSize);
        IFC(pBundleImpl->MoveLeft(streamDesc.AttributesBlobSize, blob.data()));

        // Create a media type object.
        IFC(MFCreateMediaType(&spMediaType));
        // Initialize media type's attributes
        IFC(MFInitAttributesFromBlob(spMediaType.Get(), blob.data(), streamDesc.AttributesBlobSize));
    }

done:
    if (FAILED(hr))
    {
        return HandleError(hr);
    }

    return hr;
}
