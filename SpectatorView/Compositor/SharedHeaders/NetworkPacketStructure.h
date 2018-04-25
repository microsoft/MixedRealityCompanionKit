// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#include <windows.h>
#include "Network.h"

#define IP_LENGTH 15
#define ANCHOR_NAME_LENGTH 36

enum PacketType
{
    Pose = 0,
    SpatialMapping = 1,
};

struct SVPose
{
    int header = (int)PacketType::Pose;

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
static_assert(sizeof(SVPose) <= DEFAULT_BUFLEN, 
    "SVPose cannot exceed network buffer size limit.");


struct SpatialMappingPacket
{
    int header = (int)PacketType::SpatialMapping;

    int packetStartIndex;
    int bytesWrittenThisPacket;
    int totalSpatialMappingBytes;
    int numSpatialMappingPackets;

    byte payload[SPATIAL_MAPPING_BUFSIZE];
};
static_assert(sizeof(SpatialMappingPacket) <= DEFAULT_BUFLEN,
    "SpatialMappingPacket cannot exceed network buffer size limit.");


struct ClientToServerPacket
{
    // Connect to anchor owner if:
    // 1. We do not have an anchor 
    // 2. The anchor owner's IP has changed.
    // 3. The anchor's name has changed.
    // 4. A force reconnect message has been sent.
    char anchorOwnerIP[IP_LENGTH];
    int anchorIPLength;
    int anchorPort;

    char anchorName[ANCHOR_NAME_LENGTH];
    int anchorNameLength;

    // Set to true if anchor has changed and we need to download a new one from the existing owner.
    bool forceAnchorReconnect;

    // Set to true to get spatial mapping information from SV HoloLens
    bool requestSpatialMapping;
};
static_assert(sizeof(ClientToServerPacket) <= DEFAULT_BUFLEN,
    "ClientToServerPacket cannot exceed network buffer size limit.");