// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "RealtimeServer.h"

ActivatableStaticOnlyFactory(RealtimeServerStaticsImpl);

typedef IAsyncOperationCompletedHandler<HSTRING*> IDeviceInformationOperationCompletedHandler;

RealtimeServerImpl::RealtimeServerImpl()
    : _isInitialized(true)
    , _enableAudio(false)
    , _captureStarted(false)
{
	
	// TODO: Remove hardcoded
	for (DWORD i = 0; i < VIDEO_PELS; ++i)
	{
		_videoFrameBuffer[i] = 0x0000FF00;
	}
}

RealtimeServerImpl::~RealtimeServerImpl()
{
    Uninitialize();
}

// TODO: need to pass in IMediaEncodingProfile instead of hardcoded
HRESULT RealtimeServerImpl::RuntimeClassInitialize(
	 IConnection* pConnection)
{
	 Log(Log_Level_Info, L"RealtimeServerImpl::RuntimeClassInitialize()\n");
	 NULL_CHK(pConnection);

	 // TODO: Enable audio in future version
	 _enableAudio = false;

	 ComPtr<IConnection> spConnection(pConnection);

	 // encoding profile activation
	 ComPtr<IMediaEncodingProfileStatics> spEncodingProfileStatics;
	 IFR(Windows::Foundation::GetActivationFactory(
		  Wrappers::HStringReference(RuntimeClass_Windows_Media_MediaProperties_MediaEncodingProfile).Get(),
		  &spEncodingProfileStatics));

	 ComPtr<IMediaEncodingProfile> mediaEncodingProfile;
	 IFR(spEncodingProfileStatics->CreateMp4(
		  ABI::Windows::Media::MediaProperties::VideoEncodingQuality_HD720p,
		  &mediaEncodingProfile));

	 // TODO: CreateHevc(ABI::Windows::Media::MediaProperties::VideoEncodingQuality_HD720p)

	  // remove unwanted parts of the profile
	 if (!_enableAudio)
	 {
		  mediaEncodingProfile->put_Audio(nullptr);
	 }
	 mediaEncodingProfile->put_Container(nullptr);

	 // get the audio/video profiles
	 ComPtr<IAudioEncodingProperties> audioEncodingProperties;
	 IFR(mediaEncodingProfile->get_Audio(&audioEncodingProperties));

	 ComPtr<IVideoEncodingProperties> videoEncodingProperties;
	 IFR(mediaEncodingProfile->get_Video(&videoEncodingProperties));

	 // create the custom network sink
	 ComPtr<NetworkMediaSinkImpl> networkSink;
	 IFR(Microsoft::WRL::Details::MakeAndInitialize<NetworkMediaSinkImpl>(
		  &networkSink,
		  audioEncodingProperties.Get(),
		  videoEncodingProperties.Get(),
		  spConnection.Get()));

	 // Create sink writer which will write to our custom network sink
	 ComPtr<IMFSinkWriter> spSinkWriter;

	 // TODO: https://docs.microsoft.com/en-us/windows/desktop/medfound/sink-writer-attributes
	 // Add sink writer attributes*

	 IFR(MFCreateSinkWriterFromMediaSink(networkSink.Get(), nullptr, &spSinkWriter));

	 // Set the output media type.
	 ComPtr<IMFMediaType>  spMediaTypeOut;
	 IFR(MFCreateMediaTypeFromProperties(videoEncodingProperties.Get(), &spMediaTypeOut));
	 IFR(spSinkWriter->AddStream(spMediaTypeOut.Get(), &_sinkWriterStream));

	 // Set the input media type.
	 ComPtr<IMFMediaType>  spMediaTypeIn;
	 IFR(MFCreateMediaType(&spMediaTypeIn));

	 UINT32 videoWidth, videoHeight;
	 IFR(GetVideoResolution(mediaEncodingProfile.Get(), &videoWidth, &videoHeight));

	 IFR(spMediaTypeIn->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video));
	 IFR(spMediaTypeIn->SetGUID(MF_MT_SUBTYPE, VIDEO_INPUT_FORMAT));
	 IFR(spMediaTypeIn->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive));
	 IFR(MFSetAttributeSize(spMediaTypeIn.Get(), MF_MT_FRAME_SIZE, videoWidth, videoHeight));
	 IFR(MFSetAttributeRatio(spMediaTypeIn.Get(), MF_MT_FRAME_RATE, VIDEO_FPS, 1));
	 IFR(MFSetAttributeRatio(spMediaTypeIn.Get(), MF_MT_PIXEL_ASPECT_RATIO, 1, 1));

	 IFR(spSinkWriter->SetInputMediaType(_sinkWriterStream, spMediaTypeIn.Get(), NULL));

	 // Tell the sink writer to start accepting data.
	 IFR(spSinkWriter->BeginWriting());

	 _captureStarted = true;
	 _spMediaEncodingProfile = mediaEncodingProfile;
	 _spNetworkMediaSink = networkSink;
	 _spSinkWriter = spSinkWriter;

	 return S_OK;
}

