// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.ComponentModel;

namespace HoloLensCommander
{
    /// <summary>
    /// The view model for the TagDeviceDialog object.
    /// </summary>
    partial class TagDeviceDialogViewModel : INotifyPropertyChanged
    {
        /// <summary>
        /// Event that is notified when a property value has changed.
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;

        /// <summary>
        /// Initializes a new instance of the <see cref="TagDeviceDialogViewModel" /> class.
        /// </summary>
        /// <param name="address">The address of the device.</param>
        /// <param name="tagInfo">TagInformation object containing information associated with the device.</param>
        public TagDeviceDialogViewModel(
            string address,
            TagInformation tagInfo)
        {
            this.DeviceAddress = address;
            this.Name = tagInfo.Name;
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
        /// <param name="tagInfo">The information which is to be associated with the device.</param>
        internal void UpdateUserData(TagInformation tagInfo)
        {
            tagInfo.Name = this.Name;
            tagInfo.DeployNameToDevice = this.DeployNameToDevice;
        }
    }
}
