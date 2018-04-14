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

            connectionEstablished = true;
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

bool SpectatorViewSocket::Listen()
{
    try
    {
        if (connectionEstablished)
        {
            int recvLen = sizeof(ClientToServerPacket);
            if (tcp.ReceiveData(recvbuf, recvLen))
            {
                LARGE_INTEGER qpc;
                QueryPerformanceCounter(&qpc);

                memcpy(&packet, recvbuf, recvLen);

                LONGLONG now = (qpc.QuadPart * S2HNS) / freq;
                LONGLONG networkLatency = now - packet.sentTime;

                RTT = networkLatency;
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
        currentPose.RTT = RTT;

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
            // Find a pose at least 16 ms in the past to guarantee that it isn't a predicted pose.
            GetPose(cs, -16 * MS2NS);
            if (!tcp.SendData((byte*)&currentPose, sizeof(currentPose)))
            {
                connectionEstablished = false;
            }
        }
    }
    catch (...) { }
}


