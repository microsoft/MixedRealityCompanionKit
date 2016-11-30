// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

namespace HoloLensCommander
{
    /// <summary>
    /// The view model for the TagHoloLensDialog object.
    /// </summary>
    partial class TagHoloLensDialogViewModel
    {
        /// <summary>
        /// Gets or sets the interpupilary distance
        /// </summary>
        private float ipd = 0.0f;
        public string Ipd
        {
            get
            {
                return this.ipd.ToString();
            }

            set
            {
                float f = 0f;
                if (float.TryParse(value, out f))
                {
                    if (this.ipd != f)
                    {
                        this.ipd = f;
                        NotifyPropertyChanged("Ipd");
                    }
                }
            }
        }

        /// <summary>
        /// Gets or sets the descriptive name of this HoloLens
        /// </summary>
        private string name = string.Empty;
        public string Name
        {
            get 
            {
                return this.name;
            }

            set
            {
                if (this.name != value)
                {
                    this.name = value;
                    NotifyPropertyChanged("Name");
                }
            }
        }
    }
}
