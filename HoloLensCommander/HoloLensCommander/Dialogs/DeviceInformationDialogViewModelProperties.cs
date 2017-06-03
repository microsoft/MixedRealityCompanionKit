// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Threading.Tasks;

namespace HoloLensCommander
{
    /// <summary>
    /// The view model for the DeviceInformationDialog object.
    /// </summary>
    public partial class DeviceInformationDialogViewModel
    {
        /// <summary>
        /// Gets the address of the device.
        /// </summary>
        public string DeviceAddress
        {
            get
            {
                return this.deviceMonitor.Address;
            }
        }
        
        /// <summary>
        /// Gets the machine name of the device.
        /// </summary>
        public string MachineName
        {
            get 
            {
                return this.deviceMonitor.MachineName;
            }
        }

        /// <summary>
        /// Gets the operating system platform installed on the device.
        /// </summary>
        public string OsPlatform
        {
            get 
            {
                return this.deviceMonitor.PlatformName;
            }
        }

        /// <summary>
        /// Gets the operating system version installed on the device.
        /// </summary>
        public string OsVersion
        {
            get 
            {
                return this.deviceMonitor.OperatingSystemVersion;
            }
        }
    }
}
