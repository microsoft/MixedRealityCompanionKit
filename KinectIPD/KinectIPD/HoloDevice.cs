// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Threading.Tasks;
using System.Net;
using System.Windows.Forms;
using Microsoft.Tools.WindowsDevicePortal;

namespace KinectIPD
{
    public class HoloDevice
    {
        public DevicePortal Portal;
        public string DeviceName { get; set; }
        public bool IsSaving { get; private set; }
        public virtual bool IsValid
        {
            get
            {
                return !(
                    string.IsNullOrWhiteSpace(DeviceName) ||
                    string.IsNullOrWhiteSpace(ipAddress) ||
                    string.IsNullOrWhiteSpace(username) ||
                    string.IsNullOrWhiteSpace(password)
                    );
            }
        }
        protected string ipAddress = "";
        protected string username = "";
        protected string password = "";
        protected bool autoDetectIP = false;

        public HoloDevice()
        {
        }

        public HoloDevice(string deviceName, string ipaddress, string username, string password)
        {
            DeviceName = deviceName;
            ipAddress = ipaddress;
            this.username = username;
            this.password = password;
        }

        public virtual void ResetDevice()
        {
            DeviceName = "";
            ipAddress = "";
            username = "";
            password = "";
        }

        public virtual bool Save()
        {
            return true;
        }

        public virtual bool Load()
        {
            return true;
        }

        public async Task<bool> Initialize(bool promptCredentials = false)
        {
            var result = true;
            if (promptCredentials)
            {
                result = (bool)PromptForCredentials();
            }
            if (!result)
                return false;

            Portal = new DevicePortal(
                new DefaultDevicePortalConnection(
                ipAddress,
                username,
                password));


            await Portal.Connect(null, null, autoDetectIP);
            ipAddress = Portal.Address;
            return true;
        }

        public override string ToString()
        {
            return DeviceName + " - " + ipAddress;
        }

        public bool PromptForCredentials()
        {
            Logger.Log(" PromptForCredentials");

            CredentialsDialog holoCredBox = new CredentialsDialog();
            bool? dialogResult = null;

            dialogResult = holoCredBox.ShowDialog();
            DeviceName = holoCredBox.DeviceName;
            ipAddress = holoCredBox.IPAddress;
            username = holoCredBox.UserName;
            password = holoCredBox.Password;
            autoDetectIP = holoCredBox.AutoIP;
            return (bool)dialogResult;
        }

        public async Task<bool> SubmitIPD(float ipd)
        {
            if (Portal == null)
                return false;
            IsSaving = true;
            try
            {
                await Portal.SetInterPupilaryDistance(ipd);
                Logger.Log("IPD Set to: " + ipd.ToString("0.0"));
            }
            catch (Exception ex)
            {
                IsSaving = false;
                Logger.Log("Failed to set IPD.\n" + (ex.GetType().ToString() + " - " + ex.Message));
                return false;
            }
            IsSaving = false;
            return true;
        }
    }
}
