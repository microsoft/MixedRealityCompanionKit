using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace NFCSharp
{
    public delegate void TagFoundHandler(NFCTag Tag);
    public delegate void TagLostHandler();

    public abstract class NFCReader
    {
        public NFCReader(IntPtr Handle, string ReaderName)
        {
            Name = ReaderName;
            hContext = Handle;

            pollerThread = new Thread(Poller);
        }

        public readonly string Name;
        protected readonly IntPtr hContext;

        private Thread pollerThread;

        public abstract NFCTag Connect();

        protected byte[] GetATR(IntPtr Handle, int Proto)
        {
            string rName = String.Empty;
            int rLenght = 0, ATRLen = 33, dwState = 0;
            byte[] ATRBytes = new byte[32];

            int retCode = SCW.SCardStatus(Handle, rName,
                ref rLenght, ref dwState, ref Proto, ref ATRBytes[0], ref ATRLen);

            if (retCode != SCW.SCARD_S_SUCCESS)
                throw new Exception("Failed querying tag status: " + SCW.GetScardErrMsg(retCode));

            byte[] ATR = new byte[ATRLen];
            Array.Copy(ATRBytes, ATR, ATRLen);
            return ATR;
        }

        protected NFCTag BuildTag(IntPtr Handle, int Proto, NFCReader Reader, byte[] ATR)
        {
            switch (ParseATR(ATR))
            {
                case TagType.MifareUltralightFamily:
                    return new Tags.Ultralight(Handle, Proto, Reader, ATR);

                default:
                    return new Tags.Unknown(Handle, Proto, Reader, ATR);
            }
        }

        internal abstract byte[] ParseUID(IntPtr Handle, int Proto);
        protected abstract TagType ParseATR(byte[] bATR);

        public abstract string Test();

        public abstract void LoadKey(IntPtr Handle, int Proto, KeyTypes KeyType, byte[] KeyData);
        public abstract void Authenticate(IntPtr Handle, int Proto, KeyTypes KeyType, byte Sector);

        public abstract byte[] Read(IntPtr Handle, int Proto, byte Page);
        public abstract void Write(IntPtr Handle, int Proto, byte Page, byte[] Data);

        public abstract byte[] Transmit(IntPtr Handle, int Proto, byte[] CmdBytes);

        #region Polling Ops
        public void StartPolling()
        {
            if (!pollerThread.IsAlive)
                pollerThread.Start();
        }

        public void StopPolling()
        {
            stopPollingSignal = true;
            //pollerThread.Abort();
        }

        protected void Poller()
        {
            SCW.SCARD_READERSTATE[] State = new SCW.SCARD_READERSTATE[1];
            State[0].RdrName = this.Name;
            State[0].UserData = IntPtr.Zero;
            State[0].RdrCurrState = SCW.SCARD_STATE_UNKNOWN;

            int retCode;

            retCode = SCW.SCardGetStatusChange(hContext, 100, State, 1);
            if (retCode != SCW.SCARD_S_SUCCESS)
                throw new Exception("Failed initial get status change: " + SCW.GetScardErrMsg(retCode));

            State[0].RdrCurrState = State[0].RdrEventState;

            while (!stopPollingSignal)
            {
                retCode = SCW.SCardGetStatusChange(hContext, 1000, State, 1);

                if (retCode == SCW.SCARD_E_TIMEOUT)
                    continue;
                if (retCode != SCW.SCARD_S_SUCCESS)
                    throw new Exception("Failed cycling get status change: " + SCW.GetScardErrMsg(retCode));

                if (((State[0].RdrEventState & SCW.SCARD_STATE_PRESENT) == SCW.SCARD_STATE_PRESENT) &&
                    ((State[0].RdrEventState & SCW.SCARD_STATE_CHANGED) == SCW.SCARD_STATE_CHANGED))
                {
                    try {
                        NFCTag tag = Connect();
                        foreach (Delegate d in TagFound.GetInvocationList())
                        {
                            ISynchronizeInvoke syncer = d.Target as ISynchronizeInvoke;
                            if (syncer != null)
                                syncer.BeginInvoke(d, new NFCTag[] { tag });
                            else
                                d.DynamicInvoke(tag);
                        }
                    }
                    catch(Exception ex)
                    {
                        System.Diagnostics.Debug.WriteLine("Connect failed:" + ex.ToString());
                    }
                }
                else if (((State[0].RdrEventState & SCW.SCARD_STATE_EMPTY) == SCW.SCARD_STATE_EMPTY) &&
                    ((State[0].RdrEventState & SCW.SCARD_STATE_CHANGED) == SCW.SCARD_STATE_CHANGED))
                {
                    foreach (Delegate d in TagLost.GetInvocationList())
                    {
                        ISynchronizeInvoke syncer = d.Target as ISynchronizeInvoke;
                        if (syncer != null)
                            syncer.BeginInvoke(d, null);
                        else
                            d.DynamicInvoke();
                    }
                }

                State[0].RdrCurrState = State[0].RdrEventState;
            }

        }

        protected volatile bool stopPollingSignal = false;
        public event TagFoundHandler TagFound;
        public event TagLostHandler TagLost;
        #endregion
    }
}
