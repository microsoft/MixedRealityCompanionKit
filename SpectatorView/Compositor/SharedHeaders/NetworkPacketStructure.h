// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#include <windows.h>

// Time values are in HNS

struct SVPose
{
    LONGLONG sentTime;

    // Pose
    float rotX = 0;
    float rotY = 0;
    float rotZ = 0;
    float rotW = 1;

    float posX = 0;
    float posY = 0;
    float posZ = 0;
};

struct ClientToServerPacket
{
    // Connect to anchor owner if:
    // 1. We do not have an anchor 
    // 2. The anchor owner's IP has changed.
    // 3. The anchor's name has changed.
    // 3. A force reconnect message has been sent.
    char anchorOwnerIP[15];
    int anchorIPLength;
    int anchorPort;

    char anchorName[32];
    int anchorNameLength;

    // Set to true if anchor has changed and we need to download a new one from the existing owner.
    bool forceAnchorReconnect;
};