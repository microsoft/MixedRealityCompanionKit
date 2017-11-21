// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Threading;
using System.Threading.Tasks;
using Windows.Storage;
using Windows.System;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Microsoft.Tools.WindowsDevicePortal;
using static Microsoft.Tools.WindowsDevicePortal.DevicePortal;

namespace HoloLensCommander
{
    /// <summary>
    /// The view model for the DeviceMonitorControl object.
    /// </summary>
    partial class DeviceMonitorControlViewModel : INotifyPropertyChanged, IDisposable
    {
        /// <summary>
        /// Text labels describing the power connection state.
        /// </summary>
        private static readonly string OnAcPowerLabel = "";        // EBB2 (battery with plug)
        private static readonly string OnBatteryLabel = "";        // EBA7 (battery)

        /// <summary>
        /// Text labels describing the type of device.
        /// </summary>
        private static readonly string DeviceIsHoloLensLabel = string.Empty;    // no image
        private static readonly string DeviceIsPCLabel = "";                   // E7F4 (PC monitor)
        private static readonly string DeviceIsUnknownLabel = "";              // E897 (question mark)

        /// <summary>
        /// Text label indicating that an initial connection has not yet been established
        /// with the device.
        /// </summary>
        private static readonly string ConnectionNotEstablishedLabel = "";  // E171 (exclamation point)

        /// <summary>
        /// Message to display when the heartbeat is lost.
        /// </summary>
        private static readonly string HeartbeatLostMessage = "Lost Connection to Device";

        /// <summary>
        /// The DeviceMonitor object responsible for communication with this device.
        /// </summary>
        private DeviceMonitor deviceMonitor;

        /// <summary>
        /// The DeviceMonitorControl object to which this view model is registered.
        /// </summary>
        private DeviceMonitorControl deviceMonitorControl;

        /// <summary>
        /// Have we received the first heartbeat.
        /// </summary>
        private bool firstContact;

        /// <summary>
        /// Indicates whether or not the monitor control was selected prior to loss of heartbeat.
        /// </summary>
        private bool oldIsSelected = false;

        /// <summary>
        /// Event that is notified when a property value has changed.
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;

        /// <summary>
        /// Initializes a new instance of the <see cref="DeviceMonitorControlViewModel" /> class.
        /// </summary>
        /// <param name="control">The DeviceMonitorControl to which this object is registered.</param>
        /// <param name="monitor">The DeviceMonitor responsible for communication with this device.</param>
        public DeviceMonitorControlViewModel(
            DeviceMonitorControl control,
            DeviceMonitor monitor)
        {
            this.deviceMonitorControl = control;

            this.firstContact = false;
            this.deviceMonitor = monitor;
            this.deviceMonitor.HeartbeatLost += Device_HeartbeatLost;
            this.deviceMonitor.HeartbeatReceived += Device_HeartbeatReceived;
            this.deviceMonitor.AppInstallStatus += DeviceMonitor_AppInstallStatus;

            this.SetFilter();

            this.IsConnected = true;

            this.Address = deviceMonitor.Address;
            this.IsSelected = true;

            this.RegisterCommands();
        }

        /// <summary>
        /// Finalizer so that we are assured we clean up all encapsulated resources.
        /// </summary>
        /// <remarks>Call Dispose on this object to avoid running the finalizer.</remarks>
        DeviceMonitorControlViewModel()
        {
            this.Dispose();
        }

        /// <summary>
        /// Cleans up objects managed by the DeviceMonitorControlViewModel.
        /// </summary>
        /// <remarks>
        /// Failure to call this method will result in the object not being collected until
        /// finalization occurs.
        /// </remarks>
        public void Dispose()
        {
            this.deviceMonitor.HeartbeatLost -= Device_HeartbeatLost;
            this.deviceMonitor.HeartbeatReceived -= Device_HeartbeatReceived;
            this.deviceMonitor.AppInstallStatus -= DeviceMonitor_AppInstallStatus;
            this.deviceMonitor.Dispose();
            this.deviceMonitor = null;

            GC.SuppressFinalize(this);
        }

        
        /// <summary>
        /// Closes all applications running on this device.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        internal async Task CloseAllAppsAsync()
        {
            if (this.IsConnected && this.IsSelected)
            {
                try
                {
                    await this.deviceMonitor.TerminateAllApplicationsAsync();
                }
                catch(Exception e)
                {
                    this.StatusMessage = string.Format(
                        "Failed to close one or more applications - {0}",
                        e.Message);
                }
            }
        }

