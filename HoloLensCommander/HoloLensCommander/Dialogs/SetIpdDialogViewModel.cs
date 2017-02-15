// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.ComponentModel;

namespace HoloLensCommander
{
    public partial class SetIpdDialogViewModel : INotifyPropertyChanged
    {
        /// <summary>
        /// Event that is notified when a property value has changed.
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;

        /// <summary>
        /// Initializes a new instance of the <see cref="SetIpdDialogViewModel" /> class.
        /// </summary>
        /// <param name="tagInfo">Information that has been associated with this HoloLens.</param>
        public SetIpdDialogViewModel(UserInformation userInfo)
        {
            this.Ipd = userInfo.Ipd.ToString();
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

        internal void UpdateUserData(UserInformation userInfo)
        {
            // NOTE: The SetIpdDialogViewModel.Ipd property is a string. The
            // SetIpdDialogViewModel.ipd field is a float. We return the float
            // value as that is what is expected of the caller.
            userInfo.Ipd = this.ipd;
        }
    }
}
