// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "stdafx.h"
#include "CanonSDKManager.h"

#if USE_CANON_SDK

CanonSDKManager::CanonSDKManager() :
    cameraInitialized(false),
    sdkInitialized(false),
    photoDownloading(false)
{
    InitializeCriticalSection(&vectorAccessCriticalSection);

    InitializeCamera();
}

CanonSDKManager::~CanonSDKManager()
{
    // Close the session and terminate the SDK.
    // This will turn off Live View on the camera: 
    // Turn Live View back on when you restart the application.
    EdsCloseSession(camera);
    EdsTerminateSDK();
}

void CanonSDKManager::InitializeCamera()
{
    EdsCameraListRef cameraList = NULL;
    EdsUInt32 cameraCount = 0;
    EdsDeviceInfo deviceInfo;

    EdsError err = EDS_ERR_OK;
    if (!sdkInitialized)
    {
        err = EdsInitializeSDK();

        if (err != EDS_ERR_OK)
        {
            OutputDebugString(L"Error initializing Canon SDK.\n");
        }
        else
        {
            sdkInitialized = true;
            OutputDebugString(L"Canon SDK Initialized.\n");
        }
    }

    if (err == EDS_ERR_OK)
    {
        err = EdsGetCameraList(&cameraList);
    }

    if (err == EDS_ERR_OK)
    {
        err = EdsGetChildCount(cameraList, &cameraCount);
        if (cameraCount == 0)
        {
            err = EDS_ERR_DEVICE_NOT_FOUND;
        }
    }

    // Get the first camera.
    if (err == EDS_ERR_OK)
    {
        err = EdsGetChildAtIndex(cameraList, 0, &camera);
    }

    // Get camera information.
    if (err == EDS_ERR_OK)
    {
        err = EdsGetDeviceInfo(camera, &deviceInfo);
        if (err == EDS_ERR_OK && camera == NULL)
        {
            err = EDS_ERR_DEVICE_NOT_FOUND;
        }
    }

    //Set Property Event Handler
    if (err == EDS_ERR_OK)
    {
        err = EdsSetPropertyEventHandler(camera, kEdsPropertyEvent_All, PropertyEventHandler, this);
    }

    //Set Object Event Handler
    if (err == EDS_ERR_OK)
    {
        err = EdsSetObjectEventHandler(camera, kEdsObjectEvent_All, ObjectEventHandler, this);
    }

    //Set State Event Handler
    if (err == EDS_ERR_OK)
    {
        err = EdsSetCameraStateEventHandler(camera, kEdsStateEvent_All, StateEventHandler, this);
    }

    // Open session to camera.
    if (err == EDS_ERR_OK)
    {
        err = EdsOpenSession(camera);
    }

    // Save images to host PC.
    if (err == EDS_ERR_OK)
    {
        EdsUInt32 saveTarget = kEdsSaveTo_Host;
        err = EdsSetPropertyData(camera, kEdsPropID_SaveTo, 0, sizeof(saveTarget), &saveTarget);
    }

    // Set dummy capacity so we can save images to host PC.
    EdsCapacity capacity = { 0x7FFFFFFF, 0x1000, 1 };
    err = EdsSetCapacity(camera, capacity);

    if (err == EDS_ERR_OK)
    {
        cameraInitialized = true;
    }

    // Cleanup.
    if (cameraList != NULL)
    {
        EdsRelease(cameraList);
    }
}


void CanonSDKManager::Update()
{
    if (!cameraInitialized || !sdkInitialized)
    {
        InitializeCamera();
    }
}

HRESULT CanonSDKManager::TakePictureAsync(std::wstring path)
{
    HRESULT hr = E_PENDING;

    // Do not queue another photo until the current one is captured.
    if (photoDownloading)
    {
        return hr;
    }

    photoDownloading = true;
    tempPath = path;

    EdsError err = EdsSendCommand(camera, kEdsCameraCommand_TakePicture, 0);

    if (err == EDS_ERR_OK)
    {
        EnterCriticalSection(&vectorAccessCriticalSection);
        currentlyDownloadingPictures.push_back(path);
        LeaveCriticalSection(&vectorAccessCriticalSection);
        hr = S_OK;
    }
    else
    {
        photoDownloading = false;
    }

    return hr;
}

EdsError CanonSDKManager::DownloadImageFromCamera(EdsBaseRef image)
{
    EdsStreamRef stream = NULL;
    EdsDirectoryItemInfo dirInfo;
    EdsError err = EdsGetDirectoryItemInfo(image, &dirInfo);
    std::wstring cachedPath = tempPath;

    if (err == EDS_ERR_OK)
    {
        err = EdsCreateFileStream(StringHelper::ws2s(cachedPath).c_str(), kEdsFileCreateDisposition_CreateAlways, kEdsAccess_ReadWrite, &stream);
    }
    //Set Progress callback or function will return before image is fully downloaded.
    if (err == EDS_ERR_OK)
    {
        err = EdsSetProgressCallback(stream, FinishDownloadingCallback, kEdsProgressOption_Done, this);
    }
    if (err == EDS_ERR_OK)
    {
        err = EdsDownload(image, dirInfo.size, stream);
    }
    if (err == EDS_ERR_OK)
    {
        err = EdsDownloadComplete(image);
    }

    // Cleanup.
    if (image != NULL)
    {
        err = EdsRelease(image);
        image = NULL;
    }

    if (stream != NULL)
    {
        EdsRelease(stream);
        stream = NULL;
    }

    // Remove the currently downloading image from the download list.
    EnterCriticalSection(&vectorAccessCriticalSection);
    if (std::find(currentlyDownloadingPictures.begin(), currentlyDownloadingPictures.end(), cachedPath) != currentlyDownloadingPictures.end())
    {
        currentlyDownloadingPictures.erase(std::find(currentlyDownloadingPictures.begin(), currentlyDownloadingPictures.end(), cachedPath));
    }
    LeaveCriticalSection(&vectorAccessCriticalSection);

    photoDownloading = false;
    return err;
}

BOOL CanonSDKManager::CurrentlyDownloadingPicture(std::wstring path)
{
    BOOL isDownloading = false;
    EnterCriticalSection(&vectorAccessCriticalSection);
    if (std::find(currentlyDownloadingPictures.begin(), currentlyDownloadingPictures.end(), path) != currentlyDownloadingPictures.end())
    {
        isDownloading = true;
    }
    LeaveCriticalSection(&vectorAccessCriticalSection);

    return isDownloading;
}

#endif