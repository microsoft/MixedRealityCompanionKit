// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#include "stdafx.h"

#if USE_ELGATO
// Disable warnings in downloaded Elgato Filter code.
#pragma warning (disable : 4091 )
#endif

#define DLLEXPORT __declspec(dllexport)

#include "IFrameProvider.h"
#include "DeckLinkManager.h"
#include "ElgatoFrameProvider.h"
#include "OpenCVFrameProvider.h"

#include "DirectXHelper.h"

#include "DirectoryHelper.h"
#include "Shlobj.h" // To get MyDocuments path

#include "ScreenGrab.h"
#include <wincodec.h>

#include "VideoEncoder.h"

class CompositorInterface
{
public:
	DLLEXPORT CompositorInterface();
	~CompositorInterface();

	DLLEXPORT bool Initialize(ID3D11Device* device, ID3D11ShaderResourceView* colorSRV, ID3D11Texture2D* outputTexture);
	DLLEXPORT void UpdateFrameProvider();
	DLLEXPORT void Update();
	DLLEXPORT void StopFrameProvider();

	DLLEXPORT LONGLONG GetTimestamp();
	DLLEXPORT LONGLONG GetFrameDelayMS();

	DLLEXPORT bool OutputYUV()
	{
		if (frameProvider == nullptr)
		{
			return true;
		}

		return frameProvider->OutputYUV();
	}

	// Recording
	DLLEXPORT void TakePicture(ID3D11Texture2D* outputTexture);
	DLLEXPORT bool InitializeVideoEncoder(ID3D11Device* device);
	DLLEXPORT void StartRecording();
	DLLEXPORT void StopRecording();
	DLLEXPORT bool IsVideoFrameReady();
	DLLEXPORT void RecordFrameAsync(ID3D11Texture2D* texture);
	DLLEXPORT void RecordAudioFrameAsync(BYTE* audioFrame, LONGLONG frameTime);

private:
	IFrameProvider* frameProvider;
	std::wstring outputPath;
	ID3D11Device* _device;

	LONGLONG stubVideoTime = 0;

	int photoIndex = -1;
	int videoIndex = -1;
	VideoEncoder* videoEncoder = nullptr;
	byte* photoBytes = new byte[FRAME_BUFSIZE];
	byte* videoBytes = new byte[(int)(1.5f * FRAME_WIDTH * FRAME_HEIGHT)];
};

