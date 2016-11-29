// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Net;
using Microsoft.Tools.WindowsDevicePortal;
using static Microsoft.Tools.WindowsDevicePortal.DevicePortal;

namespace HoloLensCommander
{
    /// <summary>
    /// Implementation of the Microsoft.Tools.WindowsDevicePortal.IDevicePortalConnection interface.
    /// </summary>
    class HoloLensDevicePortalConnection : IDevicePortalConnection
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="HoloLensDeviceConnection" /> class.
        /// </summary>
        /// <param name="userName">The user name used in the connection credentials.</param>
        /// <param name="password">The password used in the connection credentials.</param>
        /// <remarks>This constructor requires the HoloLens to be connected via USB at the time
        /// the connection is first established.</remarks>
        public HoloLensDevicePortalConnection(
            string userName,
            string password) : this(
            "localhost:10080", 
            userName, 
            password)
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="HoloLensDevicePortalConnection" /> class.
        /// </summary>
        /// <param name="address">The address of the device, including any port that may be required.</param>
        /// <param name="userName">The user name used in the connection credentials.</param>
        /// <param name="password">The password used in the connection credentials.</param>
        public HoloLensDevicePortalConnection(
            string address,
            string userName,
            string password)
        {
            // By default, HoloLens requires an HTTP connection unless we are communicating over USB.
            this.Connection = CreateUri(address, true);

            // Append "auto-" to the credentials to bypass CSRF token requirement on non-Get requests.
            this.Credentials = new NetworkCredential(string.Format("auto-{0}", userName), password);
        }

        /// <summary>
        /// Gets the Uri used to connect to the device.
        /// </summary>
        public Uri Connection
        { 
            get; 
            private set; 
        }

        /// <summary>
        /// Gets the credentials used to connect to the device.
        /// </summary>
        public NetworkCredential Credentials
        { 
            get; 
            private set; 
        }

        /// <summary>
        /// Gets the device's operating system family.
        /// </summary>
        public string Family
        { 
            get; 
            set;
        }

        /// <summary>
        /// Gets information about the device's operating system.
        /// </summary>
        public OperatingSystemInformation OsInfo
        { 
            get; 
            set;
        }

        /// <summary>
        /// Gets Uri used to connect to the device via Web Sockets.
        /// </summary>
        public Uri WebSocketConnection
        {
            get
            {
                if (this.Connection == null)
                {
                    return null;
                }

                // Convert the scheme from http[s] to ws[s].
                string scheme = this.Connection.Scheme.Equals("https", StringComparison.OrdinalIgnoreCase) ? "wss" : "ws";

                return new Uri(
                    string.Format("{0}://{1}",
                        scheme,
                        this.Connection.Authority));
            }
        }

        /// <summary>
        /// Updated the device connection Uri.
        /// </summary>
        /// <param name="requiresHttps">Should HTTPS be forced, regardless of the address provided?</param>
        public void UpdateConnection(bool requiresHttps)
        {
            this.Connection = CreateUri(Connection.Authority, requiresHttps);
        }

        /// <summary>
        /// Updates the device connection Uri.
        /// </summary>
        /// <param name="ipConfig">The IpConfiguration received from the Device Portal.</param>
        /// <param name="requiresHttps">Should HTTPS be forced, regardless of the address provided?</param>
        /// <param name="preservePort">Should the previous connection's port be preserved?</param>
        /// <remarks>This implementation of IDevicePortalConnection is designed specifically for HoloLens
        /// and ignores the preservePort parameter (behaving as if preserePort is always false).</remarks>
        public void UpdateConnection(
            IpConfiguration ipConfig, 
            bool requiresHttps,
            bool preservePort)
        {
            Uri newConnection = null;

            foreach (NetworkAdapterInfo adapter in ipConfig.Adapters)
            {
                foreach (IpAddressInfo addressInfo in adapter.IpAddresses)
                {
                    // We will use the first valid address.
                    if ((addressInfo.Address != "0.0.0.0") && !addressInfo.Address.StartsWith("169."))
                    {
                        newConnection = CreateUri(addressInfo.Address, requiresHttps);
                        break;
                    }
                }

                if (newConnection != null)
                {
                    this.Connection = newConnection;
                    break;
                }
            }
        }

        /// <summary>
        /// Creates a connection Uri using the appropriate Uri scheme.
        /// </summary>
        /// <param name="address">The address of the device.</param>
        /// <param name="requiresHttps">Should HTTPS be forced, regardless of the address provided?</param>
        /// <returns></returns>
        private Uri CreateUri(
            string address,
            bool requiresHttps)
        {
            return new Uri(
                String.Format("{0}://{1}", 
                    GetUriScheme(address, requiresHttps), 
                    address));
        }

        /// <summary>
        /// Gets the required Uri scheme based on the specified address.
        /// </summary>
        /// <param name="address">The device's IP address.</param>
        /// <param name="requiresHttps">Should HTTPS be forced, regardless of the address provided?</param>
        /// <returns>Returns "https" for addresses requiring security, "http" otherwise.</returns>
        private string GetUriScheme(
            string address,
            bool requiresHttps = true)
        {
            return (address.Contains("127.0.0.1") || 
                    address.Contains("localhost") || 
                    !requiresHttps) ? "http" : "https";
        }
    }
}
