using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace HoloLensCommander
{
    public partial class SetCredentialsDialogViewModel : INotifyPropertyChanged
    {
        /// <summary>
        /// Event that is notified when a property value has changed.
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;

        /// <summary>
        /// Initializes a new instance of the <see cref="SetCredentialsDialogViewModel" /> class.
        /// </summary>
        public SetCredentialsDialogViewModel(NetworkCredential credentials)
        {
            this.UserName = credentials.UserName;
            this.Password = credentials.Password;
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

        /// <summary>
        /// Update's the user selected data.
        /// </summary>
        /// <param name="credentials">The credentials to be used to connect to the device.</param>
        internal void UpdateUserData(NetworkCredential credentials)
        {
            credentials.UserName = this.UserName;
            credentials.Password = this.Password;
        }
    }
}
