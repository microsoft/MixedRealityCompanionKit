// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "stdafx.h"
#include "CalibrationApp.h"

//Get color feed from Hololens using the REST APIs:
//https://developer.microsoft.com/en-us/windows/holographic/device_portal_api_reference
//https://github.com/Microsoft/cpprestsdk/wiki/Getting-Started-Tutorial

CalibrationApp::CalibrationApp() :
    colorTexture(nullptr),
    calibrationPictureElapsedTime(0),
    photoIndex(0)
{
    deviceResources = std::make_unique<DX::DeviceResources>();
    deviceResources->RegisterDeviceNotify(this);

    InitializeCriticalSection(&photoTextureCriticalSection);
    InitializeCriticalSection(&commandCriticalSection);
    InitializeCriticalSection(&calibrationPictureCriticalSection);

    boardDimensions = cv::Size(GRID_CELLS_X - 1, GRID_CELLS_Y - 1);
    colorBytes = new BYTE[FRAME_BUFSIZE];

    // Force 60fps
    timer.SetFixedTimeStep(true);
    timer.SetTargetElapsedSeconds(1.0 / 60);

    wchar_t myDocumentsPath[1024];
    SHGetFolderPathW(0, CSIDL_MYDOCUMENTS, 0, 0, myDocumentsPath);
    outputPath = std::wstring(myDocumentsPath) + L"\\CalibrationFiles\\";
    cachedOutputPath = std::wstring(myDocumentsPath) + L"\\OldCalibrationFiles\\";

    calibrationFile = outputPath + L"CalibrationData.txt";

    DirectoryHelper::CreateOutputDirectory(outputPath);
    DirectoryHelper::CreateOutputDirectory(cachedOutputPath);

    // Move a non-empty output directory to the cached output directory.
    if (!DirectoryHelper::IsDirectoryEmpty(outputPath))
    {
        time_t currentTime = time(0);
        struct tm* t = localtime(&currentTime);

        // Create a archive directory with the current date and time for the existing calibration data.
        std::wstring archiveDirectory = cachedOutputPath + L"\\" +
            std::to_wstring(t->tm_mon + 1) + L"_" +
            std::to_wstring(t->tm_mday) + L"_" +
            std::to_wstring(t->tm_year + 1900) + L"_" +
            std::to_wstring(t->tm_hour) + L"_" +
            std::to_wstring(t->tm_min) + L"_" +
            std::to_wstring(t->tm_sec) + L"\\";

        DirectoryHelper::CreateOutputDirectory(archiveDirectory);
        DirectoryHelper::MoveFiles(outputPath, archiveDirectory);
    }
}

// Initialize the Direct3D resources required to run.
void CalibrationApp::Initialize(HWND window, int width, int height)
{
    deviceResources->SetWindow(window, width, height);

    keyboard = std::make_unique<Keyboard>();

    deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    // Calibration Images.
    colorImage_cam = cv::Mat(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC4);
    resizedColorImage_cam = cv::Mat(HOLO_HEIGHT, HOLO_WIDTH, CV_8UC4);
    colorImage_holo = cv::Mat(HOLO_HEIGHT, HOLO_WIDTH, CV_8UC4);
    latestColorMat = cv::Mat(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC4);
    cachedColorMat = cv::Mat(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC4);

    // Create an http_client to use REST APIs on the Hololens.
    http_client_config client_config;
    web::credentials cred = web::credentials(HOLOLENS_USER, HOLOLENS_PW);
    client_config.set_credentials(cred);
    client_config.set_validate_certificates(false);
    httpClient = new http_client(HOLOLENS_ADDRESS, client_config);

    // Start the application with no MRC captures on the Hololens.
    DeleteAllMRCFiles();

    colorTexture = DirectXHelper::CreateTexture(
        deviceResources->GetD3DDevice(), colorBytes,
        FRAME_WIDTH, FRAME_HEIGHT, FRAME_BPP,
        DXGI_FORMAT_R8G8B8A8_UNORM);
    srv = DirectXHelper::CreateShaderResourceView(deviceResources->GetD3DDevice(), colorTexture);

#if USE_ELGATO
    frameProvider = new ElgatoFrameProvider(true);
#endif
#if USE_DECKLINK || USE_DECKLINK_SHUTTLE
    frameProvider = new DeckLinkManager(true, true);
#endif
#if USE_OPENCV
    frameProvider = new OpenCVFrameProvider(false);
#endif

    frameProvider->Initialize(srv, nullptr);
}

