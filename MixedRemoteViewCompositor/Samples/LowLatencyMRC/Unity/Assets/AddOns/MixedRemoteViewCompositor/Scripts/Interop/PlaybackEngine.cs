// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Runtime.InteropServices;
using System.Globalization;
using System.Diagnostics;
using System.Collections.Generic;

namespace MixedRemoteViewCompositor
{
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void FrameSizeChangedHandler(UInt32 width, UInt32 height, IntPtr senderPtr);

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
    public delegate void MediaSampleUpdatedHandler(ref MediaSampleUpdateArgs args, IntPtr senderPtr);

    public class PlaybackEngine : IDisposable
    {
        public Action<object, EventArgs> Started;
        public Action<object, EventArgs> Stopped;
        public Action<object, EventArgs> Closed;
        public Action<object, FailedEventArgs> Failed;
        public Action<object, FrameSizedChangedArgs> FrameSizeChanged;

        private uint Handle { get; set; }

        private GCHandle thisObject = default(GCHandle);

        private PluginCallbackHandler createdHandler;

        private ulong sizeChangedToken = 0;
        private event FrameSizeChangedHandler sizeChangedHandler;

        private ulong sampleUpdatedToken = 0;
        private event MediaSampleUpdatedHandler sampleUpdatedHandler;

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

            this.Started?.Invoke(this, EventArgs.Empty);
        }

        public void StartTest()
        {
            int result = Wrapper.exStart(this.Handle);
            if (result != 0)
            {
                //Debug.Log("Failed second playback start");
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

            this.Stopped?.Invoke(this, EventArgs.Empty);
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

            this.Closed?.Invoke(this, EventArgs.Empty);

            this.Handle = Plugin.InvalidHandle;
        }

        public bool GetFrameData(ref MediaSampleUpdateArgs args)
        {
            var result = Wrapper.exGetFrameData(this.Handle, ref args);
            return result == 0;
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
            this.sizeChangedHandler = null;
            this.sampleUpdatedHandler = null;
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
            if (thisObject.IsAllocated)
            {
                thisObject.Free();
                thisObject = default(GCHandle);
            }

            this.disposedValue = true;
        }

        private Action<PlaybackEngine> createdCallback;
        private PlaybackEngine enginePtr;

        private bool Initialize(Action<PlaybackEngine> created, PlaybackEngine engine, Connection connection)
        {
            thisObject = GCHandle.Alloc(this, GCHandleType.Normal);

            this.createdHandler = new PluginCallbackHandler(PlaybackEngine_PluginCallbackWrapper.OnCreated_Callback);

            // TODO: Clean this up, especially on failures***
            this.createdCallback = created;
            this.enginePtr = engine;

            IntPtr thisObjectPtr = GCHandle.ToIntPtr(this.thisObject);
            return (Wrapper.exCreate(connection.Handle, this.createdHandler, thisObjectPtr) == 0);
        }

        private void OnCreated(uint handle, int result, string message)
        {
            this.Handle = handle;

            if (this.Handle != Plugin.InvalidHandle)
            {
                IntPtr thisObjectPtr = GCHandle.ToIntPtr(this.thisObject);

                this.sizeChangedHandler = new FrameSizeChangedHandler(PlaybackEngine_PluginCallbackWrapper.OnFrameSizeUpdated_Callback);
                //this.sizeChangedCallbackHandle = GCHandle.Alloc(this.sizeChangedHandler);

                Plugin.CheckResult(
                    Wrapper.exAddSizeChanged(this.Handle, this.sizeChangedHandler, thisObjectPtr, ref this.sizeChangedToken),
                    "PlaybackEngine.AddSizeChanged");

                this.sampleUpdatedHandler = new MediaSampleUpdatedHandler(PlaybackEngine_PluginCallbackWrapper.OnSampleUpdatedHandler_Callback);
                //this.sampleUpdatedCallbackHandle = GCHandle.Alloc(this.sampleUpdatedHandler);

                Plugin.CheckResult(
                    Wrapper.exAddSampleUpdated(this.Handle, this.sampleUpdatedHandler, thisObjectPtr, ref this.sampleUpdatedToken),
                    "PlaybackEngine.AddSampleUpdated");
            }
            else
            {
                // TODO: clean this up*
                this.enginePtr.Close();
                this.enginePtr.Dispose();
                this.enginePtr = null;
            }

            this.createdCallback?.Invoke(this.enginePtr);

            // Release references
            this.createdCallback = null;
            this.enginePtr = null;
        }


        private void OnFrameSizeUpdated(uint width, uint height)
        {
            this.FrameSizeChanged?.Invoke(this, new FrameSizedChangedArgs(width, height));
        }

        private void OnSampleUpdatedHandler(ref MediaSampleUpdateArgs args)
        {
        }

        private static class Wrapper
        {
            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcPlaybackCreate")]
            internal static extern int exCreate(uint connectionHandle, [MarshalAs(UnmanagedType.FunctionPtr)]PluginCallbackHandler createdCallback, IntPtr objectPtr);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcPlaybackAddSizeChanged")]
            internal static extern int exAddSizeChanged(uint playerHandle, [MarshalAs(UnmanagedType.FunctionPtr)]FrameSizeChangedHandler sizeChangedCallback, IntPtr objectPtr, ref UInt64 tokenValue);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcPlaybackRemoveSizeChanged")]
            internal static extern int exRemoveSizeChanged(uint playerHandle, UInt64 tokenValue);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcPlaybackAddSampleUpdated")]
            internal static extern int exAddSampleUpdated(uint playerHandle, [MarshalAs(UnmanagedType.FunctionPtr)]MediaSampleUpdatedHandler sampleUpdatedCallback, IntPtr objectPtr, ref UInt64 tokenValue);

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

        internal static class PlaybackEngine_PluginCallbackWrapper
        {
            [AOT.MonoPInvokeCallback(typeof(PluginCallbackHandler))]
            internal static void OnCreated_Callback(uint handle, IntPtr senderPtr, int result, string message)
            {
                var thisObj = Plugin.GetSenderObject<PlaybackEngine>(senderPtr);

                Plugin.ExecuteOnUnityThread(() => {//(thisObj, handle, result, message) => {
                    thisObj.OnCreated(handle, result, message);
                });
            }

            [AOT.MonoPInvokeCallback(typeof(FrameSizeChangedHandler))]
            internal static void OnFrameSizeUpdated_Callback(uint width, uint height, IntPtr senderPtr)
            {
                var thisObj = Plugin.GetSenderObject<PlaybackEngine>(senderPtr);

                Plugin.ExecuteOnUnityThread(() => {
                    thisObj.OnFrameSizeUpdated(width, height);
                });
            }

            [AOT.MonoPInvokeCallback(typeof(MediaSampleUpdatedHandler))]
            internal static void OnSampleUpdatedHandler_Callback(ref MediaSampleUpdateArgs args, IntPtr senderPtr)
            {
                var thisObj = Plugin.GetSenderObject<PlaybackEngine>(senderPtr);

                Plugin.ExecuteOnUnityThread(() => {
                    // TODO: implement -> issue with how to use ref variables to lambda
                    //thisObj.OnSampleUpdatedHandler(args);
                });
            }
        }
    }
}
