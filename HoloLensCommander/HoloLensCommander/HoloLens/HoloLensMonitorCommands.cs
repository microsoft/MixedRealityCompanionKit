// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;
using Windows.Security.Cryptography.Certificates;
using Microsoft.Tools.WindowsDevicePortal;
using static Microsoft.Tools.WindowsDevicePortal.DevicePortal;
using Windows.UI.Core;

namespace HoloLensCommander
{
    /// <summary>
    /// Class that provides the relevant functionality of the Windows Device Portal.
    /// </summary>
    public partial class HoloLensMonitor
    {
        /// <summary>
        /// Connects to a HoloLens using the provided credentials.
        /// </summary>
        /// <param name="userName"></param>
        /// <param name="password"></param>
        /// <returns></returns>
        public async Task ConnectAsync(
            string address,
            string userName,
            string password)
        {
            this.devicePortalConnection = new HoloLensDevicePortalConnection(
                    address,
                    userName, 
                    password);
            DevicePortal portal = new DevicePortal(this.devicePortalConnection);
            portal.ConnectionStatus += DevicePortal_ConnectionStatus;

            // We are physically connected to the device.
            // We can safely accept the device's root certificate.
            Certificate certificate = await portal.GetRootDeviceCertificateAsync(true);

            // Establish the connection to the device.
            // TODO: Add support for optionally setting the SSID and key.
            await portal.ConnectAsync(
                null,
                null,
                manualCertificate: certificate);
        }

        /// <summary>
        /// Handles the ConnectionStatus event.
        /// </summary>
        /// <param name="sender">The object which sent this event.</param>
        /// <param name="args">Event arguments.</param>
        private void DevicePortal_ConnectionStatus(
            DevicePortal sender, 
            DeviceConnectionStatusEventArgs args)
        {
            if (args.Status == DeviceConnectionStatus.Connected)
            {
                // Connection successfully established.
                this.devicePortal = sender;
                this.devicePortal.AppInstallStatus += DevicePortal_AppInstallStatus;

                // Start the heartbeat.
                Task t = CheckHeartbeatAsync();
                t.Wait();

                this.devicePortal.ConnectionStatus -= DevicePortal_ConnectionStatus;
            }
            else if (args.Status == DeviceConnectionStatus.Failed)
            {
                this.devicePortal.ConnectionStatus -= DevicePortal_ConnectionStatus;

                // Connection failed.
                throw new Exception(args.Message);
            }
        }

        /// <summary>
        /// Deletes a mixed reality file from the HoloLens.
        /// </summary>
        /// <param name="fileName">The name of the file to be deleted./param>
        /// <returns>Task object used for tracking method completion.</returns>
        public async Task DeleteMixedRealityFile(string fileName)
        {
            await this.devicePortal.DeleteMrcFileAsync(fileName);
        }

        /// <summary>
        /// Disconnects from the device by stopping the heartbeat.
        /// </summary>
        public void Disconnect()
        {
            // Stop the timer.
            this.heartbeatTimer.Change(
                Timeout.Infinite,
                Timeout.Infinite);
        }

        /// <summary>
        /// Gets the collection of installed applications.
        /// </summary>
        /// <returns>AppPackages object describing the installed applications.</returns>
        public async Task<AppPackages> GetInstalledApplicationsAsync()
        {
            return await this.devicePortal.GetInstalledAppPackagesAsync();
        }

        /// <summary>
        /// Downloads a mixed reality file.
        /// </summary>
        /// <param name="fileName">The name of the file to download</param>
        /// <returns>Byte array containing the file data.</returns>
        public async Task<byte[]> GetMixedRealityFileAsync(string fileName)
        {
            return await this.devicePortal.GetMrcFileDataAsync(
                fileName,
                false);
        }

        /// <summary>
        /// Gets the names of the mixed reality files.
        /// </summary>
        /// <returns>MrcFileList object describing the collection of files.</returns>
        public async Task<MrcFileList> GetMixedRealityFileListAsync()
        {
            return await this.devicePortal.GetMrcFileListAsync();
        }

        /// <summary>
        /// Gets the Uri providing the live mixed reality data.
        /// </summary>
        /// <returns>Uri to the mixed reality data stream.</returns>
        public Uri GetMixedRealityViewUri()
        {
            return this.devicePortal.GetLowResolutionMrcLiveStreamUri(
                true,   // Include holograms.
                true,   // Include color camera.
                true,  // Include microphone.
                true); // Include application audio.
        }

