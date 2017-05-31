// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

namespace HoloLensCommander
{
    /// <summary>
    /// Object encapsulating information associated (tagged) with a specific device.
    /// </summary>
    public class TagInformation
    {
        /// <summary>
        /// Indicates whether or not the new name should be sent to the device.
        /// </summary>
        /// <remarks>Note: Deploying the name will trigger a reboot of the device.</remarks>
        public bool DeployNameToDevice;

        /// <summary>
        /// A descriptive name assigned to the device.
        /// </summary>
        public string Name;

        /// <summary>
        /// Initializes a new instance of the <see cref="TagInformation" /> class.
        /// </summary>
        public TagInformation() : this(
            string.Empty,
            false)
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="TagInformation" /> class.
        /// </summary>
        /// <param name="name">A name associated with the device.</param>
        public TagInformation(
            string name) : this(
            name,
            false)
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="TagInformation" /> class.
        /// </summary>
        /// <param name="name">A name associated with the device.</param>
        /// <param name="deployNameToDevice">True to update the name on the device, false otherwise.</param>
        public TagInformation(
            string name,
            bool deployNameToDevice)
        {
            this.Name = name;
            this.DeployNameToDevice = deployNameToDevice;
        }
    }
}
