using System;
using NFCSharp;
using System.ComponentModel;
using System.Threading;

namespace KinectIPD
{
    class NFCHoloReader
    {
        public event DeviceArrivedHandler DeviceArrived;
        public event DeviceDepartedHandler DeviceDeparted;

        public delegate void DeviceArrivedHandler(NFCHoloDevice device);
        public delegate void DeviceDepartedHandler(NFCHoloDevice device);

        private NFCReader _nfcReader;
        private NFCTag _currentTag;

        public volatile bool ShuttingDown = false;

        public bool HasReader { get { return _nfcReader != null; } }

        private void Log(string message)
        {
            try
            {
                Logger.Log(message);
            }
            catch { }
        }

        public NFCHoloDevice CurrentDevice { get; private set; }
        

        public void Start()
        {
            StartNFC();
        }

        private void StartNFC()
        {
            if (_nfcReader != null)
                Stop();
            if (ShuttingDown)
                return;
            NFCHandler.Init();
            Log("StartNFC: " + NFCHandler.Readers.Count + " NFC readers");
            foreach (NFCReader rdr in NFCHandler.Readers)
            {
                Log("NFCHandler.Readers: " + rdr.Name);
                // Tested with ACS reader
                if (rdr.Name.ToLower().Contains("acs"))
                {
                    _nfcReader = rdr;
                    _nfcReader.TagFound += _nfcReader_TagFound;
                    _nfcReader.TagLost += _nfcReader_TagLost;
                    _nfcReader.StartPolling();
                }
            }
        }

        public void Stop()
        {
            if (!NFCHandler.IsInitialized) return;
            if (_currentTag != null)
                _currentTag.Dispose();
            try
            {
                if (_nfcReader != null)
                {
                    _nfcReader.StopPolling();
                    // spin for a second to allow the NFC reader to timeout
                    for (var i = 0; i < 100; i++)
                    {
                        Thread.Sleep(10);
                    }
                }
                NFCHandler.Release();
            }
            catch (Exception ex)
            {
                // ignore errors on cleanup
            }
        }       

        private void _nfcReader_TagLost()
        {
            Log("_nfcReader_TagLost");
            if (DeviceDeparted != null)
            {
                foreach (Delegate d in DeviceDeparted.GetInvocationList())
                {
                    ISynchronizeInvoke syncer = d.Target as ISynchronizeInvoke;
                    if (syncer != null)
                        syncer.BeginInvoke(d, new HoloDevice[] { CurrentDevice });
                    else
                        d.DynamicInvoke(CurrentDevice);
                }
            }
            _busy = false;
            _currentTag = null;
            CurrentDevice = null;
        }

        private volatile bool _busy;
        private void _nfcReader_TagFound(NFCTag tag)
        {
            // we get called twice for some reason.
            if (_busy)
                return;
            if (ShuttingDown)
                return;
            Log(" _nfcReader_TagFound");

            if (_currentTag != null && _currentTag.UID == tag.UID)
                return;

            _currentTag = tag;

            CurrentDevice = new NFCHoloDevice(_currentTag);
            foreach (Delegate d in DeviceArrived.GetInvocationList())
            {
                ISynchronizeInvoke syncer = d.Target as ISynchronizeInvoke;
                if (syncer != null)
                    syncer.BeginInvoke(d, new NFCHoloDevice[] { CurrentDevice });
                else
                    d.DynamicInvoke(CurrentDevice);
            }
        }
    }
}
