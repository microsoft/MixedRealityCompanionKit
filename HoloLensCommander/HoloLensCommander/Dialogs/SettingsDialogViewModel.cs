using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HoloLensCommander
{
    /// <summary>
    /// The view model for the SettingsDialog object.
    /// </summary>
    public partial class SettingsDialogViewModel : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        /// <summary>
        /// Initializes a new instance of the <see cref="SettingsDialogViewModel" /> class.
        /// </summary>
        /// <param name="settings"></param>
        public SettingsDialogViewModel(Settings settings)
        {
            this.AutoReconnect = settings.AutoReconnect;
            this.ExpandCredentials = settings.ExpandCredentials;
            this.ExpandNetworkSettings = settings.ExpandNetworkSettings;
            this.HeartbeatInterval = settings.HeartbeatInterval.ToString();
            this.UseInstalledCertificate = settings.UseInstalledCertificate;
        }

        /// <summary>
        /// Sends the PropertyChanged events to registered handlers.
        /// </summary>
        /// <param name="propertyName">The name of property that has changed.</param>
        private void NotifyPropertyChanged(string propertyName)
        {
            this.PropertyChanged?.Invoke(
                this, 
                new PropertyChangedEventArgs(propertyName));
        }

        internal void UpdateUserData(Settings settings)
        {
            // Update heartbeat interval first, in case the value is out of bounds.
            // This will prevent a partial settings update.

            // NOTE: The SettingsDialogViewModel.HeartbeatIntervak property is a string. The
            // SettingsDialogViewModel.heartbeatInterval field is a float. We return the float
            // value as that is what is expected of the caller.
            settings.HeartbeatInterval = this.heartbeatInterval;

            settings.AutoReconnect = this.AutoReconnect;
            settings.ExpandCredentials = this.ExpandCredentials;
            settings.ExpandNetworkSettings = this.ExpandNetworkSettings;
            settings.UseInstalledCertificate = this.UseInstalledCertificate;
        }
    }
}
