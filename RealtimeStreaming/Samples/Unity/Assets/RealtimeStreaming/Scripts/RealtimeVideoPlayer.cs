using System;
using System.Runtime.InteropServices;
using System.Text;
using UnityEngine;

namespace RealtimeStreaming
{
    [RequireComponent(typeof(Plugin))]
    public class RealtimeVideoPlayer : MonoBehaviour
    {
        public enum PlaybackState
        {
            None = 0,
            Opening,
            Buffering,
            Playing,
            Paused,
            Ended
        };

        public MeshRenderer target;
        public string ConnectTo;
        public ushort Port = 27772;

        public bool StopOnPaused = false;
        public Action<object, StateChangedEventArgs<PlaybackState>> PlayerStateChanged;

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
                            var args = new StateChangedEventArgs<PlaybackState>(this.previousState, 
                                this.currentState, this.previousConnectionState, 
                                this.connectionState);
                            this.PlayerStateChanged(this, args);
                        }
                    });
                }
            }
        }
        private ConnectionState connectionState = ConnectionState.Idle;
        private ConnectionState previousConnectionState = ConnectionState.Idle;

        public PlaybackState State
        {
            get { return this.currentState; }
            private set
            {
                if (this.currentState != value)
                {
                    this.previousState = this.currentState;
                    this.currentState = value;
                    if (this.PlayerStateChanged != null)
                    {
                        var args = new StateChangedEventArgs<PlaybackState>(this.previousState, this.currentState);
                        this.PlayerStateChanged(this, args);
                    }
                }
            }
        }
        private PlaybackState currentState = PlaybackState.None;
        private PlaybackState previousState = PlaybackState.None;

        // Components needed to interact with Plugin
        private Plugin plugin = null;
        private Connector connector = null;
        private Connection networkConnection = null;

        private bool isPaused = false;
        private bool isStarted = false;

        private void Awake()
        {
            this.plugin = this.GetComponent<Plugin>();

            thisObject = GCHandle.Alloc(this, GCHandleType.Normal);
        }

        private void Start()
        {
            this.isStarted = true;

            this.ConnectionState = ConnectionState.Idle;

#if !UNITY_EDITOR
            connecting = true;
            StartConnector();
#endif
        }

        private bool connecting = false;

#if UNITY_EDITOR
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
        }
#endif

        private void OnDisable()
        {
            // TODO: Too aggressive? Maybe stop connection & playback only?
            this.Shutdown();
        }

        private void OnDestroy()
        {
            // free unmanaged resources (unmanaged objects) and override a finalizer below.
            if (thisObject.IsAllocated)
            {
                thisObject.Free();
                thisObject = default(GCHandle);
            }

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
                this.Pause();
                //this.StopPlayback();
            }
            else
            {
                // TODO: What to do here?
                //this.InitializePlaybackEngine();
                //this.PlaybackInitialize(this.networkConnection);
            }
        }

        public void Shutdown()
        {
            if (this.connector != null)
            {
                this.StopConnector();
            }
            else
            {
                this.CloseNetworkConnection();
            }

            if (this.playbackTexture != null)
            {
                Plugin.CheckHResult(PlayerPlugin.exReleasePlayer(), "RealtimeVideoPlayer::exReleasePlayer()");
                this.playbackTexture = null;
            }
        }

#region Control Network 
        private void StartConnector()
        {
            Debug.Log("StartConnector()");
            // we own the connection so we can just stop if if needed
            StopConnector();

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
                Debug.Log("RealtimeVideoPlayer::OnConnectorStarted");

                //this.ConnectionState = ConnectionState.Connecting;
            });
        }

        private void OnConnectorFailed(object sender, FailedEventArgs args)
        {
            this.plugin.QueueAction(() =>
            {
                Debug.Log("RealtimeVideoPlayer::OnConnectorFailed");

                this.ConnectionState = ConnectionState.Failed;
            });
        }

        private void OnConnectorConnected(object sender, Connection connection)
        {
            if (connection == null)
            {
                // TODO: Need better error handling?
                Debug.Log("RealtimeVideoPlayer::OnConnectorConnected - Connection was null");
                return;
            }

            this.plugin.QueueAction(() =>
            {
                Debug.Log("RealtimeVideoPlayer::OnConnectorConnected");

                this.ConnectionState = ConnectionState.Connected;

                this.networkConnection = connection;
                this.networkConnection.Disconnected += this.OnDisconnected;
                this.networkConnection.Closed += this.OnConnectionClosed;

                this.CreateRealTimePlayer();

                this.StopConnector();
            });
        }

