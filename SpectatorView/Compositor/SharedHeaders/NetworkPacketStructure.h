// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#include <windows.h>

// Time values are in HNS

struct SVPose
{
    // Find RTT
    LONGLONG sentTime;
    // Computed from previous packet.
    LONGLONG RTT = 0;

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
    // Find RTT
    LONGLONG sentTime = 0;

    //TODO:
    // Connect to anchor owner if:
    // 1. We do not have an anchor, or 
    // 2. The anchor owner's IP has changed.
    //char* anchorOwnwerIP[15];
    //int anchorPort;

    // Set to true if anchor has changed and we need to download a new one from the existing owner.
    //bool forceAnchorReconnect;
};