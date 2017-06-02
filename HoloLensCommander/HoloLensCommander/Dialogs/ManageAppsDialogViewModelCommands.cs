// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Collections.Generic;
using System.Threading.Tasks;
using System.Windows.Input;
using static Microsoft.Tools.WindowsDevicePortal.DevicePortal;

namespace HoloLensCommander
{
    /// <summary>
    /// The view model for the ManageAppsDialog object.
    /// </summary>
    partial class ManageAppsDialogViewModel
    {
        /// <summary>
        /// Command used to close all applications running on this device.
        /// </summary>
        public ICommand CloseAllAppsCommand
        { get; private set; }

        /// <summary>
        /// Implementation of the close all applications command.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        private async Task CloseAllRunningAppsAsync()
        {
            await this.deviceMonitor.TerminateAllApplicationsAsync();
        }

        /// <summary>
        /// Command used to close a single application on this device.
        /// </summary>
        public ICommand CloseAppCommand
        { get; private set; }

        /// <summary>
        /// Implementation of the close application command.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        private async Task CloseAppAsync()
        {
            string selectedApp = this.SelectedRunningApp as string;

            // The running apps display includes the packgage name in 
            // parenthesis, this is what we need to be able to close the app.
            int index = selectedApp.LastIndexOf('(');
            string packageName = selectedApp.Substring(index + 1);
            packageName = packageName.Substring(0, packageName.Length-1);

            await this.deviceMonitor.TerminateApplicationAsync(packageName);
        }

        /// <summary>
        /// Command used to launch an application on this device.
        /// </summary>
        public ICommand LaunchAppCommand
        { get; private set; }

        /// <summary>
        /// Implementation of the launch application command.
        /// </summary>
        /// <returns>The process identifier of the launched application.</returns>
        private async Task<int> LaunchAppAsync()
        {
            string appName = this.SelectedInstalledApp as string;

            return await this.deviceMonitorControl.LaunchAppAsync(appName);
        }

        /// <summary>
        /// Command used to update the list of applications installed on this device.
        /// </summary>
        public ICommand RefreshInstalledAppsCommand
        { get; private set; }
        
        /// <summary>
        /// Implementation of the refresh installed applications command.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        private async Task RefreshInstalledAppsAsync()
        {
            AppPackages installedApps = await this.deviceMonitor.GetInstalledApplicationsAsync();

            List<string> appNames = Utilities.GetAppNamesFromPackageInfo(
                installedApps.Packages,
                true);

            this.UpdateInstalledApps(appNames);
        }
        
        /// <summary>
        /// Command used to update the list of applications running on this device.
        /// </summary>
        public ICommand RefreshRunningAppsCommand
        { get; private set; }

        /// <summary>
        /// Implementation of the referesh running applications command.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        private async Task RefreshRunningAppsAsync()
        {
            RunningProcesses runningApps = await this.deviceMonitor.GetRunningProcessesAsync();

            List<string> appNames = new List<string>();

            foreach (DeviceProcessInfo processInfo in runningApps.Processes)
            {
                if (!string.IsNullOrWhiteSpace(processInfo.PackageFullName))
                {
                    appNames.Add(string.Format(
                        "{0} ({1})",
                         processInfo.AppName,
                         processInfo.PackageFullName));
                }
            }

            appNames.Sort();
            this.UpdateRunningApps(appNames);   
        }

        /// <summary>
        /// Command used to uninstall a single application on this device.
        /// </summary>
        public ICommand UninstallAppCommand
        { get; private set; }

        /// <summary>
        /// Implementation of the uninstall application command.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        private async Task UninstallAppAsync()
        {
            string appName = this.SelectedInstalledApp as string;

            await this.deviceMonitorControl.UninstallAppAsync(appName);
            await this.RefreshInstalledAppsAsync();
        }

        public ICommand UninstallAllAppsCommand
        { get; private set; }

        private async Task UninstallAllAppsAsync()
        {
            await this.deviceMonitorControl.UninstallAllAppsAsync();
            await this.RefreshInstalledAppsAsync();
        }

        /// <summary>
        /// Updates the backing collection of installed applications.
        /// </summary>
        /// <param name="appNames">List of application names.</param>
        private void UpdateInstalledApps(List<string> appNames)
        {
            // Get the currently selected application.
            string currentSelection = this.SelectedInstalledApp as string;

            this.InstalledApps.Clear();
            foreach (string name in appNames)
            {
                this.InstalledApps.Add(name);
            }

            this.CanRunApp = false;
            if (this.InstalledApps.Count > 0)
            {
                // Set the selected item.
                if ((currentSelection != null) &&
                    this.InstalledApps.Contains(currentSelection))
                {
                    this.SelectedInstalledApp = currentSelection;
                    this.CanRunApp = true;
                }
            }
        }

        /// <summary>
        /// Updates the backing collection of running applications.
        /// </summary>
        /// <param name="appNames">List of application names.</param>
        private void UpdateRunningApps(List<string> appNames)
        {
            // Get the currently selected application.
            string currentSelection = this.SelectedRunningApp as string;

            this.RunningApps.Clear();
            foreach (string name in appNames)
            {
                this.RunningApps.Add(name);
            }

            this.CanStopApp = false;
            if (this.RunningApps.Count > 0)
            {
                // Set the selected item.
                if ((currentSelection != null) &&
                    this.RunningApps.Contains(currentSelection))
                {
                    this.SelectedRunningApp = currentSelection;
                    this.CanStopApp = true;
                }
            }
        }
    }
}
