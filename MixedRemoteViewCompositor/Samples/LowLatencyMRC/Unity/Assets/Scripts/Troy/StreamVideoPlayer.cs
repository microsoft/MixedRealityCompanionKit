using MixedRemoteViewCompositor;
using System;
using UnityEngine;

namespace LiveStreaming
{
    public enum PlayerState
    {
        Idle,
        Connecting,
        Initializing,
        Initialized,
        Starting,
        Started,
        Playing,
        Stopping,
        Stopped,
        Closing,
        Closed,
        Failed,
    }

    [RequireComponent(typeof(Plugin))]
    public class StreamVideoPlayer : MonoBehaviour
    {
        public MeshRenderer target;
        public string ConnectTo;
        public ushort Port = 27772;

        public bool StopOnPaused = false;
        public Action<object, StateChangedEventArgs<PlayerState>> PlayerStateChanged;

        public ConnectionState ConnectionState
        {
            get { return this.connectionState; }
            private set
            {
                if (this.connectionState != value)
                {
                    this.previousConnectionState = this.connectionState;
                    this.connectionState = value;
                    this.plugin.QueueAction(() =>
                    {
                        if (this.PlayerStateChanged != null)
                        {
                            var args = new StateChangedEventArgs<PlayerState>(this.previousPlayerState, this.playerState, this.previousConnectionState, this.connectionState);
                            this.PlayerStateChanged(this, args);
                        }
                    });
                }
            }
        }
        private ConnectionState connectionState = ConnectionState.Idle;
        private ConnectionState previousConnectionState = ConnectionState.Idle;

        public PlayerState PlayerState
        {
            get { return this.playerState; }
            private set
            {
                if (this.playerState != value)
                {
                    this.previousPlayerState = this.playerState;
                    this.playerState = value;
                    this.plugin.QueueAction(() =>
                    {
                        if (this.PlayerStateChanged != null)
                        {
                            var args = new StateChangedEventArgs<PlayerState>(this.previousPlayerState, this.playerState, this.previousConnectionState, this.connectionState);
                            this.PlayerStateChanged(this, args);
                        }
                    });
                }
            }
        }
        private PlayerState playerState = PlayerState.Idle;
        private PlayerState previousPlayerState = PlayerState.Idle;

        public int ImageWidth { get; private set; }
        public int ImageHeight { get; private set; }

        // Components needed to interact with Plugin
        private Plugin plugin = null;
        private Connector connector = null;
        private Connection networkConnection = null;
        private PlaybackEngine playbackEngine = null;
        private MediaSampleUpdateArgs mediaUpdateSampleArgs;
        
        private Texture2D syncTexture = null;

        private bool isPaused = false;
        private bool isStarted = false;

        private void Awake()
        {
            this.plugin = this.GetComponent<Plugin>();
        }

        private void Start()
        {
            this.isStarted = true;

            this.ConnectionState = ConnectionState.Idle;
            this.PlayerState = PlayerState.Idle;
        }

        private bool connecting = false;
        private void Update()
        {
            if (Input.GetKey(KeyCode.Space))
            {
                if (!connecting)
                {
                    connecting = true;
                    StartConnector();
                }
            }
            else if(Input.GetKey(KeyCode.P))
            {
                if (playbackEngine != null)
                {
                    playbackEngine.StartTest();
                }
            }

            if (isPaused)
            {
                return;
            }

            if (this.playbackEngine == null || this.syncTexture == null)
            {
                return;
            }

            /*
            if (!this.playbackEngine.GetFrameData(ref this.mediaUpdateSampleArgs))
            {
                Debug.Log("GetFrameData failed");
                // TODO: LOG Failure??
            }*/
        }

        private void OnDisable()
        {
            // TODO: Too aggressive? Maybe stop connection & playback only?
            this.Shutdown();
        }

        private void OnDestroy()
        {
            this.Shutdown();
        }

        private void OnApplicationPause(bool pauseStatus)
        {
            this.isPaused = pauseStatus;

            if (!this.isStarted || !this.StopOnPaused)
            {
                return;
            }

            if (this.isPaused)
            {
                this.StopPlayback();
            }
            else
            {
                // TODO: What to do here?
                this.InitializePlaybackEngine();
                //this.PlaybackInitialize(this.networkConnection);
            }
        }

        public void Shutdown()
        {
            if (this.playbackEngine != null)
            {
                this.StopPlayback();
            }            
            else if (this.connector != null)
            {
                this.StopConnector();
            }
            else
            {
                this.CloseNetworkConnection();
            }
        }

