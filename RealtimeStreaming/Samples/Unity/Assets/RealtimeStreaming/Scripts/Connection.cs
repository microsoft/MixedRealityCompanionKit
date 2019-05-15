// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Runtime.InteropServices;

namespace RealtimeStreaming
{
    public enum ConnectionState
    {
        Idle,
        Connected,
        Closed,
        Disconnected,
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

        public ConnectionState State { get; private set; }

        public Action<object, EventArgs> Disconnected;
        public Action<object, EventArgs> Closed;
        public Action<object, DataReceivedArgs> DataReceived;

        private GCHandle thisObject = default(GCHandle);

        private UInt64 disconnectedToken;
        private PluginCallbackHandler disconnectedHandler;

        private UInt64 dataReceivedToken;
        private Wrapper.DataReceivedHandler dataReceivedHandler;

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
            this.State = ConnectionState.Idle;

            this.disconnectedToken = 0;
            this.disconnectedHandler = null;

            this.dataReceivedToken = 0;
            this.dataReceivedHandler = null;
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

                // free unmanaged resources (unmanaged objects) and override a finalizer below.
                if (thisObject.IsAllocated)
                {
                    thisObject.Free();
                    thisObject = default(GCHandle);
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
                this.State = ConnectionState.Connected;

                thisObject = GCHandle.Alloc(this, GCHandleType.Normal);
                IntPtr thisObjectPtr = GCHandle.ToIntPtr(this.thisObject);

                this.disconnectedHandler = new PluginCallbackHandler(Connection_PluginCallbackWrapper.OnDisconnected_Callback);
                Plugin.CheckHResult(
                    Wrapper.exAddDisconnected(this.Handle, this.disconnectedHandler, thisObjectPtr, ref this.disconnectedToken),
                    "Connection.AddDisconnected");

                this.dataReceivedHandler = new Wrapper.DataReceivedHandler(Connection_PluginCallbackWrapper.OnDataReceived_Callback);
                Plugin.CheckHResult(
                    Wrapper.exAddReceived(this.Handle, this.dataReceivedHandler, thisObjectPtr, ref this.dataReceivedToken),
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

            Plugin.CheckHResult(
                Wrapper.exSendRawMessage(this.Handle, messageType, buf, length),
                "Connection.SendNetworkMessage");
        }

        public void Close()
        {
            if (this.Handle != Plugin.InvalidHandle)
            {
                Plugin.CheckHResult(Wrapper.exRemoveDisconnected(this.Handle, this.disconnectedToken), "Connection.Close() - RemoveDisconnected - Handle= " + this.Handle);

                Plugin.CheckHResult(Wrapper.exRemoveReceived(this.Handle, this.dataReceivedToken), "Connection.Close() - RemoveReceived - Handle= " + this.Handle);

                Plugin.CheckHResult(Wrapper.exClose(this.Handle), "Connection.Close() - CloseConnection - Handle= " + this.Handle);
            }

            this.State = ConnectionState.Closed;
            this.Closed?.Invoke(this, EventArgs.Empty);
            this.Handle = Plugin.InvalidHandle;
        }

        private void OnDisconnected(uint handle, long result, string message)
        {
            if (handle != this.Handle)
            {
                return;
            }

            this.State = ConnectionState.Disconnected;
            this.Disconnected?.Invoke(this, EventArgs.Empty);
        }

        private void OnDataReceived(uint handle, ushort messageType, int length, IntPtr buffer)
        {
            if (handle != this.Handle || buffer == IntPtr.Zero)
            {
                return;
            }

            var packetType = Enum.ToObject(typeof(DataType), messageType) as DataType?;
            if (packetType != null && packetType.Value != DataType.Unknown)
            {
                this.DataReceived?.Invoke(this, new DataReceivedArgs(packetType.Value, buffer, length));
            }
        }

        private static class Wrapper
        {
            [UnmanagedFunctionPointer(CallingConvention.StdCall)]
            internal delegate void DataReceivedHandler(uint msgHandle, IntPtr senderPtr, ushort messageType, int length, IntPtr buffer);

            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "ConnectionAddDisconnected")]
            internal static extern long exAddDisconnected(uint handle, [MarshalAs(UnmanagedType.FunctionPtr)]PluginCallbackHandler disconnectedCallback, IntPtr objectPtr, ref UInt64 tokenValue);

            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "ConnectionRemoveDisconnected")]
            internal static extern long exRemoveDisconnected(uint handle, UInt64 tokenValue);

            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "ConnectionAddReceived")]
            internal static extern long exAddReceived(uint handle, [MarshalAs(UnmanagedType.FunctionPtr)]DataReceivedHandler dataReceivedCallback, IntPtr objectPtr, ref UInt64 tokenValue);

            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "ConnectionRemoveReceived")]
            internal static extern long exRemoveReceived(uint handle, UInt64 tokenValue);

            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "ConnectionSendRawData")]
            internal static extern long exSendRawMessage(uint handle, DataType messageType, [MarshalAs(UnmanagedType.LPArray)] byte[] buf, int bufLength);

            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "ConnectionClose")]
            internal static extern long exClose(uint handle);
        }

        internal static class Connection_PluginCallbackWrapper
        {
            [AOT.MonoPInvokeCallback(typeof(PluginCallbackHandler))]
            internal static void OnDisconnected_Callback(uint handle, IntPtr senderPtr, long result)
            {
                var thisObj = Plugin.GetSenderObject<Connection>(senderPtr);

                Plugin.ExecuteOnUnityThread(() => {
                    thisObj.OnDisconnected(handle, result, string.Empty);
                });
            }

            [AOT.MonoPInvokeCallback(typeof(Wrapper.DataReceivedHandler))]
            internal static void OnDataReceived_Callback(uint msgHandle, IntPtr senderPtr, ushort messageType, int length, IntPtr buffer)
            {
                var thisObj = Plugin.GetSenderObject<Connection>(senderPtr);

                Plugin.ExecuteOnUnityThread(() => {
                    thisObj.OnDataReceived(msgHandle, messageType, length, buffer);
                });
            }
        }
    }
}