using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace HoloLensCommander
{
    public partial class SetAPItokenDialogViewModel : INotifyPropertyChanged
    {
        /// <summary>
        /// Event that is notified when a property value has changed.
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;

        public SetAPItokenDialogViewModel(UserToken apiToken)
        {
            this.APIToken= apiToken.ApiToken;
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

        internal void UpdateAPItoken(UserToken apiToken)
        {
            apiToken.ApiToken = this.APIToken;
        }
    }
}
