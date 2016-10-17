using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Timers;

namespace NFCSharp
{
    public static class NFCHandler
    {
        private static IntPtr hContext = IntPtr.Zero;

        public static int DefaultReader = 0;
        public static bool IsInitialized { get { return hContext != IntPtr.Zero; } }

        private static List<NFCReader> readers = new List<NFCReader>();
        public static readonly ReadOnlyCollection<NFCReader> Readers = readers.AsReadOnly();

        static NFCHandler()
        {
        }

        public static void Init()
        {
            if (IsInitialized) Release();

            int retCode;

            // Get context
            retCode = SCW.SCardEstablishContext(SCW.SCARD_SCOPE_USER, 0, 0, out hContext);

            if (retCode != SCW.SCARD_S_SUCCESS)
                throw new Exception("Failed extablishing context: " + SCW.GetScardErrMsg(retCode));

            // Get PC/SC readers available
            int pcchReaders = 0;
            retCode = SCW.SCardListReaders(hContext, null, null, ref pcchReaders);

            if (retCode == SCW.SCARD_E_NO_READERS_AVAILABLE) return;
            else if (retCode != SCW.SCARD_S_SUCCESS)
                throw new Exception("Failed listing readers: " + SCW.GetScardErrMsg(retCode));

            // Fill reader list
            readers.Clear();
            byte[] ReadersList = new byte[pcchReaders];
            retCode = SCW.SCardListReaders(hContext, null, ReadersList, ref pcchReaders);

            // Convert reader buffer to string
            int idxBytes = 0, idxNames = 0;
            string rdrName = "";
            string[] readersNames = new string[pcchReaders];
            while (ReadersList[idxBytes] != 0)
            {
                while (ReadersList[idxBytes] != 0)
                {
                    rdrName = rdrName + (char)ReadersList[idxBytes];
                    idxBytes++;
                }

                if (rdrName.StartsWith("DUALi"))
                    readers.Add(new Readers.DualiReader(hContext, rdrName)); 
                else if (rdrName.StartsWith("ACS"))
                    readers.Add(new Readers.ACSReader(hContext, rdrName));
                else if (rdrName.StartsWith("SCM") || (rdrName.StartsWith("Identive") && rdrName.Contains("Contactless")))
                    readers.Add(new Readers.IdentiveReader(hContext, rdrName));

                rdrName = "";
                idxBytes++;
                idxNames++;
            }

            DefaultReader = 0;
        }

        public static void Release()
        {
            if (IsInitialized)
            {
                int retCode = SCW.SCardReleaseContext(hContext);

                if (retCode != SCW.SCARD_S_SUCCESS)
                    throw new Exception("Failed release!");

                hContext = IntPtr.Zero;
                readers.Clear();
            }
        }

    }
}
