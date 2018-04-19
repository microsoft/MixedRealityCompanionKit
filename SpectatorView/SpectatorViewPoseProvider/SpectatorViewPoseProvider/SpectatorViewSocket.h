// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#include "Network.h"
#include "NetworkPacketStructure.h"
#include "CompositorConstants.h"
#include "StringHelper.h"

#include <ppltasks.h>
using namespace concurrency;

using namespace Windows::Perception;
using namespace Windows::Perception::Spatial;

// PerceptionTimestampHelper can only cache so much pose data.
#define MAX_OFFSET_SECONDS 1 

class SpectatorViewSocket
{
public:
    SpectatorViewSocket();
    ~SpectatorViewSocket();

    void SendPose(SpatialCoordinateSystem^ cs);
    bool Listen();

    std::string anchorOwnerIP;
    std::string anchorName;
    int anchorPort;
    bool ConnectToAnchorOwner = false;

private:
    WSASession session;
    TCPSocket tcp;

    SpatialCoordinateSystem^ coordinateSystem = nullptr;
    Windows::Globalization::Calendar^ calendar = nullptr;
    SpatialLocator^ locator;
    
    SVPose currentPose;

    bool connectionEstablished = false;

    LONGLONG freq;

    byte* recvbuf = new byte[DEFAULT_BUFLEN];
    ClientToServerPacket packet;

    void SetCoordinateSystem(SpatialCoordinateSystem^ cs);

    // Get time from compositor peer, find past pose, send pose back to peer.
    void GetPose(SpatialCoordinateSystem^ cs, int nsPast);
};

