// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using RealtimeStreaming.Media;
using RealtimeStreaming.Network;
using System;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Media;
using System.Windows.Threading;
using Windows.Media.MediaProperties;

namespace DesktopServerApp
{
    public class SampleManager : PropertyChangeBase
    {
        public enum ManagerStatus
        {
            Idle,
            Listening,
            Streaming
        };

        public ushort Port
        {
            get => port;
            set => SetProperty(ref port, value);
        }

        public ManagerStatus Status
        {
            get => status;
            set
            {
                SetProperty(ref status, value);
                FirePropertyChanged("IsIdle");
                FirePropertyChanged("IsStreaming");
                FirePropertyChanged("IsRunning");
            }
        }

        public bool IsIdle
        {
            get { return status == ManagerStatus.Idle; }
        }

        public bool IsStreaming
        {
            get { return status == ManagerStatus.Streaming; }
        }

        public bool IsRunning
        {
            get { return status != ManagerStatus.Idle; }
        }

        public bool MulticastDiscoveryEnabled
        {
            get => multicastDiscoveryEnabled;
            set => SetProperty(ref multicastDiscoveryEnabled, value);
        }

        public int TargetFrameRate
        {
            get => targetFrameRate;
            set => SetProperty(ref targetFrameRate, value);
        }

        public float RunningFPS
        {
            get => runningFps;
            set => SetProperty(ref runningFps, value);
        }

        public SolidColorBrush FrameColor
        {
            get => frameColor;
            set => SetProperty(ref frameColor, value);
        }

        public uint OutputWidth
        {
            get => width;
            set => SetProperty(ref width, value);
        }

        public uint OutputHeight
        {
            get => height;
            set => SetProperty(ref height, value);
        }

        public string RemoteAddress
        {
            get
            {
                if (connection != null)
                {
                    return connection.ConnectionInfo.RemoteAddress.ToString();
                }

                return "No connection";
            }
        }

        protected Connection NetworkConnection
        {
            get => connection;
            private set
            {
                connection = value;
                this.FirePropertyChanged("RemoteAddress");
            }
        }

        private ushort port = 27772;
        private bool multicastDiscoveryEnabled = true;
        private int targetFrameRate = 15;
        private uint width = 1280;
        private uint height = 720;
        private uint bpp = 4; // bytes per pixel
        private float runningFps = 0.0f;

        private ManagerStatus status = ManagerStatus.Idle;

        private SolidColorBrush frameColor = new SolidColorBrush(Colors.White);
        private Color startColor = Colors.Red;
        private Color endColor = Colors.Blue;
        private uint frameColorCount = 0;
        private uint numOfFramesForColorChange = 120;

        private readonly Dispatcher appDispatcher;
        private byte[] imageData;
        private Random rand = new Random();
        private Stopwatch stopwatch = new Stopwatch();

        private CancellationTokenSource serverStreamCts;
        private DisconnectedDelegate disconnectHandler;
        private Listener listener;
        private Connection connection;
        private RealtimeServer server;

        private readonly Guid MF_VideoFormatRgb32 = new Guid("{00000016-0000-0010-8000-00AA00389B71}");

        public SampleManager(Dispatcher dispatcher)
        {
            appDispatcher = dispatcher;
            this.Status = ManagerStatus.Idle;
        }

        public void StartServer()
        {
            if (!this.IsIdle) return;

            Task.Factory.StartNew(() => RunServer());
        }

        public void StopServer()
        {
            if (this.Status == ManagerStatus.Streaming)
            {
                StopStreaming();
            }

            this.Status = ManagerStatus.Idle;
        }

        public void StopStreaming()
        {
            serverStreamCts.Cancel();
        }

