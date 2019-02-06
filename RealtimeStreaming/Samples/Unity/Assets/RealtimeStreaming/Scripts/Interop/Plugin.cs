// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections;
using System.Collections.Generic;
using System.Globalization;
using System.Runtime.InteropServices;
using UnityEngine;

namespace RealtimeStreaming
{

    public enum NetworkMode { Listener, Connector }

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void PluginCallbackHandler(uint handle, IntPtr senderPtr, long result, [MarshalAsAttribute(UnmanagedType.LPWStr)]string message);

    public class FailedEventArgs : EventArgs
    {
        public long Result { get; private set; }
        public string Message { get; private set; }

        public FailedEventArgs(long result, string message)
        {
            this.Result = result;
            this.Message = message;
        }
    }

    public class Plugin : MonoBehaviour
    {
        public const uint InvalidHandle = 0x00000bad;

        public const string MediaUrlFormat = "mrvc://{0}:{1}";

        public const string DataUrlFormat = "data://{0}:{1}";

        public static bool IsHoloLens
        {
            get { return SystemInfo.deviceModel.ToUpperInvariant().Contains("HOLOLENS"); }
        }

        private enum PluginEvent { Update = 1, Render, Flush }

        private static readonly object eventLock = new object();
        private static readonly List<Action> actionQueue = new List<Action>();
        private readonly List<Action> actionsToProcess = new List<Action>();

        /// <summary>
        /// Helper method to ensure action is called on UI thread
        /// </summary>
        /// <param name="action">code to execute on UI thread</param>
        public void QueueAction(Action action)
        {
            lock (eventLock)
            {
                actionQueue.Add(action);
            }
        }
        
        private void OnEnable()
        {
            this.StartCoroutine("ProcessPluginEvents");
        }

        private void OnDisable()
        {
            GL.IssuePluginEvent(Wrapper.exGetPluginEventFunction(), (int)PluginEvent.Flush);
        }

        // TODO: Deprecate this in favor of execute on other thread
        private void Update()
        {
            this.GetActionsToProcess();

            for (int i = 0; i < this.actionsToProcess.Count; ++i)
            {
                var action = this.actionsToProcess[i];
                try
                {
                    action();
                }
                catch (Exception ex)
                {
                    Debug.LogWarning("Action not able to execute: " + ex);
                }
            }

            this.actionsToProcess.Clear();
        }

        private IEnumerator ProcessPluginEvents()
        {
            while (this.enabled)
            {
                // wait until update stage
                yield return new WaitForFixedUpdate();

                // Set time for the plugin
                Wrapper.exSetTime(Time.timeSinceLevelLoad);

                // issue end of frame update
                GL.IssuePluginEvent(Wrapper.exGetPluginEventFunction(), (int)PluginEvent.Update);
            }
        }

        private void GetActionsToProcess()
        {
            lock (eventLock)
            {
                for (int i = 0; i < actionQueue.Count; ++i)
                {
                    this.actionsToProcess.Add(actionQueue[i]);
                }

                actionQueue.Clear();
            }
        }

        internal static void CheckHResult(long result, string fnName)
        {
            if (result < 0)
            {
                Debug.LogError(fnName + ": 0x" + result.ToString("X", NumberFormatInfo.InvariantInfo));
            }
        }


        public static T GetSenderObject<T>(IntPtr senderPtr) where T : class
        {
            if (senderPtr == IntPtr.Zero)
            {
                // TODO: Fix debugging
                //Debug.LogError("Plugin_Callback: requires thisObjectPtr.");
                return null;
            }

            GCHandle handle = GCHandle.FromIntPtr(senderPtr);

            var thisObject = handle.Target as T;
            if (thisObject == null)
            {
                // TODO: Fix debugging
                //Debug.LogError("Plugin_Callback: thisObjectPtr is not null, but seems invalid.");
                return null;
            }

            return thisObject;
        }

        public static void ExecuteOnUnityThread(Action exec)
        {
#if UNITY_WSA_10_0
            if (!UnityEngine.WSA.Application.RunningOnAppThread())
            {
                UnityEngine.WSA.Application.InvokeOnAppThread(() =>
                {
                    exec.Invoke();
                }, false);
            }
            else
            {
                exec.Invoke();
            }
#else
                // there is still a chance the callback is on a non AppThread(callbacks genereated from WaitForEndOfFrame are not)
                // this will process the callback on AppThread on a FixedUpdate
                exec.Invoke();
#endif
        }

        private static class Wrapper
        {
            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "SetTime")]
            public static extern void exSetTime(float t);

            [DllImport("RealtimeStreaming", CallingConvention = CallingConvention.StdCall, EntryPoint = "GetPluginEventFunc")]
            public static extern IntPtr exGetPluginEventFunction();
        }
    }
}