// Executes the basic game loop.
void CalibrationApp::Tick()
{
    timer.Tick([&]()
    {
        Update(timer);
    });

    Render();
}

// Updates the world.
void CalibrationApp::Update(DX::StepTimer const& timer)
{
    prevKeyState = keyState;
    keyState = keyboard->GetState();

    if (srv != nullptr && frameProvider != nullptr && !frameProvider->IsEnabled())
    {
        frameProvider->Initialize(srv, nullptr);
    }
    else
    {
        frameProvider->Update();
    }

    // Take calibration pictures at a predetermined interval.
    TakeCalibrationPictureAtInterval(timer);

    // Clear the calibration files that have been generated.
    if (keyState.X && !prevKeyState.X)
    {
        EnterCriticalSection(&commandCriticalSection);
        DeleteOutputFiles();
        LeaveCriticalSection(&commandCriticalSection);
    }

    // Take a calibration picture.
    if (keyState.Space && !prevKeyState.Space)
    {
        pplx::create_task([=]()
        {
            TakeCalibrationPicture();
        });
    }

    // Use the calibration pictures to stereo calibrate the camera rig.
    if (keyState.Enter && !prevKeyState.Enter)
    {
        EnterCriticalSection(&commandCriticalSection);
        PerformCalibration();
        LeaveCriticalSection(&commandCriticalSection);
    }
}

// Take a picture with the camera and hololens.
// There will be approximately 0.2 seconds latency between the two snapshots.
void CalibrationApp::TakeCalibrationPicture()
{
    EnterCriticalSection(&calibrationPictureCriticalSection);
    std::wstring camPath = DirectoryHelper::FindUniqueFileName(outputPath, L"cam", L".png", photoIndex);
    std::wstring holoPath = DirectoryHelper::FindUniqueFileName(outputPath, L"holo", L".jpg", photoIndex);

    // Get latest hologram photo
    std::vector<cv::Point2f> corners;
    cv::Mat grayscaleImage;
    // Only take the picture if a chessboard is in view.
    
    // Cache the latest color frame so we do not stall the UI thread while checking if there is a chess board in frame.
    EnterCriticalSection(&photoTextureCriticalSection);
    memcpy(cachedColorMat.data, latestColorMat.data, FRAME_BUFSIZE);
    LeaveCriticalSection(&photoTextureCriticalSection);

    if (!HasChessBoard(cachedColorMat, grayscaleImage, corners))
    {
        LeaveCriticalSection(&calibrationPictureCriticalSection);
        return;
    }

    // First take a picture from the Hololens.  This will take about a second.
    TakeMRCPicture();

    // Lock the latest camera image immediately after the hololens picture has been taken.
    EnterCriticalSection(&photoTextureCriticalSection);
    memcpy(cachedColorMat.data, latestColorMat.data, FRAME_BUFSIZE);
    LeaveCriticalSection(&photoTextureCriticalSection);

    cv::imwrite(cv::String(StringHelper::ws2s(camPath)), cachedColorMat);

    // Copy the latest MRC image from the Hololens to the calibration directory.
    GetAllMRCFiles((LPTSTR)holoPath.c_str());
    // Delete all of the MRC files on the device.
    DeleteAllMRCFiles();
    LeaveCriticalSection(&calibrationPictureCriticalSection);
}

// Take calibration pictures at a predetermined interval.
void CalibrationApp::TakeCalibrationPictureAtInterval(DX::StepTimer const& timer)
{
    // Do not call this function if we do not have a positive non-zero frequency.
    if (CALIBRATION_FREQUENCY_SECONDS <= 0)
    {
        return;
    }

    calibrationPictureElapsedTime += timer.GetElapsedSeconds();
    if (calibrationPictureElapsedTime >= CALIBRATION_FREQUENCY_SECONDS)
    {
        calibrationPictureElapsedTime = 0;

        pplx::create_task([=]()
        {
            TakeCalibrationPicture();
        });
    }
}

// Delete all of the previously taken calibration files and results.
void CalibrationApp::DeleteOutputFiles()
{
    DirectoryHelper::DeleteFiles(outputPath, L".jpg");
    DirectoryHelper::DeleteFiles(outputPath, L".png");
    DirectoryHelper::DeleteFiles(outputPath, L"CalibrationData.txt");

    photoIndex = 0;
}

