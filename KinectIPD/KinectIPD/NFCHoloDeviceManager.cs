using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Threading;

namespace KinectIPD
{
    class NFCHoloDeviceManager : HoloDeviceManager
    {
        private NFCHoloReader _nfcHoloReader;
        
        public NFCHoloDeviceManager()
        {
            _nfcHoloReader = new NFCHoloReader();
            _nfcHoloReader.Start();
            _nfcHoloReader.DeviceArrived += _nfcHoloReader_DeviceArrived;
            _nfcHoloReader.DeviceDeparted += _nfcHoloReader_DeviceDeparted;
        }

        private void _nfcHoloReader_DeviceDeparted(NFCHoloDevice device)
        {
            SetCurrentDevice(null);
        }

        private void _nfcHoloReader_DeviceArrived(NFCHoloDevice device)
        {
            SetCurrentDevice(device);
        }

        public override void ShutDown()
        {
            _nfcHoloReader.Stop();
            _nfcHoloReader.DeviceArrived -= _nfcHoloReader_DeviceArrived;
            _nfcHoloReader.DeviceDeparted -= _nfcHoloReader_DeviceDeparted;
        } 
    }
}
