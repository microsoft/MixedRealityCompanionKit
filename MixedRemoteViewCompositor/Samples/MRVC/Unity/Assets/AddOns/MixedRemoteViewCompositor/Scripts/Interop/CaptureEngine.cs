// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Globalization;
using System.Runtime.InteropServices;
using UnityEngine;

namespace MixedRemoteViewCompositor
{
    public class CaptureEngine : IDisposable
    {
        public Action<object, EventArgs> Started;
        public Action<object, EventArgs> Stopped;
        public Action<object, EventArgs> Closed;
        public Action<object, FailedEventArgs> Failed;

        private uint Handle { get; set; }

        private GCHandle thisObject = default(GCHandle);

        private PluginCallbackHandler createdHandler;
        private PluginCallbackHandler startedHandler;        
        private PluginCallbackHandler stoppedHandler;

        // IDisposable
        private bool disposedValue = false;

        public static bool Create(Action<CaptureEngine> created)
        {
            if(created == null)
            {
                return false;
            }

            CaptureEngine engine = new CaptureEngine();
            if (engine == null)
            {
                return false;
            }

            if (!engine.Initialize(created, engine))
            {
                return false;
            }

            return true;
        }

        public void Start(Connection connection)
        {
            int result = 0;

            if (connection == null)
            {
                result = -1;

                if (this.Failed != null)
                {
                    this.Failed(this, new FailedEventArgs(result, string.Format("CaptureEngine.Stop(): result: 0x{0}", result.ToString("X", NumberFormatInfo.InvariantInfo))));
                }
                else
                {
                    Plugin.CheckResult(result, "CaptureEngine.Start()");
                }

                return;
            }

            result = Wrapper.exStart(this.Handle, connection.Handle, this.startedHandler);
            if (result != 0)
            {
                if (this.Failed != null)
                {
                    this.Failed(this, new FailedEventArgs(result, string.Format("CaptureEngine.Stop(): result: 0x{0}", result.ToString("X", NumberFormatInfo.InvariantInfo))));
                }
                else
                {
                    Plugin.CheckResult(result, "CaptureEngine.Start()");
                }
            }
        }

        public void Stop()
        {
            int result = Wrapper.exStop(this.Handle, this.stoppedHandler);
            if (result != 0)
            {
                if (this.Failed != null)
                {
                    this.Failed(this, new FailedEventArgs(result, string.Format("CaptureEngine.Stop(): result: 0x{0}", result.ToString("X", NumberFormatInfo.InvariantInfo))));
                }
                else
                {
                    Plugin.CheckResult(result, "CaptureEngine.Stop()");
                }

                return;
            }

            if (this.Stopped != null)
            {
                this.Stopped(this, EventArgs.Empty);
            }
        }

        public void Close()
        {
            if (this.Handle == Plugin.InvalidHandle)
            {
                return;
            }

            int result = Wrapper.exClose(this.Handle);
            if (result != 0)
            {
                Plugin.CheckResult(result, "CaptureEngine.Close()");
            }

            if (this.Closed != null)
            {
                this.Closed(this, EventArgs.Empty);
            }

            this.Handle = Plugin.InvalidHandle;
        }

        private CaptureEngine()
        {
            this.Handle = Plugin.InvalidHandle;

            this.Started = null;
            this.Stopped = null;
            this.Closed = null;
            this.Failed = null;
        }

        ~CaptureEngine()
        {
            // Do not change this code. Put cleanup code in Dispose(bool disposing).
            this.Dispose(false);
        }

        public void Dispose()
        {
            // Do not change this code. Put cleanup code in Dispose(bool disposing).
            this.Dispose(true);

            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (this.disposedValue)
            {
                return;
            }

            this.Close();

            this.disposedValue = true;
        }

        private bool Initialize(Action<CaptureEngine> created, CaptureEngine engine)
        {
            // pin this object in the GC
            thisObject = GCHandle.Alloc(this, GCHandleType.Normal);

            this.createdHandler = new PluginCallbackHandler(PluginCallbackWrapper.OnCreated_Callback); // Need callbackwrapper class for every
            this.startedHandler = new PluginCallbackHandler(PluginCallbackWrapper.OnStarted_Callback); // Need callbackwrapper class for every
            this.stoppedHandler = new PluginCallbackHandler(PluginCallbackWrapper.OnStopped_Callback); // Need callbackwrapper class for every

            return (Wrapper.exCreate(false, this.createdHandler) == 0);
        }

        internal void OnCreated(uint handle, int result, string message)
        {
             this.Handle = handle;

            if (this.Handle == Plugin.InvalidHandle)
            {
                engine.Close();
                engine.Dispose();
                engine = null;
            }

            created(engine);
        }

