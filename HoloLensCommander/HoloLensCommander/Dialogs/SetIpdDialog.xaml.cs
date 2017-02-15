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
        /// Object containing the information that is to be associated with a HoloLens.
        /// </summary>
        private UserInformation userInformation;

        /// <summary>
        /// Initializes a new instance of the <see cref="SetIpdDialog" /> class.
        /// </summary>
        /// <param name="userInfo">TagInformation object containing information associated with the HoloLens.</param>
        public SetIpdDialog(UserInformation userInfo)
        {
            this.userInformation = userInfo;

            this.DataContext = new SetIpdDialogViewModel(userInfo);
            this.InitializeComponent();
        }

        private void ContentDialog_OkClick(ContentDialog sender, ContentDialogButtonClickEventArgs args)
        {
            // Send the update to the HoloLens
            ((SetIpdDialogViewModel)this.DataContext).UpdateUserData(this.userInformation);
        }
    }
}
