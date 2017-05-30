// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Threading.Tasks;
using Microsoft.Tools.WindowsDevicePortal;
using static Microsoft.Tools.WindowsDevicePortal.DevicePortal;

namespace HoloLensCommander
{
    /// <summary>
    /// Class that provides the relevant functionality of the Windows Device Portal.
    /// </summary>
    public partial class DeviceMonitor
    {
        /// <summary>
        /// Returns the address (and port if explicitly specified) used to communicate with the device.
        /// </summary>
        public string Address
        {
            get
            {
                return this.devicePortal.Address;
            }
        }

        public Uri DevicePortalUri
        {
            get 
            {
                return this.devicePortalConnection.Connection;
            }
        }

        /// <summary>
        /// Returns the most recently cached battery state data.
        /// </summary>
        public BatteryState BatteryState
        { get; private set; }

        /// <summary>
        /// Returns the family of the attached device.
        /// </summary>
        public string DeviceFamily
        {
            get
            {
                return this.devicePortal.DeviceFamily;
            }
        }

        /// <summary>
        /// Gets or sets the current heartbeat interval, in seconds.
        /// </summary>
        private float heartbeatInterval;
        public float HeartbeatInterval
        {
            get
            {
                return this.heartbeatInterval;
            }

            set
            {
                if (this.heartbeatInterval != value)
                {
                    if ((value > Settings.MaxHeartbeatInterval) ||
                        (value < Settings.MinHeartbeatInterval))
                    {
                        throw new ArgumentOutOfRangeException(
                            string.Format(
                            "HeartbeatInterval must be between {0} and {1}, inclusive.",
                            Settings.MinHeartbeatInterval,
                            Settings.MaxHeartbeatInterval));
                    }

                    this.heartbeatInterval = value;

                    if (this.heartbeatTimerRunning)
                    {
                        // Force a heartbeat check now.
                        // We want the new interval to take effect immediately.

                        // Assigning the return value of CheckHeartbeatAsync to a Task object to avoid 
                        // warning 4014 (call is not awaited).
                        Task t = this.CheckHeartbeatAsync();
                    }
                }
            }
        }

        /// <summary>
        /// Returns the most recently cached value of the user's interpupilary distance.
        /// </summary>
        public float Ipd
        { get; private set; }

        /// <summary>
        /// Returns the name of the connected device.
        /// </summary>
        public string MachineName
        {
            get
            {
                return this.devicePortalConnection.OsInfo.Name;
            }
        }

        /// <summary>
        /// Returns the version of the operating system.
        /// </summary>
        public string OperatingSystemVersion
        {
            get
            {
                return this.devicePortal.OperatingSystemVersion;
            }
        }

        /// <summary>
        /// Returns the most recently cached device name.
        /// </summary>
        public string Name
        { get; private set; }

        /// <summary>
        /// Returns the most recently cached device network (ssid) name.
        /// </summary>
        public string NetworkName
        { get; private set; }

        public DevicePortalPlatforms Platform
        {
            get
            {
                return this.devicePortal.Platform;
            }
        }

        public string PlatformName
        {
            get
            {
                return this.devicePortal.PlatformName;
            }
        }

        /// <summary>
        /// Returns the most recently cached thermal stage.
        /// </summary>
        public ThermalStages ThermalStage
        { get; private set; }

    }
}