        #region Control Network 
        private void StartConnector()
        {
            Debug.Log("StartConnector()");
            // we own the connection so we can just stop if if needed
            StopConnector();

            this.PlayerState = PlayerState.Connecting;

            this.connector = new Connector { ConnectionUrl = string.Format(Plugin.MediaUrlFormat, this.ConnectTo, this.Port) };
            if (this.connector != null)
            {
                this.connector.Started += this.OnConnectorStarted;
                this.connector.Connected += this.OnConnectorConnected;
                this.connector.Failed += this.OnConnectorFailed;
                this.connector.StartAsync();
            }
        }

        private void StopConnector()
        {
            if (this.connector == null)
            {
                return;
            }

            this.connector.Started -= this.OnConnectorStarted;
            this.connector.Connected -= this.OnConnectorConnected;
            this.connector.Failed -= this.OnConnectorFailed;
            this.connector.Close();
            this.connector.Dispose();
            this.connector = null;
        }

        private void CloseNetworkConnection()
        {
            if (this.networkConnection == null)
            {
                return;
            }

            this.ConnectionState = ConnectionState.Closing;

            this.networkConnection.Disconnected -= this.OnDisconnected;
            this.networkConnection.Close();
        }

        #endregion

        #region Connector Callback Events

        private void OnConnectorStarted(object sender, EventArgs e)
        {
            this.plugin.QueueAction(() =>
            {
                //this.ConnectionState = ConnectionState.Connecting;
            });
        }

        private void OnConnectorFailed(object sender, FailedEventArgs args)
        {
            this.plugin.QueueAction(() =>
            {
                this.ConnectionState = ConnectionState.Failed;
                //this.ConnectionState = ManagerState.ConnectorFailed;
            });
        }

        private void OnConnectorConnected(object sender, Connection connection)
        {
            if (connection == null)
            {
                // TODO: Need better error handling?
                return;
            }

            this.plugin.QueueAction(() =>
            {
                this.ConnectionState = ConnectionState.Connected;

                this.networkConnection = connection;
                this.networkConnection.Disconnected += this.OnDisconnected;
                this.networkConnection.Closed += this.OnConnectionClosed;

                this.InitializePlaybackEngine();

                this.StopConnector();
            });
        }

        #endregion

        #region Network Connection Callback Events

        private void OnDisconnected(object sender, EventArgs e)
        {
            this.plugin.QueueAction(() =>
            {
                this.ConnectionState = ConnectionState.Disconnected;

                this.Shutdown();
            });
        }

        private void OnConnectionClosed(object sender, EventArgs e)
        {
            this.plugin.QueueAction(() =>
            {
                this.ConnectionState = ConnectionState.Closed;

                if (this.networkConnection == null)
                {
                    return;
                }

                this.networkConnection.Closed -= this.OnConnectionClosed;
                this.networkConnection.Dispose();
                this.networkConnection = null;

                //this.ConnectionState = ConnectionState.Closed;
            });
        }

        private void OnConnecitonFailed(object sender, EventArgs e)
        {
            this.plugin.QueueAction(() =>
            {
                this.ConnectionState = ConnectionState.Failed;

                Shutdown();
            });
        }

        #endregion

        #region Control Playback

        private void InitializePlaybackEngine()
        {
            // Only initialize if we have a connection and we don't already have a playback engine
            if (this.networkConnection == null || this.playbackEngine != null)
            {
                return;
            }

            if (PlaybackEngine.Create(this.networkConnection, this.OnPlaybackInitialized))
            {
                this.PlayerState = PlayerState.Initializing;
            }
            else
            {
                this.PlayerState = PlayerState.Failed;

                Shutdown();
            };
        }

        private void PlaybackStart()
        {
            // Don't start if we do not have a playback engine to start
            if (this.playbackEngine == null)
            {
                return;
            }

            this.mediaUpdateSampleArgs = new MediaSampleUpdateArgs();

            // TODO: Why was this only in framesize changed? Also hardcoded values for now
            CreateVideoTexture(1280, 720);

            this.playbackEngine.Started += this.OnPlaybackStarted;
            this.playbackEngine.Stopped += this.OnPlaybackStopped;
            this.playbackEngine.Closed += this.OnPlaybackClosed;
            this.playbackEngine.Failed += this.OnPlaybackFailed;
            this.playbackEngine.FrameSizeChanged += this.OnFrameSizeChanged;
            this.playbackEngine.Start();

            this.PlayerState = PlayerState.Starting;
        }

