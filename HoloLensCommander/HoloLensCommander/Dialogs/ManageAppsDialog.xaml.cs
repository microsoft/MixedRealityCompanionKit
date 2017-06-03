// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using Windows.UI.Xaml.Controls;

namespace HoloLensCommander
{
    /// <summary>
    /// The class that implements the application management dialog.
    /// </summary>
    public sealed partial class ManageAppsDialog : ContentDialog
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="ManageAppsDialog" /> class.
        /// </summary>
        /// <param name="monitor">The DeviceMonitor that is responsible for communication with the device.</param>
        /// <param name="monitorControl">The DeviceMonitorControl that launched this dialog.</param>
        public ManageAppsDialog(
            DeviceMonitor monitor,
            DeviceMonitorControl monitorControl)
        {
            this.DataContext = new ManageAppsDialogViewModel(
                monitor, 
                monitorControl);
            this.InitializeComponent();
        }

        /// <summary>
        /// Handle click event for the close button.
        /// </summary>
        /// <param name="sender">The object sending the event.</param>
        /// <param name="args">Event arguments.</param>
        private void ContentDialog_CloseClick(
            ContentDialog sender, 
            ContentDialogButtonClickEventArgs args)
        {
            this.Close();
        }

        /// <summary>
        /// The dialog is closing, perform any required cleanup.
        /// </summary>
        private void Close()
        {
            ((ManageAppsDialogViewModel)this.DataContext).Closing();
        }
    }
}
