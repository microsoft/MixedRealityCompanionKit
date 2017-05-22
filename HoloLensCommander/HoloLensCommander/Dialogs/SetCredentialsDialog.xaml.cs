using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

namespace HoloLensCommander
{
    public sealed partial class SetCredentialsDialog : ContentDialog
    {
        /// <summary>
        /// The credentials used when connecting to a device.
        /// </summary>
        private NetworkCredential connectionCredentials;

        /// <summary>
        /// Initializes a new instance of the <see cref="SetCredentialsDialog"/> class.
        /// </summary>
        /// <param name="credentials">Network credential object used to connect to a device.</param>
        public SetCredentialsDialog(NetworkCredential credentials)
        {
            this.connectionCredentials = credentials;

            this.DataContext = new SetCredentialsDialogViewModel(credentials);
            this.InitializeComponent();
        }

        /// <summary>
        /// Handle click event for the ok button.
        /// </summary>
        /// <param name="sender">The object sending the event.</param>
        /// <param name="args">Event arguments.</param>
        private void ContentDialog_OkClick(ContentDialog sender, 
            ContentDialogButtonClickEventArgs args)
        {
            // Return the user's selections.
            ((SetCredentialsDialogViewModel)this.DataContext).UpdateUserData(this.connectionCredentials);
        }
    }
}
