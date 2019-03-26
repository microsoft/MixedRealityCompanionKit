using System;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;
using Windows.Media.MediaProperties;
using Base;
using RealtimeStreaming.Media;
using RealtimeStreaming.Network;
using System.Windows.Threading;

namespace DesktopServerApp
{
    public class ConnectionAndServer : PropertyChangeBase
    {
        private CancellationTokenSource _cts;
        private readonly Buffers                 _buffers;
        private readonly int                     _frameDelayMs;
        private readonly PerPeriodCounter        _fpsCounter;
        private readonly Dispatcher             _dispatcher;
        private readonly bool                   _acceptMulticast;

        DisconnectedDelegate                    _handler ;

        public Connection Connection         { get; set; }
        public RealtimeServer RealtimeServer { get; set; }
        public TripleBuffer Buffer           { get; set; }

        public long FrameSentCount
        {
            get => _frameSentCount;
            private set => SetProperty(ref _frameSentCount, value);
        }
        private long _frameSentCount = 0;

        public int Fps
        {
            get => this._fps;
            set => this.SetProperty(ref this._fps, value);
        }
        private int _fps;

        public ushort Port { get; private set; }

        public ConnectionStatus ConnectionStatus
        {
            get => _connectionStatus;
            set
            {
                SetProperty(ref _connectionStatus, value);
            }
        }
        ConnectionStatus _connectionStatus;

        public ConnectionAndServer(Buffers buffers, int frameDelayMs, Dispatcher dispatcher, bool multicast = true)
        {
            _fpsCounter = new PerPeriodCounter(
                TimeSpan.FromSeconds(1),
                value => Fps = value);

            _buffers                     = buffers;
            Buffer                       = CreateNewBuffer();
            _frameDelayMs                = frameDelayMs;
            _acceptMulticast             = multicast;
            _handler                     = null;
            _dispatcher = dispatcher;
        }

        public async void InitializeListener(ushort listeningPort)
        {
            while (true)
            {
                _cts             = new CancellationTokenSource();
                var listener     = new Listener(listeningPort);
                Port             = listeningPort;
                ConnectionStatus = ConnectionStatus.Listening;
                Connection       = await listener.ListenAsync(_acceptMulticast);

                listener.Shutdown();
                listener = null;

                Port = 0;
                _handler = () =>
                {
                    Connection.Disconnected -= _handler;
                    CancelClient();
                };
                Connection.Disconnected += _handler;

                ConnectionStatus = ConnectionStatus.Connected;

                Debug.WriteLine("Creating new connection. The server will be instantiated when first image comes in");

                await RunSender();

                // sender quit, so close connection and retry
                Thread.Sleep(1000);

                StopConnection();
            }
        }
        public void ThrowIfCancelled()
        {
            _cts.Token.ThrowIfCancellationRequested();
        }

        private TripleBuffer CreateNewBuffer()
        {
            var buffer = new TripleBuffer();
            _buffers.AddBuffer(buffer);
            return buffer;
        }

        public async Task RunSender()
        {
            // This runs until the cancellation token is cancelled.
            await Task.Run(() => { RunClient(); });
            StopConnection();
        }
        public void StopConnection()
        {
            Debug.WriteLine("Destroying connection & server");
            ConnectionStatus = ConnectionStatus.Idle;
            if (Connection != null)
            {
                //Connection.Shutdown();
                Connection = null;
            }
            if (RealtimeServer != null)
            {
                RealtimeServer.Shutdown();
                RealtimeServer = null;
            }
        }
        void RunClient()
        {
            var waitDelay       = TimeSpan.FromMilliseconds(_frameDelayMs);
            var delayAfterImage = 1;
            bool cancelled      = false;
            var imageCount      = 0;

            _fpsCounter.Start();

            while (!cancelled)
            {
                try
                {
                    var image = Buffer.GetLatestImageBlocking();

                    ThrowIfCancelled();

                    if (image != null)
                    {
                        // Send Image
                        if (RealtimeServer == null)
                        {
                            Debug.WriteLine("Adding a server to the connection");
                            RealtimeServer = SpawnRealtimeStreamServer((uint)image.Width, (uint)image.Height);
                        }

                        if (RealtimeServer != null)
                        {

                            // If we run into error, then cancel this execution and reloop for new connection/setup
                            try
                            {
                                RealtimeServer.WriteFrame((uint)image.Buffer.Length, image.Buffer);
                            }
                            catch (Exception ex)
                            {
                                CancelClient();
                            }

                            UpdateFrameCountOnUiThread();

                            _fpsCounter.Increment();

                            // Send over first images without delay to fill buffer
                            if (imageCount < delayAfterImage) { imageCount++; } else { Task.Delay(waitDelay).Wait(); }
                        }
                    }
                }
                catch (OperationCanceledException)
                {
                    cancelled = true;
                }
            }
            _fpsCounter.Stop();
        }

        public void CancelClient()
        {
            _cts.Cancel();
        }
        RealtimeServer SpawnRealtimeStreamServer( uint width, uint height)
        {
            var profile = MediaEncodingProfile.CreateHevc(VideoEncodingQuality.HD720p);
            profile.Video.Width = width;
            profile.Video.Height = height;

            Guid mfVideoFormatRgb32 = new Guid("{00000016-0000-0010-8000-00AA00389B71}");

            var realtimeServer = new RealtimeServer(Connection,
                mfVideoFormatRgb32,
                profile
            );
            return realtimeServer;
        }
        async void UpdateFrameCountOnUiThread()
        {
            await _dispatcher.InvokeAsync(() => FrameSentCount++);
        }
    }
}
