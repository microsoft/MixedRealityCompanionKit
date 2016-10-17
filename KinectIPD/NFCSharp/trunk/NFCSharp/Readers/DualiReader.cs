using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NFCSharp.Readers
{
    class DualiReader : NFCReader
    {
        public DualiReader(IntPtr Handle, string ReaderName) : base(Handle, ReaderName) { }


        protected override TagType ParseATR(byte[] bATR)
        {
            if (bATR[4] == 0xF0 || bATR[4] == 0x01)
                switch (bATR[bATR.Length - 1])
                {
                    case 0x30:
                        return TagType.MifareStandard1K;
                    case 0x31:
                        return TagType.MifareUltralightFamily;
                    case 0x32:
                        return TagType.MifareStandard4K;
                    case 0x33:
                        return TagType.MifareMini;
                    case 0x34:
                        return TagType.MifarePlus2K;
                    case 0x35:
                        return TagType.MifarePlus4K;
                }
            else if (bATR[4] == 0xFD || bATR[4] == 0x02)
                return TagType.ISO15693_part1;
            else if (bATR[4] == 0xFC || bATR[4] == 0x03)
                return TagType.Felica212K;
            else if (bATR[4] == 0xF1)
                return TagType.Topaz;

            return TagType.Unknown;
        }

        internal override byte[] ParseUID(IntPtr Handle, int Proto)
        {
            byte[] ATR = GetATR(Handle, Proto);
            if (ATR[4] == 0xF0 || ATR[4] == 0x01)
            {
                byte[] returnBytes = new byte[ATR.Length - 6];
                Array.Copy(ATR, 6, returnBytes, 0, ATR.Length - 6);
                return returnBytes;
            }
            else
                return null;
        }

        public override NFCTag Connect()
        {
            IntPtr handle;
            int proto = 0;
            int retCode = SCW.SCardConnect(hContext, Name, SCW.SCARD_SHARE_SHARED,
                                  SCW.SCARD_PROTOCOL_T0 | SCW.SCARD_PROTOCOL_T1, out handle, ref proto);

            if (retCode != SCW.SCARD_S_SUCCESS)
                return null;

            byte[] ATR = GetATR(handle, proto);

            return BuildTag(handle, proto, this, ATR);
        }


        public override string Test()
        {
            throw new NotImplementedException();
        }

        public override void LoadKey(IntPtr Handle, int Proto, KeyTypes KeyType, byte[] KeyData)
        {
            if (KeyData.Length != 6)
                throw new NotSupportedException("Keys must be 6 byte long");

            byte KeyT = KeyType == KeyTypes.TypeA ? (byte)0x00 : (byte)0x04;
            byte KeyN = KeyType == KeyTypes.TypeA ? (byte)0x10 : (byte)0x11;

            Transmit(Handle, Proto, new byte[] { 0xFD, 0x2F, KeyT, KeyN, 0x06,
                KeyData[0], KeyData[1], KeyData[2], KeyData[3], KeyData[4], KeyData[5]});
            return;
        }

        public override void Authenticate(IntPtr Handle, int Proto, KeyTypes KeyType, byte Sector)
        {
            throw new NotImplementedException();
        }

        public override byte[] Read(IntPtr Handle, int Proto, byte Page)
        {
            return Transmit(Handle, Proto, new byte[] { 0xFD, 0x35, 0x00, 0xFF, 0x01, Page });
        }

        public override void Write(IntPtr Handle, int Proto, byte Page, byte[] Data)
        {
            if (Data.Length != 4) throw new Exception("Page write must be of 4 bytes");

            byte[] buffer = new byte[] { 0xFD, 0x37, 0x00, 0xFF, 0x05, Page, 0x00, 0x00, 0x00, 0x00 };
            Array.Copy(Data, 0, buffer, 5, 4);
            Transmit(Handle, Proto, buffer);
        }

        public override byte[] Transmit(IntPtr Handle, int Proto, byte[] CmdBytes)
        {
            SCW.SCARD_IO_REQUEST ioRequest = new SCW.SCARD_IO_REQUEST();
            ioRequest.dwProtocol = Proto;
            ioRequest.cbPciLength = 8;

            int rcvLenght = 32; // Use 260 to handle more intelligent smartcards
            byte[] rcvBytes = new byte[rcvLenght];

            int retCode = SCW.SCardTransmit(Handle,
                ref ioRequest, CmdBytes, CmdBytes.Length,
                ref ioRequest, rcvBytes, out rcvLenght);

            if (retCode != SCW.SCARD_S_SUCCESS)
                throw new Exception("Failed querying tag: " + SCW.GetScardErrMsg(retCode));

            if (!(rcvBytes[rcvLenght - 2] == 0x90 && rcvBytes[rcvLenght - 1] == 0x00))
            {
                if (rcvBytes[rcvLenght - 2] == 0x63 && rcvBytes[rcvLenght - 1] == 0x00)
                    throw new Exception("Operation failed!");

                if (rcvBytes[rcvLenght - 2] == 0x6A && rcvBytes[rcvLenght - 1] == 0x81)
                    throw new Exception("Operation not supported!");

                throw new Exception("Operation returned: " + rcvBytes[rcvLenght - 2].ToString("X2") + rcvBytes[rcvLenght - 1].ToString("X2"));
            }

            byte[] returnBytes = new byte[rcvLenght - 2];
            Array.Copy(rcvBytes, returnBytes, rcvLenght - 2);
            return returnBytes;
        }
    }
}
