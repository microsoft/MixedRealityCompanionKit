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

        /*
        private GCHandle thisObject = default(GCHandle);
        private PluginCallbackHandler createdHandler;
        private PluginCallbackHandler startedHandler;
        private PluginCallbackHandler stoppedHandler;
        */

        // IDisposable
        private bool disposedValue = false;

        public CaptureEngine()
        {
            this.Handle = Plugin.InvalidHandle;

            this.Started = null;
            this.Stopped = null;
            this.Closed = null;
            this.Failed = null;

            /*
            this.createdHandler = null;
            this.startedHandler = null;
            this.stoppedHandler = null;
            */

            this.Initialize();
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

            //IntPtr thisObjectPtr = GCHandle.ToIntPtr(this.thisObject);
            result = Wrapper.exInit(false, this.Handle, connection.Handle);
            uint resultu = (uint)result;
            //result = Wrapper.exStart(this.Handle, connection.Handle, enableMRC, spatialCoordinateSystemPtr, this.startedHandler, thisObjectPtr);
            if (result != 0)
            {
                string test = result.ToString("X", NumberFormatInfo.InvariantInfo);
                if (this.Failed != null)
                {
                    this.Failed(this, new FailedEventArgs(result, string.Format("CaptureEngine.Stop(): result: 0x{0}", result.ToString("X", NumberFormatInfo.InvariantInfo))));
                }
                else
                {
                    Plugin.CheckResult(result, "CaptureEngine.Start()");
                }
            }

            this.Started?.Invoke(this, EventArgs.Empty);
        }

        public void WriteFrame()
        {
            int result = Wrapper.exWrite(this.Handle);
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
        }

        public void Stop()
        {
            //IntPtr thisObjectPtr = GCHandle.ToIntPtr(this.tisObject);
            //int result = Wrapper.exStop(this.Handle, this.stoppedHandler, thisObjectPtr);
            int result = Wrapper.exStop(this.Handle);
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

            this.Stopped?.Invoke(this, EventArgs.Empty);
        }

        public void Close()
        {
            if (this.Handle == Plugin.InvalidHandle)
            {
                return;
            }

            /*
             * TODO: Figure out how to porperly close memory here
            int result = Wrapper.exClose(this.Handle);
            if (result != 0)
            {
                Plugin.CheckResult(result, "CaptureEngine.Close()");
            }*/

            this.Closed?.Invoke(this, EventArgs.Empty);

            this.Handle = Plugin.InvalidHandle;
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

            // free unmanaged resources (unmanaged objects) and override a finalizer below.
            /*
            if (thisObject.IsAllocated)
            {
                thisObject.Free();
                thisObject = default(GCHandle);
            }*/

            this.disposedValue = true;
        }

        private bool Initialize()
        {
            try
            {
                uint handle = Plugin.InvalidHandle;
                var result = Wrapper.exCreate(ref handle);

                if (handle == Plugin.InvalidHandle)
                    return false;

                this.Handle = handle;
            }
            catch(Exception ex)
            {
                return false;
            }

            return true;            
        }

        /*
        internal void OnCreated(uint handle, int result, string message)
        {
            this.Handle = handle;

            if (this.Handle == Plugin.InvalidHandle)
            {
                // TODO: Look at fixing this code***
                this.enginePtr.Close();
                this.enginePtr.Dispose();
                this.enginePtr = null;
            }

            this.createdCallback?.Invoke(this.enginePtr);

            // Release references
            this.createdCallback = null;
            this.enginePtr = null;
        }

        private void OnStarted(uint handle, int result, string message)
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

            this.Started?.Invoke(this, EventArgs.Empty);            
        }

        private void OnStopped(uint handle, int result, string message)
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

            this.Stopped?.Invoke(this, EventArgs.Empty);
        }
        */

        private static class Wrapper
        {
            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "CreateStreamCaptureEngine")]
            //internal static extern int exCreate(bool enableAudio, [MarshalAs(UnmanagedType.FunctionPtr)]PluginCallbackHandler createdCallback, IntPtr senderObject);
            internal static extern int exCreate(ref uint captureHandle);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcCaptureInit")]
            internal static extern int exInit(bool enableAudio, uint captureHandle, uint connectionHandle);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcCaptureStop")]
            internal static extern int exStop(uint captureHandle);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcCaptureWrite")]
            internal static extern int exWrite(uint captureHandle);

            /*
            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcCaptureCreate")]
            //internal static extern int exCreate(bool enableAudio, [MarshalAs(UnmanagedType.FunctionPtr)]PluginCallbackHandler createdCallback, IntPtr senderObject);
            internal static extern int exCreate(bool enableAudio);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcCaptureStart")]
            internal static extern int exStart(bool enableAudio, uint captureHandle, uint connectionHandle, [MarshalAs(UnmanagedType.FunctionPtr)]PluginCallbackHandler startedCallback, IntPtr senderObject);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcCaptureStop")]
            internal static extern int exStop(uint captureHandle, [MarshalAs(UnmanagedType.FunctionPtr)]PluginCallbackHandler stoppedCallback, IntPtr senderObject);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcCaptureClose")]
            internal static extern int exClose(uint captureHandle);
            */
        };

        /*
        internal static class CaptureEngine_PluginCallbackWrapper
        {
            [AOT.MonoPInvokeCallback(typeof(PluginCallbackHandler))]
            internal static void OnCreated_Callback(uint handle, IntPtr senderPtr, int result, string message)
            {
                var thisObj = Plugin.GetSenderObject<CaptureEngine>(senderPtr);

                Plugin.ExecuteOnUnityThread(() => {//(thisObj, handle, result, message) => {
                    thisObj.OnCreated(handle, result, message);
                });
            }

            [AOT.MonoPInvokeCallback(typeof(PluginCallbackHandler))]
            internal static void OnStarted_Callback(uint handle, IntPtr senderPtr, int result, string message)
            {
                var thisObj = Plugin.GetSenderObject<CaptureEngine>(senderPtr);

                Plugin.ExecuteOnUnityThread(() => {//(thisObj, handle, result, message) => {
                    thisObj.OnStarted(handle, result, message);
                });
            }

            [AOT.MonoPInvokeCallback(typeof(PluginCallbackHandler))]
            internal static void OnStopped_Callback(uint handle, IntPtr senderPtr, int result, string message)
            {
                var thisObj = Plugin.GetSenderObject<CaptureEngine>(senderPtr);

                Plugin.ExecuteOnUnityThread(() => {//(thisObj, handle, result, message) => {
                    thisObj.OnStopped(handle, result, message);
                });
            }
        }
        */
    }
}
