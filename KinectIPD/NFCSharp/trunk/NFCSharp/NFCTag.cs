using NdefLibrary.Ndef;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NFCSharp
{
    public abstract class NFCTag : IDisposable
    {
        public NFCTag(IntPtr Handle, int Proto, NFCReader Reader, byte[] ATRbytes)
        {
            handle = Handle;
            proto = Proto;
            reader = Reader;

            // Get Tag ATR
            bATR = ATRbytes;
            ATR = bATR.BytesToHex();

            // Get Tag UID
            bUID = reader.ParseUID(handle, proto);
            UID = bUID.BytesToHex();
        }

        protected IntPtr handle;
        protected int proto;
        protected NFCReader reader;

        public readonly byte[] bATR;
        public readonly string ATR;
        public readonly byte[] bUID;
        public readonly string UID;

        public void LoadKey(KeyTypes KeyType, byte[] KeyData)
        {
            reader.LoadKey(handle, proto, KeyType, KeyData);
        }
        public void Authenticate(KeyTypes KeyType, byte Sector)
        {
            reader.Authenticate(handle, proto, KeyType, Sector);
        }

        public byte[] Read(byte Page)
        {
            return reader.Read(handle, proto, Page);
        }
        public void Write(byte Page, byte[] Data)
        {
            reader.Write(handle, proto, Page, Data);
        }

        public abstract byte[] ReadAll();
        public abstract void WriteAll(byte[] Data);

        public abstract void NDEFFormat();
        public NdefMessage NDEFRead()
        {
            return NdefMessage.FromByteArray(ReadAll());
        }
        public void NDEFWrite(NdefMessage Message)
        {
            WriteAll(Message.ToByteArray());
        }

        public abstract void Lock();

        public void Dispose()
        {
            int retCode = SCW.SCardDisconnect(handle, SCW.SCARD_UNPOWER_CARD);

            if (retCode != SCW.SCARD_S_SUCCESS)
                throw new Exception("Failed diconnection!");
        }
    }
}
