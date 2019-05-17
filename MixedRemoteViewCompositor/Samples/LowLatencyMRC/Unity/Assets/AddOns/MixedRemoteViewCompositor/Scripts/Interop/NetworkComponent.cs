// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace MixedRemoteViewCompositor
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
        protected GCHandle connectedCallbackHandle;

        private bool disposedValue;

        public uint Handle { get { return this.handle; } }
        protected uint handle;

        protected NetworkComponent()
        {
            this.connectedHandler = null;
            this.connectedCallbackHandle = default(GCHandle);

            this.disposedValue = false;
        }

        protected bool Initialize()
        {
            // define the native/managed handler
            this.connectedHandler = (handle, result, message) =>
            {
                // create Connection and pass ownership to it
                var connection = Connection.CreateConnection(handle);

                if (connection != null)
                {
                    if (this.Connected != null)
                    {
                        this.Connected(this, connection);
                    }
                }
                else
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
                        Plugin.CheckResult(result, "NetworkComponent.Connected()");
                    }
                }
            };

            this.connectedCallbackHandle = GCHandle.Alloc(this.connectedHandler);

            return true;
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
                    Plugin.CheckResult(result, "NetworkComponent.Connected()");
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

                if (this.connectedCallbackHandle.IsAllocated)
                {
                    this.connectedCallbackHandle.Free();
                }

                this.disposedValue = true;
            }
        }
    }
}
