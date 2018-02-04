# PLEASE NOTE
This fork of the official repo contains an update for the SpectatorView sample application to work with Unity 2017.2.0+ and the latest MRTK. Other than that, no changes to the offical repo.

**Tested with:**
Unity 2017.2.0
Unity 2017.2.1
Unity 2017.3
Elgato HD60S Capture Card
GoPro Hero 5

**Please note:**
Unity 2017.2.1 currently has some issues with hologram jittering and tracking loss without recovering, so you should avoid that Version.
Unity 2017.3 has a new Setting in the XR Player Settings. Unfold the Little arrow next to "Windows Mixed Reality" and check "Enable Depth Buffer Sharing" to resolve the jittering mentioned above.
I currently recomment to use Unity 2017.2.0 for HoloLens development, but that's just my experience.

# MixedRealityCompanionKit
Mixed Reality Companion Kit -- This is a MixedRealityToolkit style repository for code bits and components that may not run directly on Microsoft HoloLens or immersive headsets but instead pair with them to build experiences targeting Windows Mixed Reality.

This project has adopted the [Microsoft Open Source Code of Conduct](https://opensource.microsoft.com/codeofconduct/). 
For more information see the [Code of Conduct FAQ](https://opensource.microsoft.com/codeofconduct/faq/) or contact [opencode@microsoft.com](mailto:opencode@microsoft.com) with any additional questions or comments.

HoloLensCompanionKit contains the following feature areas:

1. [Holographic Remoting Host](RemotingHostSample/README.md)

2. [KinectIPD](KinectIPD/README.md)

3. [MixedRemoteViewCompositor](MixedRemoteViewCompositor/README.md)

4. [SpectatorView](SpectatorView/README.md)

5. [Windows Mixed Reality Commander](HoloLensCommander/README.md) (Formerly, HoloLens Commander)

6. [Asset Bundler](Bundler/README.md)



