// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.ComponentModel;

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

        private string networkKey = string.Empty;
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

        private string ssid = string.Empty;
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
    }
}
