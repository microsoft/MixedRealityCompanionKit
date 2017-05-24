using System;
using Windows.UI.Xaml.Controls;

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
                this.appSettings.SettingsUpdated = true;
                this.appSettings.StatusMessage = "";
            }
            catch(Exception e)
            {
                // Inform the user that the settings have not been changed.
                this.appSettings.SettingsUpdated = false;
                this.appSettings.StatusMessage = e.Message;
            }
        }
    }
}
