// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Runtime.InteropServices;
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
            Closing,
            Disconnected,
            Failed,
            Ready,
        }

        public string ConnectTo;
        public ushort Port = 27772;

        public Action<object, StateChangedEventArgs<ServerState>> ServerStateChanged { get; internal set; }

        public ServerState CurrentState
        {
            get { return this.serverState; }
            private set
            {
                if (this.serverState != value)
                {
                    this.previousServerState = this.serverState;
                    this.serverState = value;
                    
                    Plugin.ExecuteOnUnityThread(() =>
                    {
                        this.ServerStateChanged?.Invoke(this,
                            new StateChangedEventArgs<ServerState>(this.previousServerState, this.serverState));
                    });
                }
            }
        }
        private ServerState serverState = ServerState.Idle;
        private ServerState previousServerState = ServerState.Idle;

        private Plugin plugin = null;
        private Listener listener;
        private Connection listenerConnection;

        private uint serverHandle = Plugin.InvalidHandle;

        private void Awake()
        {
            this.serverHandle = Plugin.InvalidHandle;

            this.plugin = this.GetComponent<Plugin>();

            this.CurrentState = ServerState.Idle;
        }

        private void OnDisable()
        {
            this.Teardown();
        }

        public void Teardown()
        {
            if (this.listener != null)
            {
                this.StopListener();
            }
            else
            {
                this.ShutdownServer();

                this.ConnectionClose();
            }
        }

        #region Listener Control & Callback Handlers
        public void StartListener()
        {
            // If listener is active, we will stop & re-create it
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

        public void StopListener()
        {
            if (this.listener == null)
            {
                return;
            }

            this.listener.Started -= this.OnListenerStarted;
            this.listener.Connected -= this.OnListenerConnected;
            this.listener.Failed -= this.OnListenerFailed;
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

                this.CreateServerForConnection(connection);

                this.StopListener();
            });
        }

    #endregion

        public bool CreateServerForConnection(Connection connection)
        {
            Debug.Log("RealtimeVideoServer::InitializeServer()");

            if (connection == null)
            {
                Debug.LogError("RealtimeVideoServer.Initialize() - requires a valid connection component to start.");
                return false;
            }

            if (this.listenerConnection != null)
            {
                Debug.LogError("RealtimeVideoServer.Initialize() - cannot start until previous instance is stopped.");
                return false;
            }

            this.listenerConnection = connection;
            this.listenerConnection.Disconnected += this.OnDisconnected;
            this.listenerConnection.Closed += this.OnConnectionClosed;

            uint handle = Plugin.InvalidHandle;
            var hr = VideoServerWrapper.exCreate(this.listenerConnection.Handle, ref handle);
            Plugin.CheckHResult(hr, "RealtimeVideoServer::exCreate");

            if (handle == Plugin.InvalidHandle || hr != Plugin.S_OK)
            {
                Debug.Log("VideoServerWrapper.exCreate - Failed");
                this.Teardown();
                return false;
            }
                
            this.serverHandle = handle;
            this.CurrentState = ServerState.Ready;

            return true;
        }

        public bool ShutdownServer()
        {
            if (!IsServerRunning())
            {
                Debug.Log("Cannot StopServer() - server not active");
                return false;
            }

            var hr = VideoServerWrapper.exShutdown(this.serverHandle);
            Plugin.CheckHResult(hr, "RealtimeVideoServer::StopServer");

            // Invalidate our handle
            this.serverHandle = Plugin.InvalidHandle;

            return hr == 0;
        }

        public bool WriteFrame(byte[] data)
        {
            if (data == null || data.Length <= 0)
            {
                Debug.LogError("RealtimeVideoServer::WriteFrame - byte[] data cannot be null");
                return false;
            }

            if (!IsServerRunning())
            {
                Debug.Log("Cannot WriteFrame() - server not active");
                return false;
            }

            var hr = VideoServerWrapper.exWrite(this.serverHandle, data, (uint)data.Length);
            Plugin.CheckHResult(hr, "RealtimeVideoServer::WriteFrame");

            return hr == Plugin.S_OK;
        }

        private bool IsServerRunning()
        {
            return this.listenerConnection != null && this.serverHandle != Plugin.InvalidHandle;
        }

        private void ConnectionClose()
        {
            if (this.listenerConnection == null)
            {
                return;
            }

            this.CurrentState = ServerState.Closing;

            this.serverHandle = Plugin.InvalidHandle;

            this.listenerConnection.Disconnected -= this.OnDisconnected;
            this.listenerConnection.Close();
        }

        private void OnDisconnected(object sender, EventArgs e)
        {
            this.plugin.QueueAction(() =>
            {
                Debug.Log("RealtimeVideoServer::OnDisconnected");

                this.CurrentState = ServerState.Disconnected;

                this.Teardown();
            });
        }

        private void OnConnectionClosed(object sender, EventArgs eventArgs)
        {
            this.plugin.QueueAction(() =>
            {
                Debug.Log("RealtimeVideoServer::OnConnectionClosed");

                this.CurrentState = ServerState.Idle;

                if (this.listenerConnection == null)
                {
                    return;
                }

                this.listenerConnection.Closed -= this.OnConnectionClosed;
                this.listenerConnection.Dispose();
                this.listenerConnection = null;
            });
        }

        private void OnConnectionFailed(object sender, EventArgs e)
        {
            Plugin.ExecuteOnUnityThread(() =>
            {
                this.CurrentState = ServerState.Failed;

                this.Teardown();
            });
        }

        private static class VideoServerWrapper
        {
            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "CreateRealtimeStreamingServer")]
            internal static extern int exCreate(uint connectionHandle, 
                ref uint serverHandle);

            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "RealtimeStreamingShutdown")]
            internal static extern int exShutdown(uint serverHandle);

            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "RealtimeStreamingWrite")]
            internal static extern int exWrite(uint serverHandle, 
                [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 2)]byte[] bufferData, 
                uint bufferSize);
        };
    }
}
