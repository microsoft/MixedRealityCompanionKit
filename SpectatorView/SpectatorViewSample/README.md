# README

## Overview
Sample Unity projects to demonstrate the end to end process of rendering an application's view over an external camera's color image.  This app is a shared experiences using one or more HoloLens devices and Unity as clients.

## Setup
+ Before launching Unity:
    + Ensure CalibrationData.txt from your calibration has been copied to the root of your **Assets** directory.
    + Build the UnityCompositorInterface and CompositorDLL in the Compositor sln (see Compositor Readme).
    + Run CopyDLL.cmd to get the compositor dependencies copied to your Unity project.
+ In the Unity Hierarchy Panel, select the **SharingToSVAdapter** GameObject:
    + Set the **Server IP** field to be the IP of your local PC.
    + Set the **Spectator View IP** field to the IP of the spectator view HoloLens attached to your camera.
    + Set the **Anchor Owner IP** field to the IP of the HoloLens that will create your shared anchor.  Leave this blank if you only have 1 device in your shared experience (It will always be responsible for creating the anchor). 
+ Walk around your playspace with all of the HoloLens devices that will be in the experience, so each device has an internal model of the current space.
+ Build and Deploy:
    + Build the application as a D3D UWP app.
    + Deploy the app to any HoloLens devices you want to see the experience that is not the HoloLens attached to the spectator view camera.
+ Build and deploy the SpectatorViewPoseProvider application to the spectator view HoloLens.
+ Press **Play** in Unity to get realtime Hologram frames for the Compositor.
+ Open the Compositor in the Unity menu bar under "Spectator View\Compositor".

    ![Compositor Menu](../DocumentationImages/Unity/Compositor_MenuItem.png)

+ Ensure the Unity Game window is visible and "edit/ project settings/ player/ Run in Background" is checked for WSA and standalone.

    ![Run In Background](../DocumentationImages/Unity/run_in_bg.png)


## Network
This sample has a simple network implementation that routes client pose data and game state to Unity.  This network sample does not relay these messages back to all of the connected clients.  For a more robust networking solution, check the [Mixed Reality Academy 250 course](https://docs.microsoft.com/en-us/windows/mixed-reality/mixed-reality-250) which uses [UNET](https://docs.unity3d.com/Manual/UNet.html).

To Add your own network messages to this network stack:
+ Update the [NetworkData](Assets/Sharing/Scripts/NetworkData.cs) script to serialize and deserialize a new network message.
+ Update the [SharingManager](Assets/Sharing/Scripts/SharingManager.cs) script to route the new network message.
+ Update the [LocalPlayerManager](Assets/Sharing/Scripts/LocalPlayerManager.cs) script to send a message from the HoloLens application.
+ Update the [RemotePlayerManager](Assets/Sharing/Scripts/RemotePlayerManager.cs) script to react to a network message in Unity that changes the state of the HoloLens that sent the message. 

The anchor data is create with the [AnchorManager](Assets/Sharing/Scripts/AnchorManager.cs) script and shared with the [AnchorNetworkTransmitter](Assets/Sharing/Scripts/AnchorNetworkTransmitter.cs) script over a [StreamSocket](https://docs.microsoft.com/en-us/uwp/api/Windows.Networking.Sockets.StreamSocket). This is the same way the anchor is shared in the MixedReality 250 course.  The SpectatorViewPoseProvider app is expecting an anchor to be shared like this.  If your shared experience does sharing differently, you will need to modify the [ConnectToServer](../SpectatorViewPoseProvider/SpectatorViewPoseProvider/AnchorImporter.cpp) function in SpectatorViewPoseProvider or the [ConnectToAnchorOwner](../SpectatorViewPoseProvider/SpectatorViewPoseProvider/SpectatorViewPoseProviderMain.cpp) function in SpectatorViewPoseProviderMain.cpp in the SpectatorViewPoseProvider app to localize in your way.  

## Adding spectator view support to your app
This sample project has been set up as a shared experience, but if you would like to add this functionality to your existing app, or would like to start from a new project, follow these instructions:
+ Copy **Assets\Addons\SpectatorViewRig** to **Assets\Addons\** in your project.
+ Add **Assets\Addons\SpectatorViewRig\Prefabs\SpectatorViewManager** to the root of your hierarchy.
+ Copy **CalibrationData.txt** to your Assets directory.
+ Populate the IP fields in SpectatorViewManager with the IP or name of your spectator view HoloLens and host device.
+ Add the Sharing prefab from **Sharing\Prefabs** to the root of your hierarchy if you want to use the sample network stack.
    + Set the relevant IP fields in this prefab.
    + Set the Object To Anchor GameObject to be the shared hologram in the experience.
+ Alternatively update the fields in **SharingToSVAdapter** to update relevant prefabs at runtime.
    + Set the **Anchor GO** and **SpectatorViewManager** fields in this gameObject to ensure the SpectatorViewManager is made a child of the correct anchor.

+ **DISCLAIMER:** Any interactions with the app will have to be networked events from your client HoloLens devices back to Unity.
    + If any app state in the HoloLens device does not get updated in Unity, it will not appear in the Compositor.

+ **DISCLAIMER:** If your application was already a shared experience, you will need to update SharingToSVAdapter to route any relevant network messages (like the anchor owner) to your SpectatorViewManager prefab.

## Picking an Anchor
When creating a shared experience, a transform in your hierarchy will be the anchor that all devices will render relative to.  In this sample, the **ObjectToAnchor** GameObject in the **AnchorManager** script on the **Sharing** prefab is the anchor. All devices will try to localize their space to this anchor, so the GameObject should be placed in a position relative to the first HoloLens that all other devices will be able to see.  In this case, the anchor is chosen to be a meter in front of the first HoloLens.  As long as other devices are looking in roughly the same direction, they should be able to localize this anchor.

## Additional Documentation
+ [Overview](../README.md)
+ [Calibration](../Calibration/README.md)
+ [Compositor](../Compositor/README.md)
+ [SpectatorViewPoseProvider](../SpectatorViewPoseProvider/README.md)
+ **Sample**