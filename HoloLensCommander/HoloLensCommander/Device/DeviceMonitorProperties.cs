// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
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
        /// Gets or set the time, in seconds, between heartbeat checks.
        /// </summary>
        public float HeartbeatIntervalSeconds
        {
            get 
            {
                return this.heartbeatInterval;
            }

            set
            {
                if ((value < MinimumHeartbeatIntervalSeconds) || (value > MaximumHeartbeatIntervalSeconds))
                {
                    throw new ArgumentException(String.Format("Heartbeat inverval must be between {0} and {1}, inclusive", 
                                                MinimumHeartbeatIntervalSeconds, MaximumHeartbeatIntervalSeconds));
                }

                this.heartbeatInterval = value;
                // TODO:
                //if (heartbeatTimer != null)
                //{
                //    heartbeatTimer.Change((Int32)(_heartbeatInterval * 1000.0f), Timeout.Infinite);
                //}
            }
        }

        /// <summary>
        /// Returns the most recently cached value of the user's interpupilary distance.
        /// </summary>
        public float Ipd
        { get; private set; }

        /// <summary>
        /// Get or set the cached name of the connected device.
        /// </summary>
        private string machineName = "";
        public string MachineName
        {
            get
            {
                return this.machineName;
            }

            private set
            {
                if (this.machineName != value)
                {
                    this.machineName = value;
                }
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
