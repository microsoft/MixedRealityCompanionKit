// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

namespace HoloLensCommander
{
    /// <summary>
    /// Object encapsulating user based information associated with a specific device.
    /// </summary>
    public class UserInformation
    {
        /// <summary>
        /// The user's interpupilary distance.
        /// </summary>
        public float Ipd;

        /// <summary>
        /// Initializes a new instance of the <see cref="UserInformation" /> class.
        /// </summary>
        public UserInformation() : this(
            0.0f)
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="UserInformation" /> class.
        /// </summary>
        /// <param ipd="name">Interpupilary distance.</param>
        public UserInformation(
            float ipd)
        {
            this.Ipd = ipd;
        }
    }
}
