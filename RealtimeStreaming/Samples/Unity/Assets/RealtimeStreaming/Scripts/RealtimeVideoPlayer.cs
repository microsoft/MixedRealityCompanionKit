// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using UnityEngine;

namespace RealtimeStreaming
{
    [RequireComponent(typeof(Plugin))]
    public class RealtimeVideoPlayer : MonoBehaviour
    {
        public enum PlaybackState
        {
            None = 0,
            Connecting,
            Playing,
            Paused,
            Ended
        };

        public string RemoteAddress;
        public ushort Port = 27772;
        public bool AutoPlay = true;

        public event EventHandler<StateChangedEventArgs<PlaybackState>> PlayerStateChanged;

        public ConnectionState NetworkState
        {
            get
            {
                if (this.networkConnection != null)
                {
                    return this.networkConnection.State;
                }

                return ConnectionState.Idle;
            }
        }

        public PlaybackState PlayerState
        {
            get { return this.currentState; }
            private set
            {
                if (this.currentState != value)
                {
                    var previousState = this.currentState;
                    this.currentState = value;
                    this.PlayerStateChanged?.Invoke(this, new StateChangedEventArgs<PlaybackState>(previousState, this.currentState));
                }
            }
        }
        private PlaybackState currentState = PlaybackState.None;

        // Components needed to interact with Plugin
        protected Plugin plugin = null;
        protected Connector connector = null;
        protected Connection networkConnection = null;

        // Texture variables
        protected uint textureWidth, textureHeight;
        public Texture2D Texture_Luma { get; private set; }
        public Texture2D Texture_Chroma { get; private set; }

        // Plugin interop callbacks
        private PlayerPlugin.PlayerCreatedCallback onCreatedCallback;
        private PlayerPlugin.StateChangedCallback onStateChangedCallback;
        private GCHandle thisObject = default(GCHandle);
        private uint playerHandle = Plugin.InvalidHandle;

        protected bool isPlayerCreated
        {
            get { return playerHandle != Plugin.InvalidHandle; }
        }

        private void Awake()
        {
            this.plugin = this.GetComponent<Plugin>();

            this.playerHandle = Plugin.InvalidHandle;

            this.PlayerState = PlaybackState.None;

            thisObject = GCHandle.Alloc(this, GCHandleType.Normal);
        }

        private void OnDestroy()
        {
            Shutdown();

            // free unmanaged resources (unmanaged objects) and override a finalizer below.
            if (thisObject.IsAllocated)
            {
                thisObject.Free();
                thisObject = default(GCHandle);
            }
        }

        public void Shutdown()
        {
            Debug.Log("RealtimeVideoPlayer::Shutdown");

            if (this.connector != null)
            {
                this.StopConnector();
            }

            if (this.isPlayerCreated)
            {
                Stop();

                CloseNetworkConnection();

                Plugin.CheckHResult(PlayerPlugin.exReleasePlayer(this.playerHandle), "RealtimeVideoPlayer::exReleasePlayer()");
                this.Texture_Luma = this.Texture_Chroma = null;
                this.onCreatedCallback = null;
                this.onStateChangedCallback = null;
                this.playerHandle = Plugin.InvalidHandle;

                PlayerState = PlaybackState.None;
            }
        }

#region Control Network 
        public void ConnectPlayer(bool useDiscovery)
        {
            if (this.connector == null && !this.isPlayerCreated)
            {
                this.PlayerState = PlaybackState.Connecting;

                connector = new Connector();

                if (this.connector != null)
                {
                    this.connector.Started += this.OnConnectorStarted;
                    this.connector.Connected += this.OnConnectorConnected;
                    this.connector.Failed += this.OnConnectorFailed;

                    if (useDiscovery)
                    {
                        this.connector.DiscoverAsync();
                    }
                    else
                    {
                        this.connector.ConnectAsync(string.Format(Plugin.MediaUrlFormat, this.RemoteAddress, this.Port));
                    }
                }
            }
        }

        private void StopConnector()
        {
            Debug.Log("RealtimeVideoPlayer::StopConnector");

            if (this.connector == null)
            {
                return;
            }

            this.connector.Started   -= this.OnConnectorStarted;
            this.connector.Connected -= this.OnConnectorConnected;
            this.connector.Failed    -= this.OnConnectorFailed;
            this.connector.Dispose();
            this.connector = null;
        }

