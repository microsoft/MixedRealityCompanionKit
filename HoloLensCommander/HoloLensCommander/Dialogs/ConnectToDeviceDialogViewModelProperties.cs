// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.ComponentModel;
using Windows.UI.Xaml;

namespace HoloLensCommander
{
    /// <summary>
    /// The view model for the ConnectToDeviceDialog object.
    /// </summary>
    public partial class ConnectToDeviceDialogViewModel
    {
        /// <summary>
        /// Gets or sets the address to be used to make the connection to the device
        /// </summary>
        private string address = string.Empty;
        public string Address
        {
            get
            {
                return this.address;
            }

            set
            {
                if (this.address != value)
                {
                    this.address = value;
                    this.NotifyPropertyChanged("Address");
                }
            }
        }

        /// <summary>
        /// Gets or sets a value indicating whether or not we are attempting to connect
        /// to a Desktop PC
        /// </summary>
        private bool connectingToDesktopPC = false;
        public bool ConnectingToDesktopPC
        {
            get
            {
                return this.connectingToDesktopPC;
            }

            set
            {
                if (this.connectingToDesktopPC != value)
                {
                    this.connectingToDesktopPC = value;
                    this.NotifyPropertyChanged("ConnectingToDesktopPC");
                }
            }
        }

        /// <summary>
        /// Gets or sets a value indicating whether or not we wish the device to be
        /// renamed upon connection
        /// </summary>
        private bool deployNameToDevice = false;
        public bool DeployNameToDevice
        {
            get
            {
                return this.deployNameToDevice;
            }

            set
            {
                if (this.deployNameToDevice != value)
                {
                    this.deployNameToDevice = value;
                    this.NotifyPropertyChanged("DeployNameToDevice");
                }
            }
        }

        /// <summary>
        /// Gets or sets the name for the device
        /// </summary>
        private string name = "";
        public string Name
        {
            get
            {
                return this.name;
            }

            set
            {
                if (this.name != value)
                {
                    this.name = value;
                    this.NotifyPropertyChanged("Name");
                }
            }
        }

        /// <summary>
        /// Gets or sets the network key used to connect to the desired SSID
        /// </summary>
        private string networkKey = "";
        public string NetworkKey
        {
            get
            {
                return this.networkKey;
            }

            set
            {
                if (this.networkKey != value)
                {
                    this.networkKey = value;
                    this.NotifyPropertyChanged("NetworkKey");
                }
            }
        }

        /// <summary>
        /// Gets or sets the password to be used to make the connection to the device
        /// </summary>
        private string password = string.Empty;
        public string Password
        {
            get
            {
                return this.password;
            }

            set
            {
                if (this.password != value)
                {
                    this.password = value;
                    this.NotifyPropertyChanged("Password");
                }
            }
        }

        /// <summary>
        /// Gets a value indicating if the credentials ui should be visible 
        /// </summary>
        private Visibility showCredentials = Visibility.Collapsed;
        public Visibility ShowCredentials
        {
            get 
            {
                return this.showCredentials;
            }

            private set
            {
                if (this.showCredentials != value)
                {
                    this.showCredentials = value;
                    this.NotifyPropertyChanged("ShowCredentials");
                }
            }
        }

        /// <summary>
        /// Gets the label to display on the show/hide credentials button
        /// </summary>
        private string showHideCredentialsButtonLabel = ExpandButtonLabel;
        public string ShowHideCredentialsButtonLabel
        {
            get 
            {
                return this.showHideCredentialsButtonLabel;
            }

            private set
            {
                if (this.showHideCredentialsButtonLabel != value)
                {
                    this.showHideCredentialsButtonLabel = value;
                    this.NotifyPropertyChanged("ShowHideCredentialsButtonLabel");
                }
            }
        }

        /// <summary>
        /// Gets the label to display on the show/hide credentials button
        /// </summary>
        private string showHideNetworkSettingsButtonLabel = ExpandButtonLabel;
        public string ShowHideNetworkSettingsButtonLabel
        {
            get
            {
                return this.showHideNetworkSettingsButtonLabel;
            }

            private set
            {
                if (this.showHideNetworkSettingsButtonLabel != value)
                {
                    this.showHideNetworkSettingsButtonLabel = value;
                    this.NotifyPropertyChanged("ShowHideNetworkSettingsButtonLabel");
                }
            }
        }
        /// <summary>
        /// Gets a value indicating if the network settings ui should be visible 
        /// </summary>
        private Visibility showNetworkSettings = Visibility.Collapsed;
        public Visibility ShowNetworkSettings
        {
            get
            {
                return this.showNetworkSettings;
            }

            private set
            {
                if (this.showNetworkSettings != value)
                {
                    this.showNetworkSettings = value;
                    this.NotifyPropertyChanged("ShowNetworkSettings");
                }
            }
        }

        /// <summary>
        /// Gets or sets the desired network access point SSID
        /// </summary>
        private string ssid = "";
        public string Ssid
        {
            get
            {
                return this.ssid;
            }

            set
            {
                if (this.ssid != value)
                {
                    this.ssid = value;
                    this.NotifyPropertyChanged("Ssid");
                }
            }
        }

        /// <summary>
        /// Gets or sets a value indicating whether or not the device connection should be updated
        /// </summary>
        private bool updateConnection = false;
        public bool UpdateConnection
        {
            get 
            {
                return this.updateConnection;
            } 

            set
            {
                if (this.updateConnection != value)
                {
                    this.updateConnection = value;
                    this.NotifyPropertyChanged("UpdateConnection");
                }
            }
        }

        /// <summary>
        /// Gets or sets the user name to be used to make the connection to the device
        /// </summary>
        private string userName = string.Empty;
        public string UserName
        {
            get
            {
                return this.userName;
            }

            set
            {
                if (this.userName != value)
                {
                    this.userName = value;
                    this.NotifyPropertyChanged("UserName");
                }
            }
        }
    }
}
