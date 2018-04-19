#pragma once

// Reference links:
// Importing/ Exporting anchors in C++
// https://docs.microsoft.com/en-us/windows/mixed-reality/shared-spatial-anchors-in-directx
// This application intercepts an anchor that has been exported from an existing shared experience, like Mixed Reality 250.
// These scripts create the anchor and setup a StreamSocket to send the anchor to whoever connects.
// https://github.com/Microsoft/MixedReality250/blob/master/Assets/AppPrefabs/Support/SharingWithUNET/Scripts/UNetAnchorManager.cs
// https://github.com/Microsoft/MixedReality250/blob/master/Assets/AppPrefabs/Support/SharingWithUNET/Scripts/GenericNetworkTransmitter.cs

// To integrate spectator view into your shared experience, 
// This application needs to connect to the IP of the anchor owner on the known anchor sharing port.

#include <collection.h>
#include <ppltasks.h>
#include <string>

using namespace concurrency;
using namespace Windows::Networking;
using namespace Windows::Networking::Sockets;
using namespace Windows::Perception::Spatial;
using namespace Windows::Storage::Streams;

class AnchorImporter
{
private:
    SpatialAnchor^ sharedAnchor = nullptr;
    
    //TODO: Get this data from SV compositor.
    Platform::String^ anchorOwnerIP;
    HostName^ anchorOwnerHost;
    int anchorPort = 11000;

    StreamSocket^ anchorConnection;
    byte* anchorData;

    task<bool> ImportAnchorDataAsync(byte* anchorData, int anchorSize);
public:
    AnchorImporter();
    ~AnchorImporter();

    bool ConnectToServer(Platform::String^ serverIP, int port = 11000);

    // Coordinate system to get pose data from existing shared experience.
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

