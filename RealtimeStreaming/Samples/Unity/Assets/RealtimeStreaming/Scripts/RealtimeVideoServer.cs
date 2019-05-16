// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Runtime.InteropServices;
using UnityEngine;

namespace RealtimeStreaming
{
    [RequireComponent(typeof(PluginUtils))]
    public class RealtimeVideoServer : MonoBehaviour
    {
        public enum ServerState
        {
            Idle,
            Listening,
            Ready,
            Failed
        }

        public enum Encoding
        {
            H264,
            H265
        }

        public ushort Port = 27772;
        public uint OutputWidth = 1280;
        public uint OutputHeight = 720;
        public Encoding OutputEncoding = Encoding.H264;

        public Action<object, StateChangedEventArgs<ServerState>> ServerStateChanged { get; internal set; }

        public ServerState CurrentState
        {
            get { return this.serverState; }
            private set
            {
                if (this.serverState != value)
                {
                    var previousServerState = this.serverState;
                    this.serverState = value;

                    PluginUtils.ExecuteOnUnityThread(() =>
                    {
                        this.ServerStateChanged?.Invoke(this, new StateChangedEventArgs<ServerState>(previousServerState, this.serverState));
                    });
                }
            }
        }
        private ServerState serverState = ServerState.Idle;

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

        protected PluginUtils plugin = null;
        protected Listener listener;
        protected Connection networkConnection;

        private uint serverHandle = PluginUtils.InvalidHandle;

        protected bool isServerRunning
        {
            get
            {
                return this.serverHandle != PluginUtils.InvalidHandle;
            }
        }

        private void Awake()
        {
            this.serverHandle = PluginUtils.InvalidHandle;

            this.plugin = this.GetComponent<PluginUtils>();

            this.CurrentState = ServerState.Idle;
        }

        public void Shutdown()
        {
            if (this.listener != null)
            {
                StopListening();
            }
            else
            {
                CloseConnection();
                DestroyServer();
            }
        }

        public void StartListening()
        {
            // If listener is active, we will stop & re-create it
            StopListening();

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

        public void StopListening()
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

        public bool WriteFrame(byte[] data)
        {
            if (data == null || data.Length <= 0)
            {
                Debug.LogError("RealtimeVideoServer::WriteFrame - byte[] data cannot be null");
                return false;
            }

            if (!isServerRunning)
            {
                Debug.LogError("Cannot WriteFrame() - server not active");
                return false;
            }

            var hr = VideoServerWrapper.exWrite(this.serverHandle, data, (uint)data.Length);
            PluginUtils.CheckHResult(hr, "RealtimeVideoServer::WriteFrame");

            return hr == PluginUtils.S_OK;
        }

        #region Listener Callbacks

        private void OnListenerStarted(object sender, EventArgs e)
        {
            this.plugin.QueueAction(() =>
            {
                Debug.Log("RealtimeVideoServer::OnListenerStarted");
                this.CurrentState = ServerState.Listening;
            });
        }

        private void OnListenerFailed(object sender, FailedEventArgs args)
        {
            this.plugin.QueueAction(() =>
            {
                Debug.Log("RealtimeVideoServer::OnListenerFailed");
                this.CurrentState = ServerState.Failed;
            });
        }

        private void OnListenerConnected(object sender, Connection connection)
        {
            this.plugin.QueueAction(() =>
            {
                Debug.Log("RealtimeVideoServer::OnListenerConnected");

                this.StopListening();

                this.CreateServer(connection);

                // this.StopListening();
            });
        }

        #endregion

        #region Server Control Helpers
        private bool CreateServer(Connection connection)
        {
            Debug.Log("RealtimeVideoServer::InitializeServer()");

            if (connection == null)
            {
                Debug.LogError("RealtimeVideoServer.Initialize() - requires a valid connection component to start.");
                return false;
            }

            if (isServerRunning)
            {
                Debug.LogError("RealtimeVideoServer.Initialize() - cannot start until previous instance is stopped.");
                return false;
            }

            this.networkConnection = connection;
            this.networkConnection.Disconnected += this.OnDisconnected;
            this.networkConnection.Closed += this.OnConnectionClosed;

            uint handle = PluginUtils.InvalidHandle;
            bool useHEVC = this.OutputEncoding == Encoding.H265;
            var hr = VideoServerWrapper.exCreate(this.networkConnection.Handle, useHEVC, this.OutputWidth, this.OutputHeight, ref handle);
            PluginUtils.CheckHResult(hr, "RealtimeVideoServer::exCreate");

            if (handle == PluginUtils.InvalidHandle || hr != PluginUtils.S_OK)
            {
                Debug.Log("VideoServerWrapper.exCreate - Failed");
                Shutdown();
                return false;
            }

            this.serverHandle = handle;
            this.CurrentState = ServerState.Ready;

            return true;
        }

        private bool DestroyServer()
        {
            if (isServerRunning)
            {
                Debug.Log("Cannot StopServer() - server not active");
                return false;
            }

            var hr = VideoServerWrapper.exShutdown(this.serverHandle);
            PluginUtils.CheckHResult(hr, "RealtimeVideoServer::StopServer");

            // Invalidate our handle
            this.serverHandle = PluginUtils.InvalidHandle;

            return hr == PluginUtils.S_OK;
        }

        #endregion

        #region Connection Callbacks & Helpers

        private void CloseConnection()
        {
            if (this.networkConnection == null)
            {
                return;
            }

            this.serverHandle = PluginUtils.InvalidHandle;

            this.networkConnection.Disconnected -= this.OnDisconnected;
            this.networkConnection.Close();
        }

        private void DisposeConnection()
        {
            if (this.networkConnection == null)
            {
                return;
            }

            this.networkConnection.Closed -= this.OnConnectionClosed;
            this.networkConnection.Dispose();
            this.networkConnection = null;
        }

        private void OnDisconnected(object sender, EventArgs e)
        {
            this.plugin.QueueAction(() =>
            {
                Debug.Log("RealtimeVideoServer::OnDisconnected");

                Shutdown();
            });
        }

        private void OnConnectionClosed(object sender, EventArgs eventArgs)
        {
            this.plugin.QueueAction(() =>
            {
                Debug.Log("RealtimeVideoServer::OnConnectionClosed");

                Shutdown();
            });
        }

        private void OnConnectionFailed(object sender, EventArgs e)
        {
            PluginUtils.ExecuteOnUnityThread(() =>
            {
                Shutdown();
                this.CurrentState = ServerState.Failed;
            });
        }
        #endregion

        private static class VideoServerWrapper
        {
            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "CreateRealtimeStreamingServer")]
            internal static extern int exCreate(uint connectionHandle, bool useHEVC, uint width, uint height, ref uint serverHandle);

            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "RealtimeStreamingShutdown")]
            internal static extern int exShutdown(uint serverHandle);

            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "RealtimeStreamingWrite")]
            internal static extern int exWrite(uint serverHandle, 
                [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 2)]byte[] bufferData, 
                uint bufferSize);
        };
    }
}
