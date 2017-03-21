// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Threading.Tasks;
using Windows.UI.Popups;

namespace HoloLensCommander
{
    /// <summary>
    /// A message dialog with yes / no buttons.
    /// </summary>
    class YesNoMessageDialog
    {
        /// <summary>
        /// The message dialog to be shown.
        /// </summary>
        private MessageDialog messageDialog;

        /// <summary>
        /// Initializes a new instance of the <see cref="YesNoMessageDialog" /> class.
        /// </summary>
        /// <param name="message">The message to display.</param>
        public YesNoMessageDialog(
            string message)
        {
            this.messageDialog = new MessageDialog(
                message);
            this.messageDialog.Commands.Add(
                new UICommand
                { Label = "Yes", Id = MessageDialogButtonId.Yes });
            this.messageDialog.Commands.Add(
                new UICommand
                { Label = "No", Id = MessageDialogButtonId.No });
        }

        /// <summary>
        /// Display the dialog.
        /// </summary>
        /// <returns>The identifier of the button clicked by the user.</returns>
        public async Task<MessageDialogButtonId> ShowAsync()
        {
            IUICommand response = await this.messageDialog.ShowAsync();

            return (MessageDialogButtonId)response.Id;
        }
    }
}
