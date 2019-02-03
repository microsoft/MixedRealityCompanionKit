// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using UnityEngine;

namespace RealtimeStreaming
{
    public class Connector : NetworkComponent
    {
        public string ConnectionUrl { get; set; }

        public Connector()
        {
            this.handle = Plugin.InvalidHandle;

            if (!this.Initialize())
            {
                Debug.LogError("There was an issue initializing the Connector.");
            };
        }

        public override void StartAsync()
        {
            IntPtr thisObjectPtr = GCHandle.ToIntPtr(this.thisObject);
            var result = Wrapper.exOpenConnection(this.ConnectionUrl, ref this.handle, this.connectedHandler, thisObjectPtr);

            Plugin.CheckResult(result, "Connector.StartAsync()");

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
            internal static extern int exOpenConnection([MarshalAsAttribute(UnmanagedType.LPWStr)]string serverUrl, ref uint handle, [MarshalAs(UnmanagedType.FunctionPtr)]PluginCallbackHandler ConnectionOpenedCallback, IntPtr senderObject);

            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "ConnectorStopAndClose")]
            internal static extern int exCloseConnector(uint handle);
        }
    }
}
