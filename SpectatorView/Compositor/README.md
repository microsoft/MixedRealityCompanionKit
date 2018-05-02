# README
## Overview
Spectator view renders holograms from Unity over a color frame from a capture card.  This uses the calibration data from the calibration app to render the holograms at the correct size and orientation.  

Use this app to take pictures and videos of your project.  Output pictures and videos will be saved to "My Documents\HologramCapture\".
The compositor runs as a Unity Editor Window when you have the spectator view addon in your project (see the sample project).

## Setup
Open the Compositor sln with Visual Studio under Compositor\Compositor.sln

### DeckLink Capture Card
If you are using a Blackmagic capture card, you will need to install the SDK and create a Visual Studio user macro for its location.
+ Download the DeckLink SDK from here: https://www.blackmagicdesign.com/support - Search for Desktop Video SDK in "Latest Downloads"
+ Extract the SDK anywhere on your computer.
+ Update the DeckLink_inc user macro in dependencies.props with the corresponding path on your computer.
+ Restart Visual Studio if the Calibration or Compositor sln's are open.

### OpenCV 3.4.1
If you are not using a capture card with an included FrameProvider, you will need to install Open CV to get color frames from a different capture device.
+ Download and install OpenCV 3.4.1 from here: http://opencv.org/
+ Extract OpenCV anywhere on your computer.
+ Update the OpenCV_vc14 user macro in dependencies.props with the corresponding path on your computer.
+ Restart Visual Studio if the Calibration or Compositor sln's are open.

![Dependencies](../DocumentationImages/dependencies.png)

### Code Changes

#### If you are shooting at a resolution other than 1080P:
+ In CompositorConstants.h in the SharedHeaders project, change **FRAME_WIDTH** and **FRAME_HEIGHT** to match the resolution you are recording.
+ Build UnityCompositorInterface and run CopyDLL.cmd
+ The DeckLinkManager will attempt to find the appropriate frame format based on your frame dimensions.  If your frame dimensions are not being picked up, you may need to modify the call to StartCapture in DeckLinkManager.cpp.

#### If you are using a different capture card that does not run on BlackMagic's DeckLink SDK:
+ First try using the OpenCV FrameProvider:
    + In CompositorShared.h in the SharedHeaders project, change **USE_OPENCV** to TRUE, change the other FrameProvider preprocessor definitions to FALSE.
    + In OpenCVFrameProvider.h, change **CAMERA_ID** to be the index of the capture device you are using, probably 0 unless you have an integrated webcam.
+ If that did not work and your capture card has an SDK, use that:
    + Extend IFrameProvider and set the frameProvider in CompositorApp.cpp to be your new IFrameProvider.
    + See DeckLinkManager.cpp for how we are doing this for the BlackMagic card.

## Application
+ When you have made all of the above code changes, build the entire solution as Release x64.
+ Run CopyDLL.cmd to get the Compositor binaries and dependencies into your Unity project.
+ Load your Unity project.
+ Open the compositor in Unity's menu bar under "Spectator View/Compositor".
+ Press Play in Unity.
+ Ensure the Unity Game window is visible and "edit/ project settings/ player/ Run in Background" is checked for WSA and standalone.

    ![Run In Background](../DocumentationImages/Unity/run_in_bg.png)

+ See sample project for more details about instrumenting your Unity project for spectator view.

    ![Compositor](../DocumentationImages/Unity/Compositor.png)

+ If you see a black screen: ensure your camera is on, lens cap is off, live view is on, and HDMI mirroring is on.
+ The spectator view Compositor window allows you to:
    + Start recording video
    + Take a picture
    + Open the directory the captures are saved to
    + Request spatial mapping data from the spectator view camera (if a SpatialMappingManager exists in your project)
    + Visualize the capture.
+ If you take a picture or video, the file will be saved in "My Documents\HologramCapture\"
+ Holograms will not move with the camera until you follow the instructions in the sample project.
+ If you have followed all of the steps in the sample project: as you move the camera around, the composite image will show the holograms where they should be with respect to your HoloLens.

Additional compositor settings can be found in the SpectatorViewManager Prefab:
   + Change hologram opacity
   + Change the frame offset (which adjusts the color timestamp to account for capture card latency)
   + Change the material your spatial mapping mesh will use to render.  There are some included spatial mapping materials under **Addons/SpectatorViewRig/Resources/Materials/SpatialMapping/**:
        + Wireframe rendering.
        + Occlusion rendering to hide holograms behind the mesh.
        + A default standard shader.  This will be used if the material is null.

These values are set on the prefab to guarantee that Unity will cache your desired values.

## Additional Documentation
+ [Overview](../README.md)
+ [Calibration](Calibration/README.md)
+ **Compositor**
+ [SpectatorViewPoseProvider](SpectatorViewPoseProvider/README.md)
+ [Sample](SpectatorViewSample/README.md)