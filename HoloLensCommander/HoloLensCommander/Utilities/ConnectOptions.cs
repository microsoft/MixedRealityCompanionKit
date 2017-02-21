// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

namespace HoloLensCommander
{
    /// <summary>
    /// Object encapsulating the options for connecting to a HoloLens.
    /// </summary>
    public class ConnectOptions
    {
        /// <summary>
        /// The IP address to be used for the connection.
        /// </summary>
        public string Address;

        /// <summary>
        /// The name of the user.
        /// </summary>
        public string UserName;

        /// <summary>
        /// The password associated with the user account.
        /// </summary>
        public string Password;

        /// <summary>
        /// Specifies whether or not the connection is to be updated.
        /// </summary>
        public bool UpdateConnection;

        /// <summary>
        /// Initializes a new instance of the <see cref="ConnectOptions" /> class.
        /// </summary>
        public ConnectOptions() : this(
            string.Empty,
            string.Empty,
            string.Empty,
            true)
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="ConnectOptions" /> class.
        /// </summary>
        /// <param name="address">The address to be used for the connection.</param>
        public ConnectOptions(
            string address,
            string userName,
            string password,
            bool updateConnection)
        {
            this.Address = address;
            this.UserName = userName;
            this.Password = password;
            this.UpdateConnection = updateConnection;
        }
    }
}
