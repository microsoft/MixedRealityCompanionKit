// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Collections.ObjectModel;
using System.ComponentModel;

namespace HoloLensCommander
{
    /// <summary>
    /// The view model for the DeviceInformationDialog object.
    /// </summary>
    public partial class DeviceInformationDialogViewModel : INotifyPropertyChanged
    {
        /// <summary>
        /// The DeviceMonitor object responsible for communication with this device.
        /// </summary>
        private DeviceMonitor deviceMonitor;

        /// <summary>
        /// Event that is notified when a property value has changed.
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;

        /// <summary>
        /// Initializes a new instance of the <see cref="DeviceInformationDialogViewModel" /> class.
        /// </summary>
        /// <param name="monitor">The DeviceMonitor responsible for communication with this device.</param>
        public DeviceInformationDialogViewModel(
            DeviceMonitor monitor)
        {
            this.deviceMonitor = monitor;
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
    }
}
