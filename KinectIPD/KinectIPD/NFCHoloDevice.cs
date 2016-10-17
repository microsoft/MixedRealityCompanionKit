using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace KinectIPD
{
    class NFCHoloDevice : HoloDevice
    {
        // Note that by default the Web Portal's username/password are stored in clear text on the NFC tag.
        // Enabling encryption will require tags that can save more data or writing will fail.
        private static bool _encryptData = false; 
        private NFCSharp.NFCTag _tag;
        public NFCHoloDevice(NFCSharp.NFCTag tag)
        {
            _tag = tag;
            if (Load())
                Initialize(false);
        }

        public override void ResetDevice()
        {
            ClearTagData();
            base.ResetDevice();
        }

        public override bool IsValid
        {
            get
            {
                return base.IsValid && IsTagValid();
            }
        }

        private bool IsTagValid()
        {
            return IsTagValid(_tag);
        }

        private static bool IsTagValid(NFCSharp.NFCTag tag)
        {
            if (tag == null)
                return false;
            try
            {
                var ndef = tag.NDEFRead();
                if (ndef.Count != 5)
                    return false;

                for (int i = 0; i < 5; i++)
                {
                    if (ndef[i].Payload == null || (_encryptData && EncDec.Unprotect(ndef[i].Payload) == null))
                        return false;
                }
                return true;
            }
            catch (Exception ex)
            {
                Logger.Log(" IsTagValid: NDEFRead failed:" + ex.HResult + " - " + ex.ToString());
                // -2146233088 is a parse error which usually means the tag isn't formatted
                if (ex.HResult != -2146233088)
                {
                    System.Diagnostics.Debug.WriteLine("NDEFRead failed:" + ex.ToString());
                    return false;
                }
            }
            return false;
        }

        private bool LoadTagData()
        {
            if (_tag == null || !IsTagValid())
                return false;
            Logger.Log(" LoadTagData loading");

            var ndef = _tag.NDEFRead();

            var data0 = ndef[0].Payload;
            var data1 = _encryptData ? EncDec.Unprotect(ndef[1].Payload) : ndef[1].Payload;
            var data2 = _encryptData ? EncDec.Unprotect(ndef[2].Payload) : ndef[2].Payload;
            var data3 = _encryptData ? EncDec.Unprotect(ndef[3].Payload) : ndef[3].Payload;
            var data4 = ndef[4].Payload;

            DeviceName = Encoding.ASCII.GetString(data0).TrimEnd(Convert.ToChar(0x0));
            ipAddress = Encoding.ASCII.GetString(data1).TrimEnd(Convert.ToChar(0x0));
            username = Encoding.ASCII.GetString(data2).TrimEnd(Convert.ToChar(0x0));
            password = Encoding.ASCII.GetString(data3).TrimEnd(Convert.ToChar(0x0));
            var temp = Encoding.ASCII.GetString(data4).TrimEnd(Convert.ToChar(0x0));
            
            return true;
        }

        private bool SaveTagData()
        {
            var message = new NdefLibrary.Ndef.NdefMessage();
            var nameRecord = new NdefLibrary.Ndef.NdefRecord(NdefLibrary.Ndef.NdefRecord.TypeNameFormatType.Unknown, new byte[0]);//.NdefTextRecord();
            nameRecord.Payload = Encoding.ASCII.GetBytes(DeviceName);
            message.Add(nameRecord);
            var ipRecord = new NdefLibrary.Ndef.NdefRecord(NdefLibrary.Ndef.NdefRecord.TypeNameFormatType.Unknown, new byte[0]);
            ipRecord.Payload = _encryptData ? EncDec.Protect(Encoding.ASCII.GetBytes(ipAddress)) : Encoding.ASCII.GetBytes(ipAddress);
            message.Add(ipRecord);
            var userNameRecord = new NdefLibrary.Ndef.NdefRecord(NdefLibrary.Ndef.NdefRecord.TypeNameFormatType.Unknown, new byte[0]);
            userNameRecord.Payload = _encryptData ? EncDec.Protect(Encoding.ASCII.GetBytes(username)) : Encoding.ASCII.GetBytes(username);
            message.Add(userNameRecord);
            var passwordRecord = new NdefLibrary.Ndef.NdefRecord(NdefLibrary.Ndef.NdefRecord.TypeNameFormatType.Unknown, new byte[0]);
            passwordRecord.Payload = _encryptData ? EncDec.Protect(Encoding.ASCII.GetBytes(password)) : Encoding.ASCII.GetBytes(password);
            message.Add(passwordRecord);

            // HACKHACK: for some reason the last record seems to get corrupted, so just write some garbage data
            var tempRecord = new NdefLibrary.Ndef.NdefRecord(NdefLibrary.Ndef.NdefRecord.TypeNameFormatType.Unknown, new byte[0]);
            tempRecord.Payload = Encoding.ASCII.GetBytes("xxxXXXXxxx");
            message.Add(tempRecord);

            Logger.Log(" Writing: " + ToString());

            try
            {
                _tag.NDEFWrite(message);
                Logger.Log(" Wrote Data");
                return Load();
            }
            catch (Exception ex)
            {
                Logger.Log(" InitializeTag: Possible error writing data to tag: " + ex.Message);
                return false;
            }
        }
        
        public override bool Save()
        {
            return SaveTagData();
        }

        public override bool Load()
        {
            return LoadTagData();
        }

        private void ClearTagData()
        {
            if (_tag == null)
                return;
            
            Logger.Log(" ClearTagData");

            try
            {
                Logger.Log(" Formatting");
                _tag.NDEFFormat();
            }
            catch (Exception ex)
            {
                Logger.Log(" " + ex.Message);
            }
            var message = new NdefLibrary.Ndef.NdefMessage();
            for (int i = 0; i < 5; i++)
            {
                var record = new NdefLibrary.Ndef.NdefRecord();
                record.Payload = new byte[0];
                message.Add(record);
            }
            try
            {
                _tag.NDEFWrite(message);
            }
            catch (Exception ex)
            {
                Logger.Log(" ClearTagData: Error writing to tag: " + ex.Message);
            }
        }
    }
}
