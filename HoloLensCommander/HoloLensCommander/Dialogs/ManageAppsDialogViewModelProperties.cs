// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Collections.ObjectModel;

namespace HoloLensCommander
{
    /// <summary>
    /// The view model for the ManageAppsDialog object.
    /// </summary>
    partial class ManageAppsDialogViewModel
    {
        /// <summary>
        /// Gets a value indicating whether or not the user has selected an application to run.
        /// </summary>
        private bool canRunApp = false;
        public bool CanRunApp
        {
            get
            {
                return this.canRunApp;
            }

            private set
            {
                if (this.canRunApp != value)
                {
                    this.canRunApp = value;
                    this.NotifyPropertyChanged("CanRunApp");
                }
            }
        }

        /// <summary>
        /// Gets a value indicating whether or not the user has selected an application to close.
        /// </summary>
        private bool canStopApp = false;
        public bool CanStopApp
        {
            get
            {
                return this.canStopApp;
            }

            private set
            {
                if (this.canStopApp != value)
                {
                    this.canStopApp = value;
                    this.NotifyPropertyChanged("CanStopApp");
                }
            }
        }

        /// <summary>
        /// Gets the address of the device on which applications are being managed.
        /// </summary>
        public string DeviceAddress
        {
            get
            {
                return this.deviceMonitor.Address;
            }
        }

        /// <summary>
        /// Gets the collection of installed applications.
        /// </summary>
        public ObservableCollection<string> InstalledApps
        { get; private set; }

        /// <summary>
        /// Gets the collection of running applications.
        /// </summary>
        public ObservableCollection<string> RunningApps
        { get; private set; }

        /// <summary>
        /// Gets the selected application in the installed apps list.
        /// </summary>
        private string selectedInstalledApp = null;
        public string SelectedInstalledApp
        {
            get
            {
                return this.selectedInstalledApp;
            }

            set
            {
                if (this.selectedInstalledApp != value)
                {
                    this.selectedInstalledApp = value;
                    this.NotifyPropertyChanged("SelectedInstalledApp");
                    this.UpdateCanRunApp();
                }
            }
        }

        /// <summary>
        /// Gets the selected application in the running apps list.
        /// </summary>
        private string selectedRunningApp = null;
        public string SelectedRunningApp
        {
            get
            {
                return this.selectedRunningApp;
            }

            set
            {
                if (this.selectedRunningApp != value)
                {
                    this.selectedRunningApp = value;
                    this.NotifyPropertyChanged("SelectedRunningApp");
                    this.UpdateCanStopApp();
                }
            }
        }

        /// <summary>
        /// Gets the message to be displayed to the user.
        /// </summary>
        private string statusMessage = string.Empty;
        public string StatusMessage
        {
            get
            {
                return this.statusMessage;
            }

            private set
            {
                if (this.statusMessage != value)
                {
                    this.statusMessage = value;
                    this.NotifyPropertyChanged("StatusMessage");
                }
            }
        }

        /// <summary>
        /// Updates the value of the CanRunApp property.
        /// </summary>
        private void UpdateCanRunApp()
        {
            if ((this.InstalledApps.Count > 0) &&
                !string.IsNullOrWhiteSpace(this.SelectedInstalledApp))
            {
                this.CanRunApp = true;
            }
            else
            {
                this.CanRunApp = false;
            }
        }

        /// <summary>
        /// Updates the value of the CanStopApp fsproperty.
        /// </summary>
        private void UpdateCanStopApp()
        {
            if ((this.InstalledApps.Count > 0) &&
                !string.IsNullOrWhiteSpace(this.SelectedRunningApp))
            {
                this.CanStopApp = true;
            }
            else
            {
                this.CanStopApp = false;
            }
        }
    }
}
