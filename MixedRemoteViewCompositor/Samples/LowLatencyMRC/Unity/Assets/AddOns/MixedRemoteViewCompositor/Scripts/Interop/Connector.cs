// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using UnityEngine;

namespace MixedRemoteViewCompositor
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
            var result = Wrapper.exOpenConnection(this.ConnectionUrl, ref this.handle, this.connectedHandler);

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
            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcConnectorCreateAndStart")]
            internal static extern int exOpenConnection([MarshalAsAttribute(UnmanagedType.LPWStr)]string serverUrl, ref uint handle, [MarshalAs(UnmanagedType.FunctionPtr)]PluginCallbackHandler ConnectionOpenedCallback);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcConnectorStopAndClose")]
            internal static extern int exCloseConnector(uint handle);
        }
    }
}
