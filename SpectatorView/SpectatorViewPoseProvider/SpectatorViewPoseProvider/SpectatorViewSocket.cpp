// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "SpectatorViewSocket.h"

SpectatorViewSocket::SpectatorViewSocket()
{
    calendar = ref new Windows::Globalization::Calendar();
    locator = SpatialLocator::GetDefault();

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    freq = frequency.QuadPart;

    create_task([&]
    {
        // Loop to accept future connections.
        while (1)
        {
            tcp.CreateServerListener();

            // Blocks, so this can be safely looped without sleeping.
            tcp.ServerEstablishConnection();
            OutputDebugString(L"Connection Created!\n");

            connectionEstablished = true;
        }
    });

    create_task([&]
    {
        while (1)
        {
            Listen();
        }
    });
}

SpectatorViewSocket::~SpectatorViewSocket()
{
}

bool SpectatorViewSocket::Listen()
{
    try
    {
        if (connectionEstablished)
        {
            int recvLen = sizeof(ClientToServerPacket);
            if (tcp.ReceiveData(recvbuf, recvLen))
            {
                memcpy(&packet, recvbuf, recvLen);

                // Get anchor owner information
                std::string ip = std::string(packet.anchorOwnerIP, packet.anchorIPLength);
                std::string name = std::string(packet.anchorName, packet.anchorNameLength);
                if (ip != anchorOwnerIP 
                    || name != anchorName 
                    || packet.forceAnchorReconnect)
                {
                    OutputDebugString(L"Found a new anchor owner: ");
                    OutputDebugString(StringHelper::s2ws(ip).c_str());
                    OutputDebugString(L", On port: ");
                    OutputDebugString(std::to_wstring(packet.anchorPort).c_str());
                    OutputDebugString(L", Named: ");
                    OutputDebugString(StringHelper::s2ws(name).c_str());
                    OutputDebugString(L"\n");

                    anchorOwnerIP = ip;
                    anchorName = name;
                    anchorPort = packet.anchorPort;

                    ConnectToAnchorOwner = true;
                }

                if (packet.requestSpatialMapping)
                {
                    OutputDebugString(L"Sending Spatial Mapping Information.\n");
                    SendSpatialMappingData = true;
                }

                return true;
            }
            else
            {
                connectionEstablished = false;
            }
        }
    }
    catch (...) { }

    return false;
}

void SpectatorViewSocket::GetPose(SpatialCoordinateSystem^ cs, int nsPast)
{
    try
    {
        // Cannot find position if we do not have a coordinate system.
        if (cs == nullptr)
        {
            return;
        }

        // Find the locator if we have not cached one.
        if (locator == nullptr)
        {
            locator = SpatialLocator::GetDefault();

            if (locator == nullptr)
            {
                return;
            }
        }

        // Create a perception timestamp at our offset time.
        calendar->SetToNow();
        if (nsPast > 0)
        {
            nsPast *= -1;
        }

        if (nsPast < -1 * MAX_OFFSET_SECONDS * S2NS)
        {
            nsPast = -1 * MAX_OFFSET_SECONDS * S2NS;
        }

        calendar->AddNanoseconds((int)nsPast);

        PerceptionTimestamp^ perceptionTimestamp = PerceptionTimestampHelper::FromHistoricalTargetTime(calendar->GetDateTime());
        if (perceptionTimestamp == nullptr)
        {
            return;
        }

        SpatialLocation^ headPose = nullptr;
        headPose = locator->TryLocateAtTimestamp(perceptionTimestamp, cs);
        if (headPose == nullptr)
        {
            return;
        }

        LARGE_INTEGER qpc;
        QueryPerformanceCounter(&qpc);

        currentPose.sentTime = (qpc.QuadPart * S2HNS) / freq;

        // Convert position and rotation to Unity space.
        Windows::Foundation::Numerics::quaternion rot = headPose->Orientation;
        currentPose.rotX = -rot.x;
        currentPose.rotY = -rot.y;
        currentPose.rotZ = rot.z;
        currentPose.rotW = rot.w;

        Windows::Foundation::Numerics::float3 pos = headPose->Position;
        currentPose.posX = pos.x;
        currentPose.posY = pos.y;
        currentPose.posZ = -pos.z;
    }
    catch (...) { }
}

void SpectatorViewSocket::SendPose(SpatialCoordinateSystem^ cs)
{
    try
    {
        if (connectionEstablished)
        {
            GetPose(cs, 0);
            if (!tcp.SendData((byte*)&currentPose, sizeof(currentPose)))
            {
                connectionEstablished = false;
            }
        }
    }
    catch (...) { }
}

void SpectatorViewSocket::SendSpatialMapping(byte* bytes, int length)
{
    // Segment bytes into packet sized chunks.
    SpatialMappingPacket packet;

    int numPackets = (int)ceil((float)length / (float)SPATIAL_MAPPING_BUFSIZE);
    int numBytesWritten = 0;

    try
    {
        if (connectionEstablished)
        {
            for (int i = 0; i < numPackets; i++)
            {
                int currentPacketLength = SPATIAL_MAPPING_BUFSIZE;
                if (numBytesWritten + currentPacketLength > length)
                {
                    currentPacketLength = length - numBytesWritten;
                }

                packet.packetStartIndex = numBytesWritten;
                packet.bytesWrittenThisPacket = currentPacketLength;
                packet.totalSpatialMappingBytes = length;
                packet.numSpatialMappingPackets = numPackets;

                memcpy(&packet.payload[0], &bytes[packet.packetStartIndex], currentPacketLength);
                numBytesWritten += currentPacketLength;

                if (!tcp.SendData((byte*)&packet, sizeof(packet)))
                {
                    connectionEstablished = false;
                }
            }
        }
    }
    catch (...) {}
}


