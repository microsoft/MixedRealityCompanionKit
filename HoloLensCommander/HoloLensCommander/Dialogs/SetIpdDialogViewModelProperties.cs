using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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