        /// <summary>
        /// Gets the collection of running processes.
        /// </summary>
        /// <returns>AppPackages object describing the running processes.</returns>
        public async Task<RunningProcesses> GetRunningProcessesAsync()
        {
            return await this.devicePortal.GetRunningProcessesAsync();
        }

        /// <summary>
        /// Installs an application on this HoloLens.
        /// </summary>
        /// <param name="appPackage">The full path to the application package.</param>
        /// <returns>Task object used for tracking method completion.</returns>
        public async Task InstallApplicationAsync(string appPackage)
        {
            await Task.Run(
                async () =>
                {
                    await this.devicePortal.InstallApplicationAsync(
                        null,
                        appPackage,
                        new List<string>());
                });
        }

        /// <summary>
        /// Launches an application on this HoloLens.
        /// </summary>
        /// <param name="appId">The application identifier.</param>
        /// <param name="packageName">The name of the application package.</param>
        /// <returns>The processes identifier of the applicaition.</returns>
        public async Task<int> LaunchApplicationAsync(
            string appId,
            string packageName)
        {
            return await this.devicePortal.LaunchApplicationAsync(
                appId,
                packageName);
        }

        /// <summary>
        /// Reboots this HoloLens.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        public async Task RebootAsync()
        {
            await this.devicePortal.RebootAsync();    
        }

        /// <summary>
        /// Updates the interpupilary distance on the HoloLens.
        /// </summary>
        /// <param name="ipd">The value to set as the user's interpupilary distance.</param>
        /// <returns>Task object used for tracking method completion.</returns>
        public async Task SetIpd(float ipd)
        {
            await this.devicePortal.SetInterPupilaryDistanceAsync(ipd);
        }

        /// <summary>
        /// Shuts down this HoloLens.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        public async Task ShutdownAsync()
        {
            await this.devicePortal.ShutdownAsync();    
        }

        /// <summary>
        /// Starts a mixed reality recording on this HoloLens.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        public async Task StartMixedRealityRecordingAsync()
        {
            await this.devicePortal.StartMrcRecordingAsync(
                true,       // Include holograms.
                true,       // Include color camera.
                true,       // Include microphone audio.
                true);      // Include application audio.
        }

        /// <summary>
        /// Stops the mixed reality recording on this HoloLens.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        public async Task StopMixedRealityRecordingAsync()
        {
            await this.devicePortal.StopMrcRecordingAsync();
        }

        /// <summary>
        /// Stops all running applications on this HoloLens.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        public async Task TerminateAllApplicationsAsync()
        {
            RunningProcesses runningApps = await this.GetRunningProcessesAsync();

            foreach (DeviceProcessInfo processInfo in runningApps.Processes)
            {
                // Do not terminate the shell.
                if (ShellApp.ToLower() == processInfo.Name.ToLower())
                {
                    continue;
                }

                if (!string.IsNullOrWhiteSpace(processInfo.PackageFullName))
                {
                    await this.TerminateApplicationAsync(processInfo.PackageFullName);
                }
            }
        }

        /// <summary>
        /// Stops a single application on this HoloLens.
        /// </summary>
        /// <param name="packageName">Package name of the application to stop.</param>
        /// <returns>Task object used for tracking method completion.</returns>
        public async Task TerminateApplicationAsync(string packageName)
        {
            await this.devicePortal.TerminateApplicationAsync(packageName) ;
        }

        /// <summary>
        /// Uninstalls an applicaiton on this HoloLens.
        /// </summary>
        /// <param name="packageName">Package name of the application to uninstall.</param>
        /// <returns>Task object used for tracking method completion.</returns>
        public async Task UninstallApplicationAsync(string packageName)
        {
            await this.devicePortal.UninstallApplicationAsync(packageName);
        }

        private void DevicePortal_AppInstallStatus(
            DevicePortal sender, 
            ApplicationInstallStatusEventArgs args)
        {
            NotifyAppInstallStatus(args);
        }

        private void NotifyAppInstallStatus(
            ApplicationInstallStatusEventArgs args)
        {
            if (!this.dispatcher.HasThreadAccess)
            {
                // Assigning the return value of RunAsync to a Task object to avoid 
                // warning 4014 (call is not awaited).
                Task t = this.dispatcher.RunAsync(
                    CoreDispatcherPriority.Normal,
                    () =>
                    {
                        this.NotifyAppInstallStatus(args);
                    }).AsTask();
                return;
            }

            this.AppInstallStatus?.Invoke(
                this, 
                args);
        }
    }
}
