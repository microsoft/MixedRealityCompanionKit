//*****************************************************************************
//
//	Copyright 2015 Microsoft Corporation
//
//	Licensed under the Apache License, Version 2.0 (the "License");
//	you may not use this file except in compliance with the License.
//	You may obtain a copy of the License at
//
//	http ://www.apache.org/licenses/LICENSE-2.0
//
//	Unless required by applicable law or agreed to in writing, software
//	distributed under the License is distributed on an "AS IS" BASIS,
//	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//	See the License for the specific language governing permissions and
//	limitations under the License.
//
//*****************************************************************************

#include "pch.h"
#include "MediaUtils.h"
#include "RealtimeMediaSource.h"
#include <intsafe.h>

using namespace ABI::Windows::Foundation::Collections;
using namespace ABI::Windows::Media;
using namespace ABI::Windows::Media::Core;
using namespace ABI::Windows::Media::MediaProperties;
using namespace ABI::Windows::Storage::Streams;
using namespace Microsoft::WRL;

#define SET_SAMPLE_ATTRIBUTE(flag, mask, pSample, flagName) \
if ((static_cast<DWORD>(SampleFlags_SampleFlag_##flagName) & mask) == static_cast<DWORD>(SampleFlags_SampleFlag_##flagName)) \
{ \
    IFR(pSample->SetUINT32( \
        MFSampleExtension_##flagName, \
        (static_cast<DWORD>(SampleFlags_SampleFlag_##flagName) & flag) == static_cast<DWORD>(SampleFlags_SampleFlag_##flagName))); \
}

// Initialize an FFmpegInteropObject
_Use_decl_annotations_
RealtimeMediaSourceImpl::RealtimeMediaSourceImpl()
{

}

_Use_decl_annotations_
RealtimeMediaSourceImpl::~RealtimeMediaSourceImpl()
{
    auto lock = _lock.Lock();
    //auto lock = m_lock.LockExclusive();

    if (m_deferral != nullptr)
    {
        m_deferral->Complete();
        m_deferral = nullptr;
    }

    if (m_mediaStreamSource != nullptr)
    {
        LOG_RESULT(m_mediaStreamSource->remove_Starting(m_startingRequestedToken));
        LOG_RESULT(m_mediaStreamSource->remove_SampleRequested(m_sampleRequestedToken));
        LOG_RESULT(m_mediaStreamSource->remove_Closed(m_closeRequestedToken));

        m_mediaStreamSource.Reset();
        m_mediaStreamSource = nullptr;
    }
    
    LOG_RESULT(_spConnection->remove_Received(_evtReceivedToken));
    _spConnection.Reset();
    _spConnection = nullptr;

    if (m_spRequest != nullptr)
    {
        m_spRequest.Reset();
        m_spRequest = nullptr;
    }

    if (m_spVideoEncoding != nullptr)
    {
        m_spVideoEncoding.Reset();
        m_spVideoEncoding = nullptr;
    }
}

_Use_decl_annotations_
HRESULT RealtimeMediaSourceImpl::RuntimeClassInitialize(
    IConnection* pConnection)
{
    NULL_CHK(pConnection);

    auto lock = _lock.Lock();
    //auto lock = m_lock.LockExclusive();

    ComPtr<IConnection> spConnection(pConnection);
    IFR(spConnection.As(&_spConnection));

    // register for message events
    auto receivedHandler =
        Callback<IBundleReceivedEventHandler>(this, &RealtimeMediaSourceImpl::OnDataReceived);
    IFR(_spConnection->add_Received(receivedHandler.Get(), &_evtReceivedToken));

    // client is connected we need to send the Describe request
    _eSourceState = SourceStreamState_Opening;

    IFR(SendDescribeRequest());

    return AsyncBase::Start();
}

_Use_decl_annotations_
HRESULT RealtimeMediaSourceImpl::CreateMediaSource(
    IVideoEncodingProperties* pVideoEncodingProperties)
{
    NULL_CHK(pVideoEncodingProperties);

    // Convert to IVideoEncodingProperties to IVideoStreamDescriptor
    ComPtr<IVideoStreamDescriptorFactory> spVideoDescFactory;
    IFR(Windows::Foundation::GetActivationFactory(
        Microsoft::WRL::Wrappers::HStringReference(RuntimeClass_Windows_Media_Core_VideoStreamDescriptor).Get(),
        &spVideoDescFactory));

    ComPtr<ABI::Windows::Media::Core::IVideoStreamDescriptor> spVideoStreamDescriptor;
    IFR(spVideoDescFactory->Create(pVideoEncodingProperties, &spVideoStreamDescriptor));

    ComPtr<IMediaStreamSourceFactory> spMSSFactory;
    IFR(Windows::Foundation::GetActivationFactory(
        Wrappers::HStringReference(RuntimeClass_Windows_Media_Core_MediaStreamSource).Get(),
        &spMSSFactory));

    // TODO: Convert IMFStreamDescriptor to IMediaStreamDescriptor???
    ComPtr<IMediaStreamDescriptor> spDescriptor;
    spVideoStreamDescriptor.As(&spDescriptor);

    // Create internal MediaStreamSource
    IFR(spMSSFactory->CreateFromDescriptor(spDescriptor.Get(), &m_mediaStreamSource));

    // Set buffer time to 0 for realtime streaming to reduce latency
    m_mediaStreamSource->put_BufferTime({ 0 });

    // setup callbacks
    auto startingCallback =
        Microsoft::WRL::Callback<IStartingEventHandler>(this, &RealtimeMediaSourceImpl::OnStarting);
    auto sampleResquestedCallback =
        Microsoft::WRL::Callback<ISampleRequestedEventHandler>(this, &RealtimeMediaSourceImpl::OnSampleRequested);
    auto closedCallback =
        Microsoft::WRL::Callback<IMediaClosedEventHandler>(this, &RealtimeMediaSourceImpl::OnClosed);

    IFR(m_mediaStreamSource->add_Starting(startingCallback.Get(), &m_startingRequestedToken));
    IFR(m_mediaStreamSource->add_SampleRequested(sampleResquestedCallback.Get(), &m_sampleRequestedToken));
    IFR(m_mediaStreamSource->add_Closed(closedCallback.Get(), &m_closeRequestedToken))

    return S_OK;
}

_Use_decl_annotations_
HRESULT RealtimeMediaSourceImpl::get_MediaStreamSource(
    ABI::Windows::Media::Core::IMediaStreamSource** ppMediaStreamSource)
{
    auto lock = _lock.Lock();
    //auto lock = m_lock.LockShared();

    return m_mediaStreamSource.CopyTo(ppMediaStreamSource);
}

_Use_decl_annotations_
HRESULT RealtimeMediaSourceImpl::get_StreamResolution(
    UINT32* pWidth,
    UINT32* pHeight)
{
    //auto lock = m_lock.LockShared();
    auto lock = _lock.Lock();

    if (m_spVideoEncoding == nullptr)
    {
        return E_FAIL;
    }

    IFR(m_spVideoEncoding->get_Height(pHeight));
    IFR(m_spVideoEncoding->get_Width(pWidth));

    return S_OK;
}

_Use_decl_annotations_
HRESULT RealtimeMediaSourceImpl::OnStarting(IMediaStreamSource* sender, IMediaStreamSourceStartingEventArgs* args)
{
    ComPtr<IMediaStreamSourceStartingRequest> spRequest;
    IFR(args->get_Request(&spRequest));

    ComPtr<ABI::Windows::Foundation::IReference<ABI::Windows::Foundation::TimeSpan>> spStartPosition;
    IFR(spRequest->get_StartPosition(&spStartPosition));

    //auto lock = m_lock.LockShared();
    //auto lock = _lock.Lock();

    ABI::Windows::Foundation::TimeSpan value;
    if (spStartPosition != nullptr)
    {
        IFR(spStartPosition->get_Value(&value));
    }

    if (spStartPosition != nullptr)
    {
        IFR(spRequest->SetActualStartPosition(value));
    }

    return S_OK;
}

_Use_decl_annotations_
HRESULT RealtimeMediaSourceImpl::OnSampleRequested(IMediaStreamSource* sender, IMediaStreamSourceSampleRequestedEventArgs* args)
{
    Log(Log_Level_Info, L"RealtimeMediaSourceImpl::OnSampleRequested()\n");

    //auto lock = m_lock.LockExclusive();
    auto lock = _lock.Lock();

    IFR(args->get_Request(&m_spRequest));

    if (m_latestSample == nullptr)
    {
		Log(Log_Level_Info, L"RealtimeMediaSourceImpl::OnSampleRequested() - No latest sample, taking deferral \n");

        if (m_deferral != nullptr) {
            Log(Log_Level_Error, L"RealtimeMediaSourceImpl::OnSampleRequested() - Got deferral when another has not completed\n");
        }

        m_spRequest->GetDeferral(&m_deferral);
    }
    else if (m_mediaStreamSource != nullptr) // teardown?
    {
        ComPtr<IMFMediaStreamSourceSampleRequest> spIMFRequest;
        IFR(m_spRequest.As(&spIMFRequest));

        IFR(spIMFRequest->SetSample(m_latestSample.Get()));

		  m_latestSample = nullptr;
		  m_spRequest = nullptr;
    }

    return S_OK;
}

_Use_decl_annotations_
HRESULT RealtimeMediaSourceImpl::OnClosed(IMediaStreamSource* sender, IMediaStreamSourceClosedEventArgs* args)
{
    //auto lock = m_lock.LockShared();
    auto lock = _lock.Lock();

    ComPtr<IMediaStreamSourceClosedRequest> spRequest;
    IFR(args->get_Request(&spRequest));

    MediaStreamSourceClosedReason reason;
    IFR(spRequest->get_Reason(&reason));

    switch (reason)
    {
    case MediaStreamSourceClosedReason_UnknownError:
        LOG_RESULT(E_UNEXPECTED)
            break;
    case MediaStreamSourceClosedReason_AppReportedError:
        LOG_RESULT(E_ABORT);
        break;
    case MediaStreamSourceClosedReason_UnsupportedProtectionSystem:
    case MediaStreamSourceClosedReason_ProtectionSystemFailure:
        LOG_RESULT(MF_E_DRM_UNSUPPORTED);
        break;
    case MediaStreamSourceClosedReason_UnsupportedEncodingFormat:
        LOG_RESULT(MF_E_UNSUPPORTED_FORMAT);
        break;
    case MediaStreamSourceClosedReason_MissingSampleRequestedEventHandler:
        LOG_RESULT(HRESULT_FROM_WIN32(ERROR_NO_CALLBACK_ACTIVE));
        break;
    }

    return S_OK;
}

//
// Data Connection
//

// Sending requests to network
_Use_decl_annotations_
HRESULT RealtimeMediaSourceImpl::SendDescribeRequest()
{
    Log(Log_Level_Info, L"RealtimeMediaSourceImpl::SendDescribeRequest()\n");

    NULL_CHK_HR(_spConnection, E_POINTER);

    return _spConnection->SendPayloadType(PayloadType_RequestMediaDescription);
}

_Use_decl_annotations_
HRESULT RealtimeMediaSourceImpl::SendStartRequest()
{
    Log(Log_Level_Info, L"RealtimeMediaSourceImpl::SendStartRequest()\n");

    NULL_CHK_HR(_spConnection, E_POINTER);

    return _spConnection->SendPayloadType(PayloadType_RequestMediaStart);
}

_Use_decl_annotations_
HRESULT RealtimeMediaSourceImpl::SendStopRequest()
{
    Log(Log_Level_Info, L"RealtimeMediaSourceImpl::SendStopRequest()\n");

    NULL_CHK_HR(_spConnection, E_POINTER);

    return _spConnection->SendPayloadType(PayloadType_RequestMediaStop);
}

_Use_decl_annotations_
HRESULT RealtimeMediaSourceImpl::OnDataReceived(
    IConnection *sender,
    IBundleReceivedArgs *args)
{
    ComPtr<IBundleReceivedArgs> spArgs(args);

    // TODO: Troy figure out right lock
    //auto lock = m_lock.LockExclusive();

    HRESULT hr = S_OK;
    PayloadType type;
    ComPtr<IDataBundle> spDataBundle;

    IFC(CheckShutdown());

    IFC(args->get_PayloadType(&type));

    Log(Log_Level_Info, L"RealtimeMediaSourceImpl::OnDataReceived(%d)\n", type);

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
		Log(Log_Level_Info, L"RealtimeMediaSourceImpl::OnDataReceived() - PayloadType_SendMediaStreamTick\n");
        // TODO: Troy how to turn on without IMFMediaSource
        //IFC(ProcessMediaTick(spDataBundle.Get()));
        break;
    };

done:
    if (FAILED(hr))
    {
        LOG_RESULT(hr);
    }
    return S_OK;
}

// Helper methods to handle received network bundles
_Use_decl_annotations_
HRESULT RealtimeMediaSourceImpl::ProcessCaptureReady()
{
    auto lock = _lock.Lock();

    Log(Log_Level_Info, L"RealtimeMediaSourceImpl::ProcessCaptureReady()\n");

    if (_eSourceState == SourceStreamState_Started || _eSourceState == SourceStreamState_Stopped)
    {
        return S_OK;
    }

    IFR(SendDescribeRequest());

    return S_OK;
}

_Use_decl_annotations_
HRESULT RealtimeMediaSourceImpl::ProcessMediaDescription(
    IDataBundle* pBundle)
{
    Log(Log_Level_Info, L"RealtimeMediaSourceImpl::ProcessMediaDescription()\n");

    if (_eSourceState == SourceStreamState_Started || _eSourceState == SourceStreamState_Stopped)
        //&& _streams.GetCount() > 0)
    {
        return S_OK;
    }

    DataBundleImpl* pBundleImpl = static_cast<DataBundleImpl*>(pBundle);
    NULL_CHK(pBundleImpl);

    if (_eSourceState != SourceStreamState_Opening)
    {
        if (_eSourceState != SourceStreamState_Stopped)
        {
            // TODO: What to do here
            //Shutdown();
        }
        else
        {
            // Server description should only be sent during opening state
            IFR(MF_E_UNEXPECTED);
        }
    }

    DWORD cbTotalLen = 0, cbHeaderSize = 0, StreamTypeHeaderSize = 0;
    MediaDescription desc;
    const DWORD c_cbMediaDescriptionSize = sizeof(MediaDescription);
    MediaTypeDescription* pMediaTypeDesc = nullptr;
    const DWORD c_cbMediaTypeDescriptionSize = sizeof(MediaTypeDescription);
    BYTE* pPtr = nullptr;
    ComPtr<IVideoEncodingProperties> spVideoEncoding;

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
        desc.StreamCount == 0 || desc.StreamCount > 2 ||
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

    IFC(GetPropertiesFromMediaDescription(&pMediaTypeDesc[0], // MediaTypeDescription* pStreamDescription,
        pBundle,
        &spVideoEncoding));

    IFC(CreateMediaSource(spVideoEncoding.Get()));

    // Save encoding internally
    m_spVideoEncoding = spVideoEncoding;

    _eSourceState = SourceStreamState_Starting;

    // Ask the server to start sending media samples to process
    IFC(SendStartRequest());

    _eSourceState = SourceStreamState_Started;

done:
    delete[] pPtr;

    return CompleteAsyncAction(hr);
}

_Use_decl_annotations_
HRESULT RealtimeMediaSourceImpl::GetPropertiesFromMediaDescription(
    MediaTypeDescription* pStreamDescription,
    IDataBundle* pAttributesBuffer,
    IVideoEncodingProperties** ppVideoEncodingProperties)
{
    NULL_CHK(pStreamDescription);
    NULL_CHK(pAttributesBuffer);

    ComPtr<IVideoEncodingProperties> spVideoProperties;

    HRESULT hr = S_OK;

    // Create Media Type from attributes blob
    ComPtr<IMFMediaType> spMediaType;
    DWORD AttributesBlobSize = 0;

    ComPtr<IDataBundle> spAttribs(pAttributesBuffer);
    ComPtr<DataBundleImpl> spAttribsImpl(static_cast<DataBundleImpl*>(spAttribs.Get()));
    NULL_CHK(spAttribsImpl);

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
    //IFC(ValidateInputMediaType(pStreamDescription->guiMajorType, pStreamDescription->guiSubType, spMediaType.Get()));
    IFC(spMediaType->SetGUID(MF_MT_MAJOR_TYPE, pStreamDescription->guiMajorType));
    IFC(spMediaType->SetGUID(MF_MT_SUBTYPE, pStreamDescription->guiSubType));

    IFC(MFCreatePropertiesFromMediaType(spMediaType.Get(), IID_PPV_ARGS(&spVideoProperties)));

    IFC(spVideoProperties.CopyTo(ppVideoEncodingProperties));
    /*
    TODO: See if I can return the streamdescriptor instead of IVideoEncodingProperties

    ComPtr<IMFStreamDescriptor> spSD;
    ComPtr<IMFMediaTypeHandler> spMediaTypeHandler;

    // Now we can create MF stream descriptor.
    IFC(MFCreateStreamDescriptor(pStreamDescription->dwStreamId, 1, spMediaType.GetAddressOf(), &spSD));
    IFC(spSD->GetMediaTypeHandler(&spMediaTypeHandler));

    // Set current media type
    IFC(spMediaTypeHandler->SetCurrentMediaType(spMediaType.Get()));
    */
done:
    delete[] pAttributes;

    return hr;
}

_Use_decl_annotations_
HRESULT RealtimeMediaSourceImpl::ProcessMediaSample(
    IDataBundle* pBundle)
{
    //auto lock = m_lock.TryLockExclusive();
    //auto lock = _lock.Lock();

    Log(Log_Level_Info, L"RealtimeMediaSourceImpl::ProcessMediaSample()\n");

	ComPtr<IMFSample> spSample;
    ComPtr<IDataBundle> spBundle(pBundle);
    DataBundleImpl* pBundleImpl = static_cast<DataBundleImpl*>(pBundle);
    NULL_CHK(pBundleImpl);

    // Only process samples when we are in started state
    if (_eSourceState != SourceStreamState_Started && _eSourceState != SourceStreamState_Shutdown)
    {
        if (FAILED(ProcessCaptureReady()))
        {
            IFR_MSG(MF_E_INVALID_STATE_TRANSITION, L"RealtimeMediaSourceImpl::ProcessMediaSample() - not in a state to receive data.");
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

    // Copy the header object
    IFC(pBundleImpl->MoveLeft(sizeof(MediaSampleHeader), &sampleHead));
    IFC(pBundleImpl->get_TotalSize(&cbTotalSize));

    if (sampleHead.cbCameraDataSize > 0)
    {
        IFC(pBundleImpl->MoveLeft(sampleHead.cbCameraDataSize, &sampleTransforms));
    }

	// Convert bundle data into IMFSample
	IFC(pBundleImpl->ToMFSample(&spSample));

	// Set appropriate attributes of sample from header data
	IFC(SetSampleAttributes(&sampleHead, spSample.Get()));

    {
        // Save the recorded sample off the network to be given to OnSampleRequested called by the internal MediaStreamSource
        // Obtain lock since the OnSampleRequested could be reading this on another thread
        //auto lock = m_lock.LockExclusive();
		auto lock = _lock.Lock();

		m_latestSample = spSample;

        if (m_deferral != nullptr) 
		{
            Log(Log_Level_Info, L"RealtimeMediaSourceImpl::ProcessMediaSample - Fire Deferral()\n");

            ComPtr<IMFMediaStreamSourceSampleRequest> spIMFRequest;
            IFC(m_spRequest.As(&spIMFRequest));

            IFC(spIMFRequest->SetSample(m_latestSample.Get()));

            IFC(m_deferral->Complete());

            m_deferral = nullptr;
            m_spRequest = nullptr;
			m_latestSample = nullptr;
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
HRESULT RealtimeMediaSourceImpl::SetSampleAttributes(
	MediaSampleHeader* pSampleHeader,
	IMFSample* pSample)
{
	IFR_MSG(pSample->SetSampleTime(pSampleHeader->hnsTimestamp), L"setting sample time");
	IFR_MSG(pSample->SetSampleDuration(pSampleHeader->hnsDuration), L"setting sample duration");

	SET_SAMPLE_ATTRIBUTE(pSampleHeader->dwFlags, pSampleHeader->dwFlagMasks, pSample, BottomFieldFirst);
	SET_SAMPLE_ATTRIBUTE(pSampleHeader->dwFlags, pSampleHeader->dwFlagMasks, pSample, CleanPoint);
	SET_SAMPLE_ATTRIBUTE(pSampleHeader->dwFlags, pSampleHeader->dwFlagMasks, pSample, DerivedFromTopField);
	SET_SAMPLE_ATTRIBUTE(pSampleHeader->dwFlags, pSampleHeader->dwFlagMasks, pSample, Discontinuity);
	SET_SAMPLE_ATTRIBUTE(pSampleHeader->dwFlags, pSampleHeader->dwFlagMasks, pSample, Interlaced);
	SET_SAMPLE_ATTRIBUTE(pSampleHeader->dwFlags, pSampleHeader->dwFlagMasks, pSample, RepeatFirstField);
	SET_SAMPLE_ATTRIBUTE(pSampleHeader->dwFlags, pSampleHeader->dwFlagMasks, pSample, SingleField);

	return S_OK;
}

_Use_decl_annotations_
HRESULT RealtimeMediaSourceImpl::ProcessMediaFormatChange(
    IDataBundle* pBundle)
{
    DataBundleImpl* pBundleImpl = static_cast<DataBundleImpl*>(pBundle);
    NULL_CHK(pBundleImpl);

    ComPtr<IMFMediaType> spMediaType;
    UINT32 bufferCount = 0;
    HRESULT hr = S_OK;

    IFC(pBundleImpl->get_BufferCount(&bufferCount));
    if (bufferCount == 0)
    {
        // investigate why
        return S_OK;
    }

    if (_eSourceState != SourceStreamState_Started && _eSourceState != SourceStreamState_Shutdown)
    {
        if (FAILED(ProcessCaptureReady()))
        {
            IFC_MSG(MF_E_INVALID_STATE_TRANSITION, L"RealtimeMediaSourceImpl::ProcessMediaFormatChange() - not in a state to receive data.");
        }
    }

    DWORD cbTotalLen = 0;
    IFC(pBundleImpl->get_TotalSize(&cbTotalLen));

    // Minimum size of the operation payload is size of Description structure
    DWORD cbTypeDescSize = sizeof(MediaTypeDescription);
    if (cbTotalLen < sizeof(Network::MediaTypeDescription))
    {
        IFC(MF_E_UNSUPPORTED_FORMAT);
    }

    MediaTypeDescription streamDesc;
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

// Handle errors
_Use_decl_annotations_
HRESULT RealtimeMediaSourceImpl::HandleError(
    HRESULT hResult)
{
    IFR(CompleteAsyncAction(hResult));

    if (_eSourceState != SourceStreamState_Shutdown)
    {
        /*
        // TODO:
        What to do with notifying of failed event?
        */
    }

    return S_OK;
}


_Use_decl_annotations_
HRESULT RealtimeMediaSourceImpl::CompleteAsyncAction(
    HRESULT hResult)
{
    Log(Log_Level_Info, L"RealtimeMediaSourceImpl::CompleteAsyncAction()\n");

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