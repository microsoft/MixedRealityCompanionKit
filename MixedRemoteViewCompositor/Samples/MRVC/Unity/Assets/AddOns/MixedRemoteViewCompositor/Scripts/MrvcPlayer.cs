// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using UnityEngine;

namespace MixedRemoteViewCompositor
{
    public enum PlayerState
    {
        Idle,
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
    public class MrvcPlayer : MonoBehaviour
    {
        public Camera SceneCamera = null;

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

        private Plugin plugin = null;
        private Connection networkConnection = null;
        private PlaybackEngine playbackEngine = null;
        private MediaSampleUpdateArgs mediaUpdateSampleArgs;
        private VideoCompositor videoCompositor = null;
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

            if (this.SceneCamera == null)
            {
                this.SceneCamera = Camera.main;
            }

            this.videoCompositor = this.SceneCamera.GetComponent<VideoCompositor>();
            if (this.videoCompositor == null)
            {
                this.videoCompositor = this.SceneCamera.gameObject.AddComponent<VideoCompositor>();
            }

            // start off disabled
            this.videoCompositor.enabled = false;
        }

        private void Update()
        {
            if (isPaused)
            {
                return;
            }

            if (this.playbackEngine == null || this.videoCompositor == null || this.syncTexture == null)
            {
                return;
            }

            GetFrameData();
        }

        private void OnDisable()
        {
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
                this.PlaybackStop();
            }
            else
            {
                this.PlaybackInitialize(this.networkConnection);
            }
        }

        public void Initialize(Connection connection)
        {
            if (connection == null)
            {
                Debug.LogError("MrvcPlayer.Initialize() - requires a valid connection component to start.");

                return;
            }

            if (this.networkConnection != null || this.playbackEngine != null)
            {
                Debug.LogError("MrvcPlayer.Initialize() - cannot start until previous version is stopped.");

                return;
            }

            this.ConnectionState = ConnectionState.Idle;
            this.PlayerState = PlayerState.Idle;

            this.PlaybackInitialize(connection);
        }

        public void Shutdown()
        {
            if (this.playbackEngine != null)
            {
                this.PlaybackStop();
            }
            else
            {
                this.ConnectionClose();
            }
        }

        private void PlaybackInitialize(Connection connection)
        {
            if (connection == null)
            {
                return;
            }

            this.networkConnection = connection;
            this.networkConnection.Disconnected += this.OnDisconnected;
            this.networkConnection.Closed += this.OnConnectionClosed;
            this.ConnectionState = ConnectionState.Connected;

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
        private void PlaybackInitialized(PlaybackEngine engine)
        {
            this.playbackEngine = engine;
            if(this.playbackEngine != null)
            {
                this.PlaybackStart();
            }

            if (this.playerState != PlayerState.Starting)
            {
                this.PlayerState = PlayerState.Failed;

                this.Shutdown();
            }
        }

        private void PlaybackStart()
        {
            if (this.playbackEngine == null)
            {
                return;
            }

            this.playbackEngine.Started += this.OnPlaybackStarted;
            this.playbackEngine.Stopped += this.OnPlaybackStopped;
            this.playbackEngine.Closed += this.OnPlaybackClosed;
            this.playbackEngine.Failed += this.OnPlaybackFailed;
            this.playbackEngine.FrameSizeChanged += this.OnFrameSizeChanged;
            this.playbackEngine.Start();

            this.mediaUpdateSampleArgs = new MediaSampleUpdateArgs();

            this.PlayerState = PlayerState.Starting;
        }
        private void PlaybackStarted()
        {
            if (this.playerState != PlayerState.Started)
            {
                this.PlayerState = PlayerState.Failed;

                Shutdown();
            }
        }

        private void PlaybackStop()
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
        private void PlaybackStopped()
        {
            this.PlaybackClose();

            if (this.playerState != PlayerState.Closing)
            {
                this.playerState = PlayerState.Failed;

                Shutdown();
            }
        }

        private void PlaybackClose()
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
        private void PlaybackClosed()
        {
            if (this.playbackEngine != null)
            {
                this.playbackEngine.Closed -= this.OnPlaybackClosed;
                this.playbackEngine.Dispose();
            }
            this.playbackEngine = null;

            if (this.playerState != PlayerState.Closed)
            {
                this.PlayerState = PlayerState.Failed;
            }

            Shutdown();
        }

        private void ConnectionClose()
        {
            if (this.networkConnection == null)
            {
                return;
            }

            this.ConnectionState = ConnectionState.Closing;

            this.networkConnection.Close();
        }
        private void ConnectionClosed()
        {
            if (this.networkConnection == null)
            {
                return;
            }

            this.networkConnection.Closed -= this.OnConnectionClosed;
            this.networkConnection.Dispose();
            this.networkConnection = null;

            this.ConnectionState = ConnectionState.Closed;
        }

        // playback events
        private void OnPlaybackInitialized(PlaybackEngine engine)
        {
            this.plugin.QueueAction(() =>
            {
                this.PlayerState = PlayerState.Initialized;

                PlaybackInitialized(engine);
            });
        }
        private void OnPlaybackStarted(object sender, EventArgs e)
        {
            this.plugin.QueueAction(() =>
            {
                this.PlayerState = PlayerState.Started;

                this.PlaybackStarted();
            });
        }
        private void OnPlaybackStopped(object sender, EventArgs e)
        {
            this.plugin.QueueAction(() =>
            {
                this.PlayerState = PlayerState.Stopped;

                this.PlaybackStopped();
            });
        }
        private void OnPlaybackClosed(object sender, EventArgs e)
        {
            this.plugin.QueueAction(() =>
            {
                this.PlayerState = PlayerState.Closed;

                this.PlaybackClosed();
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


        // internal methods
        private void GetFrameData()
        {
            // get texture and matrix data syncronously
            if (this.playbackEngine.GetFrameData(ref this.mediaUpdateSampleArgs))
            {
                if (!this.videoCompositor.enabled)
                {
                    this.videoCompositor.enabled = true;
                }

                // copy the world transforms and set texture for compositor
                this.videoCompositor.VideoTexture = this.syncTexture;
                this.videoCompositor.CameraTransform = this.mediaUpdateSampleArgs.cameraViewTransform;
                this.videoCompositor.CameraProjection = this.mediaUpdateSampleArgs.cameraProjection;
            }
        }

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

                // set the texture on the compositor shader
                if (this.videoCompositor != null)
                {
                    this.videoCompositor.VideoTexture = this.syncTexture;
                }
                else
                {
                    Debug.LogWarning("Compositor shader is not set correctly.");
                }

                this.mediaUpdateSampleArgs.videoTexture = this.syncTexture.GetNativeTexturePtr();
            }
        }

        private void DestroyVideoTexture()
        {
            this.mediaUpdateSampleArgs.videoTexture = IntPtr.Zero;

            if (this.videoCompositor != null)
            {
                this.videoCompositor.VideoTexture = null;

                this.videoCompositor.enabled = false;
            }

            if (this.syncTexture != null)
            {
                Destroy(this.syncTexture);

                this.syncTexture = null;
            }
        }


        // network event callbacks
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

                ConnectionClosed();
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

    }
}
