// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections.Generic;
using System.Globalization;
using System.Runtime.InteropServices;
using UnityEngine;

namespace RealtimeStreaming
{
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void PluginCallbackHandler(uint handle, IntPtr senderPtr, long result);

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

    public class PluginUtils : MonoBehaviour
    {
        public const uint InvalidHandle = 0x00000bad;

        public const uint S_OK = 0;

        public const string MediaUrlFormat = "mrvc://{0}:{1}";

        public const string DataUrlFormat = "data://{0}:{1}";

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
                return null;
            }

            GCHandle handle = GCHandle.FromIntPtr(senderPtr);

            var thisObject = handle.Target as T;
            if (thisObject == null)
            {
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
    }
}