// IModule
_Use_decl_annotations_
HRESULT RealtimeServerImpl::get_IsInitialized(
    _Out_ boolean *initialized)
{
    Log(Log_Level_Info, L"RealtimeServerImpl::get_IsInitialized()\n");

    NULL_CHK(initialized);

    auto lock = _lock.Lock();

    *initialized = _isInitialized;

    return S_OK;
};

_Use_decl_annotations_
HRESULT RealtimeServerImpl::Uninitialize(void)
{
    Log(Log_Level_Info, L"RealtimeServerImpl::Uninitialize()\n");

    auto lock = _lock.Lock();

    if (!_isInitialized)
    {
        return S_OK;
    }

    _isInitialized = false;

    return Shutdown();
}

_Use_decl_annotations_
HRESULT RealtimeServerImpl::Shutdown()
{
    if (!_captureStarted)
    {
        return S_OK;
    }

	auto lock = _lock.Lock();

	if (nullptr == _spSinkWriter)
	{
		return S_OK;
	}

	_captureStarted = false;

	IFR(_spSinkWriter->Finalize()); // Close sinkwriter

	_spSinkWriter = nullptr; // release sinkwriter

	// TODO: call _evtClosed???

	return S_OK;
}

/* 
	// TODO: Generate random color whole image*
	// Generate random image
	std::random_device rd;
	std::uniform_int_distribution<int> dist(0, 255);

	DWORD imgColor = (static_cast<BYTE>(dist(rd) & 0xFF) << 16)
		+ (static_cast<BYTE>(dist(rd) & 0xFF) << 8)
		+ (static_cast<BYTE>(dist(rd) & 0xFF));

	//std::fill(_videoFrameBuffer, _videoFrameBuffer + VIDEO_PELS, imgColor);

	for (int i = 0; i < VIDEO_PELS; i++)
	{
		_videoFrameBuffer[i] = imgColor;
	}

*/

_Use_decl_annotations_
HRESULT RealtimeServerImpl::WriteFrame(
	IMFMediaBuffer* pMediaBuffer)
{
	ComPtr<IMFMediaBuffer> spBuffer(pMediaBuffer);

	// Create a media sample and add the buffer to the sample.
	ComPtr<IMFSample> spSample;
	IFR(MFCreateSample(&spSample));

	IFR(spSample->AddBuffer(spBuffer.Get()));

	// Set the time stamp and the duration.
	IFR(spSample->SetSampleTime(rtStart));

	IFR(spSample->SetSampleDuration(VIDEO_FRAME_DURATION));

	// Send the sample to the Sink Writer.
	IFR(_spSinkWriter->WriteSample(_sinkWriterStream, spSample.Get()));

	rtStart += VIDEO_FRAME_DURATION;

	return S_OK;
}

_Use_decl_annotations_
HRESULT RealtimeServerImpl::GetCurrentResolution(
	UINT32* pWidth,
	UINT32* pHeight)
{
	return GetVideoResolution(_spMediaEncodingProfile.Get(), pWidth, pHeight);
}