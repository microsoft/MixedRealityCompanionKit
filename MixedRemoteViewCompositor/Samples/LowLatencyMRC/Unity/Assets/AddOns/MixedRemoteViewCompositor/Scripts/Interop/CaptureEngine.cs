// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Globalization;
using System.Runtime.InteropServices;
using UnityEngine;

namespace MixedRemoteViewCompositor
{
    [StructLayout(LayoutKind.Sequential)]
    public struct CameraMatrices
    {
        public SpatialTranformHelper.Matrix4x4 viewTransform;
        public SpatialTranformHelper.Matrix4x4 projection;
        public SpatialTranformHelper.Matrix4x4 worldToCamera;
    };

    public class CaptureEngine : IDisposable
    {
        public Action<object, EventArgs> Started;
        public Action<object, EventArgs> Stopped;
        public Action<object, EventArgs> Closed;
        public Action<object, FailedEventArgs> Failed;

        private uint Handle { get; set; }

        private PluginCallbackHandler createdHandler;
        private GCHandle createdCallbackHandle;

        private PluginCallbackHandler startedHandler;
        private GCHandle startedCallbackHandle;

        private PluginCallbackHandler stoppedHandler;
        private GCHandle stoppedCallbackHandle;

        private bool running = false;

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

        public void Start(Connection connection, bool enableMRC, IntPtr spatialCoordinateSystemPtr)
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

            result = Wrapper.exStart(this.Handle, connection.Handle, enableMRC, spatialCoordinateSystemPtr, this.startedHandler);
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

        public bool IsRunning { get { return running; }}

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

        public bool TryGetCameraMatrices(ref CameraMatrices cameraMatrices)
        {
            if (this.Handle == Plugin.InvalidHandle)
            {
                return false;
            }

            int result = Wrapper.exGetCameraMatrices(this.Handle, 0, ref cameraMatrices);    
            if (result != 0)
            {
                Plugin.CheckResult(result, "CaptureEngine.TryGetCameraMatrices()");
                return false;
            }

            return true;
        }

        public void SetSpatialCoordinateSystem(IntPtr spatialCoordinateSystemPtr)
        {
            if (this.Handle == Plugin.InvalidHandle)
            {
                return;
            }

            Wrapper.exSetSpatialCoordinateSystemPtr(this.Handle, spatialCoordinateSystemPtr);
        }


        private CaptureEngine()
        {
            this.Handle = Plugin.InvalidHandle;

            this.Started = null;
            this.Stopped = null;
            this.Closed = null;
            this.Failed = null;

            this.createdHandler = null;
            this.createdCallbackHandle = default(GCHandle);

            this.startedHandler = null;
            this.startedCallbackHandle = default(GCHandle);

            this.stoppedHandler = null;
            this.stoppedCallbackHandle = default(GCHandle);
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
            if (this.createdCallbackHandle.IsAllocated)
            {
                this.createdCallbackHandle.Free();
            }

            if (this.startedCallbackHandle.IsAllocated)
            {
                this.startedCallbackHandle.Free();
            }

            if (this.stoppedCallbackHandle.IsAllocated)
            {
                this.stoppedCallbackHandle.Free();
            }

            this.disposedValue = true;
        }

        private bool Initialize(Action<CaptureEngine> created, CaptureEngine engine)
        {
            this.createdHandler = (handle, result, message) => 
            {
                this.Handle = handle;

                if (this.Handle == Plugin.InvalidHandle)
                {
                    engine.Close();
                    engine.Dispose();
                    engine = null;
                }

                created(engine);
            };
            this.createdCallbackHandle = GCHandle.Alloc(this.createdHandler);

            this.startedHandler = this.OnStarted;
            this.startedCallbackHandle = GCHandle.Alloc(this.startedHandler);

            this.stoppedHandler = this.OnStopped;
            this.stoppedCallbackHandle = GCHandle.Alloc(this.stoppedHandler);

            return (Wrapper.exCreate(false, this.createdHandler) == 0);
        }


        private void OnStarted(uint handle, int result, string message)
        {
            if (this.Handle == Plugin.InvalidHandle)
            {
                running = false;
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

            running = true;
            if (this.Started != null)
            {
                this.Started(this, EventArgs.Empty);
            }
        }

        private void OnStopped(uint handle, int result, string message)
        {
            running = false;
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
            internal static extern int exStart(uint captureHandle, uint connectionHandle, bool enableMrc, IntPtr spatialPtr, [MarshalAs(UnmanagedType.FunctionPtr)]PluginCallbackHandler startedCallback);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcCaptureStop")]
            internal static extern int exStop(uint captureHandle, [MarshalAs(UnmanagedType.FunctionPtr)]PluginCallbackHandler stoppedCallback);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcCaptureSetSpatial")]
            internal static extern int exSetSpatialCoordinateSystemPtr(uint captureHandle, IntPtr spatialCoordinateSystemPtr);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcCaptureClose")]
            internal static extern int exClose(uint captureHandle);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcCaptureGetCameraMatrices")]
            internal static extern int exGetCameraMatrices(uint captureHandle, Int64 idx, ref CameraMatrices mats);
        };
    }
}
