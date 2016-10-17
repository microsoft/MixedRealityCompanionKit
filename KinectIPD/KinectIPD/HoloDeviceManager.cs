// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections.ObjectModel;

namespace KinectIPD
{
    class HoloDeviceManager
    {
        public event EventHandler<HoloDevice> CurrentDeviceChanged;

        public HoloDevice CurrentDevice { get; private set; }
        public ObservableCollection<HoloDevice> Devices { get; private set; }

        public HoloDeviceManager()
        {
            ResetDevices();
        }

        public void SetCurrentDevice(HoloDevice device)
        {
            CurrentDevice = device;
            if (CurrentDeviceChanged != null)
                CurrentDeviceChanged(this, CurrentDevice);
        }

        public void AddDevice(HoloDevice newDevice)
        {
            Devices.Add(newDevice);
            SetCurrentDevice(newDevice);
        }

        public void RemoveDevice(HoloDevice device)
        {
            Devices.Remove(device);
            if (CurrentDevice == null)
            {
                if (Devices.Count > 0)
                    SetCurrentDevice(Devices[0]);
                else
                    SetCurrentDevice(null);
            }
        }

        public void ResetDevices()
        {
            Devices = new ObservableCollection<HoloDevice>();
            SetCurrentDevice(null);
        }

        public virtual void ShutDown()
        {

        }
    }
}