// Check if the image has a chessboard in it.
// Output a grayscale converted image and the interior corners of the chessboard.
bool CalibrationApp::HasChessBoard(cv::Mat image, cv::Mat& grayscaleImage, std::vector<cv::Point2f>& corners)
{
    cv::cvtColor(image, grayscaleImage, CV_RGBA2GRAY);

    return cv::findChessboardCorners(grayscaleImage, boardDimensions, corners,
        cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_FAST_CHECK);
}

// Use the calibration pictures to stereo calibrate the camera rig.
void CalibrationApp::PerformCalibration()
{
    int numFiles = DirectoryHelper::NumFiles(outputPath, L".png");

    if (numFiles == 0)
    {
        OutputDebugString(L"ERROR: Please take some calibration pictures first.\n");
        return;
    }

    // Chessboard points in object space for boards that are visible in both camera and hololens pictures.
    std::vector<std::vector<cv::Point3f>> stereoObjectPoints;
    // Chessboard points in image space for boards that are visible in both camera and hololens pictures.
    std::vector<std::vector<cv::Point2f>> stereoColorImagePoints, stereoHoloImagePoints;
    // Chessboard points in image space for all boards in camera and hololens picture.
    std::vector<std::vector<cv::Point2f>> colorImagePoints, holoImagePoints;
    // Chessboard points in image space for the latest camera and hololens pictures.
    std::vector<cv::Point2f> colorCorners, holoCorners;

    OutputDebugString(L"Parsing calibration files\n");

    for (int fileIndex = 0; fileIndex < numFiles; fileIndex++)
    {
        bool colorPatternFound, holoPatternFound = false;

        // Get File Path.
        std::wstring pathRoot = outputPath + std::to_wstring(fileIndex).c_str() + L"_";
        std::wstring colPath = pathRoot + L"cam.png";

        if (DirectoryHelper::FileExists(colPath))
        {
            // Get chessboard for DSLR picture
            colorImage_cam = cv::imread(StringHelper::ws2s(colPath).c_str(), cv::IMREAD_UNCHANGED);
            cv::resize(colorImage_cam, resizedColorImage_cam, cv::Size(HOLO_WIDTH, HOLO_HEIGHT), 0, 0, cv::INTER_AREA);

            if (colorPatternFound = HasChessBoard(resizedColorImage_cam, grayscaleImage_cam, colorCorners))
            {
                cv::cornerSubPix(grayscaleImage_cam, colorCorners, cv::Size(11, 11), cv::Size(-1, -1),
                    cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));

                colorImagePoints.resize(colorImagePoints.size() + 1);
                colorImagePoints[colorImagePoints.size() - 1] = colorCorners;
            }
        }

        // Load Holo textures
        std::wstring holPath = pathRoot + L"holo.jpg";

        if (DirectoryHelper::FileExists(holPath))
        {
            colorImage_holo = cv::imread(StringHelper::ws2s(holPath).c_str(), cv::IMREAD_UNCHANGED);

            // Get chess board data from HoloLens
            if (holoPatternFound = HasChessBoard(colorImage_holo, grayscaleImage_holo, holoCorners))
            {
                cv::cornerSubPix(grayscaleImage_holo, holoCorners, cv::Size(11, 11), cv::Size(-1, -1),
                    cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));

                holoImagePoints.resize(holoImagePoints.size() + 1);
                holoImagePoints[holoImagePoints.size() - 1] = holoCorners;
            }
        }

        // If the entire chess board is found in both images, add this data to the points to calibrate.
        if (colorPatternFound && holoPatternFound)
        {
            size_t newSize = stereoObjectPoints.size() + 1;
            stereoObjectPoints.resize(newSize);
            stereoColorImagePoints.resize(newSize);
            stereoHoloImagePoints.resize(newSize);

            // Add object-space points for stereo images that both have visible chess boards.
            for (int i = 0; i < boardDimensions.height; i++)
            {
                for (int j = 0; j < boardDimensions.width; j++)
                {
                    stereoObjectPoints[stereoObjectPoints.size() - 1].push_back(
                        cv::Point3f((float)(j * CHESS_SQUARE_SIZE), (float)(i * CHESS_SQUARE_SIZE), 0.0f));
                }
            }

            stereoColorImagePoints[stereoObjectPoints.size() - 1] = colorCorners;
            stereoHoloImagePoints[stereoObjectPoints.size() - 1] = holoCorners;
        }

        OutputDebugString(std::to_wstring( (int)( ( ((float)fileIndex + 1.0f) / (float)numFiles ) * 100) ).c_str());
        OutputDebugString(L"% Complete\n");
    }

    if (colorImagePoints.size() == 0 || holoImagePoints.size() == 0 || stereoColorImagePoints.size() == 0 || stereoHoloImagePoints.size() == 0)
    {
        OutputDebugString(L"Must have valid data to perform calibration.\n");
    }

    //http://docs.opencv.org/2.4/modules/calib3d/doc/camera_calibration_and_3d_reconstruction.html#Mat initCameraMatrix2D(InputArrayOfArrays objectPoints, InputArrayOfArrays imagePoints, Size imageSize, double aspectRatio)
    // Add object-space points for all camera images.
    std::vector<std::vector<cv::Point3f>> colorObjectPoints;
    colorObjectPoints.resize(colorImagePoints.size());
    for (int colorImagePoint = 0; colorImagePoint< colorImagePoints.size(); colorImagePoint++)
    {
        for (int i = 0; i < boardDimensions.height; i++)
        {
            for (int j = 0; j < boardDimensions.width; j++)
            {
                colorObjectPoints[colorImagePoint].push_back(
                    cv::Point3f((float)(j * CHESS_SQUARE_SIZE), (float)(i * CHESS_SQUARE_SIZE), 0.0f));
            }
        }
    }

    // Add object-space points for all Hololens images.
    std::vector<std::vector<cv::Point3f>> holoObjectPoints;
    holoObjectPoints.resize(holoImagePoints.size());
    for (int holoImagePoint = 0; holoImagePoint< holoImagePoints.size(); holoImagePoint++)
    {
        for (int i = 0; i < boardDimensions.height; i++)
        {
            for (int j = 0; j < boardDimensions.width; j++)
            {
                holoObjectPoints[holoImagePoint].push_back(cv::Point3f((float)(j * CHESS_SQUARE_SIZE), (float)(i * CHESS_SQUARE_SIZE), 0.0f));
            }
        }
    }

    double apertureWidth = 0;
    double apertureHeight = 0;
    double holoFovX, holoFovY, colorFovX, colorFovY = 0;
    double focalLength = 0;
    cv::Point2d principalPoint;
    double aspectRatio = 0;

    // Calibrate the individual cameras.
    cv::Mat distCoeffColor, distCoeffHolo;
    cv::Mat colorR, holoR, colorT, holoT;
    
    cv::Mat colorMat = cv::initCameraMatrix2D(colorObjectPoints, colorImagePoints, cv::Size(HOLO_WIDTH, HOLO_HEIGHT), (double)HOLO_HEIGHT / (double)HOLO_WIDTH);
    cv::Mat holoMat = cv::initCameraMatrix2D(holoObjectPoints, holoImagePoints, cv::Size(HOLO_WIDTH, HOLO_HEIGHT), (double)HOLO_HEIGHT / (double)HOLO_WIDTH);

    OutputDebugString(L"Start Calibrating DSLR.\n");
    double colorRMS = cv::calibrateCamera(colorObjectPoints, colorImagePoints, cv::Size(HOLO_WIDTH, HOLO_HEIGHT), colorMat, distCoeffColor, colorR, colorT, CV_CALIB_USE_INTRINSIC_GUESS);
    OutputDebugString(L"Done Calibrating DSLR.\n");
    OutputDebugString(L"Start Calibrating HoloLens.\n");
    double holoRMS = cv::calibrateCamera(holoObjectPoints, holoImagePoints, cv::Size(HOLO_WIDTH, HOLO_HEIGHT), holoMat, distCoeffHolo, holoR, holoT, CV_CALIB_USE_INTRINSIC_GUESS);
    OutputDebugString(L"Done Calibrating HoloLens.\n");

    cv::calibrationMatrixValues(holoMat, cv::Size(HOLO_WIDTH, HOLO_HEIGHT), apertureWidth, apertureHeight, holoFovX, holoFovY, focalLength, principalPoint, aspectRatio);
    cv::calibrationMatrixValues(colorMat, cv::Size(HOLO_WIDTH, HOLO_HEIGHT), apertureWidth, apertureHeight, colorFovX, colorFovY, focalLength, principalPoint, aspectRatio);


    // Output rotation, translation, essential matrix, fundamental matrix.
    cv::Mat R, T, E, F;

    //http://docs.opencv.org/2.4/modules/calib3d/doc/camera_calibration_and_3d_reconstruction.html#double stereoCalibrate(InputArrayOfArrays objectPoints, InputArrayOfArrays imagePoints1, InputArrayOfArrays imagePoints2, InputOutputArray cameraMatrix1, InputOutputArray distCoeffs1, InputOutputArray cameraMatrix2, InputOutputArray distCoeffs2, Size imageSize, OutputArray R, OutputArray T, OutputArray E, OutputArray F, TermCriteria criteria, int flags)
    // Stereo calibrate the two cameras.
    OutputDebugString(L"Start stereo calibrating.\n");
    double rms = cv::stereoCalibrate(stereoObjectPoints, stereoHoloImagePoints, stereoColorImagePoints,
        holoMat, distCoeffHolo,
        colorMat, distCoeffColor,
        cv::Size(HOLO_WIDTH, HOLO_HEIGHT),
        R, T, E, F,
        CV_CALIB_FIX_INTRINSIC
    );
    OutputDebugString(L"Done stereo calibrating.\n");

    // Write calibration data file:
    // First Delete the old calibration file if one exists.
    DeleteFile(calibrationFile.c_str());

    std::ofstream calibrationfs;
    calibrationfs.open(calibrationFile.c_str());

    calibrationfs << "# Stereo RMS calibration error (Lower numbers are better)" << std::endl;
    calibrationfs << "RMS: " << rms << std::endl;

    calibrationfs << "# DSLR RMS calibration error (Lower numbers are better)" << std::endl;
    calibrationfs << "DSLR RMS: " << colorRMS << std::endl;

    calibrationfs << "# HoloLens RMS calibration error (Lower numbers are better)" << std::endl;
    calibrationfs << "HoloLens RMS: " << holoRMS << std::endl;

    calibrationfs << "# Delta in meters of Hololens from Camera:" << std::endl;
    calibrationfs << "Translation: " << T.at<double>(0, 0) << ", " << T.at<double>(1, 0) << ", " << T.at<double>(2, 0) << std::endl;

    calibrationfs << "# Row Major Matrix3x3 (This should be close to identity)" << std::endl;
    calibrationfs << "Rotation: " << R.at<double>(0, 0) << ", " << R.at<double>(0, 1) << ", " << R.at<double>(0, 2) << ", " << 
        R.at<double>(1, 0) << ", " << R.at<double>(1, 1) << ", " << R.at<double>(1, 2) << ", " << R.at<double>(2, 0) << ", " << 
        R.at<double>(2, 1) << ", " << R.at<double>(2, 2) << std::endl;

    calibrationfs << "# Field of View of DSLR:" << std::endl;
    calibrationfs << "DSLR_fov: " << colorFovX << ", " << colorFovY << std::endl;

    calibrationfs << "# Field of View of HoloLens:" << std::endl;
    calibrationfs << "Holo_fov: " << holoFovX << ", " << holoFovY << std::endl;

    calibrationfs << "# DSLR distortion coefficients:" << std::endl;
    calibrationfs << "DSLR_distortion: " << distCoeffColor.at<double>(0, 0) << ", " << distCoeffColor.at<double>(0, 1) << ", " << 
        distCoeffColor.at<double>(0, 2) << ", " << distCoeffColor.at<double>(0, 3) << ", " << distCoeffColor.at<double>(0, 4) << std::endl;

    calibrationfs << "# DSLR camera Matrix: fx, fy, cx, cy:" << std::endl;
    calibrationfs << "DSLR_camera_Matrix: " << colorMat.at<double>(0, 0) << ", " << colorMat.at<double>(1, 1) << ", " << 
        colorMat.at<double>(0, 2) << ", " << colorMat.at<double>(1, 2) << std::endl;
    calibrationfs.close();
}