        private void StopPlayback()
        {
            if (this.playbackEngine != null)
            {
                this.PlayerState = PlayerState.Stopping;

                this.playbackEngine.Stop();
            }

            if (this.playerState != PlayerState.Stopping)
            {
                this.PlayerState = PlayerState.Failed;

                Shutdown();
            }
        }

        private void ClosePlayback()
        {
            this.DestroyVideoTexture();

            if (this.playbackEngine != null)
            {
                this.PlayerState = PlayerState.Closing;

                this.playbackEngine.FrameSizeChanged -= this.OnFrameSizeChanged;
                this.playbackEngine.Started -= this.OnPlaybackStarted;
                this.playbackEngine.Stopped -= this.OnPlaybackStopped;
                this.playbackEngine.Failed -= OnPlaybackFailed;
                this.playbackEngine.Close();
            }
        }

        #endregion

        #region PlaybackEngine CallBack Events

        private void OnPlaybackInitialized(PlaybackEngine engine)
        {
            this.plugin.QueueAction(() =>
            {
                this.PlayerState = PlayerState.Initialized;

                this.playbackEngine = engine;

                this.PlaybackStart();

                if (this.playerState != PlayerState.Starting)
                {
                    this.PlayerState = PlayerState.Failed;

                    this.Shutdown();
                }
            });
        }

        private void OnPlaybackStarted(object sender, EventArgs e)
        {
            this.plugin.QueueAction(() =>
            {
                this.PlayerState = PlayerState.Started;

                if (this.playerState != PlayerState.Started)
                {
                    this.PlayerState = PlayerState.Failed;

                    Shutdown();
                }
            });
        }
        private void OnPlaybackStopped(object sender, EventArgs e)
        {
            this.plugin.QueueAction(() =>
            {
                this.PlayerState = PlayerState.Stopped;

                this.ClosePlayback();

                if (this.playerState != PlayerState.Closing)
                {
                    this.playerState = PlayerState.Failed;

                    Shutdown();
                }
            });
        }
        private void OnPlaybackClosed(object sender, EventArgs e)
        {
            this.plugin.QueueAction(() =>
            {
                this.PlayerState = PlayerState.Closed;

                if (this.playbackEngine != null)
                {
                    this.playbackEngine.Closed -= this.OnPlaybackClosed;
                    this.playbackEngine.Dispose();
                    this.playbackEngine = null;
                }

                if (this.playerState != PlayerState.Closed)
                {
                    this.PlayerState = PlayerState.Failed;
                }

                Shutdown();
            });
        }
        private void OnPlaybackFailed(object sender, FailedEventArgs args)
        {
            this.plugin.QueueAction(() =>
            {
                Debug.LogWarningFormat("MrvcPlayer failed: {0} - {1}", args.Result, args.Message);

                this.PlayerState = PlayerState.Failed;

                this.Shutdown();
            });
        }

        private void OnFrameSizeChanged(object sender, FrameSizedChangedArgs args)
        {
            this.plugin.QueueAction(() =>
            {
                this.PlayerState = PlayerState.Playing;

                CreateVideoTexture(args.Width, args.Height);
            });
        }

        #endregion


        #region Playback Video Texture Interop

        private void CreateVideoTexture(uint width, uint height)
        {
            // texture height & width
            if (this.ImageWidth != width || this.ImageHeight != height)
            {
                this.DestroyVideoTexture();

                this.ImageWidth = Convert.ToInt32(width);
                this.ImageHeight = Convert.ToInt32(height);
            }

            if (this.syncTexture == null)
            {
                // create texture to copy video frame to
                if (this.ImageWidth > 0 && this.ImageHeight > 0)
                {
                    this.syncTexture = new Texture2D(this.ImageWidth, this.ImageHeight, TextureFormat.BGRA32,
                        false, true)
                    {
                        filterMode = FilterMode.Point,
                        wrapMode = TextureWrapMode.Clamp
                    };
                    this.syncTexture.Apply();
                }

                this.mediaUpdateSampleArgs.videoTexture = this.syncTexture.GetNativeTexturePtr();

                if (this.target != null)
                {
                    this.target.material.mainTexture = syncTexture;
                }
            }
        }

        private void DestroyVideoTexture()
        {
            this.mediaUpdateSampleArgs.videoTexture = IntPtr.Zero;

            if (this.target != null)
            {
                this.target.material.mainTexture = null;
            }

            if (this.syncTexture != null)
            {
                Destroy(this.syncTexture);

                this.syncTexture = null;
            }
        }
        #endregion

    }
}