using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HoloLensCommander
{
    public partial class SetAPItokenDialogViewModel
    {
        private string apiToken = string.Empty;
        public string APIToken
        {
            get
            {
                return this.apiToken;
            }
            set
            {
                if (this.apiToken!=value)
                {
                    this.apiToken = value;
                    this.NotifyPropertyChanged(nameof(APIToken));
                }
            }
        }
    }
}
