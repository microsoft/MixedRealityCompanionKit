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
    public class Listener : NetworkComponent
    {
        public ushort Port { get; set; }

        public Listener()
        {
            this.handle = Plugin.InvalidHandle;

            if (!this.Initialize())
            {
                Debug.LogError("There was an issue initializing the Listener.");
            };
        }

        public override void StartAsync()
        {
            IntPtr thisObjectPtr = GCHandle.ToIntPtr(this.thisObject);
            var result = Wrapper.exStartListener(this.Port, ref this.handle, this.connectedHandler, thisObjectPtr);

            Plugin.CheckResult(result, "Listener.StartAsync()");

            base.OnStarted(result);
        }

        public override void Close()
        {
            base.Close();

            Wrapper.exStopListener(this.handle);
        }

        private void OnStarted()
        {

        }

        private static class Wrapper
        {
            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "ListenerCreateAndStart")]
            internal static extern int exStartListener(ushort port, ref uint listenerHandle, [MarshalAs(UnmanagedType.FunctionPtr)]PluginCallbackHandler StartedHandler, IntPtr senderObject);

            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "ListenerStopAndClose")]
            internal static extern int exStopListener(uint listenerHandle);
        };

    }
}
