using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
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
    public sealed partial class SettingsDialog : ContentDialog
    {
        /// <summary>
        /// The application settings.
        /// </summary>
        private Settings appSettings;

        /// <summary>
        /// Initializes a new instance of the <see cref="SettingsDialog" /> class.
        /// </summary>
        /// <param name="settings">The current appliation settings.</param>
        public SettingsDialog(Settings settings)
        {
            this.appSettings = settings;
            this.DataContext = new SettingsDialogViewModel(settings);

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
            try
            {
                // Return the data
                ((SettingsDialogViewModel)this.DataContext).UpdateUserData(this.appSettings);
            }
            catch(Exception e)
            {
                // Display a message indicating that settings were not changed.
                // TODO
            }
        }
    }
}
