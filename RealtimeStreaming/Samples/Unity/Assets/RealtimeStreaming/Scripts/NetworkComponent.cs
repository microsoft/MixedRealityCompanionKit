// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace RealtimeStreaming
{
    public delegate void ConnectedHandler(object sender, ref Connection connection);

    public abstract class NetworkComponent : IDisposable
    {
        public Action<object, EventArgs> Started;
        public Action<object, Connection> Connected;
        public Action<object, FailedEventArgs> Failed;
        public Action<object, EventArgs> Closed;

        // native managed callback
        protected PluginCallbackHandler connectedHandler;
        protected GCHandle thisObject = default(GCHandle);

        private bool disposedValue;

        public uint Handle { get { return this.handle; } }
        protected uint handle;

        protected NetworkComponent()
        {
            this.connectedHandler = null;

            this.disposedValue = false;
        }

        protected bool Initialize()
        {
            // define the native/managed handler
            this.connectedHandler = new PluginCallbackHandler(NetworkComponent_PluginCallbackWrapper.OnConnected_Callback);
            thisObject = GCHandle.Alloc(this, GCHandleType.Normal);

            return true;
        }

        private void OnConnected(uint handle, long result, string message)
        {
            bool failed = true;
            if (handle != Plugin.InvalidHandle)
            {
                // create Connection and pass ownership to it
                var connection = Connection.CreateConnection(handle);

                if (connection != null)
                {
                    failed = false;
                    this.Connected?.Invoke(this, connection);
                }
            }

            if (failed)
            {
                if (this.Failed != null)
                {
                    this.Failed(this,
                        new FailedEventArgs(result,
                            string.Format("NetworkComponent.Connected(): result: 0x{0} - {1}",
                                result.ToString("X", NumberFormatInfo.InvariantInfo), message)));
                }
                else
                {
                    Plugin.CheckHResult(result, "NetworkComponent.Connected()");
                }
            }
        }

        public abstract void StartAsync();

        public virtual void Close()
        {
            if (this.Closed != null)
            {
                this.Closed(this, EventArgs.Empty);
            }
        }

        protected virtual void OnStarted(int result)
        {
            if (result != 0)
            {
                if (this.Failed != null)
                {
                    this.Failed(this,
                        new FailedEventArgs(result,
                            string.Format("NetworkComponent.Started(): result: 0x{0}",
                                result.ToString("X", NumberFormatInfo.InvariantInfo))));
                }
                else
                {
                    Plugin.CheckHResult(result, "NetworkComponent.Connected()");
                }
                return;
            }
            else
            {
                // success send the started event
                if (this.Started != null)
                {
                    this.Started(this, EventArgs.Empty);
                }
            }
        }

        ~NetworkComponent()
        {
            // Do not change this code. Put cleanup code in Dispose(bool disposing).
            this.Dispose(false);
        }

        public void Dispose()
        {
            // Do not change this code. Put cleanup code in Dispose(bool disposing) below.
            this.Dispose(true);

            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!this.disposedValue)
            {
                this.Close();

                // free unmanaged resources (unmanaged objects) and override a finalizer below.
                if (thisObject.IsAllocated)
                {
                    thisObject.Free();
                    thisObject = default(GCHandle);
                }

                this.disposedValue = true;
            }
        }

        internal static class NetworkComponent_PluginCallbackWrapper
        {
            [AOT.MonoPInvokeCallback(typeof(PluginCallbackHandler))]
            internal static void OnConnected_Callback(uint handle, IntPtr senderPtr, long result)
            {
                var thisObj = Plugin.GetSenderObject<NetworkComponent>(senderPtr);

                Plugin.ExecuteOnUnityThread(() => {
                    thisObj.OnConnected(handle, result, string.Empty);
                });
            }
        }
    }
}
