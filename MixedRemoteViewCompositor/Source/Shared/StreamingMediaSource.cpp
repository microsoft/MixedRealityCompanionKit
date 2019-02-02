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
#include "Media.h"
#include "StreamingMediaSource.h"
#include <intsafe.h>
#include "MediaUtils.h"

/*
#include "MediaSampleProvider.h"
#include "H264AVCSampleProvider.h"
#include "H264SampleProvider.h"
#include "UncompressedAudioSampleProvider.h"
#include "UncompressedVideoSampleProvider.h"
#include "shcore.h"
*/

using namespace ABI::Windows::Foundation::Collections;
using namespace ABI::Windows::Media;
using namespace ABI::Windows::Media::Core;
using namespace ABI::Windows::Media::MediaProperties;
using namespace ABI::Windows::Storage::Streams;
using namespace Microsoft::WRL;

// Initialize an FFmpegInteropObject
_Use_decl_annotations_
StreamingMediaSourceImpl::StreamingMediaSourceImpl()
{

}

_Use_decl_annotations_
StreamingMediaSourceImpl::~StreamingMediaSourceImpl()
{
	auto lock = m_lock.LockExclusive();

	if (m_deferral != nullptr)
	{
		m_deferral->Complete();
		m_deferral = nullptr;
	}

	if (m_mediaStreamSource != nullptr)
	{
		m_mediaStreamSource->remove_Starting(m_startingRequestedToken);
		m_mediaStreamSource->remove_SampleRequested(m_sampleRequestedToken);
		m_mediaStreamSource->remove_Closed(m_closeRequestedToken);
		m_mediaStreamSource.Reset();
		m_mediaStreamSource = nullptr;
	}
}

_Use_decl_annotations_
HRESULT StreamingMediaSourceImpl::RuntimeClassInitialize(
	IConnection* pConnection)
{
	NULL_CHK(pConnection);

	auto lock = m_lock.LockExclusive();

	ComPtr<IConnection> spConnection(pConnection);
	IFR(spConnection.As(&_spConnection));

	// register for message events
	auto receivedHandler =
		Callback<IBundleReceivedEventHandler>(this, &StreamingMediaSourceImpl::OnDataReceived);
	IFR(_spConnection->add_Received(receivedHandler.Get(), &_evtReceivedToken));

	// client is connected we need to send the Describe request
	_eSourceState = SourceStreamState_Opening;

	IFR(SendDescribeRequest());
	// TODO: Troy just send start request???

	return AsyncBase::Start();
}

