// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using Windows.UI.Xaml.Controls;

namespace HoloLensCommander
{
    /// <summary>
    /// The class that implements the dialog used to tag the connected device with information useful to the user
    /// (ex: descriptive text, color, etc).
    /// </summary>
    public sealed partial class TagDeviceDialog : ContentDialog
    {
        /// <summary>
        /// Object containing the information that is to be associated with a device.
        /// </summary>
        private TagInformation tagInformation;

        /// <summary>
        /// Initializes a new instance of the <see cref="TagDeviceDialog" /> class.
        /// </summary>
        /// <param name="address">The address of device.</param>
        /// <param name="tagInfo">TagInformation object containing information associated with the device.</param>
        public TagDeviceDialog(
            string address,
            TagInformation tagInfo)
        {
            this.tagInformation = tagInfo;

            this.DataContext = new TagDeviceDialogViewModel(
                address,
                tagInfo);
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
            ((TagDeviceDialogViewModel)this.DataContext).UpdateUserData(this.tagInformation);
        }
    }
}
