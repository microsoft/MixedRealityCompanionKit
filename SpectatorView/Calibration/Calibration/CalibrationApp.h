// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
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
#if _DEBUG
#pragma comment(lib, "opencv_world341d")
#endif
#if !_DEBUG
#pragma comment(lib, "opencv_world341")
#endif

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
    ~CalibrationApp();

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
    void ProcessChessBoards(int currentIndex, cv::Mat& colorCameraImage);
    void UpdateChessBoardVisual(std::vector<cv::Point2f>& colorCorners);
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
    std::unique_ptr<DirectX::SpriteBatch> overlaySpriteBatch;
    std::unique_ptr<DirectX::SpriteBatch> textSpriteBatch;
    std::unique_ptr<DirectX::SpriteFont> spriteFont;
    std::wstring captureText;
    std::wstring commandText;
    std::wstring camPhotoTitleText;
    std::wstring holoPhotoTitleText;
    SpriteEffects spriteEffect = SpriteEffects_None;

    // Textures
    ID3D11Texture2D* colorTexture;
    ID3D11Texture2D* convertedColorTexture;
    ID3D11Texture2D* chessBoardTexture;
    ID3D11Texture2D* camPhotoTexture;
    ID3D11Texture2D* holoPhotoTexture;

    ID3D11ShaderResourceView* srv;
    ID3D11ShaderResourceView* convertedSrv;
    ID3D11ShaderResourceView* chessBoardSrv;
    ID3D11ShaderResourceView* camPhotoSrv;
    ID3D11ShaderResourceView* holoPhotoSrv;

    ID3D11RenderTargetView* convertedRT;

    cv::Mat latestColorMat;
    BYTE* colorBytes;
    cv::Mat chessBoardVisualMat;
    cv::Mat camPhotoMat;
    cv::Mat holoPhotoMat;
    cv::Mat greenMat;
    cv::Mat redMat;

    RECT screenRect;
    RECT camPhotoRect;
    RECT holoPhotoRect;
    RECT colorSourceRect;
    RECT holoDimRect;

    // REST
    http_client* httpClient;

    // Calibration
    cv::Size boardDimensions;
    double calibrationPictureElapsedTime;
    CRITICAL_SECTION photoTextureCriticalSection;
    CRITICAL_SECTION commandCriticalSection;
    CRITICAL_SECTION calibrationPictureCriticalSection;
    CRITICAL_SECTION chessBoardVisualCriticalSection;
    CRITICAL_SECTION photoVisualsCriticalSection;

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
    Microsoft::WRL::ComPtr<ID3D11PixelShader> forceOpaquePS;

    // Chessboard points in object space for boards that are visible in both camera and hololens pictures.
    std::vector<std::vector<cv::Point3f>> stereoObjectPoints;
    // Chessboard points in image space for boards that are visible in both camera and hololens pictures.
    std::vector<std::vector<cv::Point2f>> stereoColorImagePoints;
    std::vector<std::vector<cv::Point2f>> stereoHoloImagePoints;
    // Chessboard points in image space for all boards in camera and hololens picture.
    std::vector<std::vector<cv::Point2f>> colorImagePoints;
    std::vector<std::vector<cv::Point2f>> holoImagePoints;
    // Chessboard points in image space for the latest camera and hololens pictures.
    std::vector<cv::Point2f> colorCorners;
    std::vector<cv::Point2f> holoCorners;

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