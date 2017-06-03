// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using Windows.UI.Xaml.Controls;

namespace HoloLensCommander
{
    /// <summary>
    /// The class that implements the dialog used to connect to a device.
    /// </summary>
    public sealed partial class ConnectToDeviceDialog : ContentDialog
    {
        /// <summary>
        /// Object containing the connection options to be used.
        /// </summary>
        private ConnectOptions connectOptions;

        /// <summary>
        /// Initializes a new instance of the <see cref="ConnectToDeviceDialog" /> class.
        /// </summary>
        /// <param name="options">Options to be used when connectiong to a device.</param>
        public ConnectToDeviceDialog(ConnectOptions options)
        {
            this.connectOptions = options;
            this.DataContext = new ConnectToDeviceDialogViewModel(options);

            this.InitializeComponent();
        }

        /// <summary>
        /// Handle click event for the ok button.
        /// </summary>
        /// <param name="sender">The object sending the event.</param>
        /// <param name="args">Event arguments.</param>
        private void ContentDialog_OkClick(
            ContentDialog sender,
            ContentDialogButtonClickEventArgs args)
        {
            // Return the user's selections.
            ((ConnectToDeviceDialogViewModel)this.DataContext).UpdateUserData(this.connectOptions);
        }
    }
}
