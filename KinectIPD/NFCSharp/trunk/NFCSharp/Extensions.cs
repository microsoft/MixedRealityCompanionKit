using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NFCSharp
{
    public static class Extensions
    {
        public static string BytesToHex(this byte[] bytes)
        {
            string hex = String.Empty;
            for (int idx = 0; idx < bytes.Length; idx++)
            {
                hex = hex + " " + string.Format("{0:X2}", bytes[idx]);
            }
            return hex.Trim();
        }

        public static byte[] HexToBytes(this string payload)
        {
            payload = payload.Trim().Replace(" ", "");
            if (payload.Length % 2 > 0) return null;

            byte[] HexAsBytes = new byte[payload.Length / 2];
            for (int index = 0; index < HexAsBytes.Length; index++)
            {
                string byteValue = payload.Substring(index * 2, 2);
                HexAsBytes[index] = byte.Parse(byteValue, NumberStyles.HexNumber, CultureInfo.InvariantCulture);
            }
            return HexAsBytes;
        }
    }
}
