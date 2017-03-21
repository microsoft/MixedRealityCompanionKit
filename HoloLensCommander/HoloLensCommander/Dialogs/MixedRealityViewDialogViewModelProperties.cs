// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;

namespace HoloLensCommander
{
    /// <summary>
    /// The view model for the MixedRealityViewDialog object.
    /// </summary>
    partial class MixedRealityViewDialogViewModel
    {
        /// <summary>
        /// Gets a value indicating whether or not a live mixed reality view can be started.
        /// </summary>
        private bool canStartLiveView = true;
        public bool CanStartLiveView
        {
            get
            {
                return this.canStartLiveView;
            }

            private set 
            {
                if (this.canStartLiveView != value) 
                {
                    this.canStartLiveView = value;
                    NotifyPropertyChanged("CanStartLiveView");
                }
            }
        }

        /// <summary>
        /// Gets a value indicating whether or not a live mixed reality view can be stopped.
        /// </summary>
        private bool canStopLiveView = false;
        public bool CanStopLiveView
        {
            get
            {
                return this.canStopLiveView;
            }

            private set 
            {
                if (this.canStopLiveView != value) 
                {
                    this.canStopLiveView = value;
                    NotifyPropertyChanged("CanStopLiveView");
                }
            }
        }

        /// <summary>
        /// Gets the address of the device being viewed.
        /// </summary>
        public string DeviceAddress
        {
            get
            {
                return this.deviceMonitor.Address;
            }
        }

        /// <summary>
        /// Gets the Uri providing the live mixed reality view data.
        /// </summary>
        private Uri liveViewSource = null;
        public Uri LiveViewSource
        {
            get
            {
                return this.liveViewSource;
            }

            private set
            {
                if (this.liveViewSource != value)
                {
                    this.liveViewSource = value;
                    NotifyPropertyChanged("LiveViewSource");
                }
            }
        }

        /// <summary>
        /// Gets the message to be displayed to the user.
        /// </summary>
        private string statusMessage  = "";
        public string StatusMessage
        {
            get
            {
                return this.statusMessage;
            }

            private set
            {
                if (this.statusMessage != value)
                {
                    this.statusMessage = value;
                    this.NotifyPropertyChanged("StatusMessage");
                }
            }
        }
    }
}
