// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using Windows.UI.Xaml.Controls;

namespace HoloLensCommander
{
    /// <summary>
    /// The class that implements the dialog used to tag the connected HoloLens with information useful to the user
    /// (ex: descriptive text, color, etc).
    /// </summary>
    public sealed partial class TagHoloLensDialog : ContentDialog
    {
        /// <summary>
        /// Object containing the information that is to be associated with a HoloLens.
        /// </summary>
        private TagInformation tagInformation;

        /// <summary>
        /// Initializes a new instance of the <see cref="TagHoloLensDialog" /> class.
        /// </summary>
        /// <param name="address">The address of the HoloLens.</param>
        /// <param name="tagInfo">TagInformation object containing information associated with the HoloLens.</param>
        public TagHoloLensDialog(
            string address,
            TagInformation tagInfo)
        {
            this.tagInformation = tagInfo;

            this.DataContext = new TagHoloLensDialogViewModel(
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
            ((TagHoloLensDialogViewModel)this.DataContext).UpdateUserData(this.tagInformation);
        }
    }
}
