// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using HoloToolkit.Unity;
using HoloToolkit.Unity.SpatialMapping;
using System;
using System.IO;
using System.Net;
using System.Runtime.InteropServices;
#if NETFX_CORE
using Windows.Networking;
using Windows.Networking.Connectivity;
#endif

namespace SpectatorView
{
    public class HolographicCameraManager : SpectatorView.SV_Singleton<HolographicCameraManager>
    {
        #region DLLImports
        [DllImport("UnityCompositorInterface")]
        private static extern int GetFrameWidth();

        [DllImport("UnityCompositorInterface")]
        private static extern int GetFrameHeight();

        [DllImport("UnityCompositorInterface")]
        private static extern int GetFrameWidthHiRes();

        [DllImport("UnityCompositorInterface")]
        private static extern int GetFrameHeightHiRes();

        [DllImport("UnityCompositorInterface")]
        private static extern IntPtr GetRenderEventFunc();

        [DllImport("UnityCompositorInterface")]
        private static extern void SetAudioData(byte[] audioData);

        [DllImport("UnityCompositorInterface")]
        private static extern void Reset();

        [DllImport("UnityCompositorInterface")]
        private static extern Int64 GetCurrentUnityTime();

        [DllImport("UnityCompositorInterface")]
        private static extern bool InitializeFrameProvider();

        [DllImport("UnityCompositorInterface")]
        private static extern void StopRecording();

        [DllImport("UnityCompositorInterface")]
        private static extern void StopFrameProvider();

        [DllImport("UnityCompositorInterface")]
        private static extern void UpdateCompositor();

        [DllImport("UnityCompositorInterface")]
        private static extern void SetExplicitHoloTime(long holoTime);

        [DllImport("UnityCompositorInterface")]
        private static extern int GetFrameDelta();

        [DllImport("UnityCompositorInterface")]
        private static extern bool NewColorFrame();

        [DllImport("UnityCompositorInterface")]
        private static extern bool IsRecording();
        #endregion

        public bool IsCurrentlyActive { get; set; }

        [Header("Device IPs")]
        [Tooltip("The IP of the HoloLens device mounted to the camera.")]
        public string HolographicCameraIP;

        // Use IP to disambiguate scenarios where multiple Unity clients could be connected to the same sharing service.
        // This could happen if you have multiple Spectator View rigs in the same experience.
        [Tooltip("Set this to the IP of the machine running Unity you wish to connect this spectator view rig to.  You can leave this field blank if you are only using 1 spectator view rig.")]
        public string LocalComputerIP = string.Empty;

        [Header("Hologram Settings")]
        public Depth TextureDepth = Depth.TwentyFour;
        public AntiAliasingSamples AntiAliasing = AntiAliasingSamples.Eight;
        public FilterMode Filter = FilterMode.Trilinear;

        [Tooltip("Default alpha for the holograms in the composite video.")]
        public float DefaultAlpha = 0.9f;

        [Tooltip("Default frame offset for adjustment of camera shutter time to capture card delivery time.")]
        public float DefaultFrameOffset = 0;

        public List<string> localIPs { get; private set; }

        public enum Depth { None, Sixteen = 16, TwentyFour = 24 }
        public enum AntiAliasingSamples { One = 1, Two = 2, Four = 4, Eight = 8 };

        [HideInInspector]
        public bool frameProviderInitialized = false;

        [HideInInspector]
        public HoloToolkit.Sharing.User tppcUser = null;
        [HideInInspector]
        public HoloToolkit.Sharing.User editorUser = null;

        [HideInInspector]
        public bool requestSpatialMappingData = false;

        long prevHoloTime = 0;

        // Get the bytes of the ip so we can send it over the network.
        public byte[] localIPBytes
        {
            get
            {
                string localIP = string.Empty;
                if (localIPs.Count > 0)
                {
                    localIP = localIPs[0];
                }

                return System.Text.Encoding.ASCII.GetBytes(localIP);
            }
        }

        void Start()
        {
            IsCurrentlyActive = false;

#if !UNITY_EDITOR
            Camera[] cameras = gameObject.GetComponentsInChildren<Camera>();
            for (int i = 0; i < cameras.Length; i++)
            {
                cameras[i].enabled = false;
            }
#endif
            Initialize();
        }

