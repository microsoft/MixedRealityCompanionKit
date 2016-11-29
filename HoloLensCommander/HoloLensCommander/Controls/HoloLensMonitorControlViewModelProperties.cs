// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using Windows.UI.Xaml;

namespace HoloLensCommander
{
    /// <summary>
    /// The view model for the HoloLensMonitorControl object.
    /// </summary>
    partial class HoloLensMonitorControlViewModel
    {
        /// <summary>
        /// Gets the address of this HoloLens.
        /// </summary>
        private string address;
        public string Address
        {
            get
            {
                return this.address;
            }
            
            private set
            {
                if (this.address != value)
                {
                    this.address = value;
                    this.NotifyPropertyChanged("Address");
                }
            }
        }

        /// <summary>
        /// Gets the battery level of this HoloLens, as a percentage remaining.
        /// </summary>
        /// <remarks>The returned string is formatted to two significant decimal places. If there is no device
        /// connected, the returned value will be "--"</remarks>
        private string batteryLevel = "";
        public string BatteryLevel
        {
            get
            {
                return this.batteryLevel;
            }
            
            private set
            {
                if (this.batteryLevel != value)
                {
                    this.batteryLevel = value;
                    this.NotifyPropertyChanged("BatteryLevel");
                }
            }
        }

        /// <summary>
        /// Gets or sets a value specifying whether or not a connection has been established to this HoloLens.
        /// </summary>
        /// <remarks>This value is based on a periodic heartbeat. Loss or reestablishment of the connection
        /// may not be immediately reflected in this value.</remarks>
        private bool isConnected = false;
        public bool IsConnected
        {
            get
            {
                return this.isConnected;
            }

            set
            {
                if (this.isConnected != value)
                {
                    this.isConnected = value;
                    this.NotifyPropertyChanged("IsConnected");
                }
            }
        }

        /// <summary>
        /// Gets or sets the user specified name associated with this HoloLens.
        /// </summary>
        private string name;
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
        /// Gets a value specifying whether or not this HoloLens is connected to an AC power source.
        /// </summary>
        private string powerIndicator = OnBatteryLabel;
        public string PowerIndicator
        {
            get
            {
                return this.powerIndicator;
            }
            
            private set
            {
                if (this.powerIndicator != value)
                {
                    this.powerIndicator = value;
                    this.NotifyPropertyChanged("PowerIndicator");
                }
            }
        }

        /// <summary>
        /// Gets or sets a string representation of the interpupliary distance recorded on this HoloLens.
        /// </summary>
        private string ipd = "";
        public string Ipd
        {
            get
            {
                return this.ipd;
            }

            set
            {
                if (this.ipd != value)
                {
                    this.ipd = value;
                    this.NotifyPropertyChanged("Ipd");
                }
            }
        }

        /// <summary>
        /// Gets or sets a value specifying whether or not the monitor control is selected in the UI.
        /// </summary>
        private bool isSelected = false;
        public bool IsSelected
        {
            get
            { 
                return this.isSelected;
            }

            set
            {
                if (this.isSelected != value)
                {
                    this.isSelected = value;
                    this.NotifyPropertyChanged("IsSelected");
                }
            }
        }

        /// <summary>
        /// Gets a status message to display to the user.
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
                if (!this.statusMessage.Equals(value))
                {
                    this.statusMessage = value;
                    this.NotifyPropertyChanged("StatusMessage");
                }
            }
        }

        /// <summary>
        /// Gets a value describing the visibility of the thermal alert.
        /// </summary>
        private Visibility thermalStatus = Visibility.Collapsed;
        public Visibility ThermalStatus
        {
            get
            {
                return this.thermalStatus;
            }
            
            private set
            {
                if (this.thermalStatus != value)
                {
                    this.thermalStatus = value;
                    this.NotifyPropertyChanged("ThermalStatus");
                }
            }
        }
    }
}
