// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Runtime.InteropServices;
using System.Timers;
using UnityEngine;

namespace RealtimeStreaming
{
    [RequireComponent(typeof(Plugin))]
    public class RealtimeVideoServer : MonoBehaviour
    {
        public enum ServerState
        {
            Idle,
            ListenerStarting,
            Listening,
            ListenerConnected,
            ListenerFailed,
            Ready,
        }

        public GameObject debugTarget;

        public string ConnectTo;
        public ushort Port = 27772;

        public ServerState CurrentState {get;set;}

        //public Action<object, StateChangedEventArgs<ConnectionState>> CaptureStateChanged { get; internal set; }

        public ConnectionState ConnectionState
        {
            get { return this.connectionState; }
            private set
            {
                if (this.connectionState != value)
                {
                    this.previousConnectionState = this.connectionState;
                    this.connectionState = value;
                    /*
                    this.plugin.QueueAction(() =>
                    {
                        if (this.CaptureStateChanged != null)
                        {
                            var args = new StateChangedEventArgs<ConnectionState>(this.previousConnectionState, this.connectionState);
                            this.CaptureStateChanged(this, args);
                        }
                    });*/
                }
            }
        }
        private ConnectionState connectionState = ConnectionState.Idle;
        private ConnectionState previousConnectionState = ConnectionState.Idle;

        private Plugin plugin = null;
        private Listener listener;
        private Connection listenerConnection;

        private uint Handle { get; set; }

        public Texture2D tex;

        private byte[] _videoFrameBuffer;
        const int VIDEO_PELS = 921600;
        const int BUFFER_SIZE = VIDEO_PELS * 4;

        private Timer writeTimer;

        private void Awake()
        {
            this.Handle = Plugin.InvalidHandle;

            this.plugin = this.GetComponent<Plugin>();

            this.CurrentState = ServerState.Idle;
        }

        private void Start()
        {
            this._videoFrameBuffer = tex.GetRawTextureData();
            Debug.Log("Texture bytes: " + this._videoFrameBuffer.Length);

            if (this._videoFrameBuffer.Length != BUFFER_SIZE)
            {
                Debug.LogError("Input texture for RealtimeVideoServer.cs is not correct size. Expected to be " + BUFFER_SIZE + " bytes");
            }

            StartListener();
        }

        private void OnDisable()
        {
            this.Shutdown();
        }

        private float speed = 10.0f;

        private void Update()
        {
            if (Input.GetKey(KeyCode.P))
            {
                Debug.Log("Write frame input");
                if (this.Handle != Plugin.InvalidHandle)
                {
                    if (debugTarget != null)
                    {
                        debugTarget.transform.Rotate(Vector3.up, speed * Time.deltaTime);
                    }

                    this.WriteFrame();
                }
            }
        }

        /*
        private void OnApplicationPause(bool pauseStatus)
        {
            this.isPaused = pauseStatus;

            if (!this.isStarted || !this.StopOnPaused)
            {
                return;
            }

            if (this.isPaused)
            {
                this.StopCapture();
            }
            else
            {
                //this.InitCaptureEngine(this.networkConnection);
            }
        }*/

