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

            // blocks, so this can be safely looped without sleeping.
            tcp.ServerEstablishConnection();
            OutputDebugString(L"Connection Created!\n");
        }
    });
}

SpectatorViewSocket::~SpectatorViewSocket()
{
}

void SpectatorViewSocket::SetCoordinateSystem(SpatialCoordinateSystem^ cs)
{
    if (coordinateSystem == nullptr)
    {
        coordinateSystem = cs;
    }
}

// TODO: how often to listen?
bool SpectatorViewSocket::Listen()
{
    int recvLen = sizeof(ClientToServerPacket);
    if (tcp.ReceiveData(recvbuf, recvLen))
    {
        memcpy(&packet, recvbuf, recvLen);

        LARGE_INTEGER qpc;
        QueryPerformanceCounter(&qpc);

        LONGLONG now = (qpc.QuadPart * S2HNS) / freq;
        LONGLONG networkLatency = ((now - packet.sentTime) * HNS2NS);
        LONGLONG captureLatency = packet.captureLatency * MS2NS;
        LONGLONG offset = (captureLatency - networkLatency);
        if (offset < 0) { offset = 0; } //TODO: may need to implement cached color frames if this is the case.
        poseTimeOffset = (int)(offset + packet.additionalOffsetTime);
        return true;
    }

    return false;
}

void SpectatorViewSocket::GetPose(SpatialCoordinateSystem^ cs, int nsPast)
{
    try
    {
        SetCoordinateSystem(cs);

        // Cannot find position if we do not have a coordinate system.
        if (coordinateSystem == nullptr)
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
        headPose = locator->TryLocateAtTimestamp(perceptionTimestamp, coordinateSystem);
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
    catch (...)
    {
    }
}

//TODO: send after every listen, or send in a loop on first time offset?
//TODO: peer will need to decide on how often to send offsets too!
void SpectatorViewSocket::SendPose(SpatialCoordinateSystem^ cs)
{
    // No connections have been made.
    /*if (!Listen())
    {
        return;
    }*/
    Listen();

    GetPose(cs, poseTimeOffset);
    tcp.SendData((byte*)&currentPose, sizeof(currentPose));
}


