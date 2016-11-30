// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.ComponentModel;

namespace HoloLensCommander
{
    /// <summary>
    /// The view model for the TagHoloLensDialog object.
    /// </summary>
    partial class TagHoloLensDialogViewModel : INotifyPropertyChanged
    {
        /// <summary>
        /// Event that is notified when a property value has changed.
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;

        /// <summary>
        /// Initializes a new instance of the <see cref="TagHoloLensDialogViewModel" /> class.
        /// </summary>
        /// <param name="tagInfo">Information that has been associated with this HoloLens.</param>
        public TagHoloLensDialogViewModel(TagInformation tagInfo)
        {
            this.Name = tagInfo.Name;
            this.Ipd = tagInfo.Ipd.ToString();
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
        /// Update's the user selected install files data.
        /// </summary>
        /// <param name="tagInfo">The information which is to be associated with the HoloLens.</param>
        internal void UpdateUserData(TagInformation tagInfo)
        {
            tagInfo.Name = this.Name;

            // NOTE: The TagHoloLensDialogViewModel.Ipd property is a string. The
            // TagHoloLensDialogViewModel.ipd field is a float. We return the float
            // value as that is what is expected of the caller.
            tagInfo.Ipd = this.ipd;
        }
    }
}