        private void StartListener()
        {
            Debug.Log("StartListener()");
            
            // we own the listener so we can just stop if needed
            StopListener();

            this.CurrentState = ServerState.ListenerStarting;

            // create listener
            this.listener = new Listener { Port = this.Port };
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

        private void OnListenerStarted(object sender, EventArgs e)
        {
            this.plugin.QueueAction(() =>
            {
                Debug.Log("OnListenerStarted");

                this.CurrentState = ServerState.Listening;
            });
        }

        private void OnListenerFailed(object sender, FailedEventArgs args)
        {
            this.plugin.QueueAction(() =>
            {
                Debug.Log("OnListenerFailed");

                this.CurrentState = ServerState.ListenerFailed;
            });
        }

        private void OnListenerConnected(object sender, Connection connection)
        {
            Plugin.ExecuteOnUnityThread(() =>
            {
                Debug.Log("OnListenerConnected");

                this.CurrentState = ServerState.ListenerConnected;

                this.InitializeServer(connection);

                this.StopListener();
            });
        }

        public void InitializeServer(Connection connection)
        {
            if (connection == null)
            {
                Debug.LogError("RealtimeVideoServer.Initialize() - requires a valid connection component to start.");

                return;
            }

            if (this.listenerConnection != null)
            {
                Debug.LogError("RealtimeVideoServer.Initialize() - cannot start until previous instance is stopped.");

                return;
            }

            this.listenerConnection = connection;
            this.listenerConnection.Disconnected += this.OnDisconnected;
            this.listenerConnection.Closed += this.OnConnectionClosed;

            this.ConnectionState = ConnectionState.Connected;

            uint handle = Plugin.InvalidHandle;
            VideoServerWrapper.exCreate(this.listenerConnection.Handle, ref handle);

            if (handle == Plugin.InvalidHandle)
            {
                // TODO: Handle errors here
                Debug.Log("VideoServerWrapper.exCreate - Failed");
                this.Shutdown();
                return;
            }
                
            this.Handle = handle;

            // create timer for writing frames
            writeTimer = new Timer(1000 / 30);
            writeTimer.Elapsed += WriteTimer_Elapsed;
            writeTimer.AutoReset = true;
            writeTimer.Enabled = true;
        }

        private void WriteTimer_Elapsed(object sender, ElapsedEventArgs e)
        {
            Plugin.ExecuteOnUnityThread(() =>
            {
                WriteFrame();
            });
        }

        private bool IsServerRunning()
        {
            return this.listenerConnection != null && this.Handle != Plugin.InvalidHandle;
        }

        public bool StopServer()
        {
            if (!IsServerRunning())
            {
                Debug.Log("Cannot StopServer() - server not active");
                return false;
            }

            return (VideoServerWrapper.exStop(this.Handle) == 0);
        }

        public bool WriteFrame()
        {
            if (!IsServerRunning())
            {
                Debug.Log("Cannot WriteFrame() - server not active");
                return false;
            }

            /*
            IntPtr unmanagedArray = Marshal.AllocHGlobal(_videoFrameBuffer.Length);
            Marshal.Copy(_videoFrameBuffer, 0, unmanagedArray, _videoFrameBuffer.Length);

            VideoServerWrapper.exWrite(this.Handle, unmanagedArray, (uint)_videoFrameBuffer.Length);

            Marshal.FreeHGlobal(unmanagedArray);
            return true;
            */

            return (VideoServerWrapper.exWrite(this.Handle, _videoFrameBuffer, (uint)this._videoFrameBuffer.Length) == 0);
        }

        public void Shutdown()
        {
            // TODO: turn off listener?

            this.StopServer();

            this.ConnectionClose();
        }

        private void ConnectionClose()
        {
            if (this.listenerConnection == null)
            {
                return;
            }

            this.ConnectionState = ConnectionState.Closing;

            this.listenerConnection.Close();
        }

        private void OnDisconnected(object sender, EventArgs e)
        {
            Plugin.ExecuteOnUnityThread(() =>
            {
                this.ConnectionState = ConnectionState.Disconnected;

                this.Shutdown();
            });
        }

        private void OnConnectionClosed(object sender, EventArgs eventArgs)
        {
            Plugin.ExecuteOnUnityThread(() =>
            {
                this.ConnectionState = ConnectionState.Closed;

                if (this.listenerConnection == null)
                {
                    return;
                }

                this.listenerConnection.Closed -= this.OnConnectionClosed;
                this.listenerConnection.Dispose();
                this.listenerConnection = null;

                this.ConnectionState = ConnectionState.Closed;
            });
        }

        private void OnConnectionFailed(object sender, EventArgs e)
        {
            Plugin.ExecuteOnUnityThread(() =>
            {
                this.ConnectionState = ConnectionState.Failed;

                this.Shutdown();
            });
        }

        private static class VideoServerWrapper
        {
            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "CreateRealtimeStreamingServer")]
            //internal static extern int exCreate(bool enableAudio, [MarshalAs(UnmanagedType.FunctionPtr)]PluginCallbackHandler createdCallback, IntPtr senderObject);
            internal static extern int exCreate(uint connectionHandle, ref uint serverHandle);

            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "RealtimeStreamingStop")]
            internal static extern int exStop(uint serverHandle);

            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "RealtimeStreamingWrite")]
            internal static extern int exWrite(uint serverHandle, [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 2)]byte[] bufferData, uint bufferSize);
            //internal static extern int exWrite(uint serverHandle, IntPtr bufferData, uint bufferSize);
        };
    }
}
