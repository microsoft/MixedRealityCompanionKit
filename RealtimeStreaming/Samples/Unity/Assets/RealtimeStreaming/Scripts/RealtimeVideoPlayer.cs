using System;
using System.Runtime.InteropServices;
using System.Text;
using UnityEngine;
using UnityEngine.UI;

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

        public Text ipAddressInput;
        public MeshRenderer target;
        public string ConnectTo;
        public ushort Port = 27772;

        public bool StopOnPaused = false;
        public Action<object, StateChangedEventArgs<PlaybackState>> PlayerStateChanged;

        // TODO: move into Connection class?
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

        public PlaybackState PlayerState
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
                        var args = new StateChangedEventArgs<PlaybackState>(this.previousState,
                                this.currentState, this.previousConnectionState,
                                this.connectionState);

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

        private uint textureWidth, textureHeight;
        private Texture2D streamingTexture_Luma;
        private Texture2D streamingTexture_Chroma;

        private PlayerPlugin.StateChangedCallback stateCallback;
        private GCHandle thisObject = default(GCHandle);

        private PlayerPlugin.PlayerCreatedCallbackHandler createdHandler;

        private bool isPaused = false;
        private bool isStarted = false;
        private bool isPlayerCreated = false;
        private bool isConnectorActive = false;

        private void Awake()
        {
            this.plugin = this.GetComponent<Plugin>();

            thisObject = GCHandle.Alloc(this, GCHandleType.Normal);
        }

        private void Start()
        {
            this.isStarted = true;

            this.ConnectionState = ConnectionState.Idle;
            this.PlayerState = PlaybackState.None;
        }

#if UNITY_EDITOR
        private void Update()
        {
            if (Input.GetKey(KeyCode.P))
            {
                ConnectPlayer();
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
            this.Shutdown();

            // free unmanaged resources (unmanaged objects) and override a finalizer below.
            if (thisObject.IsAllocated)
            {
                thisObject.Free();
                thisObject = default(GCHandle);
            }
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
            }
            else
            {
                this.Play();
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
                this.Stop();

                this.CloseNetworkConnection();
            }

            if (isPlayerCreated)
            {
                Plugin.CheckHResult(PlayerPlugin.exReleasePlayer(), "RealtimeVideoPlayer::exReleasePlayer()");
                this.streamingTexture_Luma = this.streamingTexture_Chroma = null;
                isPlayerCreated = false;
            }
        }

#region Control Network 
        public void ConnectPlayer()
        {
            // we own the connection so we can just stop if if needed
            StopConnector();

            this.ConnectTo = this.ipAddressInput.text;

            this.connector = new Connector { ConnectionUrl = string.Format(Plugin.MediaUrlFormat, this.ConnectTo, this.Port) };
            if (this.connector != null)
            {
                this.connector.Started += this.OnConnectorStarted;
                this.connector.Connected += this.OnConnectorConnected;
                this.connector.Failed += this.OnConnectorFailed;
                this.connector.StartAsync();

                this.isConnectorActive = true;
            }
        }

        private void StopConnector()
        {
            if (this.connector == null)
            {
                return;
            }

            this.isConnectorActive = false;

            this.connector.Started -= this.OnConnectorStarted;
            this.connector.Connected -= this.OnConnectorConnected;
            this.connector.Failed -= this.OnConnectorFailed;
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
                // TODO: need action plan here
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

            if (isPlayerCreated)
            {
                Debug.Log("RealtimeVideoPlayer::OnConnectorConnected - Player has already been created");
                return;
            }

            this.plugin.QueueAction(() =>
            {
                Debug.Log("RealtimeVideoPlayer::OnConnectorConnected");

                // We connected so stop the Connector
                this.StopConnector();

                this.ConnectionState = ConnectionState.Connected;

                this.networkConnection = connection;
                this.networkConnection.Disconnected += this.OnDisconnected;
                this.networkConnection.Closed += this.OnConnectionClosed;

                this.CreateRealTimePlayer();
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

        private void CreateRealTimePlayer()
        {
            // Only initialize if we have a connection 
            if (this.networkConnection == null || isPlayerCreated)
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

            isPlayerCreated = true;
        }

        private void OnCreated(long result, UInt32 width, UInt32 height)
        {
            this.plugin.QueueAction(() =>
            {
                Plugin.CheckHResult(result, "RealtimeVideoPlayer::OnCreated");

                if (width <= 0 || height <= 0)
                {
                    Debug.LogError("Width or height returned from RealtimeVideoPlayer::OnCreated were <= 0");
                    this.Shutdown();
                    return;
                }

                // TODO: This is hardcoded for weird bug on ARM
                if (width > 16384 || height == 1)
                {
                    width = 1280;
                    height = 720;
                }

                this.textureWidth = width;
                this.textureHeight = height;

                Debug.Log("RealTimePlayer::OnCreated - " + width + " by " + height);

                // TODO: Add information here for YUV work

                // create native texture for playback
                IntPtr nativeTexture_L = IntPtr.Zero;
                IntPtr nativeTexture_UV = IntPtr.Zero;

                Plugin.CheckHResult(PlayerPlugin.exCreateExternalTexture(this.textureWidth,
                    this.textureHeight,
                    out nativeTexture_L,
                    out nativeTexture_UV), 
                    "RealtimeVideoPlayer::exCreateExternalTexture");

                // Create the unity texture2ds 
                this.streamingTexture_Luma =
                    Texture2D.CreateExternalTexture((int)this.textureWidth,
                    (int)this.textureHeight,
                    TextureFormat.RG16,
                    false, 
                    true,
                    nativeTexture_L);

                this.streamingTexture_Chroma =
                    Texture2D.CreateExternalTexture((int)this.textureWidth,
                    (int)this.textureHeight,
                    TextureFormat.R8,
                    false,
                    true,
                    nativeTexture_UV);

                // set texture for the shader
                // TODO: Check that shader is expected
                if (this.target != null)
                {
                    this.target.material.SetTexture("_MainTex_Luma", this.streamingTexture_Luma);
                    this.target.material.SetTexture("_MainTex_Chroma", this.streamingTexture_Chroma);
                }

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
            this.plugin.QueueAction(() =>
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
                    this.PlayerState = (PlaybackState)Enum.ToObject(typeof(PlaybackState), args.state);
                    Debug.Log("Playback State: " + stateType.ToString() + " - " + this.PlayerState.ToString());
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
            internal static extern long exCreateExternalTexture(UInt32 width, UInt32 height, out System.IntPtr texture_L, out System.IntPtr texture_UV);

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
                    Debug.Log("Player_PluginCallbackWrapper::OnCreated_Callback " + width + " x " + height);

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