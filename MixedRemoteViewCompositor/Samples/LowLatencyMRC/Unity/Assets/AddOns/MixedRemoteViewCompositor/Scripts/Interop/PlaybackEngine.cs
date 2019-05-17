// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Runtime.InteropServices;
using System.Globalization;

namespace MixedRemoteViewCompositor
{
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void FrameSizeChanged(UInt32 width, UInt32 height);

    public class FrameSizedChangedArgs : EventArgs
    {
        public UInt32 Width { get; private set; }

        public UInt32 Height { get; private set; }

        public FrameSizedChangedArgs(UInt32 width, UInt32 height)
        {
            this.Width = width;
            this.Height = height;
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct MediaSampleUpdateArgs
    {
        public IntPtr videoTexture;
        public uint width;
        public uint height;
        public ulong timestamp;
        public SpatialTranformHelper.Matrix4x4 cameraViewTransform;
        public SpatialTranformHelper.Matrix4x4 cameraProjection;
        public SpatialTranformHelper.Matrix4x4 cameraCoordinate;
        public SpatialTranformHelper.Matrix4x4 cameraAffine;
    };

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void MediaSampleUpdated(ref MediaSampleUpdateArgs args);

    public class PlaybackEngine : IDisposable
    {
        public Action<object, EventArgs> Started;
        public Action<object, EventArgs> Stopped;
        public Action<object, EventArgs> Closed;
        public Action<object, FailedEventArgs> Failed;
        public Action<object, FrameSizedChangedArgs> FrameSizeChanged;


        private uint Handle { get; set; }

        private PluginCallbackHandler createdHandler;
        private GCHandle createdCallbackHandle;

        private ulong sizeChangedToken = 0;
        private event FrameSizeChanged sizeChangedHandler;
        private GCHandle sizeChangedCallbackHandle;

        private ulong sampleUpdatedToken = 0;
        private event MediaSampleUpdated sampleUpdatedHandler;
        private GCHandle sampleUpdatedCallbackHandle;

        // IDisposable
        private bool disposedValue = false;

        public static bool Create(Connection connection, Action<PlaybackEngine> created)
        {
            if (created == null)
            {
                return false;
            }

            PlaybackEngine engine = new PlaybackEngine();
            if(engine == null)
            {
                return false;
            }

            if(!engine.Initialize(created, engine, connection))
            {
                return false;
            }

            return true;
        }

        public void Start()
        {
            int result = Wrapper.exStart(this.Handle);
            if (result != 0)
            {
                if (this.Failed != null)
                {
                    this.Failed(this, new FailedEventArgs(result, string.Format("PlaybackEngine.Start(): result: 0x{0}", result.ToString("X", NumberFormatInfo.InvariantInfo))));
                }
                else
                {
                    Plugin.CheckResult(result, "PlaybackEngine.Start()");
                }

                return;
            }

            if (this.Started != null)
            {
                this.Started(this, EventArgs.Empty);
            }
        }

        public void Stop()
        {
            var result = Wrapper.exStop(this.Handle);
            if (result != 0)
            {
                if (this.Failed != null)
                {
                    this.Failed(this, new FailedEventArgs(result, string.Format("PlaybackEngine.Stop(): result: 0x{0}", result.ToString("X", NumberFormatInfo.InvariantInfo))));
                }
                else
                {
                    Plugin.CheckResult(result, "PlaybackEngine.Stop()");
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

            // remove any tokens
            int result = Wrapper.exRemoveSizeChanged(this.Handle, sizeChangedToken);
            Plugin.CheckResult(result, "PlaybackEngine.Close() - RemoveSizeChanged");

            result = Wrapper.exRemoveSampleUpdated(this.Handle, sampleUpdatedToken);
            Plugin.CheckResult(result, "PlaybackEngine.Close() - RemoveSampleUpdated");

            result = Wrapper.exClose(this.Handle);
            Plugin.CheckResult(result, "PlaybackEngine.Close() - Close playback");

            if (this.Closed != null)
            {
                this.Closed(this, EventArgs.Empty);
            }

            this.Handle = Plugin.InvalidHandle;
        }

        public bool GetFrameData(ref MediaSampleUpdateArgs args)
        {
            return (Wrapper.exGetFrameData(this.Handle, ref args) == 0);
        }


        private PlaybackEngine()
        {
            this.Handle = Plugin.InvalidHandle;

            this.Started = null;
            this.Stopped = null;
            this.Closed = null;
            this.Failed = null;
            this.FrameSizeChanged = null;

            this.createdHandler = null;
            this.createdCallbackHandle = default(GCHandle);

            this.sizeChangedHandler = null;
            this.sizeChangedCallbackHandle = default(GCHandle);

            this.sampleUpdatedHandler = null;
            this.sampleUpdatedCallbackHandle = default(GCHandle);
        }

        ~PlaybackEngine()
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

            if (this.sizeChangedCallbackHandle.IsAllocated)
            {
                this.sizeChangedCallbackHandle.Free();
            }

            if (this.sizeChangedCallbackHandle.IsAllocated)
            {
                this.sampleUpdatedCallbackHandle.Free();
            }

            this.disposedValue = true;
        }

        private bool Initialize(Action<PlaybackEngine> created, PlaybackEngine engine, Connection conneciton)
        {
            this.createdHandler = (handle, result, message) =>
            {
                this.Handle = handle;

                if (this.Handle != Plugin.InvalidHandle)
                {
                    this.sizeChangedHandler = OnFrameSizeUpdated;
                    this.sizeChangedCallbackHandle = GCHandle.Alloc(this.sizeChangedHandler);
                    Plugin.CheckResult(
                        Wrapper.exAddSizeChanged(this.Handle, this.sizeChangedHandler, ref this.sizeChangedToken),
                        "PlaybackEngine.AddSizeChanged");

                    this.sampleUpdatedHandler = OnSampleUpdatedHandler;
                    this.sampleUpdatedCallbackHandle = GCHandle.Alloc(this.sampleUpdatedHandler);
                    Plugin.CheckResult(
                        Wrapper.exAddSampleUpdated(this.Handle, this.sampleUpdatedHandler, ref this.sampleUpdatedToken),
                        "PlaybackEngine.AddSampleUpdated");
                }
                else
                {
                    engine.Close();
                    engine.Dispose();
                    engine = null;
                }

                created(engine);
            };
            this.createdCallbackHandle = GCHandle.Alloc(this.createdHandler);

            return (Wrapper.exCreate(conneciton.Handle, this.createdHandler) == 0);
        }


        private void OnFrameSizeUpdated(uint width, uint height)
        {
            if (this.FrameSizeChanged != null)
            {
                this.FrameSizeChanged(this, new FrameSizedChangedArgs(width, height));
            }
        }

        private void OnSampleUpdatedHandler(ref MediaSampleUpdateArgs args)
        {
        }

        private static class Wrapper
        {
            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcPlaybackCreate")]
            internal static extern int exCreate(uint connectionHandle, [MarshalAs(UnmanagedType.FunctionPtr)]PluginCallbackHandler createdCallback);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcPlaybackAddSizeChanged")]
            internal static extern int exAddSizeChanged(uint playerHandle, [MarshalAs(UnmanagedType.FunctionPtr)]FrameSizeChanged sizeChangedCallback, ref UInt64 tokenValue);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcPlaybackRemoveSizeChanged")]
            internal static extern int exRemoveSizeChanged(uint playerHandle, UInt64 tokenValue);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcPlaybackAddSampleUpdated")]
            internal static extern int exAddSampleUpdated(uint playerHandle, [MarshalAs(UnmanagedType.FunctionPtr)]MediaSampleUpdated sampleUpdatedCallback, ref UInt64 tokenValue);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcPlaybackRemoveSampleUpdated")]
            internal static extern int exRemoveSampleUpdated(uint playerHandle, UInt64 tokenValue);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcPlaybackStart")]
            internal static extern int exStart(uint playerHandle);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcPlaybackStop")]
            internal static extern int exStop(uint playerHandle);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcPlaybackClose")]
            internal static extern int exClose(uint playerHandle);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcPlaybackGetFrameData")]
            internal static extern int exGetFrameData(uint playerHandle, ref MediaSampleUpdateArgs args);
        }
    }
}
