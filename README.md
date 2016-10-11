# HoloLensCompanionKit
HoloLens Companion Kit -- This is a toolkit style repo for code bits and components that *do not run on HoloLens* but pair with HoloLens to build experiences.

This project has adopted the [Microsoft Open Source Code of Conduct](https://opensource.microsoft.com/codeofconduct/). 
For more information see the [Code of Conduct FAQ](https://opensource.microsoft.com/codeofconduct/faq/) or contact [opencode@microsoft.com](mailto:opencode@microsoft.com) with any additional questions or comments.

---

## Holographic Remoting Host

This code sample shows how to create a desktop or UWP app that provides holographic content via remoting.  

Holographic remoting allows your app to host holographic content on a desktop PC, or on a UWP device such as 
the XBox One, allowing access to more system resources and making it possible to integrate holographic views 
into existing desktop PC software. This code sample includes desktop, UWP, and UWP XAML projects that all 
provide remoting content.

This code sample demonstrates the following features that are required for a holographic remoting host:

  * Establishes a connection with the HoloLens
  * Receives a data stream with input from a HoloLens
  * Renders content in a virtual holographic view
  * Streams content frames back to the HoloLens in real-time

For more info, see the [holographic remoting documentation](http://developer.microsoft.com/windows/holographic/Add_holographic_remoting).

### Build the sample

1. Download the sample from GitHub in a ZIP file, or clone the repo to your developer PC.
2. Start Microsoft Visual Studio 2015 and select **File** \> **Open** \> **Project/Solution**.
3. In the RemotingHostSample folder, double click on **RemotingHostSample.sln** to open the sample in Visual Studio.
4. Press Ctrl+Shift+B, or select **Build** \> **Build Solution**.

### Install the Remoting Player app

1. On your Microsoft HoloLens, visit the Store app and search for "Remoting Player".
2. Select the Remoting Player app.
3. Tap **Install* to start the download and install the Remoting Player app.

### Connect the remoting host sample to the Remoting Player

1. Start the Remoting Player on your Microsoft HoloLens.
2. Take note of the HoloLens IP address. It will be displayed as a hologram by the Remoting Player soon after it launches.
3. Set up the remoting host sample that you are most interested in:

  1. The UWP XAML sample includes a UI that makes it easy to connect. After launching the sample, you will type in the IP address of the HoloLens into a dialog box and click **Connect**.
  2. The UWP sample (DirectX only) will require a code modification to connect. Go to line 223 of RemotingHostSample.cpp, replace "192.168.0.1" with the IP address of your HoloLens, and rebuild and launch the app to connect.
  3. The desktop sample requires that you specify the IP address on the command-line. To do this, right-click on the project to view the **Property Pages** and select the Debug property page. Type the IP address of the HoloLens into the **Command Arguments** field and click **OK**.

4. In Visual Studio, right-click on the remoting host sample that you want to launch and select **Set as StartUp Project**.
5. Press F5 to begin.


---


## KinectIPD

KinectIPD is a utility to measure a person’s IPD (inter-pupillary distance) and optionally set the IPD on a HoloLens device automatically.  This is especially useful in situations where many users are using a HoloLens device such as demos or tradeshows.  In testing, KinectIPD was accurate to +- 2 millimeters, which should be adequate for most HoloLens scenarios.

### Building:
- Install the latest version of the Kinect SDK from: https://developer.microsoft.com/en-us/windows/kinect 
- Note: KinectIPD was primarily tested on 64bit systems.  Additional work may be required to run on 32bit systems.
 - Download the project from GitHub in a ZIP file, or clone the repo to your developer PC.
 - Start Microsoft Visual Studio 2015 and select File > Open > Project/Solution.
 - In the KinectIPD folder, double click on KinectIPD_Kiosk.sln to open the sample in Visual Studio.
 - Change the build configuration to Release/x64.
 - Press F5 to start the application.

### How to use:
Connect a Kinect 2.0 camera to a PC/Laptop and run KinectIPD.  After startup, the app will be waiting for a user to be discovered in frame.  Once it finds a face, it will instruct the user to move closer/farther away until they are at an acceptable distance.  At this point the tool will extract a rectangle around each eye, passing the data to the “PupilDetectDLL” component, where a pupil detection algorithm will be run on it.  When the pupils are discovered, the pixel coordinates are translated to 3D space and the distance between the pupils are measured.  This will be repeated for each frame until a minimum number of successful samples are discovered (20 by default), and the standard deviation of the measurements falls below a specified value (1mm by default, plus a slowly increasing tolerance over time).  When a valid IPD has been measured, based on those criteria, the app will either display the IPD to the screen, and/or write the value to a configured HoloLens device.  After a short amount of time (7.5 seconds), the app will reset back to its “attract” state, waiting for a user to enter the camera frame.

Various settings can be changed by switching into “debug mode” by pressing X or Spacebar on the keyboard.

- AutoExposure:  Enabling this option will have the app attempt to adjust the exposure of the image so that the area around the eyes is at a specified brightness.
- Exposure: The current exposure of the image.  This can be manually adjusted when not in Auto Exposure mode. In general, a brighter image will work better for image processing.  
    Note: It is recommended to have supplemental lighting (i.e. LED strip lights) on either side of the camera to light the user’s face better.
- Target Luma:  This is the value that AutoExposure will use as a target value. 
- Vertical Offset: Occasionally the Kinect face tracking will report the position of the eyes as higher/lower than they actually are – resulting in poor results when detecting pupils.  This adjustment offsets the eye boxes to accommodate the difference.
- HoloLens Programming Mode:  The application can optionally program a HoloLens device with the IPD after successful measurement. There are 3 modes available:
 -  Display Only:  The application will only display the IPD at the end of the measurement process.  The IPD can then be manually submitted to the HoloLens using another tool.
 -  Device List:  One or more devices can be specified by Name/IP/Username/Password to automatically be programmed at the end of measurement.  The IP address can be http://localhost:10080 if connected via USB, the actual network IP of the device if known, or set to auto-discover the network IP (this requires being connected to USB).
 - NFC Tag:  A tag can be placed on the HoloLens so that when the device is placed onto an NFC reader, the Name/IP/Username/Password will be read from the tag, and automatically programmed.  See below for more info on tags, readers and a mount.

### Recording/Playback:
The Kinect streams can be recorded to be played back for debugging/testing.
- To record a session, press the R button on the keyboard or the “record session” button in the debug view (press X/space to activate the debug view.)  It will record for 20 seconds maximum, or until you press the record button again.  Start the session before the user is discovered, then have them approach the camera as normal and stand in place for the rest of the session.
- To play a session, press the E button on the keyboard, or the “play session” button in the debug view (then choose the file to play back).

Caveats:
- During playback, the exposure can’t be adjusted (since we talk to the driver).  Thus you should record multiple sessions of each person at different exposure settings (and auto-exposure turned off).
- The files are LARGE – (3GB+ for 30 seconds of data).  


### NFC Reader/Tags and mount:
KinectIPD has been tested with the following NFC components:
- NFC Reader: 
    http://www.amazon.com/CycleMore-ACR122U-Contactless-Reader-Writer/dp/B016XJH5VS/ref=sr_1_4?rps=1&ie=UTF8&qid=1453925951&sr=8-4&keywords=acr122u&refinements=p_85%3A2470955011
- NFC Tags (25 in a package):
    http://www.amazon.com/SMARTRAC-3002981-Inlays-Circus-NTAG213/dp/B011DFAQQI/ref=sr_1_3?rps=1&ie=UTF8&qid=1453933803&sr=8-3&keywords=nfc+clear+tags&refinements=p_85%3A2470955011
- The included 3D printable mount model (located in the 'HololensNFC_readerMount.fbx' file) has been used successfully to allow for easy alignment of the tag to the reader.   