// Draws the scene.
void CalibrationApp::Render()
{
    // Don't try to render anything before the first Update.
    if (timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    auto device = deviceResources->GetD3DDevice();

    // Render live camera texture.
    spriteBatch->Begin(SpriteSortMode_Deferred);

    // Get the latest camera frame.
    if (colorTexture != nullptr && frameProvider != nullptr && frameProvider->IsEnabled())
    {
        DirectXHelper::GetBytesFromTexture(device, colorTexture, FRAME_BPP, colorBytes);

        // Cache the latest camera picture in memory to quickly check if there is a chess board in view.
        EnterCriticalSection(&photoTextureCriticalSection);
        latestColorMat.data = colorBytes;
        LeaveCriticalSection(&photoTextureCriticalSection);

        // Render the latest camera frame.
        if (colorTexture != nullptr)
        {
            spriteBatch->Draw(srv, screenRect, &colorSourceRect,
                Colors::White, 0.0f, XMFLOAT2(0, 0),
                SpriteEffects::SpriteEffects_None);
        }
    }

    spriteBatch->End();
    deviceResources->Present();
}

// Helper method to clear the back buffers.
void CalibrationApp::Clear()
{
    // Clear the views
    auto context = deviceResources->GetD3DDeviceContext();
    auto renderTarget = deviceResources->GetBackBufferRenderTargetView();
    auto depthStencil = deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto viewport = deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);
}

