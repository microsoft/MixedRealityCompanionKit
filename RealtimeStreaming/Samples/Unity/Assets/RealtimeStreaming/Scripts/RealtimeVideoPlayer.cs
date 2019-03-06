// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

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

        public string ConnectTo;
        public ushort Port = 27772;

        public bool StopOnPaused = false;

        public event EventHandler<StateChangedEventArgs<PlaybackState>> PlayerStateChanged;

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

                            this.PlayerStateChanged?.Invoke(this, args);
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
        public Texture2D Texture_Luma { get; private set; }
        public Texture2D Texture_Chroma { get; private set; }

        private PlayerPlugin.StateChangedCallback stateCallback;
        private GCHandle thisObject = default(GCHandle);

        private PlayerPlugin.PlayerCreatedCallbackHandler createdHandler;

        private bool isPaused = false;
        private bool isPlayerCreated = false;

        private uint playerHandle = Plugin.InvalidHandle;

        private void Awake()
        {
            this.plugin = this.GetComponent<Plugin>();

            this.playerHandle = Plugin.InvalidHandle;

            this.PlayerState = PlaybackState.None;

            thisObject = GCHandle.Alloc(this, GCHandleType.Normal);
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

            if (!this.StopOnPaused)
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

            if (this.isPlayerCreated)
            {
                this.Stop();

                this.CloseNetworkConnection();

                Plugin.CheckHResult(PlayerPlugin.exReleasePlayer(this.playerHandle), "RealtimeVideoPlayer::exReleasePlayer()");
                this.Texture_Luma = this.Texture_Chroma = null;

                isPlayerCreated = false;
            }
        }

#region Control Network 
        public void ConnectPlayer()
        {
            if (this.connector != null || this.isPlayerCreated) return;
            
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
                this.ConnectionState = ConnectionState.Connecting;
            });
        }

        private void OnConnectorFailed(object sender, FailedEventArgs args)
        {
            this.plugin.QueueAction(() =>
            {
                Debug.Log("RealtimeVideoPlayer::OnConnectorFailed");

                this.ConnectionState = ConnectionState.Failed;

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
            Plugin.CheckHResult(PlayerPlugin.exCreatePlayer(
                this.networkConnection.Handle,
                this.stateCallback,
                this.createdHandler,
                thisObjectPtr,
                ref this.playerHandle
                ), "RealtimeVideoPlayer::exCreatePlayer()");

            if (playerHandle == Plugin.InvalidHandle)
            {
                Shutdown();
                return;
            }

            isPlayerCreated = true;
        }

        private void OnCreated(long result, uint width, uint height)
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
                if (!IsValidResolution(width, height))
                {
                    Debug.Log("RealTimePlayer::OnCreated width/height invalid");

                    Plugin.CheckHResult(PlayerPlugin.exGetStreamResolution(this.playerHandle, 
                        ref this.textureWidth, ref this.textureHeight),
                        "RealtimeVideoPlayer::exGetStreamResolution");
                }
                else
                {
                    this.textureWidth = width;
                    this.textureHeight = height;
                }

                Debug.Log("RealTimePlayer::OnCreated - " + this.textureWidth + " by " + this.textureHeight);

                // TODO: Add documentation information here for YUV work

                // create native texture for playback
                IntPtr nativeTexture_L = IntPtr.Zero;
                IntPtr nativeTexture_UV = IntPtr.Zero;

                Plugin.CheckHResult(PlayerPlugin.exCreateExternalTexture(this.playerHandle,
                    this.textureWidth,
                    this.textureHeight,
                    out nativeTexture_L,
                    out nativeTexture_UV), 
                    "RealtimeVideoPlayer::exCreateExternalTexture");

                // Create the unity texture2ds 
                this.Texture_Luma =
                    Texture2D.CreateExternalTexture((int)this.textureWidth,
                    (int)this.textureHeight,
                    TextureFormat.RG16,
                    false, 
                    true,
                    nativeTexture_L);

                this.Texture_Chroma =
                    Texture2D.CreateExternalTexture((int)this.textureWidth,
                    (int)this.textureHeight,
                    TextureFormat.R8,
                    false,
                    true,
                    nativeTexture_UV);

                this.Play();
            });
        }

        public void Play()
        {
            Debug.Log("RealTimePlayer::Play");
            Plugin.CheckHResult(PlayerPlugin.exPlay(this.playerHandle), "RealTimePlayer::exPlay");

            this.PlayerState = PlaybackState.Playing;
        }

        public void Pause()
        {
            Debug.Log("RealTimePlayer::Pause");
            Plugin.CheckHResult(PlayerPlugin.exPause(this.playerHandle), "RealTimePlayer::exPause");

            this.PlayerState = PlaybackState.Paused;
        }

        public void Stop()
        {
            Debug.Log("RealTimePlayer::Stop");
            Plugin.CheckHResult(PlayerPlugin.exStop(this.playerHandle), "RealTimePlayer::exStop");

            this.PlayerState = PlaybackState.Ended;
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
            public delegate void PlayerCreatedCallbackHandler(IntPtr senderPtr, long result, uint width, uint height);

            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "CreateRealtimePlayer")]
            internal static extern long exCreatePlayer(uint connectionHandle,
                [MarshalAs(UnmanagedType.FunctionPtr)]StateChangedCallback callback, 
                [MarshalAs(UnmanagedType.FunctionPtr)]PlayerCreatedCallbackHandler createdCallback, 
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
            [AOT.MonoPInvokeCallback(typeof(PlayerPlugin.PlayerCreatedCallbackHandler))]
            internal static void OnCreated_Callback(IntPtr senderPtr, long result, uint width, uint height)
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