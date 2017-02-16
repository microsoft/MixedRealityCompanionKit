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
    public delegate void AppInstalledEventHandler(HoloLensMonitorControl sender);

    /// <summary>
    /// Delegate defining the method signature for handling the AppUninstalled event.
    /// </summary>
    /// <param name="sender">The object sending the event.</param>
    public delegate void AppUninstalledEventHandler(HoloLensMonitorControl sender);

    /// <summary>
    /// Delegate defining the method signature for handling the HoloLensDisconnected event.
    /// </summary>
    /// <param name="sender">The object sending the event.</param>
    public delegate void HoloLensDisconnectedEventHandler(HoloLensMonitorControl sender);
    
    /// <summary>
    /// Delegate defining the method signature for handling the HoloLensTagChanged event.
    /// </summary>
    /// <param name="sender">The object sending the event.</param>
    public delegate void HoloLensTagChangedEventHandler(HoloLensMonitorControl sender);

    /// <summary>
    /// The class that implements the custom xaml control used to monitor a connected HoloLens.
    /// </summary>
    public sealed partial class HoloLensMonitorControl : Page, IDisposable
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
        /// Event that is sent when the HoloLens has been disconnected.
        /// </summary>
        public event HoloLensDisconnectedEventHandler Disconnected;

        /// <summary>
        /// Event that is sent when the data tagged on the HoloLens has been changed.
        /// </summary>
        public event HoloLensTagChangedEventHandler TagChanged;

        /// <summary>
        /// Gets this page's DataContext as the underlying object's type.
        /// </summary>
        internal HoloLensMonitorControlViewModel ViewModel
        {
            get
            {
                return this.DataContext as HoloLensMonitorControlViewModel;
            }
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="HoloLensMonitorControl" /> class.
        /// </summary>
        /// <param name="monitor">The HoloLensMonitor that is responsible for communication with the HoloLens.</param>
        public HoloLensMonitorControl(
            HoloLensMonitor monitor)
        {
            this.DataContext = new HoloLensMonitorControlViewModel(
                this,
                monitor);
            this.InitializeComponent();
        }

        /// <summary>
        /// Finalizer so that we are assured we clean up all encapsulated resources.
        /// </summary>
        /// <remarks>Call Dispose on this object to avoid running the finalizer.</remarks>
        ~HoloLensMonitorControl()
        {
            Debug.WriteLine("[~HoloLensMonitorControl]");
            this.Dispose();
        }

        /// <summary>
        /// Cleans up objects managed by the HoloLensMonitorControl.
        /// </summary>
        /// <remarks>
        /// Failure to call this method will result in the object not being collected until
        /// finalization occurs.
        /// </remarks>
        public void Dispose()
        {
            Debug.WriteLine("[HoloLensMonitorControl.Dispose]");
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
        /// Closes all applications running on this HoloLens.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        internal async Task CloseAllAppsAsync()
        {
            await this.ViewModel.CloseAllAppsAsync();
        }

        /// <summary>
        /// Disconnect from this HoloLens.
        /// </summary>
        internal void Disconnect()
        {
            this.ViewModel.Disconnect();
        }

        /// <summary>
        /// Queries the HoloLens for the names of all installed applications.
        /// </summary>
        /// <returns>List of application names.</returns>
        internal async Task<List<string>> GetInstalledAppNamesAsync()
        {
            return await this.ViewModel.GetInstalledAppNamesAsync();
        }

        internal async Task GetMixedRealityFilesAsync(
            StorageFolder parentFolder,
            bool deleteAfterDownload)
        {
            await this.ViewModel.GetMixedRealityFilesAsync(
                parentFolder, 
                deleteAfterDownload);
        }

        /// <summary>
        /// Installs an application on this HoloLens.
        /// </summary>
        /// <param name="installFiles">Object describing the file(s) required to install an application.</param>
        /// <returns>Task object used for tracking method completion.</returns>
        internal async Task InstallAppAsync(AppInstallFiles installFiles)
        {
            await this.ViewModel.InstallAppAsync(installFiles.AppPackageFileName);
        }

        /// <summary>
        /// Launches an applicaiton on this HoloLens.
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
            this.Disconnected?.Invoke(this);
        }

        /// <summary>
        /// Sends the TagChanged event to registered handlers.
        /// </summary>
        internal void NotifyTagChanged()
        {
            this.TagChanged?.Invoke(this);
        }

        /// <summary>
        /// Reboots this HoloLens.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        internal async Task RebootAsync()
        {
            await this.ViewModel.RebootAsync();
        }

        /// <summary>
        /// Shuts down this HoloLens.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        internal async Task ShutdownAsync()
        {
            await this.ViewModel.ShutdownAsync();
        }

        /// <summary>
        /// Starts recording a mixed reality video on this HoloLens.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        internal async Task StartMixedRealityRecordingAsync()
        {
            await this.ViewModel.StartMixedRealityRecordingAsync();
        }

        /// <summary>
        /// Stops thea mixed reality recording on this HoloLens.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        internal async Task StopMixedRealityRecordingAsync()
        {
            await this.ViewModel.StopMixedRealityRecordingAsync();
        }

        /// <summary>
        /// Uninstalls an application on this HoloLens.
        /// </summary>
        /// <param name="appName">The name of the application to uninstall.</param>
        /// <returns>Task object used for tracking method completion.</returns>
        internal async Task UninstallAppAsync(string appName)
        {
            await this.ViewModel.UninstallAppAsync(appName);
        }
    }
}
