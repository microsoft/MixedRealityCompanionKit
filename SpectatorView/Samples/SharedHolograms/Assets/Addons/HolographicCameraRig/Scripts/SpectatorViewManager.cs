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
    public class SpectatorViewManager : SpectatorView.SV_Singleton<SpectatorViewManager>
    {
        #region DLLImports
#if UNITY_EDITOR
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
        private static extern int GetFrameDelta();

        [DllImport("UnityCompositorInterface")]
        private static extern bool IsRecording();

        [DllImport("UnityCompositorInterface")]
        private static extern bool GetEarliestHologramPose(
            out float rotX, out float rotY, out float rotZ, out float rotW,
            out float posX, out float posY, out float posZ, out float timestamp);

        [DllImport("UnityCompositorInterface")]
        private static extern void UpdateSpectatorView();

        [DllImport("UnityCompositorInterface")]
        private static extern bool NewColorFrame();

        [DllImport("UnityCompositorInterface")]
        private static extern long GetColorDuration();
#endif
        #endregion

        public bool IsCurrentlyActive { get; set; }

        [Header("Device IDs")]
        [Tooltip("The IP or device name of the HoloLens mounted to the camera.")]
        public string SpectatorViewID = string.Empty;

        [Tooltip("Comma separated IPs for other client HoloLens devices")]
        public string ClientHololensCSV = string.Empty;

        [Tooltip("Autoconnect all devices to the ServerIP's session")]
        public bool AutoConnect = true;

        [Tooltip("The IP or computer name of the device that will host the session if autoconnecting.")]
        public string ServerID = string.Empty;


        [Header("Hologram Settings")]
        public Depth TextureDepth = Depth.TwentyFour;
        public AntiAliasingSamples AntiAliasing = AntiAliasingSamples.Eight;
        public FilterMode Filter = FilterMode.Trilinear;

        [Tooltip("Default alpha for the holograms in the composite video.")]
        public float DefaultAlpha = 0.9f;

        [Tooltip("Default frame offset for adjustment of camera shutter time to capture card delivery time.")]
        public float DefaultFrameOffset = 0;

        [Tooltip("Show or hide any game object with the ToggleEnabled behavior.")]
        public bool ToggleGameObjectVisibility = false;
        private bool prevToggleVisibility = false;

        public List<string> localIPs { get; private set; }

        public enum Depth { None, Sixteen = 16, TwentyFour = 24 }
        public enum AntiAliasingSamples { One = 1, Two = 2, Four = 4, Eight = 8 };

        [HideInInspector]
        public bool frameProviderInitialized = false;

        NetworkDiscoveryWithAnchors networkDiscovery = null;
        Dictionary<string, NetworkDiscoveryWithAnchors.SessionInfo> sessionList;

        private string computerName = string.Empty;

        public bool IsSpectatorView()
        {
            return localIPs.Contains(SpectatorViewID) ||
                computerName == SpectatorViewID.Trim().ToLower();
        }

#if UNITY_EDITOR
        void OnValidate()
        {
            if (ToggleGameObjectVisibility != prevToggleVisibility)
            {
                ToggleGOVisibility();
                prevToggleVisibility = ToggleGameObjectVisibility;
            }
        }

        private void ToggleGOVisibility()
        {
            SV_ToggleEnabled[] toggledObjs = Resources.FindObjectsOfTypeAll<SV_ToggleEnabled>();
            foreach (SV_ToggleEnabled te in toggledObjs)
            {
                te.ToggleVisibility();
            }
        }
#endif

        void Start()
        {
            UnityEngine.VR.WSA.HolographicSettings.ActivateLatentFramePresentation(true);

            computerName = GetLocalComputerName().Trim().ToLower();

            networkDiscovery = null;
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

            // Get the ip of the Hololens to see if this device is the spectator view.
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

        private string GetLocalComputerName()
        {
#if WINDOWS_UWP
            foreach (Windows.Networking.HostName hostName in Windows.Networking.Connectivity.NetworkInformation.GetHostNames())
            {
                if (hostName.Type == HostNameType.DomainName)
                {

                    Debug.Log("My name is " + hostName.DisplayName);
                    string name = hostName.DisplayName;
                    return name;
                }
            }
            return "default_name";
#else
            return System.Environment.ExpandEnvironmentVariables("%ComputerName%");
#endif
        }

        void Update()
        {
#if UNITY_EDITOR
            UpdateSpectatorView();

#region Spectator View Transform
            if (IsCurrentlyActive && transform.parent != null)
            {
                if (NewColorFrame())
                {
                    if (PlayerController.Instance != null)
                    {
                        PlayerController.Instance.SetColorDuration(GetColorDuration() * 100);
                    }

                    Quaternion currRot = Quaternion.identity;
                    Vector3 currPos = Vector3.zero;
                    float nextTime = 0;
                    GetEarliestHologramPose(out currRot.x, out currRot.y, out currRot.z, out currRot.w,
                        out currPos.x, out currPos.y, out currPos.z, out nextTime);

                    transform.parent.localPosition = currPos;
                    transform.parent.localRotation = currRot;
                }
            }
#endregion

            if (Input.GetKeyDown(KeyCode.T))
            {
                prevToggleVisibility = !prevToggleVisibility;
                ToggleGameObjectVisibility = !ToggleGameObjectVisibility;

                ToggleGOVisibility();
            }

            if (!frameProviderInitialized)
            {
                frameProviderInitialized = InitializeFrameProvider();
            }
            else if (frameProviderInitialized)
            {
                UpdateCompositor();
            }
#endif

#region network state
            if (AutoConnect && 
                ServerID != string.Empty && 
                networkDiscovery == null &&
                NetworkDiscoveryWithAnchors.Instance != null)
            {
                NetworkDiscoveryWithAnchors nd = NetworkDiscoveryWithAnchors.Instance;

                if (nd.running)
                {
                    networkDiscovery = nd;
                }

                if (networkDiscovery != null)
                {
                    bool shouldHost = false;
#if UNITY_EDITOR
                    if (ServerID == "editor")
                    {
                        shouldHost = true;
                    }
#endif
                    if (localIPs.Contains(ServerID) || 
                        computerName == ServerID.Trim().ToLower() ||
                        shouldHost)
                    {
                        Debug.Log("Start Hosting");
                        // This device is the host.
                        NetworkDiscoveryWithAnchors.Instance.StartHosting("SpectatorViewSession");
                        Debug.Log("Hosting");
                    }
                    else
                    {
                        // First look for session already in session list.
                        if (!FindSession())
                        {
                            // If session does not already exist, it will be added during a change event.
                            networkDiscovery.SessionListChanged += NetworkDiscovery_SessionListChanged;
                        }
                    }
                }
            }
#endregion
        }

        public void ResetAutoconnect()
        {
            networkDiscovery = null;
        }

        private bool FindSession()
        {
            sessionList = networkDiscovery.remoteSessions;
            foreach (KeyValuePair<string, NetworkDiscoveryWithAnchors.SessionInfo> entry in sessionList)
            {
                if (entry.Value.SessionIp == ServerID ||
                    entry.Value.SessionName.Trim().ToLower() == ServerID.Trim().ToLower())
                {
                    networkDiscovery.JoinSession(entry.Value);
                    networkDiscovery.SessionListChanged -= NetworkDiscovery_SessionListChanged;
                    return true;
                }
            }

            return false;
        }

        private void NetworkDiscovery_SessionListChanged(object sender, EventArgs e)
        {
            FindSession();
        }

#if UNITY_EDITOR
        public void EnableHolographicCamera(Transform parent)
        {
            GameObject container = GameObject.Find("SpectatorView");
            if (container == null)
            {
                container = GameObject.CreatePrimitive(PrimitiveType.Cube);
                container.GetComponent<MeshRenderer>().enabled = false;
                GameObject.DestroyImmediate(container.GetComponent<Collider>());
                container.name = "SpectatorView";
            }

            container.transform.SetParent(parent);
            container.transform.localPosition = Vector3.zero;
            container.transform.localRotation = Quaternion.identity;

            gameObject.transform.parent = container.transform;
            gameObject.transform.localPosition = Calibration.Instance.Translation;
            gameObject.transform.localRotation = Calibration.Instance.Rotation;
            gameObject.transform.localScale = Vector3.one;

            ShaderManager.Instance.EnableHolographicCamera(container.transform);
            IsCurrentlyActive = true;
        }

        /// <summary>
        /// Restore the Holographic Camera to the root of the Unity Hierarchy.
        /// </summary>
        public void ResetHolographicCamera()
        {
            // Cache the last known position and rotation of the spectator view rig so we do not lose state while waiting for the new camera to come online.
            GameObject cachedSV = GameObject.Find("CachedSV");
            if (cachedSV == null)
            {
                cachedSV = GameObject.CreatePrimitive(PrimitiveType.Cube);
                cachedSV.name = "CachedSV";
                cachedSV.GetComponent<Renderer>().enabled = false;
                GameObject.DestroyImmediate(cachedSV.GetComponent<Collider>());
            }

            cachedSV.transform.position = gameObject.transform.position;
            cachedSV.transform.rotation = gameObject.transform.rotation;

            IsCurrentlyActive = false;
            gameObject.transform.parent = cachedSV.transform;

            EnableComponents();
        }

        private void EnableComponents()
        {
            gameObject.SetActive(true);
            Camera cam = gameObject.GetComponent<Camera>();
            if (cam != null)
            {
                cam.enabled = true;
            }

            SpectatorViewManager svm = gameObject.GetComponent<SpectatorViewManager>();
            if (svm != null)
            {
                svm.enabled = true;
            }

            Calibration calibration = gameObject.GetComponent<Calibration>();
            if (calibration != null)
            {
                calibration.enabled = true;
            }

            ShaderManager sm = gameObject.GetComponent<ShaderManager>();
            if (sm != null)
            {
                sm.enabled = true;
            }

            AudioListener al = gameObject.GetComponent<AudioListener>();
            if (al != null)
            {
                al.enabled = true;
            }
        }

        void OnEnable()
        {
            frameProviderInitialized = false;
            StartCoroutine("CallPluginAtEndOfFrames");
        }

        protected override void OnDestroy()
        {
            ResetCompositor();
            if (networkDiscovery != null)
            {
                networkDiscovery.SessionListChanged -= NetworkDiscovery_SessionListChanged;
            }
            networkDiscovery = null;

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
