// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Runtime.InteropServices;

namespace MixedRemoteViewCompositor
{
    public class ConnectionCallbackArgs : EventArgs
    {
        public Connection Connection { get; private set; }

        public ConnectionCallbackArgs(Connection connection)
        {
            this.Connection = connection;
        }
    }

    public enum ConnectionState
    {
        Idle,
        Connected,
        Closing,
        Closed,
        Disconnected,
        Failed
    };

    public enum DataType
    {
        Unknown = 0,
        SceneState,
        InputState,
    };

    public class DataReceivedArgs : EventArgs
    {
        public DataType Type { get; private set; }

        public byte[] Buffer { get; private set; }

        public DataReceivedArgs(DataType type, IntPtr buffer, int length)
        {
            this.Type = type;

            this.Buffer = new byte[length];

            Marshal.Copy(buffer, this.Buffer, 0, length);
        }
    };

    public class Connection : IDisposable
    {
        public uint Handle { get; private set; }

        public Action<object, EventArgs> Disconnected;
        public Action<object, EventArgs> Closed;
        public Action<object, DataReceivedArgs> DataReceived;

        private UInt64 disconnectedToken;
        private PluginCallbackHandler disconnectedHandler;
        private GCHandle disconnectedCallbackHandle;

        private UInt64 dataReceivedToken;
        private Wrapper.DataReceivedHandler dataReceivedHandler;
        private GCHandle dataReceivedCallbackHandle;

        private bool disposedValue = false; // To detect redundant calls

        public static Connection CreateConnection(uint handle)
        {
            Connection connection = new Connection();
            if (!connection.Initialize(handle))
            {
                connection = null;
            }

            return connection;
        }

        private Connection()
        {
            this.Handle = Plugin.InvalidHandle;

            this.disconnectedToken = 0;
            this.disconnectedHandler = null;
            this.disconnectedCallbackHandle = default(GCHandle);

            this.dataReceivedToken = 0;
            this.dataReceivedHandler = null;
            this.dataReceivedCallbackHandle = default(GCHandle);
        }

        ~Connection()
        {
            this.Dispose(false);
        }

        public void Dispose()
        {
            // Do not change this code. Put cleanup code in Dispose(bool disposing) above.
            this.Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!this.disposedValue)
            {
                if (disposing)
                {
                    this.Close();
                }

                if (this.disconnectedCallbackHandle.IsAllocated)
                {
                    this.disconnectedCallbackHandle.Free();
                }

                if (this.dataReceivedCallbackHandle.IsAllocated)
                {
                    this.dataReceivedCallbackHandle.Free();
                }

                this.disposedValue = true;
            }
        }

        private bool Initialize(uint connectionHandle)
        {
            bool returnResult = false;

            if (connectionHandle != Plugin.InvalidHandle)
            {
                this.Handle = connectionHandle;

                this.disconnectedHandler = (handle, result, message) =>
                {
                    if (handle != this.Handle)
                    {
                        return;
                    }

                    if (this.Disconnected != null)
                    {
                        this.Disconnected(this, EventArgs.Empty);
                    }
                };
                this.disconnectedCallbackHandle = GCHandle.Alloc(this.disconnectedHandler);
                Plugin.CheckResult(
                    Wrapper.exAddDisconnected(this.Handle, this.disconnectedHandler, ref this.disconnectedToken),
                    "Connection.AddDisconnected");

                this.dataReceivedHandler = (handle, type, length, buffer) =>
                {
                    if (handle != this.Handle || buffer == IntPtr.Zero)
                    {
                        return;
                    }

                    var packetType = Enum.ToObject(typeof(DataType), type) as DataType?;
                    if (packetType != null && packetType.Value != DataType.Unknown)
                    {
                        if (this.DataReceived == null)
                        {
                            return;
                        }

                        this.DataReceived(this, new DataReceivedArgs(packetType.Value, buffer, length));
                    }
                };
                this.dataReceivedCallbackHandle = GCHandle.Alloc(this.dataReceivedHandler);
                Plugin.CheckResult(
                    Wrapper.exAddReceived(this.Handle, this.dataReceivedHandler, ref this.dataReceivedToken),
                    "Connection.AddRecevied");

                returnResult = true;
            }

            return returnResult;
        }

        public void SendData(DataType messageType, byte[] buf, int length)
        {
            if (this.Handle == Plugin.InvalidHandle)
            {
                return;
            }

            Plugin.CheckResult(
                Wrapper.exSendRawMessage(this.Handle, messageType, buf, length),
                "Connection.SendNetworkMessage");
        }

        public void Close()
        {
            if (this.Handle != Plugin.InvalidHandle)
            {
                int result = Wrapper.exRemoveDisconnected(this.Handle, this.disconnectedToken);
                Plugin.CheckResult(result, "Connection.Close() - RemoveDisconnected");

                result = Wrapper.exRemoveReceived(this.Handle, this.dataReceivedToken);
                Plugin.CheckResult(result, "Connection.Close() - RemoveReceived");

                result = Wrapper.exClose(this.Handle);
                Plugin.CheckResult(result, "Connection.Close() - CloseConnection");
            }

            if (this.Closed != null)
            {
                this.Closed(this, EventArgs.Empty);
            }

            this.Handle = Plugin.InvalidHandle;
        }

        private static class Wrapper
        {
            [UnmanagedFunctionPointer(CallingConvention.StdCall)]
            internal delegate void DataReceivedHandler(uint msgHandle, ushort messageType, int length, IntPtr buffer);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcConnectionAddDisconnected")]
            internal static extern int exAddDisconnected(uint handle, [MarshalAs(UnmanagedType.FunctionPtr)]PluginCallbackHandler disconnectedCallback, ref UInt64 tokenValue);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcConnectionRemoveDisconnected")]
            internal static extern int exRemoveDisconnected(uint handle, UInt64 tokenValue);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcConnectionAddReceived")]
            internal static extern int exAddReceived(uint handle, [MarshalAs(UnmanagedType.FunctionPtr)]DataReceivedHandler dataReceivedCallback, ref UInt64 tokenValue);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcConnectionRemoveReceived")]
            internal static extern int exRemoveReceived(uint handle, UInt64 tokenValue);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcConnectionSendRawData")]
            internal static extern int exSendRawMessage(uint handle, DataType messageType, [MarshalAs(UnmanagedType.LPArray)] byte[] buf, int bufLength);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcConnectionClose")]
            internal static extern int exClose(uint handle);
        }
    }
}