// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#if USE_CANON_SDK

#pragma comment(lib, "EDSDK")

#include "EDSDK.h"
#include "EDSDKTypes.h"
#include "EDSDKErrors.h"

#include "Shlobj.h" // To get MyDocuments path
#include "DirectoryHelper.h"

class CanonSDKManager
{
public:
    CanonSDKManager();
    ~CanonSDKManager();

    HRESULT TakePictureAsync(std::wstring path);
    BOOL CurrentlyDownloadingPicture(std::wstring path);

    void Update();

    static EdsError EDSCALLBACK FinishDownloadingCallback(EdsUInt32 inPercent, EdsVoid* inContext, EdsBool* outCancel)
    {
        return EDS_ERR_OK;
    }

    static EdsError EDSCALLBACK ObjectEventHandler(EdsUInt32 event, EdsBaseRef ref, EdsVoid* context)
    {
        if (ref != NULL && event == kEdsObjectEvent_DirItemRequestTransfer)
        {
            ((CanonSDKManager*)context)->DownloadImageFromCamera(ref);
        }

        if (ref != NULL)
        {
            EdsRelease(ref);
        }

        return EDS_ERR_OK;
    }

    static EdsError EDSCALLBACK PropertyEventHandler(EdsUInt32 event, EdsUInt32 propertyID, EdsUInt32 param, EdsVoid* context)
    {
        return EDS_ERR_OK;
    }

    static EdsError EDSCALLBACK StateEventHandler(EdsUInt32 event, EdsUInt32 param, EdsVoid* context)
    {
        if (event == kEdsStateEvent_Shutdown)
        {
            // Camera has shutdown.  Reset camera initialization flag so we can recover when the camera is turned back on.
            ((CanonSDKManager*)context)->cameraInitialized = false;
        }
        
        return EDS_ERR_OK;
    }

    BOOL Initialized()
    {
        return sdkInitialized && cameraInitialized;
    }

private:
    EdsCameraRef camera = NULL;
    bool cameraInitialized;
    bool sdkInitialized;
    std::wstring tempPath;
    bool photoDownloading;
    CRITICAL_SECTION vectorAccessCriticalSection;

    void InitializeCamera();
    EdsError DownloadImageFromCamera(EdsBaseRef image);

    std::vector<std::wstring> currentlyDownloadingPictures;
};

#endif
