// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using UnityEngine;

namespace MixedRemoteViewCompositor
{
    public enum SelectedListener
    {
        PlaybackDevice,
        CaptureDevice
    }

    public enum ManagerState
    {
        Idle,
        CaptureStart,
        CaptureStop,
        PlaybackStart,
        PlaybackStop,
        ListenerStarting,
        Listening,
        ListenerConnected,
        ListenerFailed,
        ConnectorStarting,
        Connecting,
        ConnectorFailed,
        ConnectorConnected
    }

    public enum ComponentState
    {
        Idle,
        CaptureStarting,
        CaptureStarted,
        CaptureStopping,
        CaptureStopped,
        PlayerStarting,
        PlayerStarted,
        PlayerStopping,
        PlayerStopped
    }

    [RequireComponent(typeof(Plugin))]
    [AddComponentMenu("Mixed Remote View Compositor/Manager")]
    public class MrvcManager : MonoBehaviour
    {
        public SelectedListener SelectedListener = SelectedListener.CaptureDevice;
        public string ConnectTo;
        public ushort Port = 27772;
        public Camera SceneCamera = null;
        public bool EnableMRC = false;
        public bool StopOnPaused = false;

        public ManagerState ManagerState
        {
            get { return this.managerState; }
            private set
            {
                if (this.managerState != value)
                {
                    this.previousManagerState = this.managerState;
                    this.managerState = value;
                    this.plugin.QueueAction(() =>
                    {
                        if (this.ManagerStateChanged != null)
                        {
                            this.ManagerStateChanged(this, new StateChangedEventArgs<ManagerState>(this.previousManagerState,
                                this.managerState));
                        }
                    });
                }
            }
        }
        private ManagerState managerState = ManagerState.Idle;
        private ManagerState previousManagerState = ManagerState.Idle;

        public ComponentState ComponentState
        {
            get { return this.componentState; }
            private set
            {
                if (this.componentState != value)
                {
                    this.previousComponentState = this.componentState;
                    this.componentState = value;
                    this.plugin.QueueAction(() =>
                    {
                        if (this.ComponentStateChanged != null)
                        {
                            this.ComponentStateChanged(this, new StateChangedEventArgs<ComponentState>(this.previousComponentState,
                                this.componentState));
                        }
                    });
                }
            }
        }
        private ComponentState componentState = ComponentState.Idle;
        private ComponentState previousComponentState = ComponentState.Idle;

        private bool isPaused = false;
        private bool isStarted = false;
        private bool restartListener = false;

        private Plugin plugin;
        private Listener listener;
        private Connection listenerConnection;
        private Connector connector;
        private Connection connectorConnection;

        private MrvcPlayer playbackEngine;
        private MrvcCapture captureEngine;

        public Action<object, StateChangedEventArgs<ManagerState>> ManagerStateChanged;
        public Action<object, StateChangedEventArgs<ComponentState>> ComponentStateChanged;
        public Action<object, StateChangedEventArgs<CaptureState>> CaptureStateChanged;
        public Action<object, StateChangedEventArgs<PlayerState>> PlayerStateChanged;

        private void Awake()
        {
            this.plugin = this.GetComponent<Plugin>();
        }

        private void Start()
        {
            this.isStarted = true;

            if (SelectedListener == SelectedListener.CaptureDevice)
            {
                StartListener();
            }
        }

        private void Update()
        {
            if (this.isPaused)
            {
                return;
            }

            // look for listener restart request
            if (!this.restartListener)
            {
                return;
            }

            this.restartListener = false;

            // restart listener
            StartListener();
        }

        private void OnApplicationPause(bool pauseStatus)
        {
            this.isPaused = pauseStatus;

            if (!this.isStarted)
            {
                return;
            }

            // stop on paused?
            if (this.isPaused)
            {
            }
            else
            {
            }
        }

        private void OnDisable()
        {
            Shutdown();
        }

        private void OnDestroy()
        {
            Shutdown();
        }

        private void Shutdown()
        {
            StopPlayback();
            StopCapture();
        }

        public void StartCapture()
        {
            this.ManagerState = ManagerState.CaptureStart;

            if (this.captureEngine == null)
            {
                this.StartCaptureNetworkComponent();
            }
        }