        private void Initialize()
        {
            localIPs = new List<string>();
#if UNITY_EDITOR
            gameObject.AddComponent<Calibration>();
            gameObject.AddComponent<ShaderManager>();

            // Get the ip of the Hololens to see if this device is the Holographic Camera device.
            IPAddress[] ips = Dns.GetHostAddresses(Dns.GetHostName());
            foreach (IPAddress ip in ips)
            {
                if (ip.AddressFamily == System.Net.Sockets.AddressFamily.InterNetwork)
                {
                    localIPs.Add(ip.ToString().Trim());
                }
            }

            // Change audio listener to the holographic camera.
            AudioListener listener = Camera.main.GetComponent<AudioListener>();
            if (listener != null)
            {
                GameObject.DestroyImmediate(listener);
            }

            listener = GetComponent<AudioListener>();
            if (listener == null)
            {
                gameObject.AddComponent<AudioListener>();
            }

            if (SpatialMappingManager.Instance != null)
            {
                SpatialMappingManager.Instance.gameObject.AddComponent<SpectatorView.RemoteSpatialMappingSource>();
            }
#endif

#if NETFX_CORE
            IReadOnlyList<HostName> hosts = NetworkInformation.GetHostNames();
            foreach (HostName aName in hosts)
            {
                if (aName.Type == HostNameType.Ipv4)
                {
                    localIPs.Add(aName.ToString().Trim());
                }
            }
#endif

            foreach (string ip in localIPs)
            {
                Debug.Log("Local IP: " + ip);
            }
        }

#if UNITY_EDITOR
        public void EnableHolographicCamera(Transform parent)
        {
            gameObject.transform.parent = parent;
            gameObject.transform.localPosition = Calibration.Instance.Translation;
            gameObject.transform.localRotation = Calibration.Instance.Rotation;
            gameObject.transform.localScale = Vector3.one;

            ShaderManager.Instance.EnableHolographicCamera(parent);
            IsCurrentlyActive = true;
        }

        /// <summary>
        /// Restore the Holographic Camera to the root of the Unity Hierarchy.
        /// </summary>
        public void ResetHolographicCamera()
        {
            // Cache the last known position and rotation of the spectator view rig so we do not lose state while waiting for the new camera to come online.
            GameObject cachedTPPC = GameObject.Find("CachedTPPC");
            if (cachedTPPC == null)
            {
                cachedTPPC = GameObject.CreatePrimitive(PrimitiveType.Cube);
                cachedTPPC.name = "CachedTPPC";
                cachedTPPC.GetComponent<Renderer>().enabled = false;
            }

            cachedTPPC.transform.position = gameObject.transform.position;
            cachedTPPC.transform.rotation = gameObject.transform.rotation;

            tppcUser = null;
            IsCurrentlyActive = false;
            gameObject.transform.parent = cachedTPPC.transform;
        }

        void OnEnable()
        {
            frameProviderInitialized = false;
            StartCoroutine("CallPluginAtEndOfFrames");
        }

        protected override void OnDestroy()
        {
            ResetCompositor();

            base.OnDestroy();
        }

        public void ResetCompositor()
        {
            Debug.Log("Disposing DLL Resources.");
            Reset();

            StopFrameProvider();
            if (IsRecording())
            {
                StopRecording();
            }

            if (ShaderManager.Instance != null)
            {
                ShaderManager.Instance.Reset();
            }
        }

        private IEnumerator CallPluginAtEndOfFrames()
        {
            while (true)
            {
                // Wait until all frame rendering is done
                yield return new WaitForEndOfFrame();

                // Issue a plugin event with arbitrary integer identifier.
                // The plugin can distinguish between different
                // things it needs to do based on this ID.
                // For our simple plugin, it does not matter which ID we pass here.
                GL.IssuePluginEvent(GetRenderEventFunc(), 1);
            }
        }

        void Update()
        {
            if (!frameProviderInitialized)
            {
                frameProviderInitialized = InitializeFrameProvider();
            }
            else if (frameProviderInitialized)
            {
                UpdateCompositor();
            }
        }

        void OnPreRender()
        {
            prevHoloTime = GetCurrentUnityTime();

            // Set delta time for TPPC HoloLens to report poses relative to color frame time.
            if (NewColorFrame())
            {
                SetExplicitHoloTime(prevHoloTime);

                int deltaTime = GetFrameDelta();
                if (deltaTime > 0) { deltaTime *= -1; }

                // Convert to nanoseconds.
                deltaTime *= 100;

                SpectatorView.SV_CustomMessages.Instance.SendTimeOffset(deltaTime);
            }
        }

        // Send audio data to Compositor.
        void OnAudioFilterRead(float[] data, int channels)
        {
            Byte[] audioBytes = new Byte[data.Length * 2];

            for (int i = 0; i < data.Length; i++)
            {
                // Rescale float to short range for encoding.
                short audioEntry = (short)(data[i] * short.MaxValue);
                BitConverter.GetBytes(audioEntry).CopyTo(audioBytes, i * 2);
            }

            SetAudioData(audioBytes);
        }
#endif

    }
}