        private void CloseNetworkConnection()
        {
            Debug.Log("RealtimeVideoPlayer::CloseNetworkConnection");

            if (this.networkConnection == null)
            {
                return;
            }

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
                this.PlayerState = PlaybackState.None;
            });
        }

        private void OnConnectorFailed(object sender, FailedEventArgs args)
        {
            this.plugin.QueueAction(() =>
            {
                Debug.Log("RealtimeVideoPlayer::OnConnectorFailed");

                this.PlayerState = PlaybackState.None;

                StopConnector();
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
                StopConnector();

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
                this.Shutdown();
            });
        }

        private void OnConnectionClosed(object sender, EventArgs e)
        {
            this.plugin.QueueAction(() =>
            {
                Debug.Log("RealtimeVideoPlayer::OnConnectionClosed");

                if (this.networkConnection == null)
                {
                    return;
                }

                Shutdown();
            });
        }

        #endregion

        #region Control Playback

        private void CreateRealTimePlayer()
        {
            Debug.Log("RealtimeVideoPlayer::CreateRealTimePlayer");

            // Only initialize if we have a connection and don't already have a player
            if (this.networkConnection == null || isPlayerCreated)
            {
                Debug.Log("RealtimeVideoPlayer::CreateRealTimePlayer - Connection=null or isPlayerCreated");
                return;
            }

            this.onCreatedCallback = new PlayerPlugin.PlayerCreatedCallback(Player_PluginCallbackWrapper.OnCreated_Callback);
            this.onStateChangedCallback = new PlayerPlugin.StateChangedCallback(Player_PluginCallbackWrapper.OnStateChanged_Callback);

            IntPtr thisObjectPtr = GCHandle.ToIntPtr(this.thisObject);

            // Create our RealtimePlayer at the Plugin C++ layer
            Plugin.CheckHResult(PlayerPlugin.exCreatePlayer(
                this.networkConnection.Handle,
                this.onStateChangedCallback,
                this.onCreatedCallback,
                thisObjectPtr,
                ref this.playerHandle
                ), "RealtimeVideoPlayer::exCreatePlayer()");

            if (!isPlayerCreated)
            {
                Shutdown();
                return;
            }
        }

        /// <summary>
        /// Callback from the Native plugin when a realtime player has been created at the c++ level
        /// </summary>
        /// <param name="result">HRESULT from creation</param>
        /// <param name="width">streaming texture width</param>
        /// <param name="height">streaming texture height</param>
        private void OnCreated(long result, uint width, uint height)
        {
            this.plugin.QueueAction(() =>
            {
                Debug.Log("RealtimeVideoPlayer::OnCreated");
                Plugin.CheckHResult(result, "RealtimeVideoPlayer::OnCreated");

                // Weird bug seen on ARM. width/height parameters passed into this function are invalid values
                Plugin.CheckHResult(PlayerPlugin.exGetStreamResolution(this.playerHandle, ref width, ref height),
                        "RealtimeVideoPlayer::exGetStreamResolution");

                if (!IsValidResolution(width, height))
                {
                    Debug.LogError("RealtimeVideoPlayer::OnCreated() Invalid streaming resolution width=" + width + " - height=" + height);
                    Shutdown();
                    return;
                }

                this.textureWidth = width;
                this.textureHeight = height;
                IntPtr nativeTexturePtr_L = IntPtr.Zero;
                IntPtr nativeTexturePtr_UV = IntPtr.Zero;

                // It is more performant to utilize YUV texture for video streaming/rendering
                // https://docs.microsoft.com/en-us/windows/desktop/medfound/recommended-8-bit-yuv-formats-for-video-rendering

                // Get pointers to the Luma/Chroma textures being used by MediaFoundation in the native plugin
                Plugin.CheckHResult(PlayerPlugin.exCreateExternalTexture(this.playerHandle,
                    this.textureWidth, this.textureHeight,
                    out nativeTexturePtr_L, out nativeTexturePtr_UV), 
                    "RealtimeVideoPlayer::exCreateExternalTexture");

                this.Texture_Luma = Texture2D.CreateExternalTexture((int)this.textureWidth,
                    (int)this.textureHeight,
                    TextureFormat.RG16,
                    false, 
                    true,
                    nativeTexturePtr_L);

                this.Texture_Chroma = Texture2D.CreateExternalTexture((int)this.textureWidth,
                    (int)this.textureHeight,
                    TextureFormat.R8,
                    false,
                    true,
                    nativeTexturePtr_UV);

                if (AutoPlay)
                {
                    Play();
                }
            });
        }

        public void Play()
        {
            if (!this.isPlayerCreated) return;

            Debug.Log("RealTimePlayer::Play");

            // Run plugin command on background thread and not UI thread. Weird bug where MF locks the thread
            Task task = new Task(() =>
            {
                Plugin.CheckHResult(PlayerPlugin.exPlay(this.playerHandle), "RealTimePlayer::exPlay");
                this.plugin.QueueAction(() => this.PlayerState = PlaybackState.Playing);
            });
            task.Start();
        }

        public void Pause()
        {
            if (!this.isPlayerCreated) return;

            Debug.Log("RealTimePlayer::Pause");

            // Run plugin command on background thread and not UI thread. Weird bug where MF locks the thread
            Task task = new Task(() =>
            {
                Plugin.CheckHResult(PlayerPlugin.exPause(this.playerHandle), "RealTimePlayer::exPause");
                this.plugin.QueueAction(() => this.PlayerState = PlaybackState.Paused);
            });
            task.Start();
        }

        public void Stop()
        {
            if (!this.isPlayerCreated) return;

            Debug.Log("RealTimePlayer::Stop");

            // Run plugin command on background thread and not UI thread. Weird bug where MF locks the thread
            Task task = new Task(() =>
            {
                Plugin.CheckHResult(PlayerPlugin.exStop(this.playerHandle), "RealTimePlayer::exStop");
                this.plugin.QueueAction(() => this.PlayerState = PlaybackState.Ended);
            });
            task.Start();
        }

        private void OnStateChanged(PlayerPlugin.PLAYBACK_STATE args)
        {
            this.plugin.QueueAction(() =>
            {
                var stateType = (PlayerPlugin.StateType)Enum.ToObject(typeof(PlayerPlugin.StateType), args.type);

                // TODO: Update player state based on this*

                switch (stateType)
                {
                    case PlayerPlugin.StateType.StateChanged:
                        this.PlayerState = (PlaybackState)Enum.ToObject(typeof(PlaybackState), args.state);
                        Debug.Log("Playback State: " + stateType.ToString() + " - " + this.PlayerState.ToString());
                        break;
                    case PlayerPlugin.StateType.Opened:
                        PlayerPlugin.MEDIA_DESCRIPTION description = args.description;
                        Debug.Log("Media Opened: " + description.ToString());
                        break;
                    case PlayerPlugin.StateType.Failed:
                        Plugin.CheckHResult(args.hresult, "RealtimeVideoPlayer::OnStateChanged");
                        break;
                    default:
                        break;
                }
            });
        }

        private const uint MIN_D3D_RESOLUTION = 1;
        private const uint MAX_D3D_RESOLUTION = 16384;
        private bool IsValidResolution(uint width, uint height)
        {
            return width >= MIN_D3D_RESOLUTION && width <= MAX_D3D_RESOLUTION
                                               && height >= MIN_D3D_RESOLUTION && height <= MAX_D3D_RESOLUTION;
        }

        #endregion

        #region Realtime Player Plugin
        internal static class PlayerPlugin
        {
            // NOTE: Enums & structs must match values in RealtimeMediaPlayer.h in Native Plugin
            public enum StateType
            {
                None = 0,
                Opened,
                StateChanged,
                Failed,
            };

            [StructLayout(LayoutKind.Sequential, Pack = 4)]
            public struct MEDIA_DESCRIPTION
            {
                public UInt32 width;
                public UInt32 height;

                public override string ToString()
                {
                    StringBuilder sb = new StringBuilder();
                    sb.AppendLine("width: " + width);
                    sb.AppendLine("height: " + height);
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

            [UnmanagedFunctionPointer(CallingConvention.StdCall)]
            public delegate void StateChangedCallback(IntPtr senderPtr, PLAYBACK_STATE args);

            [UnmanagedFunctionPointer(CallingConvention.StdCall)]
            public delegate void PlayerCreatedCallback(IntPtr senderPtr, long result, uint width, uint height);

            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "CreateRealtimePlayer")]
            internal static extern long exCreatePlayer(uint connectionHandle,
                [MarshalAs(UnmanagedType.FunctionPtr)]StateChangedCallback callback, 
                [MarshalAs(UnmanagedType.FunctionPtr)]PlayerCreatedCallback createdCallback, 
                IntPtr objectPtr,
                ref uint playerHandle);

            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "ReleaseRealtimePlayer")]
            internal static extern long exReleasePlayer(uint playerHandle);

            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "GetPlayerStreamResolution")]
            internal static extern long exGetStreamResolution(uint playerHandle, ref uint width, ref uint height);

            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "CreateRealtimePlayerTexture")]
            internal static extern long exCreateExternalTexture(uint playerHandle, uint width, uint height, out System.IntPtr texture_L, out System.IntPtr texture_UV);

            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "RealtimePlayerPlay")]
            internal static extern long exPlay(uint playerHandle);

            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "RealtimePlayerPause")]
            internal static extern long exPause(uint playerHandle);

            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "RealtimePlayerStop")]
            internal static extern long exStop(uint playerHandle);
        }

        internal static class Player_PluginCallbackWrapper
        {
            [AOT.MonoPInvokeCallback(typeof(PlayerPlugin.PlayerCreatedCallback))]
            internal static void OnCreated_Callback(IntPtr senderPtr, long result, uint width, uint height)
            {
                var thisObj = Plugin.GetSenderObject<RealtimeVideoPlayer>(senderPtr);
                
                Plugin.ExecuteOnUnityThread(() => {
                    Debug.Log("Player_PluginCallbackWrapper::OnCreated_Callback " + width + " x " + height);

                    thisObj.OnCreated(result, width, height);
                });
            }

            [AOT.MonoPInvokeCallback(typeof(PlayerPlugin.StateChangedCallback))]
            internal static void OnStateChanged_Callback(IntPtr senderPtr, PlayerPlugin.PLAYBACK_STATE args)
            {
                var thisObj = Plugin.GetSenderObject<RealtimeVideoPlayer>(senderPtr);

                Plugin.ExecuteOnUnityThread(() => {
                    thisObj.OnStateChanged(args);
                });
            }
        }
#endregion
    }
}