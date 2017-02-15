// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Threading.Tasks;

namespace HoloLensCommander
{
    /// <summary>
    /// The view model for the HoloLensInformationDialog object.
    /// </summary>
    public partial class HoloLensInformationDialogViewModel
    {
        /// <summary>
        /// Gets the address of the HoloLens.
        /// </summary>
        public string DeviceAddress
        {
            get
            {
                return this.holoLensMonitor.Address;
            }
        }
        
        /// <summary>
        /// Gets the machine name of the HoloLens.
        /// </summary>
        public string MachineName
        {
            get 
            {
                Task<string> t = this.holoLensMonitor.GetMachineNameAsync();
                t.ConfigureAwait(false);
                t.Wait();
                return t.Result;
            }
        }

        /// <summary>
        /// Gets the operating system platform installed on the HoloLens.
        /// </summary>
        public string OsPlatformName
        {
            get 
            {
                return this.holoLensMonitor.PlatformName;
            }
        }

        /// <summary>
        /// Gets the operating system version installed on the HoloLens.
        /// </summary>
        public string OsVersion
        {
            get 
            {
                return this.holoLensMonitor.OperatingSystemVersion;
            }
        }
    }
}