// Message handlers
void CalibrationApp::OnActivated()
{
    // TODO: Game is becoming active window.
}

void CalibrationApp::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void CalibrationApp::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void CalibrationApp::OnResuming()
{
    timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void CalibrationApp::OnWindowSizeChanged(int width, int height)
{
    if (!deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}

// Properties
void CalibrationApp::GetDefaultSize(int& width, int& height) const
{
    width = FRAME_WIDTH;
    height = FRAME_HEIGHT;
}

// These are the resources that depend on the device.
void CalibrationApp::CreateDeviceDependentResources()
{
    auto context = deviceResources->GetD3DDeviceContext();

    spriteBatch = std::make_unique<SpriteBatch>(context);

    // Camera
    colorSourceRect.left = 0;
    colorSourceRect.top = 0;
    colorSourceRect.right = FRAME_WIDTH;
    colorSourceRect.bottom = FRAME_HEIGHT;
}

// Allocate all memory resources that change on a window SizeChanged event.
void CalibrationApp::CreateWindowSizeDependentResources()
{
    screenRect.left = 0;
    screenRect.top = 0;
    screenRect.right = (LONG)deviceResources->GetScreenViewport().Width;
    screenRect.bottom = (LONG)deviceResources->GetScreenViewport().Height;
}

void CalibrationApp::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
    spriteBatch.reset();

    frameProvider->Dispose();
    SafeRelease(srv);
}

void CalibrationApp::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}

