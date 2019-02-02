// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "CaptureEngine.h"

ActivatableStaticOnlyFactory(CaptureEngineStaticsImpl);

typedef IAsyncOperationCompletedHandler<HSTRING*> IDeviceInformationOperationCompletedHandler;

inline HRESULT FindFrameRateRangeFromMediaProperties(
    _In_ IVectorView<ABI::Windows::Media::MediaProperties::IMediaEncodingProperties*>* propertiesList,
    _Out_ UINT32* puMin,
    _Out_ UINT32* puMax) throw()
{
    NULL_CHK(propertiesList);
    NULL_CHK(puMin);
    NULL_CHK(puMax);

    UINT32 uSize = 0;
    IFR(propertiesList->get_Size(&uSize));
    if (uSize > 0)
    {
        UINT32 uMax = 0;
        for (UINT32 i = 0; i < uSize; ++i)
        {
            ComPtr<IMediaEncodingProperties> spProperties;
            IFR(propertiesList->GetAt(i, &spProperties));

            // Must be video properties
            ComPtr<IVideoEncodingProperties> spVideoProperties;
            IFR(spProperties.As(&spVideoProperties));

            ComPtr<IMediaRatio> spFrameRate;
            IFR(spVideoProperties->get_FrameRate(&spFrameRate));

            UINT32 uFrameRateNum = 0;
            IFR(spFrameRate->get_Numerator(&uFrameRateNum));
            UINT32 uFrameRateDenom = 0;
            IFR(spFrameRate->get_Denominator(&uFrameRateDenom));
            if (uFrameRateDenom > 0)
            {
                uFrameRateNum /= uFrameRateDenom;
            }

            if (uFrameRateNum > uMax)
            {
                uMax = uFrameRateNum;
            }
        }

       * puMin = 1;
       * puMax = uMax;
    }

    return S_OK;
}

CaptureEngineImpl::CaptureEngineImpl()
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

CaptureEngineImpl::~CaptureEngineImpl()
{
    Uninitialize();
}

HRESULT CaptureEngineImpl::RuntimeClassInitialize()
{
    return S_OK;
}

// IModule
_Use_decl_annotations_
HRESULT CaptureEngineImpl::get_IsInitialized(
    _Out_ boolean *initialized)
{
    Log(Log_Level_Info, L"CaptureEngineImpl::get_IsInitialized()\n");

    NULL_CHK(initialized);

    auto lock = _lock.Lock();

    *initialized = _isInitialized;

    return S_OK;
};

_Use_decl_annotations_
HRESULT CaptureEngineImpl::Uninitialize(void)
{
    Log(Log_Level_Info, L"CaptureEngineImpl::Uninitialize()\n");

    auto lock = _lock.Lock();

    if (!_isInitialized)
    {
        return S_OK;
    }

    _isInitialized = false;

    return Shutdown();
}

_Use_decl_annotations_
HRESULT CaptureEngineImpl::Init(boolean enableAudio, IConnection *connection)
{
    Log(Log_Level_Info, L"CaptureEngineImpl::Init()\n");
	NULL_CHK(connection);

	_enableAudio = enableAudio;

    ComPtr<ICaptureEngine> spThis(this);
    ComPtr<IConnection> spConnection(connection);

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

    // create the custome sink
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

	ComPtr<IMFMediaType>  spMediaTypeOut;
	ComPtr<IMFMediaType>  spMediaTypeIn;

	IFR(MFCreateMediaTypeFromProperties(videoEncodingProperties.Get(), &spMediaTypeOut));

	// Set the output media type.
	/*
	IFR(MFCreateMediaType(&spMediaTypeOut));
	IFR(spMediaTypeOut->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video));
	IFR(spMediaTypeOut->SetGUID(MF_MT_SUBTYPE, VIDEO_ENCODING_FORMAT));
	IFR(spMediaTypeOut->SetUINT32(MF_MT_AVG_BITRATE, VIDEO_BIT_RATE));
	IFR(spMediaTypeOut->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive));
	IFR(MFSetAttributeSize(spMediaTypeOut, MF_MT_FRAME_SIZE, VIDEO_WIDTH, VIDEO_HEIGHT));
	IFR(MFSetAttributeRatio(spMediaTypeOut, MF_MT_FRAME_RATE, VIDEO_FPS, 1));
	IFR(MFSetAttributeRatio(spMediaTypeOut, MF_MT_PIXEL_ASPECT_RATIO, 1, 1));
	*/
	IFR(spSinkWriter->AddStream(spMediaTypeOut.Get(), &_sinkWriterStream));
		

	// Set the input media type.
	UINT32 videoWidth, videoHeight;
	IFR(GetVideoResolution(mediaEncodingProfile.Get(), &videoWidth, &videoHeight));

	IFR(MFCreateMediaType(&spMediaTypeIn));
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

_Use_decl_annotations_
HRESULT CaptureEngineImpl::Shutdown()
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


_Use_decl_annotations_
HRESULT CaptureEngineImpl::WriteFrame()
	//const LONGLONG& rtStart        // Time stamp.
{

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

	ComPtr<IMFSample> spSample;
	ComPtr<IMFMediaBuffer> spBuffer;

	UINT32 videoWidth, videoHeight;
	IFR(GetVideoResolution(_spMediaEncodingProfile.Get(), &videoWidth, &videoHeight));

	const LONG cbWidth = 4 * videoWidth;
	const DWORD cbBuffer = cbWidth * videoHeight;

	BYTE *pData = NULL;

	// Create a new memory buffer.
	IFR(MFCreateMemoryBuffer(cbBuffer, &spBuffer));

	// Lock the buffer and copy the video frame to the buffer.
	IFR(spBuffer->Lock(&pData, NULL, NULL));

	IFR(MFCopyImage(
		pData,                      // Destination buffer.
		cbWidth,                    // Destination stride.
		(BYTE*)_videoFrameBuffer,    // First row in source image.
		cbWidth,                    // Source stride.
		cbWidth,                    // Image width in bytes.
		videoHeight                 // Image height in pixels.
	));

	if (spBuffer)
	{
		spBuffer->Unlock();
	}

	// Set the data length of the buffer.
	IFR(spBuffer->SetCurrentLength(cbBuffer));

	// Create a media sample and add the buffer to the sample.
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

/*
// Factory method
_Use_decl_annotations_
HRESULT CaptureEngineStaticsImpl::Create(ICaptureEngine** ppCaptureEngine)
{
	//NULL_CHK(ppCaptureEngine);
	return MakeAndInitialize<CaptureEngineImpl>(ppCaptureEngine);

	/*
    // create capture and kick off the init
    ComPtr<ICaptureEngine> spCaptureEngine;
    IFR(MakeAndInitialize<CaptureEngineImpl>(&spCaptureEngine));

	engine = &spCaptureEngine;

    //ComPtr<IAsyncAction> spInitAsync;
	//IFR(spCaptureEngine->Init(enableAudio)
    //IFR(spCaptureEngine->InitAsync(enableAudio, &spInitAsync));
	return S_OK;
	
}*/
