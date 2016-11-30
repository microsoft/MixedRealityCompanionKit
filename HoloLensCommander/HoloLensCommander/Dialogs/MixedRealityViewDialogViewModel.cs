// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.ComponentModel;

namespace HoloLensCommander
{
    /// <summary>
    /// The view model for the MixedRealityViewDialog object.
    /// </summary>
    partial class MixedRealityViewDialogViewModel : INotifyPropertyChanged
    {
        /// The HoloLensMonitor object responsible for communication with this HoloLens.
        /// </summary>
        private HoloLensMonitor holoLensMonitor;

        /// <summary>
        /// Event that is notified when a property value has changed.
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;

        /// <summary>
        /// Initializes a new instance of the <see cref="MixedRealityViewDialogViewModel" /> class.
        /// </summary>
        /// <param name="monitor">The HoloLensMonitor responsible for communication with this HoloLens.</param>
        public MixedRealityViewDialogViewModel(
            HoloLensMonitor monitor)
        {
            this.holoLensMonitor = monitor;

            this.StartLiveView();
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
        /// Starts a live mixed reality view
        /// </summary>
        private void StartLiveView()
        {
            // To conserve bandwidth, we will use the low resolution feed
            try
            {
                this.LiveViewSource = this.holoLensMonitor.GetMixedRealityViewUri();
                
                this.CanStartLiveView = false;
                this.CanStopLiveView = true;

                this.StatusMessage = "Live Mixed Reality view started. It may take a few seconds for the view to appear.";
            }
            catch(Exception e)
            {
                this.StatusMessage = string.Format(
                    "Unable to start the live Mixed Reality view - {0}",
                    e.Message);
            }
        }
    }
}