        /// <summary>
        /// Queries the device for the names of all installed applications.
        /// </summary>
        /// <returns>List of application names.</returns>
        internal async Task<List<string>> GetInstalledAppNamesAsync()
        {
            List<string> appNames = new List<string>();

            if (this.IsConnected && this.IsSelected)
            {
                try
                {
                    AppPackages installedApps = await this.deviceMonitor.GetInstalledApplicationsAsync();
                    appNames = Utilities.GetAppNamesFromPackageInfo(
                        installedApps.Packages,
                        true);
                }
                catch
                {
                }
            }
            
            return appNames;
        }

        /// <summary>
        /// Downloads mixed reality files from the device.
        /// </summary>
        /// <param name="parentFolder">The parent folder which will contain the device specific folder.</param>
        /// <param name="deleteAfterDownload">Value indicating whether or not files are to be deleted
        /// from the device after they have been downloaded.</param>
        /// <returns>The name of the folder in to which the files were downloaded.</returns>
        internal async Task<string> GetMixedRealityFilesAsync(
            StorageFolder parentFolder,
            bool deleteAfterDownload)
        {
            string folderName = null;

            if (this.IsConnected && this.IsSelected)
            {
                try
                {
                    MrcFileList fileList = await this.deviceMonitor.GetMixedRealityFileListAsync();
                    
                    if (fileList.Files.Count != 0)
                    {
                        // Create the folder for this device's files.
                        StorageFolder folder = await parentFolder.CreateFolderAsync(
                            (string.IsNullOrWhiteSpace(this.Name) ? this.Address : this.Name),
                            CreationCollisionOption.OpenIfExists);
                        folderName = folder.Name;

                        foreach (MrcFileInformation fileInfo in fileList.Files)
                        {
                            try
                            {
                                byte[] fileData = await this.deviceMonitor.GetMixedRealityFileAsync(fileInfo.FileName);

                                StorageFile file = await folder.CreateFileAsync(
                                    fileInfo.FileName,
                                    CreationCollisionOption.ReplaceExisting);

                                using (Stream stream = await file.OpenStreamForWriteAsync())
                                {
                                    await stream.WriteAsync(fileData, 0, fileData.Length);
                                    await stream.FlushAsync();
                                }

                                this.StatusMessage = string.Format(
                                    "{0} downloaded",
                                    fileInfo.FileName);

                                if (deleteAfterDownload)
                                {
                                    await this.deviceMonitor.DeleteMixedRealityFile(fileInfo.FileName);
                                }
                            }
                            catch(Exception e)
                            {
                                this.StatusMessage = string.Format(
                                    "Failed to download {0} - {1}",
                                    fileInfo.FileName,
                                    e.Message);
                            }
                        }
                    }
                }
                catch(Exception e)
                {
                    this.StatusMessage = string.Format(
                        "Failed to get mixed reality files - {0}",
                        e.Message);
                }
            }
            
            return folderName;   
        }

        /// <summary>
        /// Installs an application on this device.
        /// </summary>
        /// <param name="installFiles">Object describing the file(s) required to install an application.</param>
        /// <returns>Task object used for tracking method completion.</returns>
        internal async Task InstallAppAsync(AppInstallFiles installFiles)
        {
            if (this.IsConnected && this.IsSelected)
            {
                try
                {
                    await this.deviceMonitor.InstallApplicationAsync(installFiles);

                    this.deviceMonitorControl.NotifyAppInstall();
                }
                catch(Exception e)
                {
                    this.StatusMessage = string.Format(
                        "Failed to install {0} - {1}",
                        installFiles.AppPackageFile.Name,
                        e.Message);
                }
            }
        }

