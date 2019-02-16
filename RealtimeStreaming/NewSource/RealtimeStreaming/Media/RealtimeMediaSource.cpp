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

#include "Network/DataBundle.h"
#include "Network/DataBuffer.h"
#include "Network/DataBundleArgs.h"

#include <intsafe.h>

using namespace winrt;
using namespace RealtimeStreaming::Media::implementation;
using namespace RealtimeStreaming::Common;

using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Media;
using namespace Windows::Media::Core;
using namespace Windows::Media::MediaProperties;
using namespace Windows::Storage::Streams;

#define SET_SAMPLE_ATTRIBUTE(flag, mask, pSample, flagName) \
if ((static_cast<DWORD>(SampleFlags::SampleFlag_##flagName) & mask) == static_cast<DWORD>(SampleFlags::SampleFlag_##flagName)) \
{ \
    IFT(pSample->SetUINT32( \
        MFSampleExtension_##flagName, \
        (static_cast<DWORD>(SampleFlags::SampleFlag_##flagName) & flag) == static_cast<DWORD>(SampleFlags::SampleFlag_##flagName))); \
}

// Initialize an FFmpegInteropObject
_Use_decl_annotations_
RealtimeMediaSource::RealtimeMediaSource()
{

}

_Use_decl_annotations_
RealtimeMediaSource::~RealtimeMediaSource()
{
    //auto lock = _lock.Lock();
    slim_lock_guard guard(m_lock);

    if (m_deferral != nullptr)
    {
        m_deferral.Complete();
        m_deferral = nullptr;
    }

    // Remove event handlers
    if (m_mediaStreamSource != nullptr)
    {
        m_mediaStreamSource.Starting(m_startingRequestedToken);
        m_mediaStreamSource.SampleRequested(m_sampleRequestedToken);
        m_mediaStreamSource.Closed(m_closeRequestedToken);
        m_mediaStreamSource = nullptr;
    }

    if (m_connection != nullptr)
    {
        m_connection.Received(m_evtReceivedToken);
        m_connection = nullptr;
    }

    m_spRequest = nullptr;
    m_spVideoEncoding = nullptr;
}

_Use_decl_annotations_
IAsyncAction RealtimeMediaSource::InitAsync(
    Network::Connection connection)
{
    slim_lock_guard guard(m_lock);
    //auto lock = _lock.Lock();
    //auto lock = m_lock.LockExclusive();

    m_connection = connection;

    // register for message events
    m_evtReceivedToken = m_connection.Received({ this, &RealtimeMediaSource::OnDataReceived });

    // client is connected we need to send the Describe request
    m_eSourceState = SourceStreamState::Opening;
    m_lastTimeStamp = 0;
    m_signal = CreateEvent(nullptr, true, false, nullptr);

    // Send request to our server to send a Media Description with properties of live stream media data
    SendDescribeRequest();

    // Don't return initialization until we receive a describe request
    co_await winrt::resume_on_signal(m_signal.get());
}

_Use_decl_annotations_
MediaStreamSource RealtimeMediaSource::MediaStreamSource()
{
    slim_shared_lock_guard const guard(m_lock);
    //auto lock = _lock.Lock();

    return m_mediaStreamSource;
}

_Use_decl_annotations_
VideoEncodingProperties RealtimeMediaSource::VideoProperties()
{
    slim_shared_lock_guard const guard(m_lock);
    //auto lock = _lock.Lock();

    return m_spVideoEncoding;
}

_Use_decl_annotations_
void RealtimeMediaSource::OnStarting(Windows::Media::Core::MediaStreamSource const& sender,
    MediaStreamSourceStartingEventArgs const& args)
{
    Log(Log_Level_Info, L"RealtimeMediaSource::OnStarting()\n");
    // TODO: revisit nullable stuff
    /*
    MediaStreamSourceStartingRequest request = args.Request;

    IReference<TimeSpan> startPosition = request.StartPosition;

    Windows::Foundation::TimeSpan value;
    if (startPosition != nullptr)
    {
        startPosition.get_Value
      spRequest->SetActualStartPosition(value));
    }
    */
}

_Use_decl_annotations_
void RealtimeMediaSource::OnSampleRequested(Windows::Media::Core::MediaStreamSource const& sender,
    MediaStreamSourceSampleRequestedEventArgs const& args)
{
    Log(Log_Level_Info, L"RealtimeMediaSource::OnSampleRequested()\n");

    slim_lock_guard guard(m_lock);
    //auto lock = _lock.Lock();

    m_spRequest = args.Request;

    if (m_latestSample == nullptr)
    {
        Log(Log_Level_Info, L"RealtimeMediaSourceImpl::OnSampleRequested() - No latest sample, taking deferral \n");

        if (m_deferral != nullptr) {
            Log(Log_Level_Error, L"RealtimeMediaSourceImpl::OnSampleRequested() - Got deferral when another has not completed\n");
        }

        m_deferral = m_spRequest.GetDeferral();
    }
    else if (m_mediaStreamSource != nullptr) // TODO: add else for teardown?
    {
        com_ptr<IMFMediaStreamSourceSampleRequest> spIMFRequest = m_spRequest.as< IMFMediaStreamSourceSampleRequest>();

        IFT(spIMFRequest->SetSample(m_latestSample.get()));

        m_latestSample = nullptr;
        m_spRequest = nullptr;
    }
}

_Use_decl_annotations_
void RealtimeMediaSource::OnClosed(Windows::Media::Core::MediaStreamSource const& sender, MediaStreamSourceClosedEventArgs const& args)
{
    Log(Log_Level_Info, L"RealtimeMediaSource::OnClosed()\n");

    //auto lock = _lock.Lock();

    MediaStreamSourceClosedRequest request = args.Request;

    switch (request.Reason)
    {
    case MediaStreamSourceClosedReason::UnknownError:
        LOG_RESULT(E_UNEXPECTED)
        break;
    case MediaStreamSourceClosedReason::AppReportedError:
        LOG_RESULT(E_ABORT);
        break;
    case MediaStreamSourceClosedReason::UnsupportedProtectionSystem:
    case MediaStreamSourceClosedReason::ProtectionSystemFailure:
        LOG_RESULT(MF_E_DRM_UNSUPPORTED);
        break;
    case MediaStreamSourceClosedReason::UnsupportedEncodingFormat:
        LOG_RESULT(MF_E_UNSUPPORTED_FORMAT);
        break;
    case MediaStreamSourceClosedReason::MissingSampleRequestedEventHandler:
        LOG_RESULT(HRESULT_FROM_WIN32(ERROR_NO_CALLBACK_ACTIVE));
        break;
    }
}

//
// Data Connection
//

// Sending requests to network
_Use_decl_annotations_
void RealtimeMediaSource::SendDescribeRequest()
{
    Log(Log_Level_Info, L"RealtimeMediaSourceImpl::SendDescribeRequest()\n");

    NULL_THROW(m_connection, E_POINTER);

    m_connection.SendPayloadTypeAsync(PayloadType::RequestMediaDescription);
}

_Use_decl_annotations_
void RealtimeMediaSource::SendStartRequest()
{
    Log(Log_Level_Info, L"RealtimeMediaSourceImpl::SendStartRequest()\n");

    NULL_THROW(m_connection, E_POINTER);

    m_connection.SendPayloadTypeAsync(PayloadType::RequestMediaStart);
}

_Use_decl_annotations_
void RealtimeMediaSource::SendStopRequest()
{
    Log(Log_Level_Info, L"RealtimeMediaSourceImpl::SendStopRequest()\n");

    NULL_THROW(m_connection, E_POINTER);

    m_connection.SendPayloadTypeAsync(PayloadType::RequestMediaStop);
}

_Use_decl_annotations_
void RealtimeMediaSource::OnDataReceived(
    Network::Connection const& sender,
    Network::DataBundleArgs const& args)
{
    // TODO: Troy figure out right lock
    //auto lock = m_lock.LockExclusive();

    PayloadType type = args.PayloadType;

    IFT(CheckShutdown());

    Log(Log_Level_Info, L"RealtimeMediaSourceImpl::OnDataReceived(%d)\n", type);

    Network::DataBundle data = args.Bundle;

    switch (type)
    {
    case PayloadType::State_CaptureReady:
        ProcessCaptureReady();
        break;
    case PayloadType::SendMediaDescription:
        IFT(ProcessMediaDescription(args.Bundle));
        break;
    case PayloadType::SendMediaSample:
        IFT(ProcessMediaSample(args.Bundle));
        break;
    case PayloadType::SendFormatChange:
        IFT(ProcessMediaFormatChange(args.Bundle));
        break;
    case PayloadType::SendMediaStreamTick:
        Log(Log_Level_Info, L"RealtimeMediaSourceImpl::OnDataReceived() - PayloadType::SendMediaStreamTick\n");
        // TODO: Troy how to turn on without IMFMediaSource
        //IFC(ProcessMediaTick(spDataBundle.get()));
        break;
    };
}

// Helper methods to handle received network bundles
_Use_decl_annotations_
HRESULT RealtimeMediaSource::ProcessCaptureReady()
{
    slim_lock_guard guard(m_lock);

    Log(Log_Level_Info, L"RealtimeMediaSourceImpl::ProcessCaptureReady()\n");

    if (m_eSourceState == SourceStreamState::Started || m_eSourceState == SourceStreamState::Stopped)
    {
        return;
    }

    SendDescribeRequest();

    return S_OK;
}

_Use_decl_annotations_
HRESULT RealtimeMediaSource::ProcessMediaDescription(
    Network::DataBundle const& dataBundle)
{
    Log(Log_Level_Info, L"RealtimeMediaSourceImpl::ProcessMediaDescription()\n");

    slim_lock_guard guard(m_lock);

    if (m_eSourceState == SourceStreamState::Started 
        || m_eSourceState == SourceStreamState::Stopped)
        //&& _streams.GetCount() > 0)
    {
        return;
    }

    if (m_eSourceState != SourceStreamState::Opening)
    {
        if (m_eSourceState != SourceStreamState::Stopped)
        {
            // TODO: What to do here
            //Shutdown();
        }
        else
        {
            // Server description should only be sent during opening state
            IFT(MF_E_UNEXPECTED);
        }
    }

    DWORD cbTotalLen = dataBundle.TotalSize;
    DWORD cbHeaderSize = 0, StreamTypeHeaderSize = 0;
    MediaDescription desc;
    const DWORD c_cbMediaDescriptionSize = sizeof(MediaDescription);
    const DWORD c_cbMediaTypeDescriptionSize = sizeof(MediaTypeDescription);
    HRESULT hr = S_OK;

    auto dataBundleImpl = dataBundle.as<IDataBundlePriv>();
    MediaDescription desc;
    MediaTypeDescription* pMediaTypeDesc;
    
    BYTE* pPtr = nullptr;

    // Minimum size of the operation payload is size of Description structure
    // + one mediaType description
    if (dataBundle.TotalSize < (c_cbMediaDescriptionSize + c_cbMediaTypeDescriptionSize))
    {
        IFC(MF_E_UNSUPPORTED_FORMAT);
    }

    // Copy description.
    IFC(dataBundleImpl->MoveLeft(c_cbMediaDescriptionSize, &desc));

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

    HRESULT hr = S_OK;;

    // make buffer that will hold the streamTypeHeader data
    pPtr = new (std::nothrow) BYTE[StreamTypeHeaderSize];
    if (pPtr == nullptr)
    {
        IFC(E_OUTOFMEMORY);
    }

    // copy the mediaType header from the bundle
    IFC(dataBundleImpl->MoveLeft(StreamTypeHeaderSize, pPtr));

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


    m_spVideoEncoding =
        CreatePropertiesFromMediaDescription(pMediaTypeDesc[0], // MediaTypeDescription* pStreamDescription,
            dataBundle);

    IMediaStreamDescriptor mediaStreamDescriptor = VideoStreamDescriptor(m_spVideoEncoding);

    // Create internal MediaStreamSource
    m_mediaStreamSource = Windows::Media::Core::MediaStreamSource(mediaStreamDescriptor);

    // Set buffer time to 0 and IsLive true for realtime streaming to reduce latency
    TimeSpan span{ 0 };
    m_mediaStreamSource.BufferTime(span);
    m_mediaStreamSource.IsLive = true;

    m_startingRequestedToken = m_mediaStreamSource.Starting({ this, &RealtimeMediaSource::OnStarting });
    m_sampleRequestedToken = m_mediaStreamSource.SampleRequested({ this, &RealtimeMediaSource::OnSampleRequested });
    m_closeRequestedToken = m_mediaStreamSource.Closed({ this, &RealtimeMediaSource::OnClosed });

    m_eSourceState = SourceStreamState::Starting;

    // Ask the server to start sending media samples to process
    SendStartRequest();

    m_eSourceState = SourceStreamState::Started;

done:
    delete[] pPtr;

    // TODO: signal async method from initasync*
    return hr;
    //return CompleteAsyncAction(hr);
}

_Use_decl_annotations_
VideoEncodingProperties RealtimeMediaSource::CreatePropertiesFromMediaDescription(
    MediaTypeDescription streamDescription,
    Network::DataBundle attributesBuffer)
{
    VideoEncodingProperties videoProps;
    auto attributesBufferImpl = attributesBuffer.as<IDataBundlePriv>();

    // Create Media Type from attributes blob
    com_ptr<IMFMediaType> spMediaType;
    DWORD AttributesBlobSize = attributesBuffer.TotalSize;

    // Create a media type object.
    IFT(MFCreateMediaType(spMediaType.put()));

    if (AttributesBlobSize < streamDescription.AttributesBlobSize
        ||
        streamDescription.AttributesBlobSize == 0)
    {
        // Invalid stream description
        IFT(MF_E_UNSUPPORTED_FORMAT);
    }

    // Prepare buffer where we will copy attributes to
    BYTE* pAttributes = new (std::nothrow) BYTE[streamDescription.AttributesBlobSize];
    if (pAttributes == nullptr)
    {
        IFT(E_OUTOFMEMORY);
    }

    HRESULT hr = S_OK;

    // Move the memory
    IFC(attributesBufferImpl->MoveLeft(streamDescription.AttributesBlobSize, pAttributes));

    // Initialize media type's attributes
    IFC(MFInitAttributesFromBlob(spMediaType.get(), pAttributes, streamDescription.AttributesBlobSize));
    //IFC(ValidateInputMediaType(pStreamDescription->guiMajorType, pStreamDescription->guiSubType, spMediaType.get()));
    IFC(spMediaType->SetGUID(MF_MT_MAJOR_TYPE, streamDescription.guiMajorType));
    IFC(spMediaType->SetGUID(MF_MT_SUBTYPE, streamDescription.guiSubType));


    // TROY: TODO. Figure out CPP winrt for this?
    IFC(MFCreatePropertiesFromMediaType(spMediaType.get(), IID_PPV_ARGS(&winrt::get_unknown(videoProps))));

    /*
    TODO: See if I can return the streamdescriptor instead of IVideoEncodingProperties

    com_ptr<IMFStreamDescriptor> spSD;
    com_ptr<IMFMediaTypeHandler> spMediaTypeHandler;

    // Now we can create MF stream descriptor.
    IFC(MFCreateStreamDescriptor(pStreamDescription->dwStreamId, 1, spMediaType.GetAddressOf(), &spSD));
    IFC(spSD->GetMediaTypeHandler(&spMediaTypeHandler));

    // Set current media type
    IFC(spMediaTypeHandler->SetCurrentMediaType(spMediaType.get()));
    */
done:
    delete[] pAttributes;

    IFT(hr);

    return videoProps;
}

_Use_decl_annotations_
HRESULT RealtimeMediaSource::ProcessMediaSample(
    Network::DataBundle const& dataBundle)
{
    slim_lock_guard guard(m_lock);

    auto dataBundleImpl = dataBundle.as<IDataBundlePriv>();

    Log(Log_Level_Info, L"RealtimeMediaSourceImpl::ProcessMediaSample()\n");

    com_ptr<IMFSample> spSample;

    // Only process samples when we are in started state
    if (m_eSourceState != SourceStreamState::Started && m_eSourceState != SourceStreamState::Shutdown)
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

    MediaSampleHeader sampleHead = {};

    // Copy the header object
    IFR(dataBundleImpl->MoveLeft(sizeof(MediaSampleHeader), &sampleHead));

    {
        //auto lock = _lock.Lock();

        Log(Log_Level_Info, L"RealtimeMediaSourceImpl::ProcessMediaSample() - CurrentTS: %d - Last TS:%d \n", sampleHead.hnsTimestamp, m_lastTimeStamp);

        // Drop sample if it's timestamp is in the past of what we have processed
        if (m_lastTimeStamp > sampleHead.hnsTimestamp)
        {
            // TODO: Complete defferal????
            return S_OK;
        }

        m_lastTimeStamp = sampleHead.hnsTimestamp;
    }

    // Convert bundle data into IMFSample
    IFR(dataBundleImpl->ToMFSample(spSample.put()));

    // Set appropriate attributes of sample from header data
    IFR(SetSampleAttributes(sampleHead, spSample.get()));

    {
        // Save the recorded sample off the network to be given to OnSampleRequested called by the internal MediaStreamSource
        // Obtain lock since the OnSampleRequested could be reading this on another thread
        //auto lock = _lock.Lock();

        m_latestSample = spSample;

        if (m_deferral != nullptr)
        {
            Log(Log_Level_Info, L"RealtimeMediaSourceImpl::ProcessMediaSample - Fire Deferral()\n");

            com_ptr<IMFMediaStreamSourceSampleRequest> spIMFRequest = m_spRequest.as< IMFMediaStreamSourceSampleRequest>();

            IFR(spIMFRequest->SetSample(m_latestSample.get()));

            m_deferral.Complete();

            m_deferral = nullptr;
            m_spRequest = nullptr;
            m_latestSample = nullptr;
        }
    }

    return S_OK;
}

_Use_decl_annotations_
HRESULT RealtimeMediaSource::SetSampleAttributes(
    MediaSampleHeader const& sampleHeader,
    IMFSample* pSample)
{
    // TODO: Confirm setsampletime 0 is good for live streaming with MediaStreamSource
    //IFR_MSG(pSample->SetSampleTime(pSampleHeader->hnsTimestamp), L"setting sample time");
    IFR_MSG(pSample->SetSampleTime(0), L"setting sample time");
    IFR_MSG(pSample->SetSampleDuration(sampleHeader.hnsDuration), L"setting sample duration");

    SET_SAMPLE_ATTRIBUTE(sampleHeader.dwFlags, sampleHeader.dwFlagMasks, pSample, BottomFieldFirst);
    SET_SAMPLE_ATTRIBUTE(sampleHeader.dwFlags, sampleHeader.dwFlagMasks, pSample, CleanPoint);
    SET_SAMPLE_ATTRIBUTE(sampleHeader.dwFlags, sampleHeader.dwFlagMasks, pSample, DerivedFromTopField);
    SET_SAMPLE_ATTRIBUTE(sampleHeader.dwFlags, sampleHeader.dwFlagMasks, pSample, Discontinuity);
    SET_SAMPLE_ATTRIBUTE(sampleHeader.dwFlags, sampleHeader.dwFlagMasks, pSample, Interlaced);
    SET_SAMPLE_ATTRIBUTE(sampleHeader.dwFlags, sampleHeader.dwFlagMasks, pSample, RepeatFirstField);
    SET_SAMPLE_ATTRIBUTE(sampleHeader.dwFlags, sampleHeader.dwFlagMasks, pSample, SingleField);

    return S_OK;
}

_Use_decl_annotations_
HRESULT RealtimeMediaSource::ProcessMediaFormatChange(
    Network::DataBundle const& dataBundle)
{
    com_ptr<IMFMediaType> spMediaType;

    auto dataBundleImpl = dataBundle.as<IDataBundlePriv>();

    if (dataBundle.BufferCount == 0)
    {
        // investigate why
        return S_OK;
    }

    if (m_eSourceState != SourceStreamState::Started && m_eSourceState != SourceStreamState::Shutdown)
    {
        if (FAILED(ProcessCaptureReady()))
        {
            IFR_MSG(MF_E_INVALID_STATE_TRANSITION, L"RealtimeMediaSourceImpl::ProcessMediaFormatChange() - not in a state to receive data.");
        }
    }

    DWORD cbTotalLen = dataBundle.TotalSize;

    // Minimum size of the operation payload is size of Description structure
    DWORD cbTypeDescSize = sizeof(MediaTypeDescription);
    if (cbTotalLen < sizeof(MediaTypeDescription))
    {
        IFR(MF_E_UNSUPPORTED_FORMAT);
    }

    MediaTypeDescription streamDesc;
    IFR(dataBundleImpl->MoveLeft(cbTypeDescSize, &streamDesc));

    if (cbTotalLen != cbTypeDescSize + streamDesc.AttributesBlobSize
        ||
        streamDesc.AttributesBlobSize == 0)
    {
        IFR(MF_E_UNSUPPORTED_FORMAT);
    }

    {
        // Prepare buffer where we will copy attributes to
        std::vector<byte> blob(streamDesc.AttributesBlobSize);
        IFR(dataBundleImpl->MoveLeft(streamDesc.AttributesBlobSize, blob.data()));

        // Create a media type object.
        IFR(MFCreateMediaType(spMediaType.put));

        // Initialize media type's attributes
        IFR(MFInitAttributesFromBlob(spMediaType.get(), blob.data(), streamDesc.AttributesBlobSize));
    }

    return S_OK;
}
