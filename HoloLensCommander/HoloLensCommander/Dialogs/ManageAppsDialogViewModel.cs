// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Threading.Tasks;
using Microsoft.Tools.WindowsDevicePortal;

namespace HoloLensCommander
{
    /// <summary>
    /// The view model for the ManageAppsDialog fsobject.
    /// </summary>
    partial class ManageAppsDialogViewModel : INotifyPropertyChanged
    {
        /// <summary>
        /// The HoloLensMonitor object responsible for communication with this HoloLens.
        /// </summary>
        private HoloLensMonitor holoLensMonitor;

        /// <summary>
        /// The HoloLensMonitorControl object that launched this dialog.
        /// </summary>
        private HoloLensMonitorControl holoLensMonitorControl;

        /// <summary>
        /// Event that is notified when a property value has changed.
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;

        /// <summary>
        /// Initializes a new instance of the <see cref="ManageAppsDialogViewModel" /> class.
        /// </summary>
        /// <param name="monitor">The HoloLensMonitor responsible for communication with this HoloLens.</param>
        /// <param name="monitorControl">Instance of the control that launched this dialog.</param>
        public ManageAppsDialogViewModel(
            HoloLensMonitor monitor,
            HoloLensMonitorControl monitorControl)
        {
            this.holoLensMonitor = monitor;
            this.holoLensMonitorControl = monitorControl;
            this.holoLensMonitor.AppInstallStatus += HoloLensMonitor_AppInstallStatus;

            this.InstalledApps = new ObservableCollection<string>();
            this.RunningApps = new ObservableCollection<string>();

            this.RegisterCommands();

            // Assigning the return value of the following methods to a Task object to avoid 
            // warning 4014 (call is not awaited).
            Task t = this.RefreshInstalledAppsAsync();
            t = this.RefreshRunningAppsAsync();
        }

        /// <summary>
        /// Performs required cleanup when the dialog is being closed.
        /// </summary>
        internal void Closing()
        {
            this.holoLensMonitor.AppInstallStatus -= HoloLensMonitor_AppInstallStatus;
        }

        /// <summary>
        /// Handles the ApplicationInstallStatus event.
        /// </summary>
        /// <param name="sender">The object which sent this event.</param>
        /// <param name="args">Event arguments.</param>
        private void HoloLensMonitor_AppInstallStatus(
            HoloLensMonitor sender, 
            ApplicationInstallStatusEventArgs args)
        {
            if (args.Status == ApplicationInstallStatus.Completed)
            {
                // Assigning the return value of RefreshInstalledAppsAsync to a Task object to avoid 
                // warning 4014 (call is not awaited).
                Task t = this.RefreshInstalledAppsAsync();
            }
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
        /// Registers the command supported by this object.
        /// </summary>
        private void RegisterCommands()
        {
            this.CloseAllAppsCommand = new Command(
                async (parameter) =>
                {
                    await this.CloseAllRunningAppsAsync();
                    await this.RefreshRunningAppsAsync();
                });

            this.CloseAppCommand = new Command(
                async (parameter) =>
                {
                    await this.CloseAppAsync();
                    await this.RefreshRunningAppsAsync();
                });

            this.LaunchAppCommand = new Command(
                async (parameter) =>
                {
                    await this.LaunchAppAsync();
                    await this.RefreshRunningAppsAsync();
                });

            this.RefreshInstalledAppsCommand = new Command(
                async (parameter) =>
                {
                    await this.RefreshInstalledAppsAsync();
                });

            this.RefreshRunningAppsCommand = new Command(
                async (parameter) =>
                {
                    await this.RefreshRunningAppsAsync();
                });

            this.UninstallAppCommand = new Command(
                async (parameter) =>
                {
                    await this.UninstallAppAsync();
                    await this.RefreshInstalledAppsAsync();
                });
        }
    }
}
