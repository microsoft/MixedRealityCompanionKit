// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "stdafx.h"
#include "OpenCVFrameProvider.h"

#if USE_OPENCV

OpenCVFrameProvider::OpenCVFrameProvider()
{
}


OpenCVFrameProvider::~OpenCVFrameProvider()
{
	DeleteCriticalSection(&lock);
}

HRESULT OpenCVFrameProvider::Initialize(ID3D11ShaderResourceView* colorSRV, ID3D11Texture2D* outputTexture)
{
	if (IsEnabled())
	{
		return S_OK;
	}

	InitializeCriticalSection(&lock);
	InitializeCriticalSection(&frameAccessCriticalSection);

	_colorSRV = colorSRV;
	if (colorSRV != nullptr)
	{
		colorSRV->GetDevice(&_device);
	}

	HRESULT hr = E_PENDING;
	videoCapture = new cv::VideoCapture(CAMERA_ID);

	// Attempt to update camera resolution to desired resolution.
	// Note: This may fail, and your capture will resume at the camera's native resolution.
	// In this case, the Update loop will print an error with the expected frame resolution.
	videoCapture->set(cv::CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	videoCapture->set(cv::CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);

	videoCapture->open(CAMERA_ID);
	if (IsEnabled())
	{
		hr = S_OK;
	}

	return hr;
}

void OpenCVFrameProvider::Update()
{
	if (!IsEnabled() ||
		_colorSRV == nullptr ||
		_device == nullptr)
	{
		return;
	}

	// Get frame time.
	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);

	if (videoCapture->grab())
	{
		cachedTimestamp = time.QuadPart;

		concurrency::create_task([=]
		{
			cv::Mat frame;
			videoCapture->retrieve(frame);

			double width = videoCapture->get(cv::CAP_PROP_FRAME_WIDTH);
			double height = videoCapture->get(cv::CAP_PROP_FRAME_HEIGHT);

			if (width != FRAME_WIDTH)
			{
				OutputDebugString(L"ERROR: captured width does not equal FRAME_WIDTH.  Expecting: ");
				OutputDebugString(std::to_wstring(width).c_str());
				OutputDebugString(L"\n");
			}

			if (height != FRAME_HEIGHT)
			{
				OutputDebugString(L"ERROR: captured height does not equal FRAME_HEIGHT.  Expecting: ");
				OutputDebugString(std::to_wstring(height).c_str());
				OutputDebugString(L"\n");
			}

			// OpenCV returns frames in RGB, convert to BGRA
			DirectXHelper::ConvertRGBtoBGRA(frame.data, tmpData, width, height, true);
			EnterCriticalSection(&lock);
			dirtyFrame = false;

			memcpy(cachedFrame, tmpData, FRAME_BUFSIZE);

			LeaveCriticalSection(&lock);
		});

		if (_device != nullptr && _colorSRV != nullptr)
		{
			EnterCriticalSection(&lock);
			if (!dirtyFrame)
			{
				DirectXHelper::UpdateSRV(_device, _colorSRV, cachedFrame, FRAME_WIDTH * FRAME_BPP);
				dirtyFrame = true;
			}
			LeaveCriticalSection(&lock);

			EnterCriticalSection(&frameAccessCriticalSection);
			isVideoFrameReady = true;
			LeaveCriticalSection(&frameAccessCriticalSection);
		}
	}
}

bool OpenCVFrameProvider::IsVideoFrameReady()
{
	EnterCriticalSection(&frameAccessCriticalSection);
	bool ret = isVideoFrameReady;
	if (isVideoFrameReady)
	{
		isVideoFrameReady = false;
	}
	LeaveCriticalSection(&frameAccessCriticalSection);

	return ret;
}

LONGLONG OpenCVFrameProvider::GetTimestamp()
{
	return cachedTimestamp;
}

LONGLONG OpenCVFrameProvider::GetDurationHNS()
{
	return (1.0f / 60.0f) * QPC_MULTIPLIER;
}

bool OpenCVFrameProvider::IsEnabled()
{
	if (videoCapture != nullptr)
	{
		return videoCapture->isOpened();
	}

	return false;
}

void OpenCVFrameProvider::Dispose()
{
	if (videoCapture != nullptr)
	{
		videoCapture->release();
		videoCapture = nullptr;
	}
}

#endif
