// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using RealtimeStreaming.Media;
using RealtimeStreaming.Network;
using System.Threading;
using Windows.Media.MediaProperties;

namespace Viewer
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class Capture : Page
    {
        private Listener listener = null;
        private Connection connection = null;
        private RealtimeServer rtServer = null;

        private byte[] frameBuffer;
        const int BUFFER_SIZE = 1280 * 720 * 4;

        private Timer writeFrameTimer;

        public Capture()
        {
            this.InitializeComponent();

            frameBuffer = new byte[BUFFER_SIZE];
            for(int i = 2; i < BUFFER_SIZE; i += 4)
            {
                frameBuffer[i] = 0xFF;
            }
        }

        private async void bnListen_Click(object sender, RoutedEventArgs e)
        {
            if(this.listener != null)
            {
                CloseConnection();
            }

            ushort port = 0;
            if (UInt16.TryParse(txPort.Text, out port))
            {
                this.listener = new Listener(port);

                this.connection = await this.listener.ListenAsync();
                if (this.connection != null)
                {
                    this.connection.Disconnected += Connection_Disconnected;
                    this.connection.Received += Connection_Received;

                    StartCapture();
                }
            }
        }

        private void StartCapture()
        {
            var p = MediaEncodingProfile.CreateHevc(VideoEncodingQuality.HD720p);
            p.Video.Width = 1000;
            p.Video.Height = 1000;

            Guid MFVideoFormat_RGB32 = new Guid("{00000016-0000-0010-8000-00AA00389B71}");

            this.rtServer = RealtimeServer.Create(this.connection,
                MFVideoFormat_RGB32,
                MediaEncodingProfile.CreateHevc(VideoEncodingQuality.HD720p)
                );

            if (this.rtServer != null)
            {
                writeFrameTimer = null;
                var autoEvent = new AutoResetEvent(false);
                writeFrameTimer = new Timer(OnWriteFrame, autoEvent, 0, 1000 / 30);
            }
        }

        private void OnWriteFrame(object state)
        {
            if (this.rtServer != null)
            {
                rtServer.WriteDirect(frameBuffer);
            }
        }

        private void bnClose_Click(object sender, RoutedEventArgs e)
        {
            CloseConnection();
        }

        private void Connection_Disconnected(Connection sender)
        {
            CloseConnection();
        }

        private void Connection_Received(Connection sender, BundleReceivedArgs args)
        {

        }

        private async void CloseConnection()
        {
            if (this.rtServer != null)
            {
                this.rtServer.Shutdown();
                this.rtServer.Uninitialize();
                this.rtServer = null;
            }

            if (this.connection != null)
            {
                this.connection.Disconnected -= Connection_Disconnected;
                this.connection.Received -= Connection_Received;
                this.connection.Uninitialize();
                this.connection = null;
            }

            if (this.listener != null)
            {
                this.listener.Uninitialize();
                this.listener = null;
            }
        }
    }
}