        /// <summary>
        /// Wipes camera roll contents on this device.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        internal async Task WipeCameraRollAsync()
        {
            if (this.IsConnected && this.IsSelected)
            {
                try
                {
                    MrcFileList fileList = await this.deviceMonitor.GetMixedRealityFileListAsync();

                    foreach (MrcFileInformation fileInfo in fileList.Files)
                    {
                        await this.deviceMonitor.DeleteMixedRealityFile(fileInfo.FileName);
                    }
                    this.StatusMessage = "Camera roll wiped successfully.";
                }
                catch (Exception e)
                {
                    this.StatusMessage = string.Format("Unable to delete all camera roll files. {0} ", e.Message);
                }
            }
        }

        /// <summary>
        /// Launches an applicaiton on this device.
        /// </summary>
        /// <param name="appName">The name of the application to launch.</param>
        /// <returns>The process identifier of the running application.</returns>
        internal async Task<uint> LaunchAppAsync(string appName)
        {
            uint processId = 0;

            if (this.IsConnected && this.IsSelected)
            {
                try
                {
                    string appId = null;
                    string packageName = null;

                    AppPackages installedApps = await this.deviceMonitor.GetInstalledApplicationsAsync();

                    foreach (PackageInfo packageInfo in installedApps.Packages)
                    {
                        if (appName == packageInfo.Name)
                        {
                            appId = packageInfo.AppId;
                            packageName = packageInfo.FullName;
                            break;    
                        }
                    }

                    if (!string.IsNullOrWhiteSpace(appId) &&
                        !string.IsNullOrWhiteSpace(packageName))
                    {
                        processId = await this.deviceMonitor.LaunchApplicationAsync(
                            appId, 
                            packageName);
                    }

                    Task t = new Task(
                        async () =>
                        {
                            await WatchProcess(processId,
                                appName,
                                2);
                        });
                    t.Start();
                }
                catch(Exception e)
                {
                    this.StatusMessage = string.Format(
                        "Failed to launch {0} - {1}",
                        appName,
                        e.Message);
                }
            }

            return processId;
        }

        /// <summary>
        /// Launches the default web browser and connects to the Windows Device Portal on this devcie.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        private async Task LaunchDevicePortalAsync()
        {
            await Launcher.LaunchUriAsync(this.deviceMonitor.DevicePortalUri);
        }

        /// <summary>
        /// Displays the manage applications dialog.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        private async Task ManageAppsAsync()
        {
            ContentDialog dialog = new ManageAppsDialog(this.deviceMonitor, this.deviceMonitorControl);
            await dialog.ShowAsync();
        }
        
        /// <summary>
        /// Displays the mixed reality view dialog.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        private async Task MixedRealityViewAsync()
        {
            ContentDialog dialog = new MixedRealityViewDialog(this.deviceMonitor);
            await dialog.ShowAsync();
        }

        /// <summary>
        /// Reboots this device.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        internal async Task RebootAsync()
        {
            if (this.IsConnected && this.IsSelected)
            {
                this.StatusMessage = "Rebooting";

                await this.deviceMonitor.RebootAsync();

                this.IsConnected = false;
            }
        }

        /// <summary>
        /// Displays the set IPD dialog.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        internal async Task SetIpdAsync()
        {
            UserInformation userInfo = new UserInformation();
            float.TryParse(this.Ipd, out userInfo.Ipd);

            ContentDialog dialog = new SetIpdDialog(
                this.deviceMonitor.Address, 
                userInfo);
            ContentDialogResult result = await dialog.ShowAsync().AsTask<ContentDialogResult>();;

            // Primary button == "Set"
            if (result == ContentDialogResult.Primary)
            {
                // Update the IPD on the device
                try
                {
                    await this.deviceMonitor.SetIpd(userInfo.Ipd);
                }
                catch (Exception e)
                {
                    this.StatusMessage = string.Format(
                        "Unable to update the IPD - {0}",
                        e.Message);
                }
            }
        }

        /// <summary>
        /// Displays the device information dialog.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        internal async Task ShowDeviceInfoAsync()
        {
            ContentDialog dialog = new DeviceInformationDialog(this.deviceMonitor);
            await dialog.ShowAsync().AsTask<ContentDialogResult>();
        }