// Take a picture from the Hololens.
void CalibrationApp::TakeMRCPicture()
{
    uri_builder builder(U("/api/holographic/mrc/photo"));
    builder.append_query(U("pv"), U("true"));
    builder.append_query(U("holo"), U("false"));

    try
    {
        httpClient->request(methods::POST, builder.to_string()).wait();
    }
    catch (const std::exception &e)
    {
        OutputDebugString(L"Error taking MRC Photo: ");
        OutputDebugString(StringHelper::s2ws(e.what()).c_str());
        OutputDebugString(L"\n");
    }
}

// Delete all of the MRC files on the Hololens.
void CalibrationApp::DeleteAllMRCFiles()
{
    uri_builder builder(U("/api/holographic/mrc/files"));

    try
    {
        auto task = httpClient->request(methods::GET, builder.to_string()).then([](http_response response) -> pplx::task<json::value>
        {
            // If we get a valid response, return the extracted json. 
            if (response.status_code() == status_codes::OK)
            {
                return response.extract_json();
            }

            // Otherwise return empty json. 
            return pplx::task_from_result(json::value());
        })
            .then([=](pplx::task<json::value> previousTask)
        {
            const json::value& jv = previousTask.get();

            // Iterate over the json to get the MRC file names.
            for (auto iter = jv.as_object().cbegin(); iter != jv.as_object().cend(); iter++)
            {
                // Iterate over values in children to find the filename key.
                json::array children = iter->second.as_array();

                for (int i = 0; i < children.size(); i++)
                {
                    for (auto iter2 = children[i].as_object().cbegin(); iter2 != children[i].as_object().cend(); iter2++)
                    {
                        if (iter2->first == L"FileName")
                        {
                            // Convert this filename to base64.
                            LPTSTR encodedString;
                            StringHelper::base64_encode(StringHelper::ws2s(iter2->second.as_string()), encodedString);
                            DeleteMRCFile(encodedString);
                            HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, encodedString);
                        }
                    }
                }
            }
        });

        // Wait for deletion before returning.
        task.wait();
    }
    catch (const std::exception &e)
    {
        OutputDebugString(L"Error deleting MRC files: ");
        OutputDebugString(StringHelper::s2ws(e.what()).c_str());
        OutputDebugString(L"\n");
    }
}

