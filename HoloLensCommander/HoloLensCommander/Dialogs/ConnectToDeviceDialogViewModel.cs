// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.ComponentModel;

namespace HoloLensCommander
{
    /// <summary>
    /// The view model for the ConnectToDeviceDialog object.
    /// </summary>
    public partial class ConnectToDeviceDialogViewModel : INotifyPropertyChanged
    {
        /// <summary>
        /// Event that is notified when a property value has changed.
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;

        /// <summary>
        /// Initializes a new instance of the <see cref="ConnectToDeviceDialogViewModel" /> class.
        /// </summary>
        public ConnectToDeviceDialogViewModel(ConnectOptions options)
        {
            this.Address = options.Address;
            this.UpdateConnection = options.UpdateConnection;
        }

        /// <summary>
        /// Sends the PropertyChanged events to registered handlers.
        /// </summary>
        /// <param name="propertyName">The name of property that has changed.</param>
        private void NotifyPropertyChanged(string propertyName)
        {
            this.PropertyChanged?.Invoke(
                this,
                new PropertyChangedEventArgs(propertyName));
        }

        /// <summary>
        /// Update's the user selected data.
        /// </summary>
        /// <param name="connectOptions">The options to be used when connecting to the device.</param>
        internal void UpdateUserData(ConnectOptions connectOptions)
        {
            connectOptions.Address = this.Address;
            connectOptions.Ssid = this.Ssid;
            connectOptions.NetworkKey = this.NetworkKey;
            connectOptions.UpdateConnection = this.UpdateConnection;
        }
    }
}
