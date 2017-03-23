## Windows Mixed Reality Commander

Windows Mixed Reality Commander is a Universal Windows application that demonstrates using the Windows Device Portal in a classroom environment. It enables observation and management of one or more HoloLens and/or Windows PC devices. Along with managing a classroom, Windows Mixed Reality Commander can be used to effectively control devices in a demo environment.

This application is built upon the open source [Windows Device Portal Wrapper](https://github.com/Microsoft/WindowsDevicePortalWrapper) project and is used by the Microsoft HoloLens team in the Holographic Academy.

![Windows Mixed Reality Commander](ReadmeImages/MainWindow.png)

### Requirements
Windows Mixed Reality Commander requires:
- A Windows 10 Device (Currently tested on x86 and x64)
- A HoloLens or Windows PC (In developer mode with the [Windows Device Portal](https://developer.microsoft.com/en-us/windows/holographic/using_the_windows_device_portal#setting_up_hololens_to_use_windows_device_portal) enabled and configured)

### Building
Building Windows Mixed Reality Commander requires [Visual Studio 2015 Community and the Windows 10 SDK](https://developer.microsoft.com/en-us/windows/downloads). Visual Studio 2015 Professional and Enterise are also supported.
- Load HoloLensCommander.sln
- Build and deploy the solution.


### Key User Interface Elements

#### Device Connection

![Device Connection Controls](ReadmeImages/ConnectionControls.png)

##### Connect

The Connect button reads the user name and password, then displays a dialog allowing you to target a specific device.

Before attempting to connect to a device, please be sure that the Windows Device Portal is enabled and paired with your desired credentials.

`Note: Windows PCs must be rebooted after enabling the Windows Device Portal.`

![Connection Dialog](ReadmeImages/ConnectDialog.png)

- Address

Enter the IP Address in the Address field. If you are connecting to a Windows PC, you must also provide the port number (typically 50443). To determine the correct address to specify:

* HoloLens

Run the Settings application, select Network & Internet > Wi-Fi.

* Windows PC

Run the Settings application, select Update & Security > For Developers.

- SSID and Key

If you wish to set your device onto a specific network access point, enter the SSID and network key in these boxes.

- Update Connection

Checking Update Connection instructs Windows Mixed Reality Commander to query the device for the first valid (non-169.x.x.x) IP address it reports. 

If you specified an SSID, this value may change, and the connection update may fail. Please retry the connection again without setting the SSID and verify the correct network configuration in the Settings application.

##### User name

This is the user name that will be used to connect to the Windows Device Portal service running on your device. This value can be changed at any time as well as persisted for future use.

##### Password

This is the password that will be used to connect to the Windows Device Portal service running on your device. This value can be changed at any time as well as persisted for future use.

##### Addional Options

The additional options (…) button displays a menu that provide less common connection related functionality.

- Reconnect to previous session

This option allows you to reconnect to the devices that were connected in a previous session of Windows Mixed Reality Commander.

For this to be successful, devices need to have been connected using a common set of credentials and the credentials must be set in the User name and Password fields before selecting this option.

`
Note: Reconnect to previous session will not appear in the additional options menu after the following:
* A reconnection has been attempted
* The Connect button has been used one or more times
`
- Set credentials as new default

This stores the current values for user name and password in a text file that resides in the Windows Mixed Reality Commander’s application data folder. 

`Note: This data is stored in clear text and should not be considered secure storage.`

- Use default credentials

If you have persisted the device credentials, you can use the Use default credentials to instruct Windows Mixed Reality Commander in the case where they may have been overridden for a specific device.

##### Connection Shortcut for Development Machiens and Side-Loading

Getting the IP address from your HoloLens can be a more time-consuming process than getting it from your Windows PC (ex: ipconfig).

Users of the Windows Device Portal are aware of the ability to connect to an attached HoloLens using the loopback address (http://127.0.0.1:10080) when the Windows Phone IP over USB Transport service is installed (available as part of the Windows 10 SDK) and running.

To take advantage of this feature, Windows Mixed Reality Commander will default to this loopback address if the Connect dialog’s Address field is left blank.

`
Note: By default, UWP applications are subject to network isolation. For development and side-load scenarios, your PC can be configured to allow an application to make loopback connections.

Building Windows Mixed Reality Commander using Visual Studio, on the PC on which it will be run, will automatically configure to allow loopback connections.

For side-load scenarios, please refer to the documentation describing network isolation troubleshooting (the application container name for Windows Mixed Reality Commander is “HoloLensCommander_ksbfhw2wnm4q4”).
`

#### Common Applications

The applications section displays and controls applications that are common to all selected devices. Each time a device is selected or deselected, this list is updated.

![Common Application Controls](ReadmeImages/ApplicationMangementControls.png)

The Close All button will stop all running UWP applications on the selected devices. To be more selective in closing an application, you will need to use the Manage apps option for each device.


#### Mixed Reality Capture

The mixed reality capture section allows you to start and stop a recording on the selected devices. You can also save them to the PC running Windows Mixed Reality Commander and optionally delete them from the device after they have been saved locally.

![Mixed Reality Capture Controls](ReadmeImages/MixedRealityControls.png)

Files are saved to your PC’s Pictures Library under HoloLensCommander, and each device’s files will be placed in a folder named after the device’s address or tag.

`Note: Windows PC devices do not support Mixed Reality Capture.`


#### Device control

The device control section allows you to bulk reboot or shutdown the selected devices. It also allows you to instruct Windows Mixed Reality Commander to forget all connected devices.

![Device Control Controls](ReadmeImages/DeviceControlControls.png)

![Reboot Button](RebootImages/RebootButton.png) Reboots the devices selected in the device list. Devices will show “Lost connected to the device” while rebooting and will reactivate upon completion.

![Shutdown Button](RebootImages/ShutdownButton.png) Shuts down the selected devices. Devices remain in the list and will report “Lost connection to the device”.

![Forget Devices Button](ReadmeImages/ForgetDevicesButton.png) Forgets all connected devices by removing them from the devices list and updating the application state.


#### Device List

When you connect a device (HoloLens or Windows PC) an entry will be created in the device list (right side of the UI).

![Device List](ReadmeImages/DeviceList.png)

In the previous image, the upper device is a HoloLens and the lower is a Windows PC. You can differentiate, at a glance by the items highlighted in the red and blue boxes.

The HoloLens supports viewing and setting the interpupillary distance (IPD) and provides controls to view and update the value (red box).

Windows PC devices do not support the IPD controls and have a small icon (blue box) between the additional options button (…) and the address.





