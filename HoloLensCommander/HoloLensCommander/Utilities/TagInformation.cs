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
        /// A descriptive name assigned to the device.
        /// </summary>
        public string Name;

        /// <summary>
        /// Initializes a new instance of the <see cref="TagInformation" /> class.
        /// </summary>
        public TagInformation() : this(
            string.Empty)
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="TagInformation" /> class.
        /// </summary>
        /// <param name="name">The descriptive name associated with the device.</param>
        public TagInformation(
            string name)
        {
            this.Name = name;
        }
    }
}
