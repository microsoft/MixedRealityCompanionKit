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
        /// Gets or sets the address to be used to make the connection to the HoloLens
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
                    NotifyPropertyChanged("Address");
                }
            }
        }

        private bool updateConnection = true;
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
                    NotifyPropertyChanged("UpdateConnection");
                }
            }
        }
    }
}
