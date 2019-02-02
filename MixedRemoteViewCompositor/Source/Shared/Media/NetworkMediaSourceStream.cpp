// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "NetworkMediaSourceStream.h"
#include "Media.h"

#define SET_SAMPLE_ATTRIBUTE(flag, mask, pSample, flagName) \
if ((static_cast<DWORD>(SampleFlags_SampleFlag_##flagName) & mask) == static_cast<DWORD>(SampleFlags_SampleFlag_##flagName)) \
{ \
    IFR(pSample->SetUINT32( \
        MFSampleExtension_##flagName, \
        (static_cast<DWORD>(SampleFlags_SampleFlag_##flagName) & flag) == static_cast<DWORD>(SampleFlags_SampleFlag_##flagName))); \
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceStreamImpl::CreateInstance(
    MediaTypeDescription* pStreamDescription, 
    IDataBundle* pAttributesBuffer, 
    NetworkMediaSourceImpl* pSource,
    NetworkMediaSourceStreamImpl** ppStream)
{
    NULL_CHK(pStreamDescription);
    NULL_CHK(pAttributesBuffer);
    NULL_CHK(pSource);
    NULL_CHK(ppStream);

    ComPtr<NetworkMediaSourceStreamImpl> spStream = Microsoft::WRL::Make<NetworkMediaSourceStreamImpl>(pSource);

    NULL_CHK_HR(spStream.Get(), E_OUTOFMEMORY);

    IFR(spStream->Initialize(pStreamDescription, pAttributesBuffer));

    NULL_CHK_HR(spStream, E_NOT_SET);

    return spStream.CopyTo(ppStream);
}

_Use_decl_annotations_
NetworkMediaSourceStreamImpl::NetworkMediaSourceStreamImpl(NetworkMediaSourceImpl* pSource)
    : _spSource(pSource)
    , _eSourceState(SourceStreamState_Invalid)
    , _fActive(false)
    , _flRate(1.0f)
    , _fVideo(false)
    , _eDropMode(MF_DROP_MODE_NONE)
    , _fDiscontinuity(false)
    , _fDropTime(false)
    , _fInitDropTime(false)
    , _fWaitingForCleanPoint(true)
    , _hnsStartDroppingAt(0)
    , _hnsAmountToDrop(0)
{
}

_Use_decl_annotations_
NetworkMediaSourceStreamImpl::~NetworkMediaSourceStreamImpl()
{
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceStreamImpl::RuntimeClassInitialize(
    MediaTypeDescription* pStreamDescription,
    IDataBundle* pAttributesBuffer,
    IMFMediaSource* pParent)
{
    NULL_CHK(pParent);

    ComPtr<IMFMediaSource> spSource(pParent);
    IFR(spSource.As(&_spSource));

    return Initialize(pStreamDescription, pAttributesBuffer);
}


// IMFMediaEventGenerator methods.
// Note: These methods call through to the event queue helper object.
_Use_decl_annotations_
IFACEMETHODIMP NetworkMediaSourceStreamImpl::BeginGetEvent(
    IMFAsyncCallback* pCallback, 
    IUnknown* punkState)
{
    HRESULT hr = S_OK;

    auto lock = static_cast<NetworkMediaSourceImpl*>(_spSource.Get())->Lock();

    hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        hr = _spEventQueue->BeginGetEvent(pCallback, punkState);
    }

    return hr;
}

_Use_decl_annotations_
IFACEMETHODIMP NetworkMediaSourceStreamImpl::EndGetEvent(IMFAsyncResult* pResult, IMFMediaEvent** ppEvent)
{
    HRESULT hr = S_OK;

    auto lock = static_cast<NetworkMediaSourceImpl*>(_spSource.Get())->Lock();

    hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        hr = _spEventQueue->EndGetEvent(pResult, ppEvent);
    }

    return hr;
}

_Use_decl_annotations_
IFACEMETHODIMP NetworkMediaSourceStreamImpl::GetEvent(DWORD dwFlags, IMFMediaEvent** ppEvent)
{
    // NOTE:
    // GetEvent can block indefinitely, so we don't hold the lock.
    // This requires some juggling with the event queue pointer.

    HRESULT hr = S_OK;

    ComPtr<IMFMediaEventQueue> spQueue;

    {
        auto lock = static_cast<NetworkMediaSourceImpl*>(_spSource.Get())->Lock();

        // Check shutdown
        hr = CheckShutdown();

        // Get the pointer to the event queue.
        if (SUCCEEDED(hr))
        {
            spQueue = _spEventQueue;
        }
    }

    // Now get the event.
    if (SUCCEEDED(hr))
    {
        hr = spQueue->GetEvent(dwFlags, ppEvent);
    }

    return hr;
}

_Use_decl_annotations_
IFACEMETHODIMP NetworkMediaSourceStreamImpl::QueueEvent(MediaEventType met, REFGUID guidExtendedType, HRESULT hrStatus, PROPVARIANT const* pvValue)
{
    HRESULT hr = S_OK;

    auto lock = static_cast<NetworkMediaSourceImpl*>(_spSource.Get())->Lock();

    hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        hr = _spEventQueue->QueueEventParamVar(met, guidExtendedType, hrStatus, pvValue);
    }

    return hr;
}

// IMFMediaStream methods

_Use_decl_annotations_
IFACEMETHODIMP NetworkMediaSourceStreamImpl::GetMediaSource(IMFMediaSource** ppMediaSource)
{
    if (ppMediaSource == nullptr)
    {
        return E_INVALIDARG;
    }

    HRESULT hr = S_OK;

    auto lock = static_cast<NetworkMediaSourceImpl*>(_spSource.Get())->Lock();

    hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
       * ppMediaSource = _spSource.Get();
        (*ppMediaSource)->AddRef();
    }

    return hr;
}

_Use_decl_annotations_
IFACEMETHODIMP NetworkMediaSourceStreamImpl::GetStreamDescriptor(IMFStreamDescriptor** ppStreamDescriptor)
{
    if (ppStreamDescriptor == nullptr)
    {
        return E_INVALIDARG;
    }

    HRESULT hr = S_OK;

    auto lock = static_cast<NetworkMediaSourceImpl*>(_spSource.Get())->Lock();

    hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
       * ppStreamDescriptor = _spStreamDescriptor.Get();
        (*ppStreamDescriptor)->AddRef();
    }

    return hr;
}

_Use_decl_annotations_
IFACEMETHODIMP NetworkMediaSourceStreamImpl::RequestSample(
    IUnknown* pToken)
{
    auto lock = static_cast<NetworkMediaSourceImpl*>(_spSource.Get())->Lock();

    HRESULT hr = S_OK;

    IFC(CheckShutdown());

    if (_eSourceState != SourceStreamState_Started)
    {
        // We cannot be asked for a sample unless we are in started state
        IFC(MF_E_INVALIDREQUEST);
    }

    // Put token onto the list to return it when we have a sample ready
    IFC(_tokens.InsertBack(pToken));

	//tokenTest = pToken;
	tokenTest = pToken;
	ULONG refCount = pToken->AddRef();
	
    // Trigger sample delivery
    IFC(DeliverSamples());

done:
    if (FAILED(hr))
    {
        HandleError(hr);
    }

    return S_OK;
}


// IMFQualityAdvise methods
_Use_decl_annotations_
IFACEMETHODIMP NetworkMediaSourceStreamImpl::SetDropMode(
    MF_QUALITY_DROP_MODE eDropMode)
{
    auto lock = static_cast<NetworkMediaSourceImpl*>(_spSource.Get())->Lock();

    HRESULT hr = S_OK;
    
    IFC(CheckShutdown());

    // Only support one drop mode
    if ((eDropMode < MF_DROP_MODE_NONE) || (eDropMode >= MF_DROP_MODE_2))
    {
        IFC(MF_E_NO_MORE_DROP_MODES);
    }

    if (_eDropMode != eDropMode)
    {
        _eDropMode = eDropMode;
        _fWaitingForCleanPoint = true;

        _samples.Clear();

        Log(Log_Level_Info, L"MediaSourceStream Setting drop mode to %d\n", _eDropMode);
    }

done:
    return hr;
}

_Use_decl_annotations_
IFACEMETHODIMP NetworkMediaSourceStreamImpl::SetQualityLevel(
    MF_QUALITY_LEVEL eQualityLevel)
{
    return MF_E_NO_MORE_QUALITY_LEVELS;
}

_Use_decl_annotations_
IFACEMETHODIMP NetworkMediaSourceStreamImpl::GetDropMode(
    MF_QUALITY_DROP_MODE* peDropMode)
{
    NULL_CHK(peDropMode);

    auto lock = static_cast<NetworkMediaSourceImpl*>(_spSource.Get())->Lock();

    IFR(CheckShutdown());

   * peDropMode = _eDropMode;

    return S_OK;
}

_Use_decl_annotations_
IFACEMETHODIMP NetworkMediaSourceStreamImpl::GetQualityLevel(
    MF_QUALITY_LEVEL* peQualityLevel)
{
    return(E_NOTIMPL);
}

_Use_decl_annotations_
IFACEMETHODIMP NetworkMediaSourceStreamImpl::DropTime(
    LONGLONG hnsAmountToDrop)
{
    HRESULT hr = S_OK;

    auto lock = static_cast<NetworkMediaSourceImpl*>(_spSource.Get())->Lock();

    hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        if (hnsAmountToDrop > 0)
        {
            _fDropTime = true;
            _fInitDropTime = true;
            _hnsAmountToDrop = hnsAmountToDrop;
            Log(Log_Level_Warning, L"Dropping time hnsAmountToDrop=%I64d\n", hnsAmountToDrop);
        }
        else if (hnsAmountToDrop == 0)
        {
            // Reset time dropping
            Log(Log_Level_Warning, L"Disabling dropping time\n");
            ResetDropTime();
        }
        else
        {
            hr = E_INVALIDARG;
        }
    }

    return hr;
}


// IMFQualityAdvise2 methods
_Use_decl_annotations_
IFACEMETHODIMP NetworkMediaSourceStreamImpl::NotifyQualityEvent(
    IMFMediaEvent* pEvent, 
    DWORD* pdwFlags)
{
    NULL_CHK(pdwFlags);
    NULL_CHK(pEvent);

    HRESULT hr = S_OK;

    auto lock = static_cast<NetworkMediaSourceImpl*>(_spSource.Get())->Lock();

   * pdwFlags = 0;

    hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        MediaEventType met;
        hr = pEvent->GetType(&met);

        if (SUCCEEDED(hr) && met == MEQualityNotify)
        {
            GUID guiExtendedType;
            hr = pEvent->GetExtendedType(&guiExtendedType);

            if (SUCCEEDED(hr) && guiExtendedType == MF_QUALITY_NOTIFY_SAMPLE_LAG)
            {
                PROPVARIANT var;
                PropVariantInit(&var);

                hr = pEvent->GetValue(&var);
                LONGLONG hnsSampleLatency = var.hVal.QuadPart;

                if (SUCCEEDED(hr))
                {
                    if (_eDropMode == MF_DROP_MODE_NONE && hnsSampleLatency > 30000000)
                    {
                        Log(Log_Level_Warning, L"MediaSourceStream::NotifyQualityEvent: Entering drop mode, hnsSampleLatency = %I64d\n", hnsSampleLatency);
                        hr = SetDropMode(MF_DROP_MODE_1);
                    }
                    else if (_eDropMode == MF_DROP_MODE_1 && hnsSampleLatency < 0)
                    {
                        Log(Log_Level_Info, L"MediaSourceStream::NotifyQualityEvent: Leaving drop mode\n");
                        hr = SetDropMode(MF_DROP_MODE_NONE);
                    }
                    else
                    {
                        Log(Log_Level_All, L"MediaSourceStream::NotifyQualityEvent: Sample latency %I64d\n", hnsSampleLatency);
                    }
                }

                PropVariantClear(&var);
            }
        }
    }

    return hr;
}


// IMFGetService methods
_Use_decl_annotations_
HRESULT NetworkMediaSourceStreamImpl::GetService(REFGUID guidService, REFIID riid, LPVOID* ppvObject)
{
    HRESULT hr = S_OK;

    NULL_CHK(ppvObject);

    auto lock = static_cast<NetworkMediaSourceImpl*>(_spSource.Get())->Lock();

   * ppvObject = nullptr;

    hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        if (MF_QUALITY_SERVICES == guidService)
        {
            this->QueryInterface(riid, ppvObject);
        }
        else
        {
            hr = MF_E_UNSUPPORTED_SERVICE;
        }
    }

    return hr;
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceStreamImpl::Start()
{
    HRESULT hr = S_OK;

    auto lock = static_cast<NetworkMediaSourceImpl*>(_spSource.Get())->Lock();

    hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        if (_eSourceState == SourceStreamState_Stopped ||
            _eSourceState == SourceStreamState_Started)
        {
            _eSourceState = SourceStreamState_Started;
            // Inform the client that we've started
            hr = QueueEvent(MEStreamStarted, GUID_NULL, S_OK, nullptr);
        }
        else
        {
            hr = MF_E_INVALID_STATE_TRANSITION;
        }
    }

    if (FAILED(hr))
    {
        HandleError(hr);
    }

    return S_OK;
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceStreamImpl::Stop()
{
    HRESULT hr = S_OK;

    auto lock = static_cast<NetworkMediaSourceImpl*>(_spSource.Get())->Lock();

    hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        if (_eSourceState == SourceStreamState_Started)
        {
            _eSourceState = SourceStreamState_Stopped;
            _tokens.Clear();
            _samples.Clear();
            // Inform the client that we've stopped.
            hr = QueueEvent(MEStreamStopped, GUID_NULL, S_OK, nullptr);
        }
        else
        {
            hr = MF_E_INVALID_STATE_TRANSITION;
        }
    }

    if (FAILED(hr))
    {
        HandleError(hr);
    }

    return S_OK;
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceStreamImpl::SetRate(float flRate)
{
    HRESULT hr = S_OK;

    auto lock = static_cast<NetworkMediaSourceImpl*>(_spSource.Get())->Lock();

    IFC(CheckShutdown());

    _flRate = flRate;
    if (_flRate != 1.0f)
    {
        CleanSampleQueue();
    }

done:
    if (FAILED(hr))
    {
        HandleError(hr);
    }

    return S_OK;
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceStreamImpl::Flush()
{
    auto lock = static_cast<NetworkMediaSourceImpl*>(_spSource.Get())->Lock();

    _tokens.Clear();
    _samples.Clear();

    _fDiscontinuity = false;

    ResetDropTime();

    return S_OK;
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceStreamImpl::Shutdown()
{
    auto lock = static_cast<NetworkMediaSourceImpl*>(_spSource.Get())->Lock();

    HRESULT hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        Flush();

        if (_spEventQueue)
        {
            _spEventQueue->Shutdown();
             _spEventQueue.Reset();
        }

        _spStreamDescriptor.Reset();
        _eSourceState = SourceStreamState_Shutdown;
    }

    return hr;
}


// Processes media sample received from the header
_Use_decl_annotations_
HRESULT NetworkMediaSourceStreamImpl::ProcessSample(
    MediaSampleHeader* pSampleHeader, 
    MediaSampleTransforms* pSampleTransforms,
    IMFSample* pSample)
{
    NULL_CHK(pSampleHeader);
    NULL_CHK(pSample);

    auto lock = static_cast<NetworkMediaSourceImpl*>(_spSource.Get())->Lock();

    HRESULT hr = S_OK;

    IFC(CheckShutdown());

    // Set sample attributes
    IFC(SetSampleAttributes(pSampleHeader, pSampleTransforms, pSample));

    // Check if we are in propper state if so deliver the sample otherwise just skip it and don't treat it as an error.
    if (_eSourceState == SourceStreamState_Started)
    {
        // Put sample on the list
        IFC(_samples.InsertBack(pSample));

        // Deliver Samples 
        IFC(DeliverSamples());
    }
    else
    {
        IFC_MSG(MF_E_UNEXPECTED, L"NetworkMediaSourceStreamImpl::ProcessSample() - Not in the correct state");
    }

done:
    if (FAILED(hr))
    {
        HandleError(hr);
    }

    return hr;
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceStreamImpl::ProcessTick(
    MediaStreamTick* pTickHeader, 
    IMFAttributes* pAttributes)
{
    NULL_CHK(pAttributes);

    auto lock = static_cast<NetworkMediaSourceImpl*>(_spSource.Get())->Lock();

    HRESULT hr = S_OK;

    IFC(CheckShutdown());

    // Check if we are in propper state if so deliver the sample otherwise just skip it and don't treat it as an error.
    if (_eSourceState == SourceStreamState_Started)
    {
        // Put sample on the list
        IFC(_samples.InsertBack(pAttributes));

        // Deliver samples
        IFC(DeliverSamples());
    }
    else
    {
        IFC(MF_E_UNEXPECTED);
    }

done:
    if (FAILED(hr))
    {
        HandleError(hr);
    }

    return hr;
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceStreamImpl::ProcessFormatChange(
    IMFMediaType* pMediaType)
{
    NULL_CHK(pMediaType);

    auto lock = static_cast<NetworkMediaSourceImpl*>(_spSource.Get())->Lock();

    HRESULT hr = S_OK;

    IFC(CheckShutdown());

    // Check if we are in proper state if so deliver the sample otherwise just skip it and don't treat it as an error.
    if (_eSourceState == SourceStreamState_Started)
    {
        // Put sample on the list
        IFC(_samples.InsertBack(pMediaType));

        // Deliver samples
        IFC(DeliverSamples());
    }
    else
    {
        IFC(MF_E_UNEXPECTED);
    }

done:
    if (FAILED(hr))
    {
        HandleError(hr);
    }

    return hr;
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceStreamImpl::SetActive(
    bool fActive)
{
    auto lock = static_cast<NetworkMediaSourceImpl*>(_spSource.Get())->Lock();

    HRESULT hr = CheckShutdown();

    if (SUCCEEDED(hr))
    {
        if (_eSourceState != SourceStreamState_Stopped &&
            _eSourceState != SourceStreamState_Started)
        {
            hr = MF_E_INVALIDREQUEST;
        }
    }

    if (SUCCEEDED(hr))
    {
        _fActive = fActive;
    }
    else
    {
        IFR(hr);
    }

    return hr;
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceStreamImpl::Initialize(
    MediaTypeDescription* pStreamDescription, 
    IDataBundle* pAttributesBuffer)
{
    NULL_CHK(pStreamDescription);
    NULL_CHK(pAttributesBuffer);

    HRESULT hr = S_OK;

    // Create the media event queue.

    ComPtr<IMFMediaType> spMediaType;
    ComPtr<IMFStreamDescriptor> spSD;
    ComPtr<IMFMediaTypeHandler> spMediaTypeHandler;
    DWORD AttributesBlobSize = 0;

    ComPtr<IDataBundle> spAttribs(pAttributesBuffer);
    ComPtr<DataBundleImpl> spAttribsImpl(static_cast<DataBundleImpl*>(spAttribs.Get()));
    NULL_CHK(spAttribsImpl);

    IFR(MFCreateEventQueue(&_spEventQueue));

    _fVideo = (pStreamDescription->guiMajorType == MFMediaType_Video);

    // Create a media type object.
    IFR(MFCreateMediaType(&spMediaType));
    IFR(pAttributesBuffer->get_TotalSize(&AttributesBlobSize));

    if (AttributesBlobSize < pStreamDescription->AttributesBlobSize 
        || 
        pStreamDescription->AttributesBlobSize == 0)
    {
        // Invalid stream description
        IFR(MF_E_UNSUPPORTED_FORMAT);
    }

    // Prepare buffer where we will copy attributes to
    BYTE* pAttributes = new (std::nothrow) BYTE[pStreamDescription->AttributesBlobSize];
    NULL_CHK_HR(pAttributes, E_OUTOFMEMORY);

    // Move the memory
    IFC(spAttribsImpl->MoveLeft(pStreamDescription->AttributesBlobSize, pAttributes));

    // Initialize media type's attributes
    IFC(MFInitAttributesFromBlob(spMediaType.Get(), pAttributes, pStreamDescription->AttributesBlobSize));
    IFC(ValidateInputMediaType(pStreamDescription->guiMajorType, pStreamDescription->guiSubType, spMediaType.Get()));
    IFC(spMediaType->SetGUID(MF_MT_MAJOR_TYPE, pStreamDescription->guiMajorType));

    IFC(spMediaType->SetGUID(MF_MT_SUBTYPE, pStreamDescription->guiSubType));

    // Now we can create MF stream descriptor.
    IFC(MFCreateStreamDescriptor(pStreamDescription->dwStreamId, 1, spMediaType.GetAddressOf(), &spSD));
    IFC(spSD->GetMediaTypeHandler(&spMediaTypeHandler));

    // Set current media type
    IFC(spMediaTypeHandler->SetCurrentMediaType(spMediaType.Get()));

    _spStreamDescriptor = spSD;
    _dwId = pStreamDescription->dwStreamId;

    // State of the stream is started.
    _eSourceState = SourceStreamState_Stopped;

done:
    delete [] pAttributes;

    return hr;
}

// Deliver samples for every request client made
_Use_decl_annotations_
HRESULT NetworkMediaSourceStreamImpl::DeliverSamples()
{
    // do we have samples to deliver
    while (!_samples.IsEmpty() && !_tokens.IsEmpty())
    {
        ComPtr<IUnknown> spEntry;
        ComPtr<IMFSample> spSample;
        ComPtr<IMFMediaType> spMediaType;
        ComPtr<IUnknown> spToken;
        BOOL fDrop = FALSE;

        // Get the entry
        IFR_MSG(_samples.RemoveFront(&spEntry), L"DeliverSamples() _tokens is empty");

        // process the sample only if there are tokens waiting for it
        if (SUCCEEDED(spEntry.As(&spSample)))
        {
            fDrop = ShouldDropSample(spSample.Get());

            if (!fDrop)
            {
                // Get the request token
                IFR_MSG(_tokens.RemoveFront(&spToken), L"MediaStream::DeliverSamples() _tokens is empty");

                if (spToken)
                {
                    // If token was not null set the sample attribute.
                    LOG_RESULT_MSG(spSample->SetUnknown(MFSampleExtension_Token, spToken.Get()), L"MFSampleExtension_Token");
                }

                if (_fDiscontinuity)
                {
                    // If token was not null set the sample attribute.
                    LOG_RESULT_MSG(spSample->SetUINT32(MFSampleExtension_Discontinuity, TRUE), L"setting MFSampleExtension_Discontinuity");
                    _fDiscontinuity = false;
                }

                // Send a sample event.
                LOG_RESULT_MSG(_spEventQueue->QueueEventParamUnk(MEMediaSample, GUID_NULL, S_OK, spSample.Get()), L"send sample event");
            }
            else
            {
                _fDiscontinuity = true;
            }
        }
        else if (SUCCEEDED(spEntry.As(&spMediaType)))
        {
            // Send a format change event.
            LOG_RESULT_MSG(_spEventQueue->QueueEventParamUnk(MEStreamFormatChanged, GUID_NULL, S_OK, spMediaType.Get()), L"MEStreamFormatChanged");
        }
        else
        {
            CleanSampleQueue();

            _fDiscontinuity = true;
        }
    }

    if (!_samples.IsEmpty())
    {
        CleanSampleQueue();

        _fDiscontinuity = true;
    }

    return S_OK;
}

_Use_decl_annotations_
void NetworkMediaSourceStreamImpl::HandleError(HRESULT hErrorCode)
{
    if (hErrorCode != MF_E_SHUTDOWN)
    {
        // Send MEError to the client
        hErrorCode = QueueEvent(MEError, GUID_NULL, hErrorCode, nullptr);
    }
}

_Use_decl_annotations_
HRESULT NetworkMediaSourceStreamImpl::SetSampleAttributes(
    MediaSampleHeader* pSampleHeader, 
    MediaSampleTransforms* pSampleTransforms,
    IMFSample* pSample)
{
    //IFR_MSG(pSample->SetSampleTime(pSampleHeader->hnsTimestamp), L"setting sample time");
	// TODO: Troy hack to test 0 presentation time
	IFR_MSG(pSample->SetSampleTime(0), L"setting sample time");
    IFR_MSG(pSample->SetSampleDuration(pSampleHeader->hnsDuration), L"setting sample duration");

    SET_SAMPLE_ATTRIBUTE(pSampleHeader->dwFlags, pSampleHeader->dwFlagMasks, pSample, BottomFieldFirst);
    SET_SAMPLE_ATTRIBUTE(pSampleHeader->dwFlags, pSampleHeader->dwFlagMasks, pSample, CleanPoint);
    SET_SAMPLE_ATTRIBUTE(pSampleHeader->dwFlags, pSampleHeader->dwFlagMasks, pSample, DerivedFromTopField);
    SET_SAMPLE_ATTRIBUTE(pSampleHeader->dwFlags, pSampleHeader->dwFlagMasks, pSample, Discontinuity);
    SET_SAMPLE_ATTRIBUTE(pSampleHeader->dwFlags, pSampleHeader->dwFlagMasks, pSample, Interlaced);
    SET_SAMPLE_ATTRIBUTE(pSampleHeader->dwFlags, pSampleHeader->dwFlagMasks, pSample, RepeatFirstField);
    SET_SAMPLE_ATTRIBUTE(pSampleHeader->dwFlags, pSampleHeader->dwFlagMasks, pSample, SingleField);

    //if (nullptr != pSampleTransforms)
    {
        //IFR_MSG(pSample->SetBlob(MFSampleExtension_PinholeCameraIntrinsics, (UINT8*)&pSampleHeader->cameraIntrinsics, sizeof(pSampleHeader->cameraIntrinsics)), L"setting camera intrinsics");
        IFR_MSG(pSample->SetBlob(Spatial_CameraTransform, (UINT8*)&pSampleHeader->worldToCameraMatrix, sizeof(pSampleTransforms->worldToCameraMatrix)), L"setting camera tranform");
        IFR_MSG(pSample->SetBlob(MFSampleExtension_Spatial_CameraProjectionTransform, (UINT8*)&pSampleHeader->cameraProjectionTransform, sizeof(pSampleTransforms->cameraProjectionTransform)), L"setting camera projection matrix");
        IFR_MSG(pSample->SetBlob(MFSampleExtension_Spatial_CameraViewTransform, (UINT8*)&pSampleHeader->cameraViewTransform, sizeof(pSampleTransforms->cameraViewTransform)), L"setting camera view transform");
    }

    return S_OK;
}

_Use_decl_annotations_
bool NetworkMediaSourceStreamImpl::ShouldDropSample(
    IMFSample* pSample)
{
    if (!_fVideo)
    {
        return false;
    }

    bool fCleanPoint = MFGetAttributeUINT32(pSample, MFSampleExtension_CleanPoint, 0) > 0;
    bool fDrop = _flRate != 1.0f && !fCleanPoint;

    LONGLONG hnsTimeStamp = 0;
    LOG_RESULT_MSG(pSample->GetSampleTime(&hnsTimeStamp), L"GetSampleTime");

    LONGLONG hnsSampleDuration = 0;
    LOG_RESULT_MSG(pSample->GetSampleDuration(&hnsSampleDuration), L"GetSampleDuration");
    if (!fDrop && _fDropTime)
    {
        if (_fInitDropTime)
        {
            _hnsStartDroppingAt = hnsTimeStamp;
            _fInitDropTime = false;
        }

        fDrop = hnsTimeStamp < (_hnsStartDroppingAt + _hnsAmountToDrop);
        if (!fDrop)
        {
            Log(Log_Level_Info, L"Ending dropping time on sample ts=%I64d _hnsStartDroppingAt=%I64d _hnsAmountToDrop=%I64d\n",
                hnsTimeStamp, _hnsStartDroppingAt, _hnsAmountToDrop);
            ResetDropTime();
        }
        else
        {
            Log(Log_Level_Info, L"Dropping sample ts=%I64d _hnsStartDroppingAt=%I64d _hnsAmountToDrop=%I64d\n",
                hnsTimeStamp, _hnsStartDroppingAt, _hnsAmountToDrop);
        }
    }

    return fDrop;
}

_Use_decl_annotations_
void NetworkMediaSourceStreamImpl::CleanSampleQueue()
{
    auto pos = _samples.FrontPosition();

    ComPtr<IMFSample> spSample;
    if (_fVideo)
    {
        // For video streams leave first key frame.
        ComPtr<IUnknown> spEntry;
        for (; SUCCEEDED(_samples.GetItemPos(pos, &spEntry)); pos = _samples.Next(pos))
        {
            if (SUCCEEDED(spEntry.As(&spSample)) 
                && 
                MFGetAttributeUINT32(spSample.Get(), MFSampleExtension_CleanPoint, 0))
            {
                break;
            }
        }
    }

    _samples.Clear();

    if (spSample != nullptr)
    {
        LOG_RESULT_MSG(_samples.InsertFront(spSample.Get()), L"adding sample to list");
    }
}

_Use_decl_annotations_
void NetworkMediaSourceStreamImpl::ResetDropTime()
{
    _fDropTime = false;
    _fInitDropTime = false;
    _hnsStartDroppingAt = 0;
    _hnsAmountToDrop = 0;
    _fWaitingForCleanPoint = true;
}