// Delete a particular MRC file from the Hololens.  FileName must be base64 encoded.
void CalibrationApp::DeleteMRCFile(LPTSTR fileName)
{
    uri_builder builder(U("/api/holographic/mrc/file"));
    builder.append_query(U("filename"), fileName);

    try
    {
        httpClient->request(methods::DEL, builder.to_string()).get();
    }
    catch (const std::exception &e)
    {
        OutputDebugString(L"Error Deleting MRC file: ");
        OutputDebugString(StringHelper::s2ws(e.what()).c_str());
        OutputDebugString(L"\n");
    }
}

// Get all of the MRC files on the Hololens and copy to the input path.
void CalibrationApp::GetAllMRCFiles(LPTSTR holoPath)
{
    uri_builder builder(U("/api/holographic/mrc/files"));

    try
    {
        auto task = httpClient->request(methods::GET, builder.to_string()).then([](http_response response) -> pplx::task<json::value>
        {
            // If we get a valid response, return the extracted json. 
            if (response.status_code() == status_codes::OK)
            {
                return response.extract_json();
            }

            // Otherwise return empty json. 
            return pplx::task_from_result(json::value());
        })
            .then([=](pplx::task<json::value> previousTask)
        {
            const json::value& jv = previousTask.get();

            // Iterate over the json to get the MRC file names.
            for (auto iter = jv.as_object().cbegin(); iter != jv.as_object().cend(); iter++)
            {
                // Iterate over values in children to find the filename key.
                json::array children = iter->second.as_array();

                for (int i = 0; i < children.size(); i++)
                {
                    for (auto iter2 = children[i].as_object().cbegin(); iter2 != children[i].as_object().cend(); iter2++)
                    {
                        if (iter2->first == L"FileName")
                        {
                            // Convert this filename to base64.
                            LPTSTR encodedString;
                            StringHelper::base64_encode(StringHelper::ws2s(iter2->second.as_string()), encodedString);
                            GetMRCFile(encodedString, holoPath);
                            HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, encodedString);
                        }
                    }
                }
            }
        });

        // Wait for files to copy before returning.
        task.wait();
    }
    catch (const std::exception &e)
    {
        OutputDebugString(L"Error Getting MRC files: ");
        OutputDebugString(StringHelper::s2ws(e.what()).c_str());
        OutputDebugString(L"\n");
    }
}

// Copy a particular MRC file from the Hololens to the local PC.  FileName must be base64 encoded.
void CalibrationApp::GetMRCFile(LPTSTR fileName, LPTSTR holoPath)
{
    try
    {
        auto fileStream = std::make_shared<concurrency::streams::ostream>();

        // Open stream to output file.
        pplx::task<void> requestTask = concurrency::streams::fstream::open_ostream(holoPath).then([=](concurrency::streams::ostream outFile)
        {
            *fileStream = outFile;

            uri_builder builder(U("/api/holographic/mrc/file"));
            builder.append_query(U("filename"), fileName);
            builder.append_query(U("op"), U("stream"));

            return httpClient->request(methods::GET, builder.to_string()).get();
        })

            // Handle response headers arriving.
            .then([=](http_response response)
        {
            // Write response body into the file.
            return response.body().read_to_end(fileStream->streambuf());
        })

            // Close the file stream.
            .then([=](size_t)
        {
            return fileStream->close().get();
        });

        // Wait for file copy to complete before returning.
        requestTask.wait();
    }
    catch (const std::exception &e)
    {
        OutputDebugString(L"Error Getting MRC file: ");
        OutputDebugString(StringHelper::s2ws(e.what()).c_str());
        OutputDebugString(L"\n");
    }
}