        private async void RunServer()
        {
            do
            {
                this.Status = ManagerStatus.Listening;
                this.listener = new Listener(this.Port);
                this.NetworkConnection = await listener.ListenAsync(this.MulticastDiscoveryEnabled);

                listener.Shutdown();
                listener = null;

                disconnectHandler = () =>
                {
                    this.NetworkConnection.Disconnected -= disconnectHandler;
                    this.serverStreamCts.Cancel();
                };

                this.NetworkConnection.Disconnected += disconnectHandler;

                await Task.Run(() => { Stream(); });

                // sender quit, so close connection and retry
                Thread.Sleep(1000);
            }
            while (!IsIdle);
        }

        private void Stream()
        {
            var delayAfterImage = 1;
            bool cancelled = false;
            var imageCount = 0;

            this.Status = ManagerStatus.Streaming;
            this.serverStreamCts = new CancellationTokenSource();

            while (!cancelled)
            {
                var waitDelay = TimeSpan.FromMilliseconds(1000.0f / TargetFrameRate);

                try
                {
                    serverStreamCts.Token.ThrowIfCancellationRequested();

                    if (this.server == null)
                    {
                        this.InitServer();
                    }

                    stopwatch.Start();

                    // If we run into error, then cancel this execution and reloop for new connection/setup
                    try
                    {
                        UpdateImageData();

                        this.server.WriteFrame((uint)this.imageData.Length, this.imageData);
                    }
                    catch (Exception ex)
                    {
                        serverStreamCts.Cancel();
                    }

                    // Send over first images without delay to fill buffer
                    if (imageCount < delayAfterImage) { imageCount++; } else { Task.Delay(waitDelay).Wait(); }

                    stopwatch.Stop();

                    float period = stopwatch.ElapsedMilliseconds;
                    UpdateUIThread(() => this.RunningFPS = 1000.0f / period);

                    stopwatch.Reset();
                }
                catch (OperationCanceledException)
                {
                    cancelled = true;
                }
            }

            ShutdownStream();
        }

        private void UpdateImageData()
        {
            frameColorCount++;
            if (frameColorCount >= numOfFramesForColorChange)
            {
                frameColorCount = 0;
                startColor = endColor;
                endColor = Color.FromRgb((byte)rand.Next(1, 255), (byte)rand.Next(1, 255), (byte)rand.Next(1, 255));
            }

            var c = GetColor((float)frameColorCount / numOfFramesForColorChange);

            UpdateUIThread(() => this.FrameColor = new SolidColorBrush(c));

            Parallel.For(0, this.imageData.Length / bpp, index =>
            {
                long byteIdx = index << 2; // multiply by 4

                imageData[byteIdx] = c.B;
                imageData[byteIdx + 1] = c.G;
                imageData[byteIdx + 2] = c.R;
                imageData[byteIdx + 3] = c.A;
            });
        }

        private void InitServer()
        {
            var profile = MediaEncodingProfile.CreateHevc(VideoEncodingQuality.HD720p);

            profile.Video.Width = width;
            profile.Video.Height = height;

            imageData = new byte[width * height * bpp];

            this.server = new RealtimeServer(this.NetworkConnection,
                MF_VideoFormatRgb32,
                profile
            );
        }

        private void ShutdownStream()
        {
            Debug.WriteLine("Destroying connection & server");
            if (this.NetworkConnection != null)
            {
                this.NetworkConnection.Shutdown();
                this.NetworkConnection = null;
            }

            if (this.server != null)
            {
                this.server.Shutdown();
                this.server = null;
            }

            this.startColor = Colors.Red;
            this.endColor = Colors.Blue;

            UpdateUIThread(() => this.FrameColor = new SolidColorBrush(Colors.White));
        }

        public Color GetColor(float weight)
        {
            return Color.FromRgb(
                (byte)Math.Round(startColor.R * (1 - weight) + endColor.R * weight),
                (byte)Math.Round(startColor.G * (1 - weight) + endColor.G * weight),
                (byte)Math.Round(startColor.B * (1 - weight) + endColor.B * weight));
        }

        private async void UpdateUIThread(Action a)
        {
            await appDispatcher.InvokeAsync(a);
        }
    }
}
