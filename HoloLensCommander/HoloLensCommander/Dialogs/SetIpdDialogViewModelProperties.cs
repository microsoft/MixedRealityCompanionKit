// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

namespace HoloLensCommander
{
    public partial class SetIpdDialogViewModel
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
    }
}
