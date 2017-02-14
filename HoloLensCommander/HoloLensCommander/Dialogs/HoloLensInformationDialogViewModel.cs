// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Collections.ObjectModel;
using System.ComponentModel;

namespace HoloLensCommander
{
    /// <summary>
    /// The view model for the HoloLensInformationDialog object.
    /// </summary>
    public partial class HoloLensInformationDialogViewModel : INotifyPropertyChanged
    {
        /// <summary>
        /// The HoloLensMonitor object responsible for communication with this HoloLens.
        /// </summary>
        private HoloLensMonitor holoLensMonitor;

        /// <summary>
        /// Event that is notified when a property value has changed.
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;

        /// <summary>
        /// Initializes a new instance of the <see cref="HoloLensInformationDialogViewModel" /> class.
        /// </summary>
        /// <param name="monitor">The HoloLensMonitor responsible for communication with this HoloLens.</param>
        public HoloLensInformationDialogViewModel(
            HoloLensMonitor monitor)
        {
            this.holoLensMonitor = monitor;
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
