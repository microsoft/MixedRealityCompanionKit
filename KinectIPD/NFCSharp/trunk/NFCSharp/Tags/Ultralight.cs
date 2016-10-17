using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NFCSharp.Tags
{
    class Ultralight : NFCTag
    {
        public Ultralight(IntPtr Handle, int Proto, NFCReader Reader, byte[] ATRbytes) : base(Handle, Proto, Reader, ATRbytes) { }

        public override void NDEFFormat()
        {
            byte[] check;
            
            check = Read(0x02);
            if (check[2] != 0x00 || check[3] != 0x00)
                throw new Exception("Format failure: tag is locked!");

            check = Read(0x03);
            if (check[0] != 0x00 || check[1] != 0x00 || check[2] != 0x00 || check[3] != 0x00)
                throw new Exception("Format failure: tag is formatted!");

            //Write(0x03, new byte[] { 0xE1, 0x10, 0x06, 0x00 }); // CC for Ultralight
            Write(0x03, new byte[] { 0xE1, 0x10, 0x12, 0x00 }); // CC for NTAG203/UltralightC

            Write(0x04, new byte[] { 0x03, 0x00, 0xFE, 0x00 }); // Empty NDEF record plus terminator
        }

        public override byte[] ReadAll()
        {
            const byte chunkSize = 0x04;
            //const byte bytesToRead = 48;
            const byte bytesToRead = 144;

            byte[] rawData = new byte[bytesToRead];
            int pagesToRead = (bytesToRead / chunkSize);
            for (byte i = 0; i < pagesToRead; i++)
            {
                byte pageIndex = (byte)(i + chunkSize);
                byte[] pageContent = Read(pageIndex);
                Array.Copy(pageContent, 0, rawData, i * chunkSize, chunkSize);
            }
            return rawData;
        }

        public override void WriteAll(byte[] Data)
        {
            for (int i = 0; i < (Data.Length / 4); i++)
            {
                byte[] buffer = new byte[4];
                Array.Copy(Data, i * 4, buffer, 0, 4);
                Write((byte)(i + 4), buffer);
            }
        }

        public override void Lock()
        {
            Write(0x02, new byte[] { 0x00, 0x00, 0xFF, 0xFF });
        }
    }
}
