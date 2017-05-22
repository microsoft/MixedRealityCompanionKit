using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HoloLensCommander
{
    public partial class SetCredentialsDialogViewModel
    {
        /// <summary>
        /// Gets or sets the user name to be used when connecting to a device.
        /// </summary>
        private string userName = string.Empty;
        public string UserName
        {
            get
            {
                return this.userName;
            }

            set
            {
                if (this.userName != value)
                {
                    this.userName = value;
                    this.NotifyPropertyChanged("UserName");
                }
            }
        }

        /// <summary>
        /// Gets or sets the password to be used when connecting to a device.
        /// </summary>
        private string password = string.Empty;
        public string Password
        {
            get
            {
                return this.password;
            }

            set
            {
                if (this.password != value)
                {
                    this.password = value;
                    this.NotifyPropertyChanged("Password");
                }
            }
        }

    }
}