        public void StopCapture()
        {
            this.ManagerState = ManagerState.CaptureStop;

            if (this.captureEngine != null)
            {
                this.StopMrvcCapture();
            }
            this.StopCaptureNetworkComponent();
        }
        public void StartPlayback()
        {
            this.ManagerState = ManagerState.PlaybackStart;

            if (this.playbackEngine == null)
            {
                this.StartPlaybackNetworkComponent();
            }
        }
        public void StopPlayback()
        {
            this.ManagerState = ManagerState.PlaybackStop;

            if (this.playbackEngine != null)
            {
                this.StopMrvcPlayer();
            }
            this.StopPlaybackNetworkComponent();
        }
        public void SetMRC(bool value)
        {
            this.EnableMRC = value;
        }

        public void SetConnectorAddress(string ipAddress)
        {
            this.ConnectTo = ipAddress;
        }

        private void StartCaptureNetworkComponent()
        {
            if (this.SelectedListener == SelectedListener.CaptureDevice)
            {
                StartListener();
            }
            else
            {
                StartConnector();
            }
        }
        private void StopCaptureNetworkComponent()
        {
            if (this.SelectedListener == SelectedListener.CaptureDevice)
            {
                StopListener();
            }
            else
            {
                StopConnector();
            }
        }

        private void StartPlaybackNetworkComponent()
        {
            if (this.SelectedListener == SelectedListener.CaptureDevice)
            {
                StartConnector();
            }
            else
            {
                StartListener();
            }
        }
        private void StopPlaybackNetworkComponent()
        {
            if (this.SelectedListener == SelectedListener.CaptureDevice)
            {
                StopConnector();
            }
            else
            {
                StopListener();
            }
        }

        private void StartListener()
        {
            Debug.Log("StartListener()");
            // we own the listener so we can just stop if needed
            StopListener();

            this.ManagerState = ManagerState.ListenerStarting;

            // create listener
            this.listener = new Listener {Port = this.Port};
            if (this.listener != null)
            {
                this.listener.Started += this.OnListenerStarted;
                this.listener.Connected += this.OnListenerConnected;
                this.listener.Failed += this.OnListenerFailed;
                this.listener.StartAsync();
            }
        }
        private void StopListener()
        {
            if (this.listener == null)
            {
                return;
            }

            this.listener.Started -= this.OnListenerStarted;
            this.listener.Connected -= this.OnListenerConnected;
            this.listener.Failed -= this.OnListenerFailed;
            this.listener.Close();
            this.listener.Dispose();
            this.listener = null;
        }

