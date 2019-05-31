# Real-time Streaming Core Source

## Overview

The core source code for real-time streaming is built into a DLL plugin. At it's core, he plugin leverages [C++/WinRT](https://docs.microsoft.com/en-us/windows/uwp/cpp-and-winrt-apis/intro-to-using-cpp-with-winrt), [Media Foundation](https://docs.microsoft.com/en-us/windows/desktop/medfound/microsoft-media-foundation-sdk), and [UWP Sockets](https://docs.microsoft.com/en-us/windows/uwp/networking/sockets).

The visual studio solution is organized into three projects: *Shared*, *UWP*, and *Win32*. The *Shared* Visual Studio project contains all of the source code while the other respective projects are just for building the appropriate binaries. Under the *Shared* project, the code is organized into the following folders.

- *Common* - Helper classes & code
- *Media* - Classes primarily interacting with [Media Foundation](https://docs.microsoft.com/en-us/windows/desktop/medfound/microsoft-media-foundation-sdk)
- *Network* - Classes primarily sending/receiving data over the network via sockets
- *Plugin* - Code to offer interface interop between Unity & the plugin source code
- *Unity* - Helper headers for Unity interop

## Building

When building the **RealtimeStreaming.sln**, the visual studio projects (Win32 & UWP csproj) have been pre-configured to auto-place the compiled binaries into the appropriate folders for each sample folder based on the architecture being built (i.e ARM, x86, x64). Generally, the plugin should be built targeting *Release*.

**NOTE:** Look at the build output to confirm binaries are bin-placed into the appropriate folders. If the binaries are currently in use by a running application, then Visual Studio will fail to write/copy into the existing DLL files.

## WinRT Interfaces

Generally for most core classes in the plugin, there is a cpp file, a header file, and an idl file. The former two files are common for C++ development while the third IDL file is specific to [C++/WinRT](https://docs.microsoft.com/en-us/windows/uwp/cpp-and-winrt-apis/intro-to-using-cpp-with-winrt). The IDL files are written in the [Microsoft Interface Definition Language (MIDL)](https://docs.microsoft.com/en-us/uwp/midl-3/intro) and are used to define the public interfaces for the associated class which is generally compiled into the winmd (windows definition file). 

Learn more about [Authoring APIs with C++/WinRT](https://docs.microsoft.com/en-us/windows/uwp/cpp-and-winrt-apis/author-apis).

## Network

The network classes utilize UWP Sockets to set up a TCP connection between the client & server. The TCP connection is established by utilizing the [StreamSocketListener](https://docs.microsoft.com/en-us/uwp/api/Windows.Networking.Sockets.StreamSocketListener) class and the [StreamSocket](https://docs.microsoft.com/en-us/uwp/api/Windows.Networking.Sockets.StreamSocket) class. The **Connector** class leverages the [StreamSocket](https://docs.microsoft.com/en-us/uwp/api/Windows.Networking.Sockets.StreamSocket) class to target a specfic IP address & port which is the location of the endpoint server waiting with a running [StreamSocketListener](https://docs.microsoft.com/en-us/uwp/api/Windows.Networking.Sockets.StreamSocketListener) class. 

By passing the UDP discovery parameter flag to the **Connector** and **Listener**, then the **Connector** will attempt to broadcast it's desire for a server to any listening **Listeners** via the [DatagramSocket](https://docs.microsoft.com/en-us/uwp/api/Windows.Networking.Sockets.DatagramSocket) class. 

**NOTE:** If there are multiple active **Listener** instances on the same network, there is no guarantee which server will be the one to establish a connection with the client **Connector**. Further, UDP packets can be easily lost if the access points and device endpoints are not configured properly.

The plugin sends data and Media samples across the network by utilizing the [DataBuffer](source/shared/databuffer.h) class. This class effectively acts as a wrapper, with some additional functionality, around an [IMFMediaBuffer](https://docs.microsoft.com/en-us/windows/desktop/api/mfobjects/nn-mfobjects-imfmediabuffer) while also fulfilling the [IBufferByteAccess](https://docs.microsoft.com/en-us/previous-versions/hh846267%28v%3Dvs.85%29) interface to support [IBuffer](https://docs.microsoft.com/en-us/uwp/api/windows.storage.streams.ibuffer) through the UWP sockets.

## Logging

To control the level of logging in this plugin, look at the [ErrorHandling.h](source/shared/errorhandling.h) file and the associated *LOG_LEVEL* definition value.

Further, the code base utilize various logging & error throwing macros such as IFR and IFT. These are short-hand helpers for various scenarios.

- IFR == If fail throw exception.
- IFR == If fail return, etc.

## More information

- [C++/WinRT] (https://docs.microsoft.com/en-us/windows/uwp/cpp-and-winrt-apis/intro-to-using-cpp-with-winrt)
- [Visual Studio](https://visualstudio.microsoft.com/vs/whatsnew/)
- [Windows 10 SDK](https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk)
- [Media Foundation](https://docs.microsoft.com/en-us/windows/desktop/medfound/microsoft-media-foundation-sdk)
- [UWP Sockets](https://docs.microsoft.com/en-us/windows/uwp/networking/sockets)