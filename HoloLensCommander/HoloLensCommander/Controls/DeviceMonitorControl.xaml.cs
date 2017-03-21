// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading.Tasks;
using Windows.Storage;
using Windows.UI.Xaml.Controls;

namespace HoloLensCommander
{
    /// <summary>
    /// Delegate defining the method signature for handling the AppInstalled event.
    /// </summary>
    /// <param name="sender">The object sending the event.</param>
    public delegate void AppInstalledEventHandler(DeviceMonitorControl sender);

    /// <summary>
    /// Delegate defining the method signature for handling the AppUninstalled event.
    /// </summary>
    /// <param name="sender">The object sending the event.</param>
    public delegate void AppUninstalledEventHandler(DeviceMonitorControl sender);

    /// <summary>
    /// Delegate defining the method signature for handling the DeviceDisconnected event.
    /// </summary>
    /// <param name="sender">The object sending the event.</param>
    public delegate void DeviceDisconnectedEventHandler(DeviceMonitorControl sender);
    
    /// <summary>
    /// Delegate defining the method signature for handling the IsSelectedChanged event.
    /// </summary>
    /// <param name="sender">The object sending the event.</param>
    public delegate void IsSelectionChangedEventHander(DeviceMonitorControl sender);

    /// <summary>
    /// Delegate defining the method signature for handling the TagChanged event.
    /// </summary>
    /// <param name="sender">The object sending the event.</param>
    public delegate void TagChangedEventHandler(DeviceMonitorControl sender);

    /// <summary>
    /// The class that implements the custom xaml control used to monitor a connected device.
    /// </summary>
    public sealed partial class DeviceMonitorControl : Page, IDisposable
    {
        /// <summary>
        /// Event that is sent when an application install has completed.
        /// </summary>
        public event AppInstalledEventHandler AppInstalled;

        /// <summary>
        /// Event that is sent when an application uninstall has completed.
        /// </summary>
        public event AppUninstalledEventHandler AppUninstalled;

        /// <summary>
        /// Event that is sent when the device has been disconnected.
        /// </summary>
        public event DeviceDisconnectedEventHandler DeviceDisconnected;

        /// <summary>
        /// Event that is sent when the device's selection state has been changed.
        /// </summary>
        public event IsSelectionChangedEventHander SelectedChanged;

        /// <summary>
        /// Event that is sent when the data tagged to the device has been changed.
        /// </summary>
        public event TagChangedEventHandler TagChanged;

        /// <summary>
        /// Gets this page's DataContext as the underlying object's type.
        /// </summary>
        internal DeviceMonitorControlViewModel ViewModel
        {
            get
            {
                return this.DataContext as DeviceMonitorControlViewModel;
            }
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="DeviceMonitorControl" /> class.
        /// </summary>
        /// <param name="monitor">The DeviceMonitor that is responsible for communication with the device.</param>
        public DeviceMonitorControl(
            DeviceMonitor monitor)
        {
            this.DataContext = new DeviceMonitorControlViewModel(
                this,
                monitor);
            this.InitializeComponent();
        }

        /// <summary>
        /// Finalizer so that we are assured we clean up all encapsulated resources.
        /// </summary>
        /// <remarks>Call Dispose on this object to avoid running the finalizer.</remarks>
        ~DeviceMonitorControl()
        {
            this.Dispose();
        }

        /// <summary>
        /// Cleans up objects managed by the DeviceMonitorControl.
        /// </summary>
        /// <remarks>
        /// Failure to call this method will result in the object not being collected until
        /// finalization occurs.
        /// </remarks>
        public void Dispose()
        {
            try
            {
                this.ViewModel.Dispose();
            }
            catch
            {
                // TODO: Investigate what causes this.DataContext to throw.
            }
            GC.SuppressFinalize(this);
        }

        /// <summary>
        /// Closes all applications running on this device.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        internal async Task CloseAllAppsAsync()
        {
            await this.ViewModel.CloseAllAppsAsync();
        }

        /// <summary>
        /// Disconnect from this device.
        /// </summary>
        internal void Disconnect()
        {
            this.ViewModel.Disconnect();
        }

        /// <summary>
        /// Queries the device for the names of all installed applications.
        /// </summary>
        /// <returns>List of application names.</returns>
        internal async Task<List<string>> GetInstalledAppNamesAsync()
        {
            return await this.ViewModel.GetInstalledAppNamesAsync();
        }

        /// <summary>
        /// Downloads the mixed reality capture files on the device.
        /// </summary>
        /// <param name="parentFolder">The name of the folder in which to download the files.</param>
        /// <param name="deleteAfterDownload">True to remove the downloaded files from the device.</param>
        /// <returns>Task object used for tracking method completion.</returns>
        internal async Task GetMixedRealityFilesAsync(
            StorageFolder parentFolder,
            bool deleteAfterDownload)
        {
            await this.ViewModel.GetMixedRealityFilesAsync(
                parentFolder, 
                deleteAfterDownload);
        }

        /// <summary>
        /// Installs an application on this device.
        /// </summary>
        /// <param name="installFiles">Object describing the file(s) required to install an application.</param>
        /// <returns>Task object used for tracking method completion.</returns>
        internal async Task InstallAppAsync(AppInstallFiles installFiles)
        {
            await this.ViewModel.InstallAppAsync(installFiles);
        }

        /// <summary>
        /// Launches an application on this device.
        /// </summary>
        /// <param name="appName">The name of the application to launch.</param>
        /// <returns>The process identifier of the running application.</returns>
        internal async Task<int> LaunchAppAsync(string appName)
        {
            return await this.ViewModel.LaunchAppAsync(appName);
        }

        /// <summary>
        /// Sends the AppIninstalled event to registered handlers.
        /// </summary>
        internal void NotifyAppInstall()
        {
            this.AppInstalled?.Invoke(this);
        }

        /// <summary>
        /// Sends the AppUninstalled event to registered handlers.
        /// </summary>
        internal void NotifyAppUninstall()
        {
            this.AppUninstalled?.Invoke(this);
        }

        /// <summary>
        /// Sends the Disconnected event to registered handlers.
        /// </summary>
        internal void NotifyDisconnected()
        {
            this.DeviceDisconnected?.Invoke(this);
        }

        /// <summary>
        /// Sends the SelectedChanged event to registered handlers.
        /// </summary>
        internal void NotifySelectedChanged()
        {
            this.SelectedChanged?.Invoke(this);
        }

        /// <summary>
        /// Sends the TagChanged event to registered handlers.
        /// </summary>
        internal void NotifyTagChanged()
        {
            this.TagChanged?.Invoke(this);
        }

        /// <summary>
        /// Reboots this device.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        internal async Task RebootAsync()
        {
            await this.ViewModel.RebootAsync();
        }

        /// <summary>
        /// Shuts down this device.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        internal async Task ShutdownAsync()
        {
            await this.ViewModel.ShutdownAsync();
        }

        /// <summary>
        /// Starts recording a mixed reality video on this device.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        internal async Task StartMixedRealityRecordingAsync()
        {
            await this.ViewModel.StartMixedRealityRecordingAsync();
        }

        /// <summary>
        /// Stops thea mixed reality recording on this device.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        internal async Task StopMixedRealityRecordingAsync()
        {
            await this.ViewModel.StopMixedRealityRecordingAsync();
        }

        /// <summary>
        /// Uninstalls an application on this device.
        /// </summary>
        /// <param name="appName">The name of the application to uninstall.</param>
        /// <returns>Task object used for tracking method completion.</returns>
        internal async Task UninstallAppAsync(string appName)
        {
            await this.ViewModel.UninstallAppAsync(appName);
        }
    }
}
