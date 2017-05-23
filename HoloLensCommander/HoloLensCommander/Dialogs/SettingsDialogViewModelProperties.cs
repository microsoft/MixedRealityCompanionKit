using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HoloLensCommander
{
    public partial class SettingsDialogViewModel
    {
        private bool autoReconnect = false;
        public bool AutoReconnect
        {
            get 
            {
                return this.autoReconnect;
            }

            set
            {
                if (this.autoReconnect != value)
                {
                    this.autoReconnect = value;
                    this.NotifyPropertyChanged("AutoReconnect");
                }
            }
        }

        private int heartbeatInterval = Settings.DefaultHeartbeatInterval;
        public string HeartbeatInterval
        {
            get 
            {
                return this.heartbeatInterval.ToString();
            }

            set 
            {
                int i = 0;
                if (int.TryParse(value, out i))
                {
                    this.heartbeatInterval = i;
                    this.NotifyPropertyChanged("HeartbeatInterval");    
                }
            }
        }
    }
}
