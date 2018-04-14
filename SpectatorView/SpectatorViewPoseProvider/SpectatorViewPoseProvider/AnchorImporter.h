#pragma once
// https://docs.microsoft.com/en-us/windows/mixed-reality/shared-spatial-anchors-in-directx
// https://github.com/Microsoft/MixedReality250/blob/master/Assets/AppPrefabs/Support/SharingWithUNET/Scripts/UNetAnchorManager.cs
// https://github.com/Microsoft/MixedReality250/blob/master/Assets/AppPrefabs/Support/SharingWithUNET/Scripts/GenericNetworkTransmitter.cs

// Receive an anchor from an existing shared experience with a known server IP and port.

#include <ppltasks.h>

using namespace concurrency;
using namespace Windows::Foundation;
using namespace Windows::Networking;
using namespace Windows::Networking::Sockets;
using namespace Windows::Perception::Spatial;

class AnchorImporter
{
private:
    enum AnchorImportStatus
    {
        Disconnected,
        WaitingForConnection,
    };

    AnchorImportStatus importStatus = AnchorImportStatus::Disconnected;

    SpatialAnchor^ sharedAnchor = nullptr;
    
    //TODO: Get this data from SV compositor.
    Platform::String^ anchorOwnerIP;
    HostName^ anchorOwnerHost;
    int anchorPort = 11000;

    StreamSocket^ anchorConnection;

public:
    AnchorImporter();
    ~AnchorImporter();

    bool ConnectToServer(Platform::String^ serverIP, int port = 11000);

    // Coordinate system to get pose data from in shared experience.
    // If this is null, fall back to stationary reference frame's coordinate system.
    SpatialCoordinateSystem^ GetSharedAnchorCoordinateSystem()
    {
        if (sharedAnchor == nullptr)
        {
            return nullptr;
        }

        return sharedAnchor->CoordinateSystem;
    }

};