        /// <summary>
        /// Shuts down this device.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        internal async Task ShutdownAsync()
        {
            if (this.IsConnected && this.IsSelected)
            {
                this.StatusMessage = "Shutting down";

                await this.deviceMonitor.ShutdownAsync();

                this.IsConnected = false;
            }
        }

        /// <summary>
        /// Starts recording a mixed reality video on this device.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        internal async Task StartMixedRealityRecordingAsync()
        {
            if (this.IsConnected && this.IsSelected)
            {
                this.StatusMessage = "Starting mixed reality recording";

                await this.deviceMonitor.StartMixedRealityRecordingAsync();
            }
        }

        /// <summary>
        /// Stops the mixed reality recording on this device.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        internal async Task StopMixedRealityRecordingAsync()
        {
            if (this.IsConnected && this.IsSelected)
            {
                await this.deviceMonitor.StopMixedRealityRecordingAsync();

                this.StatusMessage = "Mixed reality recording stopped";
            }
        }

        /// <summary>
        /// Displays the tag device dialog.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        private async Task TagDeviceAsync()
        {
            TagInformation tagInfo = new TagInformation();
            tagInfo.Name = this.Name;

            ContentDialog dialog = new TagDeviceDialog(
                this.deviceMonitor.Address,
                tagInfo);
            ContentDialogResult result = await dialog.ShowAsync().AsTask<ContentDialogResult>();;

            // Primary button == "Ok"
            if (result == ContentDialogResult.Primary)
            {
                this.Name = tagInfo.Name;

                if (tagInfo.DeployNameToDevice)
                {
                    // Set the device name.
                    if (await this.deviceMonitor.SetDeviceNameAsync(this.Name))
                    {
                        // Reboot the device so the name change takes effect.
                        await this.deviceMonitor.RebootAsync();
                    }
                }

                this.deviceMonitorControl.NotifyTagChanged();
            }
        }

        /// <summary>
        /// Unintalls an application on this device.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        internal async Task UninstallAppAsync(string appName)
        {
            if (this.IsConnected && this.IsSelected)
            {
                try
                {
                    AppPackages installedApps = await this.deviceMonitor.GetInstalledApplicationsAsync();

                    string packageName = Utilities.GetPackageNameFromAppName(
                        appName,
                        installedApps);
                
                    if (packageName == null)
                    {
                        throw new Exception(
                            string.Format("App ({0}) could not be found",
                            appName));
                    }

                    await this.deviceMonitor.UninstallApplicationAsync(packageName);

                    this.StatusMessage = "Uninstall complete";

                    this.deviceMonitorControl.NotifyAppUninstall();
                }
                catch (Exception e)
                {
                    this.StatusMessage = string.Format(
                        "Failed to uninstall {0} - {1}",
                        appName,
                        e.Message);
                }
            }
        }

        /// <summary>
        /// Uninstalls all side-loaded apps on selected devices
        /// </summary>
        /// <returns></returns>
        internal async Task UninstallAllAppsAsync()
        {
            if (this.IsConnected && this.IsSelected)
            {
                try
                {
                    AppPackages installedApps = await this.deviceMonitor.GetInstalledApplicationsAsync();
                    foreach (PackageInfo packageInfo in installedApps.Packages)
                    {
                        if (packageInfo.IsSideloaded())
                        {
                            await this.deviceMonitor.UninstallApplicationAsync(packageInfo.FullName);
                        }
                    }
                    this.StatusMessage = "Successfully uninstalled all apps.";

                    this.deviceMonitorControl.NotifyAppUninstall();
                }
                catch (Exception e)
                {
                    this.StatusMessage = string.Format("Failed to uninstall all apps. {0} ", e.Message);
                }
            }
        }

        /// <summary>
        /// Handles the ApplicationInstallStatus event.
        /// </summary>
        /// <param name="sender">The object which sent this event.</param>
        /// <param name="args">Event arguments.</param>
        private void DeviceMonitor_AppInstallStatus(
            DeviceMonitor sender, 
            ApplicationInstallStatusEventArgs args)
        {
            this.StatusMessage = args.Message;
        }

