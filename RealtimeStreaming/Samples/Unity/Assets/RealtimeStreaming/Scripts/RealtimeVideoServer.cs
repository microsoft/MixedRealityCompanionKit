// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Runtime.InteropServices;
using System.Timers;
using UnityEngine;
using UnityEngine.UI;

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
        public RawImage debugImg;

        private WebCamTexture webcam;
        private Color32[] webcam_interop;
        private byte[] frameBuffer;

        private Timer writeTimer;

        private void Awake()
        {
            this.Handle = Plugin.InvalidHandle;

            this.plugin = this.GetComponent<Plugin>();

            this.CurrentState = ServerState.Idle;
        }

        private void Start()
        {
            webcam = new WebCamTexture(1280, 720);
            debugImg.texture = webcam;
            debugImg.material.mainTexture = webcam;
            webcam.Play();

            webcam_interop = new Color32[webcam.width * webcam.height];

            frameBuffer = new byte[webcam.width * webcam.height * 4];

            StartListener();
        }

        private void OnDisable()
        {
            this.Shutdown();
        }

        private float speed = 10.0f;

        private void Update()
        {
            if (this.Handle != Plugin.InvalidHandle)
            {
                this.WriteFrame();

                if (debugTarget != null)
                {
                    debugTarget.transform.Rotate(Vector3.up, speed * Time.deltaTime);
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
            Debug.Log("RealtimeVideoServer::InitializeServer()");

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
        }

        private void StartTimer()
        {
            // create timer for writing frames
            writeTimer = new Timer(1000 / 30);
            writeTimer.Elapsed += WriteTimer_Elapsed;
            writeTimer.AutoReset = true;
            writeTimer.Enabled = true;
        }

        private void WriteTimer_Elapsed(object sender, ElapsedEventArgs e)
        {
            this.plugin.QueueAction(() =>
            //Plugin.ExecuteOnUnityThread(() =>
            {
                Debug.Log("Timer!");
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

            webcam.GetPixels32(webcam_interop);

            // TODO: Parrelize copy?
            int byteIdx = 0;
            bool notAlpha = false;
            for (int i = 0; i < webcam_interop.Length; i++)
            {
                // TODO: webcamtexture vertically flipped?
                /*
                Color32 c = !webcam.videoVerticallyMirrored ?
                    webcam_interop[webcam_interop.Length - i - 1] :
                    webcam_interop[i];
                */

                Color32 c = webcam_interop[webcam_interop.Length - i - 1];

                frameBuffer[byteIdx] = c.b;
                frameBuffer[byteIdx + 1] = c.g;
                frameBuffer[byteIdx + 2] = c.r;
                frameBuffer[byteIdx + 3] = c.a;

                if (c.a < 255)
                    notAlpha = true;

                byteIdx += 4;
            }

            if (notAlpha)
                Debug.Log("Pixel had < 255 alpha");

            return (VideoServerWrapper.exWrite(this.Handle, frameBuffer, (uint)this.frameBuffer.Length) == 0);
        }

        public void Shutdown()
        {
            // TODO: turn off listener?

            if (writeTimer != null)
            {
                writeTimer.Close();
                writeTimer = null;
            }

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
            this.plugin.QueueAction(() =>
            {
                this.ConnectionState = ConnectionState.Disconnected;

                this.Shutdown();
            });
        }

        private void OnConnectionClosed(object sender, EventArgs eventArgs)
        {
            this.plugin.QueueAction(() =>
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
            internal static extern int exCreate(uint connectionHandle, 
                ref uint serverHandle);

            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "RealtimeStreamingStop")]
            internal static extern int exStop(uint serverHandle);

            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "RealtimeStreamingWrite")]
            internal static extern int exWrite(uint serverHandle, 
                [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 2)]byte[] bufferData, 
                uint bufferSize);
        };
    }
}
