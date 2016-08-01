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
  2. The UWP sample (DirectX only) will require a code modification to connect. Go to line 223 of RemotingHostSample223.cpp, replace "192.168.0.1" with the IP address of your HoloLens, and rebuild and launch the app to connect.
  3. The desktop sample requires that you specify the IP address on the command-line. To do this, right-click on the project to view the **Property Pages** and select the Debug property page. Type the IP address of the HoloLens into the **Command Arguments** field and click **OK**.

4. In Visual Studio, right-click on the remoting host sample that you want to launch and select **Set as StartUp Project**.
5. Press F5 to begin.

---
