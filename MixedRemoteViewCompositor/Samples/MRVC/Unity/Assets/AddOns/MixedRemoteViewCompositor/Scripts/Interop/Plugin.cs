// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections;
using System.Collections.Generic;
using System.Globalization;
using System.Runtime.InteropServices;
using UnityEngine;

namespace MixedRemoteViewCompositor
{

    public enum NetworkMode { Listener, Connector }

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate void PluginCallbackHandler(uint handle, IntPtr senderPtr, int result, [MarshalAsAttribute(UnmanagedType.LPWStr)]string message);

    public class FailedEventArgs : EventArgs
    {
        public int Result { get; private set; }
        public string Message { get; private set; }

        public FailedEventArgs(int result, string message)
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

        private void Awake()
        {
            Wrapper.exSetStreamingAssetsPath(Application.streamingAssetsPath);
        }
        
        private void OnEnable()
        {
            this.StartCoroutine("ProcessPluginEvents");
        }

        private void OnDisable()
        {
            GL.IssuePluginEvent(Wrapper.exGetPluginEventFunction(), (int)PluginEvent.Flush);
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

        internal static void CheckResult(int result, string fnName)
        {
            if (result < 0)
            {
                Debug.LogError(fnName + ": 0x" + result.ToString("X", NumberFormatInfo.InvariantInfo));
            }
        }

        private static class Wrapper
        {
            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcSetStreamingAssetsPath")]
            public static extern void exSetStreamingAssetsPath([MarshalAsAttribute(UnmanagedType.LPWStr)]string path);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcSetTime")]
            public static extern void exSetTime(float t);

            [DllImport("MixedRemoteViewCompositor", CallingConvention = CallingConvention.StdCall, EntryPoint = "MrvcGetPluginEventFunc")]
            public static extern IntPtr exGetPluginEventFunction();
        }
    }
}
