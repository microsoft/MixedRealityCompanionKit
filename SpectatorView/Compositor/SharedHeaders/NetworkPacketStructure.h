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
    // to find RTT
    LONGLONG sentTime = 0;
};