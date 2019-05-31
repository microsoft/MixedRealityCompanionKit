
# Real-time Streaming WPF Server Sample

## Overview

This code shows how to utilize the real-time streaming plugin in a non-UWP environment by leveraging the [Desktop Bridge](https://docs.microsoft.com/en-us/windows/msix/desktop/desktop-to-uwp-root). The sample application works by creating a  generates a solid color image that lerps to a new color over time which is streamed through a **RealtimeServer** object. The user is able to change the target framerate, port number, UDP discovery, and the output resolution.  

Once the server WPF application is running, select the desired properties for the server and click *Start Server*.

![SampleApp.PNG](SampleApp.PNG)

## Project Structure

There are three projects in the Visual Studio Solution: 

- *DesktopAppPackaging* - This is the target project to build. It wraps the other two applications into one.
- *DesktopServerApp* - This is the primary logic of the sample application & UI. The classes here leverages the plugin DLL classes directly in C#.
- *DummyUwpAppForPackagingOnly* - This is just a dummy app project to be able to interface with the necessary UWP APIs. It is a mostly empty stub project.

The UI can be modified in the [MainWindow.xaml](DesktopServerApp\MainWindow.xaml) and the resulting controller logic is mostly within the [ServerManager.cs](DesktopServerApp\ServerManager.cs)

## Building

After opening the *RealtimeStreaming.sln* under the WPF folder, you can build the WPF application on your local machine. The *DesktopAppPackaging* project should be set as the StartUp project and ideally build against *Release* and *x64*.

You may have to re-reference the Windows winmd. Under the *DesktopServerApp* project, expand *References*, if there is a warning icon for the *Windows* item then you need to find your local copy. Right-click *References*, and then *Add Reference*, then select the *Browse* tab and then the *Browse* button on the bottom. Navigate to the appropriate *Windows.winmd* file for your installed Windows SDK environment. The path will be comparable to this: **"C:\Program Files (x86)\Windows Kits\10\UnionMetadata\10.0.18306.0\Windows.winmd"**.

## More Information
- [Desktop Bridge](https://docs.microsoft.com/en-us/windows/msix/desktop/desktop-to-uwp-root)