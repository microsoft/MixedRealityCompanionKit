using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NFCSharp.Readers
{
    class IdentiveReader : NFCReader
    {
        public IdentiveReader(IntPtr Handle, string ReaderName) : base(Handle, ReaderName) { }


        protected override TagType ParseATR(byte[] bATR)
        {
            TagType Type = TagType.Unknown;

            // Parse ATR received
            string RIDVal, sATRStr, lATRStr, tmpVal;
            int indx, indx2;

            // Mifare cards using ISO 14443 Part 3 Supplemental Document
            if (bATR.Length > 14)
            {
                RIDVal = sATRStr = lATRStr = "";

                for (indx = 7; indx <= 11; indx++)
                {
                    RIDVal = RIDVal + " " + string.Format("{0:X2}", bATR[indx]);
                }

                for (indx = 0; indx <= 4; indx++)
                {
                    tmpVal = bATR[indx].ToString();

                    for (indx2 = 1; indx2 <= 4; indx2++)
                    {
                        tmpVal = Convert.ToString(Convert.ToInt32(tmpVal) / 2);
                    }

                    if (((indx == '1') & (tmpVal == "8")))
                    {
                        lATRStr = lATRStr + "8X";
                        sATRStr = sATRStr + "8X";
                    }
                    else
                    {
                        if (indx == 4)
                        {
                            lATRStr = lATRStr + " " + string.Format("{0:X2}", bATR[indx]);
                        }
                        else
                        {
                            lATRStr = lATRStr + " " + string.Format("{0:X2}", bATR[indx]);
                            sATRStr = sATRStr + " " + string.Format("{0:X2}", bATR[indx]);
                        }
                    }
                }

                if (RIDVal != "A0 00 00 03 06")
                {
                    switch (bATR[12])
                    {
                        case 0:
                            Type = TagType.Unknown;
                            break;
                        case 1:
                            Type = TagType.ISO14443A_part1;
                            break;
                        case 2:
                            Type = TagType.ISO14443A_part2;
                            break;
                        case 3:
                            Type = TagType.ISO14443A_part3;
                            break;
                        case 5:
                            Type = TagType.ISO14443B_part1;
                            break;
                        case 6:
                            Type = TagType.ISO14443B_part2;
                            break;
                        case 7:
                            Type = TagType.ISO14443B_part3;
                            break;
                        case 9:
                            Type = TagType.ISO15693_part1;
                            break;
                        case 10:
                            Type = TagType.ISO15693_part2;
                            break;
                        case 11:
                            Type = TagType.ISO15693_part3;
                            break;
                        case 12:
                            Type = TagType.ISO15693_part4;
                            break;
                        case 13:
                            Type = TagType.ContactCard7816_10_IIC;
                            break;
                        case 14:
                            Type = TagType.ContactCard7816_10_ExtendedIIC;
                            break;
                        case 15:
                            Type = TagType.ContactCard7816_10_2WBP;
                            break;
                        case 16:
                            Type = TagType.ContactCard7816_10_3WBP;
                            break;
                    }
                }

                // Felica and Topaz Cards
                if (bATR[12] == 0x03)
                {
                    if (bATR[13] == 0xF0)
                    {
                        switch (bATR[14])
                        {
                            case 0x11:
                                Type = TagType.Felica212K;
                                break;
                            case 0x12:
                                Type = TagType.Felica424K;
                                break;
                            case 0x04:
                                Type = TagType.Topaz;
                                break;
                        }
                    }
                }

                if (bATR[12] == 0x03)
                {
                    if (bATR[13] == 0x00)
                    {
                        switch (bATR[14])
                        {
                            case 0x01:
                                Type = TagType.MifareStandard1K;
                                break;
                            case 0x02:
                                Type = TagType.MifareStandard4K;
                                break;
                            case 0x03:
                                Type = TagType.MifareUltralightFamily;
                                break;
                            case 0x26:
                                Type = TagType.MifareMini;
                                break;
                        }
                    }
                    else
                    {
                        if (bATR[13] == 0xFF)
                        {
                            switch (bATR[14])
                            {
                                case 0x09:
                                    Type = TagType.MifareMini;
                                    break;
                            }
                        }
                    }
                }

            }

            //4.2. Mifare DESFire card using ISO 14443 Part 4
            if (bATR.Length == 11)
            {
                RIDVal = "";
                for (indx = 4; indx <= 9; indx++)
                {
                    RIDVal = RIDVal + " " + string.Format("{0:X2}", bATR[indx]);
                }

                if (RIDVal == " 06 75 77 81 02 80")
                {
                    Type = TagType.MifareDESFire;
                }

            }

            return Type;
        }

        internal override byte[] ParseUID(IntPtr Handle, int Proto)
        {
            return Transmit(Handle, Proto, new byte[] { 0xFF, 0xCA, 0x00, 0x00, 0x00 });
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

            byte KeyT = KeyType == KeyTypes.TypeA ? (byte)0x60 : (byte)0x61;

            Transmit(Handle, Proto, new byte[] { 0xFF, 0x82, 0x00, KeyT, 0x06,
                KeyData[0], KeyData[1], KeyData[2], KeyData[3], KeyData[4], KeyData[5]});
            return;
        }

        public override void Authenticate(IntPtr Handle, int Proto, KeyTypes KeyType, byte Sector)
        {
            byte KeyT = KeyType == KeyTypes.TypeA ? (byte)0x60 : (byte)0x61;

            Transmit(Handle, Proto, new byte[] { 0xFF, 0x86, 0x00, 0x00, 0x05,
                0x01, 0x00, Sector, KeyT, 0x01});
            return;
        }

        public override byte[] Read(IntPtr Handle, int Proto, byte Page)
        {
            return Transmit(Handle, Proto, new byte[] { 0xFF, 0xB0, 0x00, Page, 0x04 });
        }

        public override void Write(IntPtr Handle, int Proto, byte Page, byte[] Data)
        {
            if (Data.Length != 4) throw new Exception("Page write must be of 4 bytes");

            byte[] buffer = new byte[] { 0xFF, 0xD6, 0x00, Page, 0x04, 0x00, 0x00, 0x00, 0x00 };
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
