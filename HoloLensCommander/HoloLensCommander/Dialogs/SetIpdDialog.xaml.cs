// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using Windows.UI.Xaml.Controls;

namespace HoloLensCommander
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class SetIpdDialog : ContentDialog
    {
        /// <summary>
        /// Object containing the user specific information (ex: IPD) that is to be associated with a device.
        /// </summary>
        private UserInformation userInformation;

        /// <summary>
        /// Initializes a new instance of the <see cref="SetIpdDialog" /> class.
        /// </summary>
        /// <param name="address">The address of the device.</param>
        /// <param name="userInfo">UserInformation object containing information associated with the device.</param>
        public SetIpdDialog(
            string address,
            UserInformation userInfo)
        {
            this.userInformation = userInfo;

            this.DataContext = new SetIpdDialogViewModel(
                address,
                userInfo);
            this.InitializeComponent();
        }

        private void ContentDialog_OkClick(ContentDialog sender, ContentDialogButtonClickEventArgs args)
        {
            // Return the data
            ((SetIpdDialogViewModel)this.DataContext).UpdateUserData(this.userInformation);
        }
    }
}
