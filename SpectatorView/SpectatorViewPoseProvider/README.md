# README
## Overview
The SpectatorViewPoseProvider application runs on the spectator view HoloLens to provide pose data and spatial mapping information to the spectator view compositor.

## Lifecycle
+ The application launches and observes spatial mapping data for 10 seconds against a [SpatialCoordinateSystem](https://docs.microsoft.com/en-us/uwp/api/windows.perception.spatial.spatialcoordinatesystem) relative to its startup location.
+ The application listens on a known port for a connected client (the Unity compositor).
+ Once a connection has been established, the application will send the connected compositor it's pose information relative to its current coordinate system.
+ The application will also start listening on the socket for information from the compositor about the anchor in the shared experience.
+ Once the application receives anchor information, it creates a [StreamSocket](https://docs.microsoft.com/en-us/uwp/api/Windows.Networking.Sockets.StreamSocket) to connect to the HoloLens device that is sharing its anchor.
+ When an anchor is downloaded over this socket, the application imports the anchor, and sends poses relative to this coordinate system if it can be located.
+ If a new anchor is located, old spatial mapping surfaces will be discarded and new ones will be found relative to the new anchor's coordinate system.
+ The application also listens for messages that it should send spatial mapping information to the compositor.  When this happens, the spatial mapping surface observer will activate for another 10 seconds and send mesh information relative to the anchor.
+ If the compositor disconnects, the application will start listening for a new compositor.

## Anchor
This application imports anchors over a StreamSocket on a known port to match the way anchors are [created](https://github.com/Microsoft/MixedReality250/blob/master/Assets/AppPrefabs/Support/SharingWithUNET/Scripts/UNetAnchorManager.cs) and [shared](https://github.com/Microsoft/MixedReality250/blob/master/Assets/AppPrefabs/Support/SharingWithUNET/Scripts/GenericNetworkTransmitter.cs) in the [Mixed Reality 250 Academy course](https://docs.microsoft.com/en-us/windows/mixed-reality/mixed-reality-250).

If your shared experience does sharing differently, you will need to modify the [ConnectToServer](SpectatorViewPoseProvider/AnchorImporter.cpp) function in AnchorImporter.cpp or the [ConnectToAnchorOwner](SpectatorViewPoseProvider/SpectatorViewPoseProviderMain.cpp) function in SpectatorViewPoseProviderMain.cpp to localize in your way.  

## Additional Documentation
+ [Overview](../README.md)
+ [Calibration](../Calibration/README.md)
+ [Compositor](../Compositor/README.md)
+ **SpectatorViewPoseProvider**
+ [Sample](../SpectatorViewSample/README.md)