        private void StartConnector()
        {
            // we own the connection so we can just stop if if needed
            StopConnector();

            this.ManagerState = ManagerState.ConnectorStarting;

            this.connector = new Connector {ConnectionUrl = string.Format(Plugin.MediaUrlFormat, this.ConnectTo, this.Port)};
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

        private void StartCaptureFromConnection(Connection connection)
        {
            if (connection == null)
            {
                return;
            }

            if (this.captureEngine == null)
            {
                StartMrvcCapture(connection);
            }
        }
        private void StartPlaybackFromConnection(Connection connection)
        {
            if (connection == null)
            {
                return;
            }

            if (this.playbackEngine == null)
            {
                StartMrvcPlayer(connection);
            }
        }

        private void StartMrvcCapture(Connection connection)
        {
            if (connection == null)
            {
                return;
            }

            this.ComponentState = ComponentState.CaptureStarting;

            if (this.captureEngine == null)
            {
                this.captureEngine = this.gameObject.AddComponent<MrvcCapture>();
                if (this.captureEngine != null)
                {
                    this.captureEngine.CaptureStateChanged += this.OnCaptureStateChanged;
                    this.captureEngine.EnableMRC = this.EnableMRC;
                    this.captureEngine.StopOnPaused = this.StopOnPaused;
                    this.captureEngine.Initialize(connection);
                }
            }
        }
        private void StopMrvcCapture()
        {
            if (this.captureEngine == null)
            {
                return;
            }

            this.ComponentState = ComponentState.CaptureStopping;

            // wait for the MrvcCapture to change 
            // to Closed state before completing
            this.captureEngine.Shutdown();
        }
        private void CompleteStopMrvcCapture()
        {
            if (this.captureEngine == null)
            {
                return;
            }

            // unsubscribe to events
            this.captureEngine.CaptureStateChanged -= this.OnCaptureStateChanged;

            DestroyImmediate(this.captureEngine, true);

            this.captureEngine = null;

            this.ComponentState = ComponentState.CaptureStopped;
        }

        private void StartMrvcPlayer(Connection connection)
        {
            if (connection == null)
            {
                return;
            }

            this.ComponentState = ComponentState.PlayerStarting;

            if (this.playbackEngine == null)
            {
                this.playbackEngine = this.gameObject.AddComponent<MrvcPlayer>();
                if (this.playbackEngine != null)
                {
                    this.playbackEngine.PlayerStateChanged += this.OnPlayerStateChanged;
                    this.playbackEngine.SceneCamera = this.SceneCamera;
                    this.playbackEngine.StopOnPaused = this.StopOnPaused;
                    this.playbackEngine.Initialize(connection);
                }
            }
        }
        private void StopMrvcPlayer()
        {
            if (this.playbackEngine == null)
            {
                return;
            }

            this.ComponentState = ComponentState.PlayerStopping;

            // wait for the MrvcPlayer to change 
            // to Closed state before completing
            this.playbackEngine.Shutdown();
        }
        private void CompleteStopMrvcPlayer()
        {
            if (this.playbackEngine == null)
            {
                return;
            }

            // unsubscribe from events
            this.playbackEngine.PlayerStateChanged -= this.OnPlayerStateChanged;

            DestroyImmediate(this.playbackEngine, true);

            this.playbackEngine = null;

            this.ComponentState = ComponentState.PlayerStopped;
        }

        private void OnListenerStarted(object sender, EventArgs e)
        {
            this.plugin.QueueAction(() =>
            {
                this.ManagerState = ManagerState.Listening;
            });
        }
        private void OnListenerFailed(object sender, FailedEventArgs args)
        {
            this.plugin.QueueAction(() =>
            {
                this.ManagerState = ManagerState.ListenerFailed;
            });
        }
        private void OnListenerConnected(object sender, Connection connection)
        {
            this.listenerConnection = connection;

            this.plugin.QueueAction(() =>
            {
                this.ManagerState = ManagerState.ListenerConnected;

                if (this.SelectedListener == SelectedListener.CaptureDevice)
                {
                    this.StartCaptureFromConnection(this.listenerConnection);
                }
                else
                {
                    this.StartPlaybackFromConnection(this.listenerConnection);
                }

                this.listenerConnection = null;

                this.StopListener();
            });
        }

        private void OnConnectorStarted(object sender, EventArgs e)
        {
            this.plugin.QueueAction(() =>
            {
                this.ManagerState = ManagerState.Connecting;
            });
        }
        private void OnConnectorFailed(object sender, FailedEventArgs args)
        {
            this.plugin.QueueAction(() =>
            {
                this.ManagerState = ManagerState.ConnectorFailed;
            });
        }
        private void OnConnectorConnected(object sender, Connection connection)
        {
            this.connectorConnection = connection;

            this.plugin.QueueAction(() =>
            {
                this.ManagerState = ManagerState.ConnectorConnected;

                if (this.SelectedListener == SelectedListener.CaptureDevice)
                {
                    this.StartPlaybackFromConnection(this.connectorConnection);
                }
                else
                {
                    this.StartCaptureFromConnection(this.connectorConnection);
                }

                this.connectorConnection = null;

                this.StopConnector();
            });
        }

        private void OnCaptureStateChanged(object sender, StateChangedEventArgs<CaptureState> args)
        {
            if (this.CaptureStateChanged != null)
            {
                this.CaptureStateChanged(this, args);
            }

            if (args.CurrentState == CaptureState.Closed && args.ConnectionState == ConnectionState.Closed)
            {
                this.plugin.QueueAction(() =>
                {
                    CompleteStopMrvcCapture();

                    restartListener = true;
                });
            }
        }

        private void OnPlayerStateChanged(object sender, StateChangedEventArgs<PlayerState> args)
        {
            if (this.PlayerStateChanged != null)
            {
                this.PlayerStateChanged(this, args);
            }

            if (args.CurrentState == PlayerState.Closed && args.ConnectionState == ConnectionState.Closed)
            {
                this.plugin.QueueAction(() =>
                {
                    CompleteStopMrvcPlayer();
                });
            }
        }
    }
}
