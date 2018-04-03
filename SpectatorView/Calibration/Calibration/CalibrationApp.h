// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include "SpriteBatch.h"
#include "DirectXHelper.h"

// REST
//https://github.com/Microsoft/cpprestsdk/wiki/Getting-Started-Tutorial
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

//https://msdn.microsoft.com/en-us/library/jj950082.aspx
#include <cpprest/json.h>

#include "StringHelper.h"

#include "WICTextureLoader.h"
#include "DirectoryHelper.h"
#include "Shlobj.h" // To get MyDocuments path

#include <iostream>
#include <fstream>

// FrameProviders:
#include "IFrameProvider.h"
#if USE_ELGATO
#include "ElgatoFrameProvider.h"
#endif
#if USE_DECKLINK
#include "DeckLinkManager.h"
#endif
#if USE_OPENCV
#include "OpenCVFrameProvider.h"
#endif

#include "ReadData.h"

#include "opencv2/opencv.hpp"

//TODO: Update with the 3.x version of OpenCV you are using.
#pragma comment(lib, "opencv_world341")

// For REST calls
using namespace web::json;
using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams
using namespace DirectX;

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class CalibrationApp : public DX::IDeviceNotify
{
public:

    CalibrationApp();

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();
    void Render();

    // Rendering helpers
    void Clear();

    // IDeviceNotify
    virtual void OnDeviceLost() override;
    virtual void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const;

private:

    void Update(DX::StepTimer const& timer);

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // REST
    void DeleteAllMRCFiles();
    void DeleteMRCFile(LPTSTR fileName);
    void GetAllMRCFiles(LPTSTR holoPath);
    void GetMRCFile(LPTSTR fileName, LPTSTR outputHoloPath);
    void TakeMRCPicture();

    // Calibration
    void PerformCalibration();
    void TakeCalibrationPicture();
    void TakeCalibrationPictureAtInterval(DX::StepTimer const& timer);
    void DeleteOutputFiles();
    bool HasChessBoard(cv::Mat image, cv::Mat& grayscaleImage, std::vector<cv::Point2f>& corners);

	void Blit(ID3D11ShaderResourceView* source, ID3D11RenderTargetView* dest, ID3D11PixelShader* shader);

    // Device resources.
    std::unique_ptr<DX::DeviceResources> deviceResources;

    // Rendering loop timer.
    DX::StepTimer timer;

    std::unique_ptr<DirectX::SpriteBatch> spriteBatch;

    // Textures
    ID3D11Texture2D* colorTexture;
	ID3D11Texture2D* convertedColorTexture;

	ID3D11ShaderResourceView* srv;
	ID3D11ShaderResourceView* convertedSrv;
	
	ID3D11RenderTargetView* convertedRT;

    cv::Mat latestColorMat;
    cv::Mat cachedColorMat;
    BYTE* colorBytes;
    cv::Mat colorImage_cam;
    cv::Mat resizedColorImage_cam;
    cv::Mat grayscaleImage_cam;
    cv::Mat colorImage_holo;
    cv::Mat grayscaleImage_holo;

    RECT screenRect;
    RECT colorSourceRect;

    // REST
    http_client* httpClient;

    // Calibration
    cv::Size boardDimensions;
    double calibrationPictureElapsedTime;
    CRITICAL_SECTION photoTextureCriticalSection;
    CRITICAL_SECTION commandCriticalSection;
    CRITICAL_SECTION calibrationPictureCriticalSection;

    // Current photo number we are on.
    int photoIndex;
    std::wstring outputPath;
    std::wstring cachedOutputPath;
    std::wstring calibrationFile;

    // Color camera source
    IFrameProvider* frameProvider;

    std::unique_ptr<DirectX::Keyboard> keyboard;
    DirectX::Keyboard::State keyState;
    DirectX::Keyboard::State prevKeyState;

	Microsoft::WRL::ComPtr<ID3D11PixelShader> yuv2rgbPS;

	struct CONVERSION_PARAMETERS
	{
		int width;
		int height;
		byte na[8];
	};

	static_assert(!(sizeof(CONVERSION_PARAMETERS) % 16),
		"CONVERSION_PARAMETERS needs to be 16 bytes aligned");

	CONVERSION_PARAMETERS yuv2rgbParameters;
	Microsoft::WRL::ComPtr<ID3D11Buffer> conversionParamBuffer;
};