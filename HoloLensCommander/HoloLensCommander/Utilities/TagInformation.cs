// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

namespace HoloLensCommander
{
    /// <summary>
    /// Object encapsulating information associated (tagged) with a specific HoloLens.
    /// </summary>
    public class TagInformation
    {
        /// <summary>
        /// The user's interpupilary distance.
        /// </summary>
        public float Ipd;

        /// <summary>
        /// A descriptive name assigned to the HoloLens.
        /// </summary>
        public string Name;

        /// <summary>
        /// Initializes a new instance of the <see cref="TagInformation" /> class.
        /// </summary>
        public TagInformation() : this(
            string.Empty,
            0f)
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="TagInformation" /> class.
        /// </summary>
        /// <param name="name">The descriptive name associated with the HoloLens.</param>
        /// <param name="ipd">The interpupilary distance to be sent to the HoloLens.</param>
        public TagInformation(
            string name,
            float ipd)
            {
                this.Name = name;
                this.Ipd = ipd;
            }
    }
}