_Use_decl_annotations_
HRESULT StreamingMediaSourceImpl::CreateMediaSource()
{
	// TODO: Need to modify this from being hard-coded to based on descriptor downloaded overnet
	// TODO: Need to accomodate audio profiles?

	ComPtr<IMediaEncodingProfileStatics> spEncodingProfileStatics;
	IFR(Windows::Foundation::GetActivationFactory(
		Wrappers::HStringReference(RuntimeClass_Windows_Media_MediaProperties_MediaEncodingProfile).Get(),
		&spEncodingProfileStatics));

	//ComPtr<IMediaEncodingProfile> spMediaEncodingProfile;
	IFR(spEncodingProfileStatics->CreateMp4(
		ABI::Windows::Media::MediaProperties::VideoEncodingQuality_HD720p,
		&m_spMediaEncodingProfile));
		
	// TODO: Remove for audio support
	m_spMediaEncodingProfile->put_Audio(nullptr);

	// Get IVideoEncodingProperties from IMediaEncodingProfile
	ComPtr<IVideoEncodingProperties> spVideoEncodingProperties;
	IFR(m_spMediaEncodingProfile->get_Video(&spVideoEncodingProperties));

	// Conver to IVideoEncodingProperties to IVideoStreamDescriptor
	ComPtr<IVideoStreamDescriptorFactory> spVideoDescFactory;
	IFR(Windows::Foundation::GetActivationFactory(
		Microsoft::WRL::Wrappers::HStringReference(RuntimeClass_Windows_Media_Core_VideoStreamDescriptor).Get(), 
		&spVideoDescFactory));

	ComPtr<ABI::Windows::Media::Core::IVideoStreamDescriptor> spVideoStreamDescriptor;
	IFR(spVideoDescFactory->Create(spVideoEncodingProperties.Get(), &spVideoStreamDescriptor));

	ComPtr<IMediaStreamSourceFactory> spMSSFactory;
	IFR(Windows::Foundation::GetActivationFactory(
		Wrappers::HStringReference(RuntimeClass_Windows_Media_Core_MediaStreamSource).Get(),
		&spMSSFactory));

	ComPtr<IMediaStreamDescriptor> spDescriptor;
	spVideoStreamDescriptor.As(&spDescriptor);
	
	// Create internal MediaStreamSource
	IFR(spMSSFactory->CreateFromDescriptor(spDescriptor.Get(), &m_mediaStreamSource));
	
	// Set buffer time to 0 for realtime streaming to reduce latency
	m_mediaStreamSource->put_BufferTime({ 0 });

	// setup callbacks
	auto startingCallback =
		Microsoft::WRL::Callback<IStartingEventHandler>(this, &StreamingMediaSourceImpl::OnStarting);
	auto sampleResquestedCallback =
		Microsoft::WRL::Callback<ISampleRequestedEventHandler>(this, &StreamingMediaSourceImpl::OnSampleRequested);
	auto closedCallback =
		Microsoft::WRL::Callback<IMediaClosedEventHandler>(this, &StreamingMediaSourceImpl::OnClosed);

	IFR(m_mediaStreamSource->add_Starting(startingCallback.Get(), &m_startingRequestedToken));
	IFR(m_mediaStreamSource->add_SampleRequested(sampleResquestedCallback.Get(), &m_sampleRequestedToken));
	IFR(m_mediaStreamSource->add_Closed(closedCallback.Get(), &m_closeRequestedToken))
	
	return S_OK;
}

_Use_decl_annotations_
HRESULT StreamingMediaSourceImpl::get_MediaStreamSource(
	ABI::Windows::Media::Core::IMediaStreamSource** ppMediaStreamSource)
{
	auto lock = m_lock.LockShared();

	return m_mediaStreamSource.CopyTo(ppMediaStreamSource);
}