#endregion

#region Network Connection Callback Events

        private void OnDisconnected(object sender, EventArgs e)
        {
            this.plugin.QueueAction(() =>
            {
                Debug.Log("RealtimeVideoPlayer::OnDisconnected");

                this.ConnectionState = ConnectionState.Disconnected;

                this.Shutdown();
            });
        }

        private void OnConnectionClosed(object sender, EventArgs e)
        {
            this.plugin.QueueAction(() =>
            {
                Debug.Log("RealtimeVideoPlayer::OnConnectionClosed");

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

        private void OnConnectionFailed(object sender, EventArgs e)
        {
            this.plugin.QueueAction(() =>
            {
                Debug.Log("RealtimeVideoPlayer::OnConnectionFailed");

                this.ConnectionState = ConnectionState.Failed;

                Shutdown();
            });
        }

        #endregion

        #region Control Playback

        private uint textureWidth, textureHeight;
        private Texture2D playbackTexture;
        private PlayerPlugin.StateChangedCallback stateCallback;
        private GCHandle thisObject = default(GCHandle);

        private PlayerPlugin.PlayerCreatedCallbackHandler createdHandler;

        private void CreateRealTimePlayer()
        {
            // Only initialize if we have a connection and we don't already have a playback engine
            if (this.networkConnection == null)
            {
                return;
            }

            this.createdHandler = new PlayerPlugin.PlayerCreatedCallbackHandler(Player_PluginCallbackWrapper.OnCreated_Callback);
            this.stateCallback = new PlayerPlugin.StateChangedCallback(Player_PluginCallbackWrapper.OnStateChanged_Callback);

            IntPtr thisObjectPtr = GCHandle.ToIntPtr(this.thisObject);

            // Create our RealtimePlayer at the Plugin C++ layer
            Plugin.CheckHResult(PlayerPlugin.exCreatePlayer(this.networkConnection.Handle,
                this.stateCallback,
                this.createdHandler,
                thisObjectPtr
                ), "RealtimeVideoPlayer::exCreatePlayer()");
        }

        private void OnCreated(long result, UInt32 width, UInt32 height)
        {
            Plugin.ExecuteOnUnityThread(() =>
            {
                Plugin.CheckHResult(result, "RealtimeVideoPlayer::OnCreated");

                // TODO: Check width & height are not zero

                this.textureWidth = width;
                this.textureHeight = height;

                Debug.Log("RealTimePlayer::OnCreated - " + width + " by " + height);

                // create native texture for playback
                IntPtr nativeTexture = IntPtr.Zero;
                Plugin.CheckHResult(PlayerPlugin.exCreateExternalTexture(this.textureWidth,
                    this.textureHeight,
                    out nativeTexture), 
                    "RealtimeVideoPlayer::exCreateExternalTexture");

                // TODO: textureformat on callback*

                // create the unity texture2d 
                this.playbackTexture =
                    Texture2D.CreateExternalTexture((int)this.textureWidth,
                    (int)this.textureHeight,
                    TextureFormat.BGRA32,
                    //TextureFormat.RGBA32,
                    false, 
                    true,
                    nativeTexture);

                // set texture for the shader
                this.target.material.mainTexture = this.playbackTexture;

                Debug.Log("RealTimePlayer::CreateStreamingTexture");

                this.Play();
            });
        }

        public void Play()
        {
            Debug.Log("RealTimePlayer::Play");
            Plugin.CheckHResult(PlayerPlugin.exPlay(), "RealTimePlayer::exPlay");
        }

        public void Pause()
        {
            Debug.Log("RealTimePlayer::Pause");
            Plugin.CheckHResult(PlayerPlugin.exPause(), "RealTimePlayer::exPause");
        }

        public void Stop()
        {
            Debug.Log("RealTimePlayer::Stop");
            Plugin.CheckHResult(PlayerPlugin.exStop(), "RealTimePlayer::exStop");
        }

        private void MediaPlayback_Changed(PlayerPlugin.PLAYBACK_STATE args)
        {
            Plugin.ExecuteOnUnityThread(() =>
            {
                OnStateChanged(args);
            });
        }

        private void OnStateChanged(PlayerPlugin.PLAYBACK_STATE args)
        {
            var stateType = (PlayerPlugin.StateType)Enum.ToObject(typeof(PlayerPlugin.StateType), args.type);

            switch (stateType)
            {
                case PlayerPlugin.StateType.StateType_StateChanged:
                    this.State = (PlaybackState)Enum.ToObject(typeof(PlaybackState), args.state);
                    Debug.Log("Playback State: " + stateType.ToString() + " - " + this.State.ToString());
                    break;
                case PlayerPlugin.StateType.StateType_Opened:
                    PlayerPlugin.MEDIA_DESCRIPTION description = args.description;
                    Debug.Log("Media Opened: " + description.ToString());
                    break;
                case PlayerPlugin.StateType.StateType_Failed:
                    Plugin.CheckHResult(args.hresult, "RealtimeVideoPlayer::OnStateChanged");
                    break;
                default:
                    break;
            }
        }

        #endregion

        #region Realtime Player Plugin

        internal static class PlayerPlugin
        {
            public enum StateType
            {
                StateType_None = 0,
                StateType_Opened,
                StateType_StateChanged,
                StateType_Failed,
            };

            [StructLayout(LayoutKind.Sequential, Pack = 4)]
            public struct MEDIA_DESCRIPTION
            {
                public UInt32 width;
                public UInt32 height;
                public Int64 duration;
                public byte isSeekable;

                public override string ToString()
                {
                    StringBuilder sb = new StringBuilder();
                    sb.AppendLine("width: " + width);
                    sb.AppendLine("height: " + height);
                    sb.AppendLine("duration: " + duration);
                    sb.AppendLine("canSeek: " + isSeekable);

                    return sb.ToString();
                }
            };

            [StructLayout(LayoutKind.Explicit, Pack = 4)]
            public struct PLAYBACK_STATE
            {
                [FieldOffset(0)]
                public UInt16 type;

                [FieldOffset(4)]
                public UInt16 state;

                [FieldOffset(4)]
                public Int64 hresult;

                [FieldOffset(4)]
                public MEDIA_DESCRIPTION description;
            };

            public delegate void StateChangedCallback(PLAYBACK_STATE args);

            [UnmanagedFunctionPointer(CallingConvention.StdCall)]
            public delegate void PlayerCreatedCallbackHandler(IntPtr senderPtr, long result, UInt32 width, UInt32 height);

            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "CreateRealtimePlayer")]
            internal static extern long exCreatePlayer(uint connectionHandle,
                [MarshalAs(UnmanagedType.FunctionPtr)]StateChangedCallback callback, 
                [MarshalAs(UnmanagedType.FunctionPtr)]PlayerCreatedCallbackHandler createdCallback, 
                IntPtr objectPtr);

            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "ReleaseRealtimePlayer")]
            internal static extern long exReleasePlayer();

            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "CreateRealtimePlayerTexture")]
            internal static extern long exCreateExternalTexture(UInt32 width, UInt32 height, out System.IntPtr playbackTexture);

            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "RealtimePlayerPlay")]
            internal static extern long exPlay();

            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "RealtimePlayerPause")]
            internal static extern long exPause();

            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "RealtimePlayerStop")]
            internal static extern long exStop();
        }

        internal static class Player_PluginCallbackWrapper
        {
            [AOT.MonoPInvokeCallback(typeof(PlayerPlugin.PlayerCreatedCallbackHandler))]
            internal static void OnCreated_Callback(IntPtr senderPtr, long result, UInt32 width, UInt32 height)
            {
                var thisObj = Plugin.GetSenderObject<RealtimeVideoPlayer>(senderPtr);

                Plugin.ExecuteOnUnityThread(() => {
                    thisObj.OnCreated(result, width, height);
                });
            }

            [AOT.MonoPInvokeCallback(typeof(PlayerPlugin.StateChangedCallback))]
            internal static void OnStateChanged_Callback(PlayerPlugin.PLAYBACK_STATE args)
            {
                // TODO: Troy to re-anble this code path
                /*
                var thisObj = Plugin.GetSenderObject<RealtimeVideoPlayer>(senderPtr);

                Plugin.ExecuteOnUnityThread(() => {
                    thisObj.OnStateChanged(args);
                });
                */
            }
        }
#endregion
    }
}