        /// <summary>
        /// Handles the HeartbeatLost event.
        /// </summary>
        /// <param name="sender">The object which sent this event.</param>
        private void Device_HeartbeatLost(DeviceMonitor sender)
        {
            this.IsConnected = false;

            this.StatusMessage = HeartbeatLostMessage;

            // Handle whether or not we were previously selected
            if (!this.oldIsSelected &&
                this.IsSelected)
            {
                this.IsSelected = false;
                this.oldIsSelected = true;
            }

            // Update the heartbeat based UI
            this.PowerIndicator = OnBatteryLabel;
            this.BatteryLevel = "--";
            this.ThermalStatus = Visibility.Collapsed;
            this.Ipd = "--";
        }

        /// <summary>
        /// Handles the HeartbeatReceived event.
        /// </summary>
        /// <param name="sender">The object which sent this event.</param>
        private void Device_HeartbeatReceived(DeviceMonitor sender)
        {
            this.IsConnected = true;

            // Did we recover from a heartbeat loss?
            if (this.StatusMessage == HeartbeatLostMessage)
            {
                this.ClearStatusMessage();
            }

            // Handle whether or not we were previously selected
            if (this.oldIsSelected &&
                !this.IsSelected)
            {
                this.IsSelected = true;
                this.oldIsSelected = false;
            }

            // Was this the first time we received a heartbeat?
            if (!this.firstContact)
            {
                // Cause common apps to be refreshed.
                this.deviceMonitorControl.NotifySelectedChanged();
                this.firstContact = true;

            }

            if (this.Address != this.deviceMonitor.Address)
            {             
                // Update the device address display.
                this.Address = this.deviceMonitor.Address;
                ((DeviceMonitorControlViewModel)(this.deviceMonitorControl.DataContext)).Address = this.Address;

                // Re-save the collection...
                this.deviceMonitorControl.NotifyTagChanged();
            }

            // Update the UI
            this.SetFilter();
            this.PowerIndicator = sender.BatteryState.IsOnAcPower ? OnAcPowerLabel : OnBatteryLabel;
            this.BatteryLevel = string.Format("{0}%", sender.BatteryState.Level.ToString("#.00"));
            if (this.deviceMonitor.Platform == DevicePortalPlatforms.HoloLens)
            {
                this.ThermalStatus = (sender.ThermalStage == ThermalStages.Normal) ? Visibility.Collapsed : Visibility.Visible;
                this.Ipd = sender.Ipd.ToString();
            }
        }

        /// <summary>
        /// Method used by WatchProcess to ensure that the status message update is performed on the ui thread.
        /// </summary>
        /// <param name="message">The message to display to the user.</param>
        private void MarshalStatusMessageUpdate(string message)
        {
            CoreDispatcher dispatcher = this.deviceMonitorControl.Dispatcher;
            if (!dispatcher.HasThreadAccess)
            {
                // Assigning the return value of RunAsync to a Task object to avoid 
                // warning 4014 (call is not awaited).
                Task t = dispatcher.RunAsync(
                    CoreDispatcherPriority.Normal,
                    () =>
                    {
                        this.MarshalStatusMessageUpdate(message);
                    }).AsTask();
                return;
            }

            this.StatusMessage = message;
        }

        /// <summary>
        /// Sends the PropertyChanged events to registered handlers.
        /// </summary>
        /// <param name="propertyName">The name of property that has changed.</param>
        private void NotifyPropertyChanged(string propertyName)
        {
            this.PropertyChanged?.Invoke(
                this, 
                new PropertyChangedEventArgs(propertyName));
        }

        /// <summary>
        /// Registers the commands supported by this object.
        /// </summary>
        private void RegisterCommands()
        {
            this.ClearStatusMessageCommand = new Command(
                (parameter) =>
                {
                    this.ClearStatusMessage();
                });

            this.DisconnectCommand = new Command(
                (parameter) =>
                {
                    this.Disconnect();
                });

            this.SetIpdCommand = new Command(
                async (parameter) =>
                {
                    try
                    {
                        await this.SetIpdAsync();
                    }
                    catch(Exception e)
                    {
                        this.StatusMessage = string.Format(
                            "Failed to set IPD ({0})",
                            e.Message);
                    }
                });

            this.SetTagCommand = new Command(
                async (parameter) =>
                {
                    try
                    {
                        await this.TagDeviceAsync();
                    }
                    catch (Exception e)
                    {
                        this.StatusMessage = string.Format(
                            "Failed to set the device name ({0})",
                            e.Message);
                    }
                });

            this.ShowContextMenuCommand = new Command(
                async (parameter) =>
                {
                    await this.ShowContextMenuAsync(parameter);
                });
        }