_Use_decl_annotations_
HRESULT StreamingMediaSourceImpl::OnStarting(IMediaStreamSource* sender, IMediaStreamSourceStartingEventArgs* args)
{
	ComPtr<IMediaStreamSourceStartingRequest> spRequest;
	IFR(args->get_Request(&spRequest));

	ComPtr<ABI::Windows::Foundation::IReference<ABI::Windows::Foundation::TimeSpan>> spStartPosition;
	IFR(spRequest->get_StartPosition(&spStartPosition));

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

/*
_Use_decl_annotations_
HRESULT StreamingMediaSourceImpl::CreateTestFrame()
{
	ComPtr<IRandomAccessStream> spRandomAccessStream;
	IFR(Windows::Foundation::ActivateInstance(
		Microsoft::WRL::Wrappers::HStringReference(RuntimeClass_Windows_Storage_Streams_InMemoryRandomAccessStream).Get(), 
		&spRandomAccessStream));

	ComPtr<IOutputStream> spOutputStream;
	IFR(spRandomAccessStream->GetOutputStreamAt(0, &spOutputStream));

	ComPtr<ABI::Windows::Storage::Streams::IDataWriterFactory> spDataWriterFactory;
	IFR(Windows::Foundation::GetActivationFactory(
		Microsoft::WRL::Wrappers::HStringReference(RuntimeClass_Windows_Storage_Streams_DataWriter).Get(), 
		&spDataWriterFactory));
	
	ComPtr<IDataWriter> spDataWriter;
	IFR(spDataWriterFactory->CreateDataWriter(*spOutputStream.GetAddressOf(), &spDataWriter));

	// TODO: modify this
	//BYTE *pData = (BYTE *)_videoFrameBuffer;
	//IFR(spDataWriter->WriteBytes(VIDEO_PELS * sizeof(DWORD), pData));

	ComPtr<ABI::Windows::Storage::Streams::IBuffer> spBuffer;
	IFR(spDataWriter->DetachBuffer(&spBuffer));

	ComPtr<ABI::Windows::Media::Core::IMediaStreamSampleStatics> spFactory;
	IFR(Windows::Foundation::GetActivationFactory(
		Microsoft::WRL::Wrappers::HStringReference(RuntimeClass_Windows_Media_Core_MediaStreamSample).Get(), 
		&spFactory));

	ComPtr<IMediaStreamSample> spSample;
	IFR(spFactory->CreateFromBuffer(spBuffer.Get(), pts, &spSample));
	IFR(spSample->put_Duration(0));// TODO: true?
}*/

_Use_decl_annotations_
HRESULT StreamingMediaSourceImpl::OnSampleRequested(IMediaStreamSource* sender, IMediaStreamSourceSampleRequestedEventArgs* args)
{
	Log(Log_Level_Info, L"StreamingMediaSourceImpl::OnSampleRequested()\n");

	//ComPtr<IMediaStreamSourceSampleRequest> spRequest;
	IFR(args->get_Request(&m_spRequest));

	//auto lock = m_lock.LockShared();
	auto lock = m_lock.LockExclusive();

	if (m_latestSample == nullptr)
	{
		/*
		if (_deferral != nullptr) {
			RTC_LOG(LS_ERROR) << "Got deferral when another hasn't completed.";
		}*/

		//ComPtr<IMediaStreamSourceSampleRequestDeferral> deferral;
		m_spRequest->GetDeferral(&m_deferral);
	}
	else if (m_mediaStreamSource != nullptr) // teardown?
	{
		ComPtr<IMFMediaStreamSourceSampleRequest> spIMFRequest;
		IFR(m_spRequest.As(&spIMFRequest));

		IFR(spIMFRequest->SetSample(m_latestSample.Get()));
	}

	/*
	ComPtr<IMediaStreamDescriptor> spDescriptor;
	IFR(spRequest->get_StreamDescriptor(&spDescriptor));

	ComPtr<IAudioStreamDescriptor> spAudioDescriptor;
	bool isAudioRequest = SUCCEEDED(spDescriptor.As(&spAudioDescriptor));

	ComPtr<IVideoStreamDescriptor> spVideoDescriptor;
	bool isVideoRequest = SUCCEEDED(spDescriptor.As(&spVideoDescriptor));

	auto lock = m_lock.LockShared();
	if (m_mediaStreamSource != nullptr)
	{
		IMFMediaStreamSourceSampleRequest
		spRequest->

		ComPtr<IMediaStreamSample> spSample = nullptr;
		if (isAudioRequest && m_audioSampleProvider != nullptr)
		{
			IFR(m_audioSampleProvider->GetNextSample(&spSample));
		}
		else if (isVideoRequest && m_videoSampleProvider != nullptr)
		{
			IFR(m_videoSampleProvider->GetNextSample(&spSample));
		}

		IFR(spRequest->put_Sample(spSample.Get()));
	}
	*/
	return S_OK;
}

_Use_decl_annotations_
HRESULT StreamingMediaSourceImpl::OnClosed(IMediaStreamSource* sender, IMediaStreamSourceClosedEventArgs* args)
{
	auto lock = m_lock.LockShared();

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
HRESULT StreamingMediaSourceImpl::SendDescribeRequest()
{
	Log(Log_Level_Info, L"NetworkMediaSourceImpl::SendDescribeRequest()\n");

	NULL_CHK_HR(_spConnection, E_POINTER);

	return _spConnection->SendPayloadType(PayloadType_RequestMediaDescription);
}

_Use_decl_annotations_
HRESULT StreamingMediaSourceImpl::SendStartRequest()
{
	Log(Log_Level_Info, L"NetworkMediaSourceImpl::SendStartRequest()\n");

	NULL_CHK_HR(_spConnection, E_POINTER);

	return _spConnection->SendPayloadType(PayloadType_RequestMediaStart);
}

_Use_decl_annotations_
HRESULT StreamingMediaSourceImpl::SendStopRequest()
{
	Log(Log_Level_Info, L"NetworkMediaSourceImpl::SendStopRequest()\n");

	NULL_CHK_HR(_spConnection, E_POINTER);

	return _spConnection->SendPayloadType(PayloadType_RequestMediaStop);
}

_Use_decl_annotations_
HRESULT StreamingMediaSourceImpl::OnDataReceived(
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
HRESULT StreamingMediaSourceImpl::ProcessCaptureReady()
{
	Log(Log_Level_Info, L"StreamingMediaSourceImpl::ProcessCaptureReady()\n");
	/*
	if (_eSourceState == SourceStreamState_Stopped)
		//&& _streams.GetCount() > 0)
	{
		return S_OK;
	}

	if (_eSourceState == SourceStreamState_Invalid)
		//|| _streams.GetCount() == 0)
	{
		_eSourceState = SourceStreamState_Opening;

		auto lock = m_lock.LockShared();

		IFR(CheckShutdown());

		ComPtr<SourceOperation> spInitOp;
		spInitOp.Attach(new (std::nothrow) SourceOperation(SourceOperation::Operation_Init));
		NULL_CHK_HR(spInitOp, E_OUTOFMEMORY);

		// Queue asynchronous stop
		return QueueOperation(spInitOp.Get());
	}

	return HandleError(MF_E_INVALID_STATE_TRANSITION);
	*/
	return S_OK;
}

_Use_decl_annotations_
HRESULT StreamingMediaSourceImpl::ProcessMediaDescription(
	IDataBundle* pBundle)
{
	Log(Log_Level_Info, L"NetworkMediaSourceImpl::ProcessMediaDescription()\n");

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

	//pBundle, &pMediaTypeDesc[0]
	//MediaTypeDescription* pStreamDescription, IDataBundle* pAttributesBuffer,
	IFC(CreateMediaSource());

	// Everything succeeded we are in stopped state now
	_eSourceState = SourceStreamState_Starting;

	//SendStopRequest();
	SendStartRequest();

	_eSourceState = SourceStreamState_Started;

done:
	delete[] pPtr;

	return CompleteAsyncAction(hr);
}

_Use_decl_annotations_
HRESULT StreamingMediaSourceImpl::ProcessMediaSample(
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

	// Copy the header object
	IFC(pBundleImpl->MoveLeft(sizeof(MediaSampleHeader), &sampleHead));
	IFC(pBundleImpl->get_TotalSize(&cbTotalSize));

	if (sampleHead.cbCameraDataSize > 0)
	{
		IFC(pBundleImpl->MoveLeft(sampleHead.cbCameraDataSize, &sampleTransforms));
	}

	{
		// Save the recorded sample off the network to be given to OnSampleRequested called by the internal MediaStreamSource
		// Obtain lock since the OnSampleRequested could be reading this on another thread
		auto lock = m_lock.LockExclusive();

		//IFC(pBundleImpl->ToMFSample(&spSample));
		IFC(pBundleImpl->ToMFSample(&m_latestSample));

		if (m_deferral != nullptr) {

			ComPtr<IMFMediaStreamSourceSampleRequest> spIMFRequest;
			IFC(m_spRequest.As(&spIMFRequest));

			IFC(spIMFRequest->SetSample(m_latestSample.Get()));

			IFC(m_deferral->Complete());

			m_deferral = nullptr;
			m_spRequest = nullptr;
		}

		//IFC(pStreamImpl->ProcessSample(&sampleHead, (sampleHead.cbCameraDataSize > 0) ? &sampleTransforms : nullptr, spSample.Get()));
		// Set sample attributes
		//IFC(SetSampleAttributes(pSampleHeader, pSampleTransforms, pSample));
	}

done:
	if (FAILED(hr))
	{
		return HandleError(hr);
	}

	return hr;
}

_Use_decl_annotations_
HRESULT StreamingMediaSourceImpl::ProcessMediaFormatChange(
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
			IFC_MSG(MF_E_INVALID_STATE_TRANSITION, L"NetworkMediaSourceImpl::ProcessMediaFormatChange() - not in a state to receive data.");
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
HRESULT StreamingMediaSourceImpl::HandleError(
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
HRESULT StreamingMediaSourceImpl::CompleteAsyncAction(
	HRESULT hResult)
{
	Log(Log_Level_Info, L"StreamingMediaSourceImpl::CompleteAsyncAction()\n");

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