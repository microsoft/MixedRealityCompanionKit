// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using Windows.UI;

namespace HoloLensCommander
{
    public class ConnectionInformation
    {
        /// <summary>
        /// The address of the device.
        /// </summary>
        public string Address;

        /// <summary>
        /// Descriptive text, typically a name or location, assigned to the device.
        /// </summary>
        public string Name;

        /// <summary>
        /// Initializes a new instance of the <see cref="ConnectionInformation" /> class.
        /// </summary>
        public ConnectionInformation() : this(string.Empty)
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="ConnectionInformation" /> class.
        /// </summary>
        /// <param name="address">The address of the device.</param>
        public ConnectionInformation(string address) : this(
            address,
            string.Empty)
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="ConnectionInformation" /> class.
        /// </summary>
        /// <param name="address">The address of the device.</param>
        /// <param name="name">Descriptive text assigned to the device.</param>
        public ConnectionInformation(
            string address,
            string name)
        {
            this.Address = address;
            this.Name = name;
        }

    }
}
