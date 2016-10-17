using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using NFCSharp;

namespace TestApp
{
    public partial class theForm : Form
    {
        public theForm()
        {
            InitializeComponent();
        }

        private NFCTag theTag = null;

        private void bInitialize_Click(object sender, EventArgs e)
        {
            NFCHandler.Init();

            dReaders.Items.Clear();
            foreach (NFCReader rdr in NFCHandler.Readers)
                dReaders.Items.Add(rdr.Name);

            if (dReaders.Items.Count > 0)
                dReaders.SelectedIndex = 0;
        }

        private void bConnect_Click(object sender, EventArgs e)
        {
            if (!NFCHandler.IsInitialized) return;
            
            theTag = NFCHandler.Readers[dReaders.SelectedIndex].Connect();
            if (theTag != null)
            {
                tATR.Text = theTag.ATR;
                tUID.Text = theTag.UID;
                tType.Text = theTag.GetType().Name;
            }
        }

        private void bRaw_Click(object sender, EventArgs e)
        {
            if (!NFCHandler.IsInitialized) return;

            if (theTag != null)
            {
                tData.Text = theTag.ReadAll().BytesToHex();
            }
        }

        private void bRead_Click(object sender, EventArgs e)
        {
            if (!NFCHandler.IsInitialized) return;

            if (theTag != null)
            {
                byte[] buffer = new byte[16];
                for (int i = 0; i <= 3; i++)
                {
                    Array.Copy(theTag.Read((byte)(i + 4)), 0, buffer, i * 4, 4);
                }
                Guid id = new Guid(buffer);

                tData.Text = id.ToString();
            }
        }

        private void bWrite_Click(object sender, EventArgs e)
        {
            if (!NFCHandler.IsInitialized) return;

            if (theTag != null)
            {
                byte[] buffer = Guid.NewGuid().ToByteArray();
                for (int i = 0; i <= 3; i++)
                {
                    theTag.Write((byte)(i + 4), buffer);
                }
            }
        }

        private void bDisconnect_Click(object sender, EventArgs e)
        {
            if (!NFCHandler.IsInitialized) return;

            if (theTag != null)
            {
                tATR.Text = tUID.Text = tType.Text = tData.Text = "";
                theTag.Dispose();
                theTag = null;
            }
        }

        private void bRelease_Click(object sender, EventArgs e)
        {
            if (!NFCHandler.IsInitialized) return;

            NFCHandler.Release();
            dReaders.Items.Clear();
        }

        private void theForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (theTag != null)
                theTag.Dispose();
            NFCHandler.Release();
            System.Environment.Exit(0);
        }

        private void btTogglePoll_Click(object sender, EventArgs e)
        {
            if (!NFCHandler.IsInitialized) return;

            if (btTogglePoll.Text.StartsWith("Start"))
            {
                NFCHandler.Readers[dReaders.SelectedIndex].TagFound += theForm_TagFound;
                NFCHandler.Readers[dReaders.SelectedIndex].TagLost += TheForm_TagLost; ;
                NFCHandler.Readers[dReaders.SelectedIndex].StartPolling();
                btTogglePoll.Text = "Stop Polling";
            }
            else
            {
                NFCHandler.Readers[dReaders.SelectedIndex].StopPolling();
                btTogglePoll.Text = "Start Polling";
            }
        }

        private void TheForm_TagLost()
        {
            tATR.Text = tUID.Text = tType.Text = tData.Text = "";
        }

        private void theForm_TagFound(NFCTag Tag)
        {
            if (Tag != null)
            {
                tATR.Text = Tag.ATR;
                tUID.Text = Tag.UID;
                tType.Text = Tag.GetType().Name;
            }
        }

    }
}
