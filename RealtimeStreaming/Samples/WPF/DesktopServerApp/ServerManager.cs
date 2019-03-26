using Base;
using StreamerSource.Grabber;
using System;
using System.Threading;
using System.Windows.Threading;
using VisionGrabberSource;

namespace DesktopServerApp
{
    public enum ConnectionStatus
    {
        Idle,
        Listening,
        Connected
    }
    public class ServerManager : PropertyChangeBase
    {
        private const int GrabberVideoWidth = 1920;
        private const int GrabberVideoHeight = 1080;
        private Buffers _buffers;
        private ushort _listeningPort = 27772;
        private ISource _source;
        private RealtimeServers _realtimeServers;
        private readonly Dispatcher _dispatcher;
        private int _maxListenerCount = 2;
        private int _frameDelayMs = 15;
        private readonly object _startStopListeningLock = new object();
        private Timer _FpsUpdateTimer;
        private BeaconLib.Beacon _beacon;
        private bool _isIdle = true;

        public RealtimeServers RealtimeServers
        {
            get => _realtimeServers;
            set => SetProperty(ref _realtimeServers, value);
        }
        public bool IsIdle
        {
            get => _isIdle;
            set => SetProperty(ref _isIdle, value);
        }
        public int MaxListenerCount
        {
            get => _maxListenerCount;
            set => SetProperty(ref _maxListenerCount, value);
        }
        public ushort ListeningPort
        {
            get => _listeningPort;
            set => SetProperty(ref _listeningPort, value);
        }
        public int FrameDelayMs
        {
            get => _frameDelayMs;
            set => SetProperty(ref _frameDelayMs, value);
        }
        public ServerManager(Dispatcher dispatcher)
        {
            _dispatcher = dispatcher;
            _beacon = new BeaconLib.Beacon("streamingServer", () => GetPort());
            var dummy = _beacon.StartAsync();
        }
        private ushort? GetPort()
        {
            if (RealtimeServers == null) return null;
            var server = RealtimeServers.GetNextListeningServer();
            if (server == null)
                return null;
            ushort port = server.Port;
            Console.WriteLine($"Advertising port {port}");
            return port;
        }
        public void InitializeGrabberAndBuffers()
        {
            _buffers = new Buffers();
            var visionGrabber = new Vision(_buffers);
            
            
            if (visionGrabber.IsInitialized())
            {
                _source = visionGrabber;
            }
            else
            {
                _source?.Dispose();

                var tiGrabber = new GrabberSourceTl(_buffers, 0);

                if (tiGrabber.IsGrabberAvailable())
                {
                    _source = tiGrabber;
                }
                else
                {
                    tiGrabber.Close();
                    tiGrabber.Dispose();
                    _source = new GrabberSourceSimulator(_buffers);
                }
            }
            
            //_source = new GrabberSourceSimulator(_buffers);

            _source.Open(0, 0, GrabberVideoWidth, GrabberVideoHeight);
            _source.SetInterval(16); // 60 fps goal
            _source.Start();

        }
        public void StartListening()
        {
            lock (_startStopListeningLock)
            {
                IsIdle = false;

                RealtimeServers = new RealtimeServers(_maxListenerCount + 1);

                // Create backup server that doesn't listen to multicast channel
                var defaultServer = new ConnectionAndServer(_buffers, _frameDelayMs, _dispatcher, false);
                defaultServer.InitializeListener(_listeningPort);
                RealtimeServers.AddServer(defaultServer);

#pragma warning disable 4014
                // spawn as many listeners as we can have
                for (ushort i = 0; i < _maxListenerCount; i++)
                {
                    var realtimeServer = new ConnectionAndServer(_buffers, _frameDelayMs, _dispatcher);
                    realtimeServer.InitializeListener((ushort)(_listeningPort + i + 1));
                    RealtimeServers.AddServer(realtimeServer);
                }
#pragma warning restore
            }
        }
    }
}
