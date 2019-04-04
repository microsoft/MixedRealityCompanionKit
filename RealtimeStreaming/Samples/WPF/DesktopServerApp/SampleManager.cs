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
            Sending
        };

        private ushort port = 27772;
        private bool multicastDiscoveryEnabled = true;
        private int frameDelayMs = 15;
        private uint width = 1280;
        private uint height = 720;
        private uint bpp = 4;
        private float fps = 0.0f;

        private ManagerStatus status = ManagerStatus.Idle;

        private SolidColorBrush frameColor = new SolidColorBrush(Colors.White);
        private Color startColor = Colors.Red;
        private Color endColor = Colors.Blue;
        private uint frameColorCount = 0;
        private uint numOfFramesForColorChange = 120;

        private readonly Dispatcher _dispatcher;
        private byte[] imageData;
        private Random rand = new Random();
        private Stopwatch stopwatch = new Stopwatch();

        private CancellationTokenSource _cts;
        private DisconnectedDelegate _handler;
        private Listener listener;
        private Connection connection;
        private RealtimeServer server;

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
            }
        }

        public bool IsIdle
        {
            get
            {
                return status == ManagerStatus.Idle;
            }
        }

        public bool MulticastDiscoveryEnabled
        {
            get => multicastDiscoveryEnabled;
            set => SetProperty(ref multicastDiscoveryEnabled, value);
        }

        public int FrameDelayMs
        {
            get => frameDelayMs;
            set => SetProperty(ref frameDelayMs, value);
        }

        public float RunningFPS
        {
            get => fps;
            set => SetProperty(ref fps, value);
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

        public SampleManager(Dispatcher dispatcher)
        {
            _dispatcher = dispatcher;
            this.Status = ManagerStatus.Idle;
        }

        public void Start()
        {
            if (!this.IsIdle) return;

            Task.Factory.StartNew(() => RunLoop());
        }

        private void InitServer()
        {
            var profile = MediaEncodingProfile.CreateHevc(VideoEncodingQuality.HD720p);

            profile.Video.Width = width;
            profile.Video.Height = height;

            imageData = new byte[width * height * bpp];

            Guid mfVideoFormatRgb32 = new Guid("{00000016-0000-0010-8000-00AA00389B71}");

            this.server = new RealtimeServer(this.connection,
                mfVideoFormatRgb32,
                profile
            );
        }

        private void StopServer()
        {
            Debug.WriteLine("Destroying connection & server");
            if (this.connection != null)
            {
                //this.connection.Shutdown();
                this.connection = null;
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

        private async void RunLoop()
        {
            while (true)
            {
                _cts = new CancellationTokenSource();

                this.Status = ManagerStatus.Listening;
                this.listener = new Listener(this.Port);
                this.connection = await listener.ListenAsync(this.MulticastDiscoveryEnabled);

                listener.Shutdown();
                listener = null;

                _handler = () =>
                {
                    this.connection.Disconnected -= _handler;
                    _cts.Cancel();
                };

                this.connection.Disconnected += _handler;

                this.Status = ManagerStatus.Sending;

                await Task.Run(() => { RunServer(); });

                // sender quit, so close connection and retry
                Thread.Sleep(1000);

                StopServer();
            }
        }

        private void RunServer()
        {
            var waitDelay = TimeSpan.FromMilliseconds(frameDelayMs);
            var delayAfterImage = 1;
            bool cancelled = false;
            var imageCount = 0;

            while (!cancelled)
            {
                try
                {
                    _cts.Token.ThrowIfCancellationRequested();

                    // Send Image
                    if (this.server == null)
                    {
                        this.InitServer();
                    }

                    stopwatch.Start();

                    // If we run into error, then cancel this execution and reloop for new connection/setup
                    try
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

                        this.server.WriteFrame((uint)this.imageData.Length, this.imageData);
                    }
                    catch (Exception ex)
                    {
                        _cts.Cancel();
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
            await _dispatcher.InvokeAsync(a);
        }
    }
}