        /// <summary>
        /// Determine the correct setting for the Filter property.
        /// </summary>
        private void SetFilter()
        {
            switch (this.deviceMonitor.Platform)
            {
                case DevicePortalPlatforms.HoloLens:
                    this.Filter = DeviceFilters.HoloLens;
                    this.DeviceTypeLabel = DeviceIsHoloLensLabel;
                    this.IpdVisibility = Visibility.Visible;
                    break;

                case DevicePortalPlatforms.Windows:
                    this.Filter = DeviceFilters.Desktop;
                    this.DeviceTypeLabel = DeviceIsPCLabel;
                    this.IpdVisibility = Visibility.Collapsed;
                    break;

                case DevicePortalPlatforms.VirtualMachine:
                    switch (this.deviceMonitor.DeviceFamily)
                    {
                        case "Windows.Desktop":
                            this.Filter = DeviceFilters.Desktop;
                            this.DeviceTypeLabel = DeviceIsPCLabel;
                            this.IpdVisibility = Visibility.Collapsed;
                            break;

                        case "Windows.Holographic":
                            this.Filter = DeviceFilters.HoloLens;
                            this.DeviceTypeLabel = DeviceIsHoloLensLabel;
                            this.IpdVisibility = Visibility.Visible;
                            break;

                        default:
                            this.Filter = DeviceFilters.None;
                            this.DeviceTypeLabel = DeviceIsUnknownLabel;
                            this.IpdVisibility = Visibility.Collapsed;
                            break;
                    }
                    break;

                default:
                    this.Filter = DeviceFilters.None;
                    this.DeviceTypeLabel = this.firstContact ? DeviceIsUnknownLabel : ConnectionNotEstablishedLabel;
                    this.IpdVisibility = Visibility.Collapsed;
                    break;
            }
        }


        /// <summary>
        /// Monitors running processes and when the specified id is no longer running, update's the status message ui.
        /// </summary>
        /// <param name="processId">The process identifier to watch.</param>
        ///  <param name="appName">The name of the application associated with the process identifier.</param>
        /// <param name="waitSeconds">Time, in seconds, to wait between checking on the processes.</param>
        /// <returns>Task object used for tracking method completion.</returns>
        private async Task WatchProcess(
            uint processId,
            string appName,
            float waitSeconds)
        {
            ManualResetEvent resetEvent = new ManualResetEvent(false);
            int waitTime = (int)(waitSeconds * 1000.0f);

            Timer timer = new Timer(
                WatchProcessCallback,
                resetEvent,
                Timeout.Infinite,
                Timeout.Infinite);

            RunningProcesses runningProcesses = null;
            bool processIsRunning = false;

            try
            {
                do
                {
                    MarshalStatusMessageUpdate(string.Format(
                        "Waiting for {0} to exit",
                        appName));

                    resetEvent.Reset();
                    timer.Change(0, waitTime);
                    resetEvent.WaitOne(waitTime * 2);   // Wait no longer than twice the specified time.
                    runningProcesses = await this.deviceMonitor.GetRunningProcessesAsync();
                    processIsRunning = runningProcesses.Contains((int)processId);
                }
                while(processIsRunning);

                    MarshalStatusMessageUpdate(string.Format(
                        "{0} has exited",
                        appName));
            }
            catch(Exception e)
            {
                MarshalStatusMessageUpdate(string.Format(
                    "Cannot determine the execution state of {0} - {1}",
                    appName,
                    e.Message));
            }

            timer.Change(
                Timeout.Infinite,
                Timeout.Infinite);
            timer.Dispose();
            timer = null;
        }

        /// <summary>
        /// Timer callback event used when watching a process.
        /// </summary>
        /// <param name="data">The manual reset event that is being waited upon.</param>
        private void WatchProcessCallback(object data)
        {
            ManualResetEvent resetEvent = data as ManualResetEvent;
            resetEvent.Set();
        }
    }
}
