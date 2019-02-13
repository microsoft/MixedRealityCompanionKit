// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "RealtimeServer.h"

namespace winrt;
namespace RealtimeStreaming::Media::implementation;

RealtimeServer::RealtimeServer(
    Connection connection,
    GUID inputMediaType,
    MediaEncodingProfile mediaEncodingProperties)
    : m_spMediaEncodingProfile(mediaEncodingProperties)
{
    Log(Log_Level_Info, L"RealtimeServerImpl::RuntimeClassInitialize()\n");

    // TODO: Currently hardcoded to not accept audio
    m_spMediaEncodingProfile.Audio = nullptr;
    m_spMediaEncodingProfile.Container = nullptr;

    // create the custom network sink
    m_spNetworkMediaSink = NetworkMediaSink(m_spMediaEncodingProfile.Audio(),
        m_spMediaEncodingProfile.Video(),
        connection);

    // Create sink writer which will write to our custom network sink
    com_ptr<IMFSinkWriter> spSinkWriter;

    // TODO: https://docs.microsoft.com/en-us/windows/desktop/medfound/sink-writer-attributes
    // Add sink writer attributes*

    IFR(MFCreateSinkWriterFromMediaSink(m_spNetworkMediaSink,
        nullptr,
        &spSinkWriter));

    // Set the output media type.
    com_ptr<IMFMediaType>  spMediaTypeOut;
    IFR(MFCreateMediaTypeFromProperties(videoEncodingProperties.get(), &spMediaTypeOut));
    IFR(spSinkWriter->AddStream(spMediaTypeOut.get(), &m_sinkWriterStream));

    // Set the input media type.
    com_ptr<IMFMediaType>  spMediaTypeIn;
    IFR(MFCreateMediaType(&spMediaTypeIn));

    UINT32 videoWidth, videoHeight;
    IFR(GetVideoResolution(mediaEncodingProfile.get(), &videoWidth, &videoHeight));

    IFR(spMediaTypeIn->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video));
    IFR(spMediaTypeIn->SetGUID(MF_MT_SUBTYPE, inputMediaType));
    IFR(spMediaTypeIn->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive));
    IFR(MFSetAttributeSize(spMediaTypeIn.get(), MF_MT_FRAME_SIZE, videoWidth, videoHeight));
    IFR(MFSetAttributeRatio(spMediaTypeIn.get(), MF_MT_FRAME_RATE, VIDEO_FPS, 1));
    IFR(MFSetAttributeRatio(spMediaTypeIn.get(), MF_MT_PIXEL_ASPECT_RATIO, 1, 1));

    IFR(spSinkWriter->SetInputMediaType(m_sinkWriterStream, spMediaTypeIn.get(), NULL));

    // Tell the sink writer to start accepting data.
    IFR(spSinkWriter->BeginWriting());

    m_mediaInputFormat = inputMediaType;

    m_spSinkWriter = spSinkWriter;

    return S_OK;
}

RealtimeServer::~RealtimeServer()
{
    Uninitialize();
}


_Use_decl_annotations_
void RealtimeServer::Shutdown()
{
    auto lock = _lock.Lock();

    if (nullptr == m_spSinkWriter)
    {
        return S_OK;
    }

    IFT(m_spSinkWriter->Finalize()); // Close sinkwriter

    m_spSinkWriter = nullptr; // release sinkwriter
}

_Use_decl_annotations_
void RealtimeServer::WriteFrame(
    UINT32 bufferSize,
    BYTE* pBuffer)
{
    HRESULT hr = S_OK;

    IFC(NULL_CHK(pBuffer));

    // Create MediaBuffer
    com_ptr<IMFMediaBuffer> spBuffer;

    auto videoProps = m_spMediaEncodingProfile.Video;

    IFC(CreateIMFMediaBuffer(videoProps.Width,
        videoProps.Height,
        bufferSize,
        pBuffer,
        &spBuffer));

    // Create a media sample and add the buffer to the sample.
    com_ptr<IMFSample> spSample;
    IFC(MFCreateSample(&spSample));

    IFC(spSample->AddBuffer(spBuffer.get()));

    // Set the time stamp and the duration.
    IFC(spSample->SetSampleTime(rtStart));

    IFC(spSample->SetSampleDuration(VIDEO_FRAME_DURATION));

    // Send the sample to the Sink Writer.
    IFC(m_spSinkWriter->WriteSample(m_sinkWriterStream, spSample.get()));

    rtStart += VIDEO_FRAME_DURATION;

done:
    IFT(hr);
}

_Use_decl_annotations_
VideoEncodingProperties RealtimeServer::VideoProperties()
{
    return m_videoProperties;
}