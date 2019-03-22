// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Runtime.InteropServices;
using UnityEngine;

namespace RealtimeStreaming
{
    public class Connector : NetworkComponent
    {
        public Connector()
        {
            this.handle = Plugin.InvalidHandle;

            if (!this.Initialize())
            {
                Debug.LogError("There was an issue initializing the Connector.");
            };
        }

        public void ConnectAsync(string connectionUrl)
        {
            IntPtr thisObjectPtr = GCHandle.ToIntPtr(this.thisObject);
            var result = Wrapper.exOpenConnection(connectionUrl, ref this.handle, this.connectedHandler, thisObjectPtr);

            Plugin.CheckHResult(result, "Connector.ConnectAsync()");

            base.OnStarted(result);
        }

        public void DiscoverAsync()
        {
            IntPtr thisObjectPtr = GCHandle.ToIntPtr(this.thisObject);
            var result = Wrapper.exDiscoverConnection(ref this.handle, this.connectedHandler, thisObjectPtr);

            Plugin.CheckHResult(result, "Connector.DiscoverAsync()");

            base.OnStarted(result);
        }

        public override void Close()
        {
            Wrapper.exCloseConnector(this.handle);

            base.Close();
        }

        private static class Wrapper
        {
            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "ConnectorCreateAndStart")]
            internal static extern int exOpenConnection([MarshalAsAttribute(UnmanagedType.LPWStr)]string serverUrl, 
                ref uint handle, 
                [MarshalAs(UnmanagedType.FunctionPtr)]PluginCallbackHandler ConnectionOpenedCallback, 
                IntPtr senderObject);

            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "ConnectorCreateAndDiscover")]
            internal static extern int exDiscoverConnection(ref uint handle,
                            [MarshalAs(UnmanagedType.FunctionPtr)]PluginCallbackHandler ConnectionOpenedCallback,
                            IntPtr senderObject);

            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "ConnectorStopAndClose")]
            internal static extern int exCloseConnector(uint handle);
        }
    }
}
