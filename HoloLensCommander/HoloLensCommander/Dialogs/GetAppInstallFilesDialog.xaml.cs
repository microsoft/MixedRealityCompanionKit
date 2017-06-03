// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

namespace HoloLensCommander
{
    /// <summary>
    /// The class that implements the select application files dialog.
    /// </summary>
    public sealed partial class GetAppInstallFilesDialog : ContentDialog
    {
        /// <summary>
        /// Object containing the file(s) required to install an application.
        /// </summary>
        private AppInstallFiles appInstallFiles;

        /// <summary>
        /// Initializes a new instance of the <see cref="GetAppInstallFilesDialog" /> class.
        /// </summary>
        /// <param name="installFiles">The object in which the user's selected file(s) will be returned to the caller.</param>
        public GetAppInstallFilesDialog(AppInstallFiles installFiles)
        {
            this.appInstallFiles = installFiles;
            this.DataContext = new GetAppInstallFilesDialogViewModel();
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
            ((GetAppInstallFilesDialogViewModel)this.DataContext).UpdateUserData(this.appInstallFiles);
        }

        /// <summary>
        /// Handle check event for the dependencies radio buttons.
        /// </summary>
        /// <param name="sender">The object sending the event.</param>
        /// <param name="e">Event arguments.</param>
        private void RadioButton_Checked(object sender, RoutedEventArgs e)
        {
            ((GetAppInstallFilesDialogViewModel)this.DataContext).ProcessorArchitectureValue = ((RadioButton)sender).Content.ToString();
        }
    }
}
