# Mixed Remote View Compositor

## Overview
Mixed Remote View Compositor(MRVC) provides the ability for developers to incorporate near real-time viewing of HoloLens experiences from within a viewing application. This is achieved through low level Media Foundation components that use a lightweight network layer to transmit the data from the device to a remote pc viewing application. Through the [Media Foundation](https://msdn.microsoft.com/en-us/library/windows/desktop/ms696274(v=vs.85).aspx) [capture pipeline](https://msdn.microsoft.com/en-us/library/windows/desktop/hh447846(v=vs.85).aspx), the [sink components](https://msdn.microsoft.com/en-us/library/windows/desktop/ms701626(v=vs.85).aspx) capture and encode the live camera data with its associated data. The data is then transmitted to the remote viewing application that will be decoded and displayed with respect to the device transformations.

There are two methods of operation:

1. Low latency mixed reality capture (MRC)
    + Provides the ability to create a MRC companion app experience with lower latency (~150ms) 
    + Similar to Device Portal and Companion app experiences for [mixed reality capture](https://developer.microsoft.com/en-us/windows/holographic/mixed_reality_capture_for_developers)
    + [Getting Started](Samples/LowLatencyMRC/README.md)

2. Mixed Remote View Compositing
    + Provides the ability to composite the mixed reality capture on a remote machine, allowing the HoloLens user experience to remain at full frame rate
    + Capture HoloLens camera images (PV camera) with its transformation data and transmits data to a remote application. 
    + Using an additional network connection, synchronize scene and input state data to reflect any changes to the world objects.
    + Composite a rendered frame of the scene object with PV camera image to re-create a mixed reality image.
    + [Getting Started](Samples/MRVC/README.md)

## What files are included?
**Builds** - This folder will be populate with the libraries built from the Source project. see **Source** for build instructions.

**Samples** - sample code

- LowLatencyMRC
	- Unity - sample application that demonstrates using library to capture MRC on the HoloLens
    - UWP - sample viewing application to view live MRC video from the HoloLens 
    - [Learn More...](Samples/LowLatencyMRC/README.md)

- MRVC
	- Sample Unity project that will produce both the HoloLens experience applicaiton and the remote viewing application using the same codebase.
	- Demostrates how to use the plug-in and other additional files to demonstrate serializing data to send to a remote application. 
	- [Learn More...](Samples/MRVC/README.md)
	
	***Note:** the network layer code was designed for use with the media pipeline only. It's overloaded use in this sample is provided as an example to demonstrate sending scene and input state data to the remote application. Consider using a different networking framework to sync scene and input states.*

**Source** - Native C++ files used to build components for Win32 and UWP applications. These include the Unity plug-in wrapper, Network and Media Foundation components. [Learn More...](Source/README.md)

### Additional Resources
Developers should already be familiar with and have previous experience with developing HoloLens and/or Unity Applications. Here are resources to get started:

- [HoloLens Academy](https://developer.microsoft.com/en-us/windows/holographic/academy)
- [HoloLens Documentation](https://developer.microsoft.com/en-us/windows/holographic/documentation)
- [Develop UWP apps](https://developer.microsoft.com/en-us/windows/develop)
- [Unity - Learn](https://unity3d.com/learn)

For more technical background on Media Foundation:

- [Microsoft Media Foundation](https://msdn.microsoft.com/en-us/library/windows/desktop/ms694197(v=vs.85).aspx)
- [Media Sources](https://msdn.microsoft.com/en-us/library/windows/desktop/ms697527(v=vs.85).aspx)
- [Media Sinks](https://msdn.microsoft.com/en-us/library/windows/desktop/ms701626(v=vs.85).aspx)
