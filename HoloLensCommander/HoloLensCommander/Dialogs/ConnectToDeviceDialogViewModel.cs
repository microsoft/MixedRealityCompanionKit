// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.ComponentModel;
using Windows.UI.Xaml;

namespace HoloLensCommander
{
    /// <summary>
    /// The view model for the ConnectToDeviceDialog object.
    /// </summary>
    public partial class ConnectToDeviceDialogViewModel : INotifyPropertyChanged
    {
        private static readonly string ExpandButtonLabel = "";     // + sign
        private static readonly string CollapseButtonLabel = "";   // -+ sign

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
            this.ConnectingToDesktopPC = options.ConnectingToDesktopPC;

            this.Name = options.Name;
            this.DeployNameToDevice = options.DeployNameToDevice;

            // Set the opposite of what we want and call the toggle method.
            this.ShowCredentials = options.ExpandCredentials ? Visibility.Collapsed : Visibility.Visible;
            this.ShowHideCredentials();

            this.UserName = options.UserName;
            this.Password = options.Password;

            // Set the opposite of what we want and call the toggle method.
            this.ShowNetworkSettings = options.ExpandNetworkSettings ? Visibility.Collapsed : Visibility.Visible;
            this.ShowHideNetworkSettings();

            this.Ssid = options.Ssid;
            this.NetworkKey = options.NetworkKey;

            this.UseInstalledCertificate = options.UseInstalledCertificate;

            this.UpdateConnection = options.UpdateConnection;

            this.RegisterCommands();
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
        /// Registers commands supported by this object.
        /// </summary>
        private void RegisterCommands()
        {
            this.ShowHideCredentialsCommand = new Command(
                (parameter) =>
                {
                    this.ShowHideCredentials();
                });

            this.ShowHideNetworkSettingsCommand = new Command(
                (parameter) =>
                {
                    this.ShowHideNetworkSettings();
                });
        }

        /// <summary>
        /// Update's the user selected data.
        /// </summary>
        /// <param name="connectOptions">The options to be used when connecting to the device.</param>
        internal void UpdateUserData(ConnectOptions connectOptions)
        {
            connectOptions.Address = this.Address;
            connectOptions.ConnectingToDesktopPC = this.ConnectingToDesktopPC;

            connectOptions.Name = this.Name;
            connectOptions.DeployNameToDevice = this.DeployNameToDevice;

            connectOptions.UserName = this.UserName;
            connectOptions.Password = this.Password;

            connectOptions.Ssid = this.Ssid;
            connectOptions.NetworkKey = this.NetworkKey;

            connectOptions.UseInstalledCertificate = this.UseInstalledCertificate;

            connectOptions.UpdateConnection = this.UpdateConnection;
        }
    }
}