        internal void OnStarted(uint handle, int result, string message)
        {
            if (this.Handle == Plugin.InvalidHandle)
            {
                if (this.Failed != null)
                {
                    this.Failed(this, new FailedEventArgs(result, string.Format("PlaybackEngine.Created(): result: 0x{0} - {1}", result.ToString("X", NumberFormatInfo.InvariantInfo), message)));
                }
                else
                {
                    Plugin.CheckResult(result, "CaptureEngine.Started()");
                }

                return;
            }

            if (this.Started != null)
            {
                this.Started(this, EventArgs.Empty);
            }
        }

        internal void OnStopped(uint handle, int result, string message)
        {
            if (result != 0)
            {
                if (this.Failed != null)
                {
                    this.Failed(this, new FailedEventArgs(result, string.Format("PlaybackEngine.Created(): result: 0x{0} - {1}", result.ToString("X", NumberFormatInfo.InvariantInfo), message)));
                }
                else
                {
                    Plugin.CheckResult(result, "CaptureEngine.Stopped()");
                }

                return;
            }

            if (this.Stopped != null)
            {
                this.Stopped(this, EventArgs.Empty);
            }
        }

        private static class Wrapper
        {
            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcCaptureCreate")]
            internal static extern int exCreate(bool enableAudio, [MarshalAs(UnmanagedType.FunctionPtr)]PluginCallbackHandler createdCallback);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcCaptureAddClosed")]
            internal static extern int exAddClosed(uint handle, [MarshalAs(UnmanagedType.FunctionPtr)]PluginCallbackHandler closedCallback, ref UInt64 tokenValue);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcCaptureRemoveClosed")]
            internal static extern int exRemoveClosed(uint handle, UInt64 tokenValue);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcCaptureStart")]
            internal static extern int exStart(uint captureHandle, uint connectionHandle, [MarshalAs(UnmanagedType.FunctionPtr)]PluginCallbackHandler startedCallback);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcCaptureStop")]
            internal static extern int exStop(uint captureHandle, [MarshalAs(UnmanagedType.FunctionPtr)]PluginCallbackHandler stoppedCallback);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcCaptureClose")]
            internal static extern int exClose(uint captureHandle);
        };
    }

    internal static class PluginCallbackWrapper
    {
        private static CaptureEngine GetCaptureEngine(IntPtr senderPtr)
        {
            if (senderPtr == IntPtr.Zero)
            {
                Debug.LogError("Plugin_Callback: requires thisObjectPtr.");

                return;
            }

            GCHandle handle = GCHandle.FromIntPtr(senderPtr);

            var thisObject = handle.Target as CaptureEngine;
            if (thisObject == null)
            {
                Debug.LogError("Plugin_Callback: thisObjectPtr is not null, but seems invalid.");

                return;
            }

        }

        // TODO: Move to utils
        public static void ExecuteOnCorrectThread(Func exec)
        {
            #if UNITY_WSA_10_0
                if (!UnityEngine.WSA.Application.RunningOnAppThread())
                {
                    UnityEngine.WSA.Application.InvokeOnAppThread(() =>
                    {
                        exec();
                    }, false);
                }
                else
                {
                    exec();
                }
            #else
                // there is still a chance the callback is on a non AppThread(callbacks genereated from WaitForEndOfFrame are not)
                // this will process the callback on AppThread on a FixedUpdate
                exec();
            #endif
        }

        [AOT.MonoPInvokeCallback(typeof(Wrapper.StateChangedCallback))]
        internal static void OnCreated_Callback(uint handle, IntPtr senderPtr, int result, string message)
        {
            var thisObj = GetCaptureEngine(senderPtr);
            ExecuteOnCorrectThread((thisObj, handle, result, message) => {
                thisObj.OnCreated(handle, result, message);
            });
        }

        [AOT.MonoPInvokeCallback(typeof(Wrapper.StateChangedCallback))]
        internal static void OnStarted_Callback(uint handle, IntPtr senderPtr, int result, string message)
        {
            var thisObj = GetCaptureEngine(senderPtr);
            ExecuteOnCorrectThread((thisObj, handle, result, message) => {
                thisObj.OnStarted(handle, result, message);
            });
        }

        [AOT.MonoPInvokeCallback(typeof(Wrapper.StateChangedCallback))]
        internal static void OnStopped_Callback(uint handle, IntPtr senderPtr, int result, string message)
        {
            var thisObj = GetCaptureEngine(senderPtr);
            ExecuteOnCorrectThread((thisObj, handle, result, message) => {
                thisObj.OnStopped(handle, result, message);
            });
        }
    }
}
