// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

namespace SpectatorView
{
    public class SpectatorViewManager : SV_Singleton<SpectatorViewManager>
    {
#if UNITY_EDITOR
        #region DLLImports
        [DllImport("UnityCompositorInterface")]
        private static extern void GetPose(out Vector3 pos, out Quaternion rot, int frameOffset);

        [DllImport("UnityCompositorInterface")]
        private static extern void SetSpectatorViewIP(string ip);

        [DllImport("UnityCompositorInterface")]
        private static extern bool InitializeFrameProvider();

        [DllImport("UnityCompositorInterface")]
        private static extern void UpdateCompositor();

        [DllImport("UnityCompositorInterface")]
        private static extern void StopFrameProvider();

        [DllImport("UnityCompositorInterface")]
        private static extern void ResetSV();

        [DllImport("UnityCompositorInterface")]
        private static extern bool IsRecording();

        [DllImport("UnityCompositorInterface")]
        private static extern void StopRecording();

        [DllImport("UnityCompositorInterface")]
        private static extern bool SetAnchorData(string ip, int port, string name);

        [DllImport("UnityCompositorInterface")]
        private static extern bool ForceAnchorReconnect();

        [DllImport("UnityCompositorInterface")]
        private static extern void ResetPoseCache();
        #endregion

        [Header("Visuals")]
        [Tooltip("Hologram transparency.")]
        [Range(0, 1)]
        public float Alpha = 0.95f;
        private float prevAlpha;

        // If the hologram texture moves earlier than the color texture, choose a higher value.
        // If the color texture moves earlier than the hologram texture, choose a lower value.
        // 
        // Default frame offset was found with a Canon 5D MkIII using a Blackmagic DeckLink Intensity Pro 4K.
        // Your frame offsets may be different depending on your camera and capture card.
        [Header("Timing")]
        [Tooltip("Number of frames of latency between camera capture and frame delivery.")]
        [Range(0, 10)]
        public int FrameOffset = 4;

        [Tooltip("If poses to color frames have de-synced, force a reset.")]
        public bool ResetHologramSynchronization = false;

        [Header("Connection")]
        [Tooltip("IP of the spectator view device.")]
        public string SpectatorViewHoloLensIP;

        [Tooltip("IP of the HoloLens that created the anchor in the shared experience you are spectating.")]
        public string AnchorOwnerIP;

        public int AnchorPort = 11000;

        private string AnchorName;

        Vector3 pos = Vector3.zero;
        Quaternion rot = Quaternion.identity;

        [HideInInspector]
        public bool frameProviderInitialized = false;

        // Spatial Mapping:
        [Header("Spatial Mapping")]
        public Material SpatialMappingMaterial;
        private Material prevSpatialMaterial;

        SpatialMapping spatialMapping;

        private void Awake()
        {
            // Remove the default audio listener, there can only be one at a time.
            if (Camera.main != null)
            {
                AudioListener listener = Camera.main.GetComponent<AudioListener>();
                if (listener != null)
                {
                    listener.enabled = false;
                }
            }

            ResetHologramSynchronization = false;

            prevSpatialMaterial = SpatialMappingMaterial;
            spatialMapping = new SpatialMapping();

            InvokeRepeating("SendAnchorInformationToPoseProvider", 10, 10);
        }

        private void OnEnable()
        {
            if (SpectatorViewHoloLensIP != String.Empty)
            {
                SetSpectatorViewIP(SpectatorViewHoloLensIP.Trim());
            }

            if (AnchorOwnerIP != string.Empty && AnchorName != string.Empty)
            {
                SetAnchorInformation(AnchorOwnerIP, AnchorPort, AnchorName);
            }

            prevAlpha = Alpha;
        }

        public void CreateSpectatorViewConnection(string ip)
        {
            SpectatorViewHoloLensIP = ip;
            SetSpectatorViewIP(SpectatorViewHoloLensIP.Trim());
        }

        public void SetAnchorInformation(string ip, int port, string name)
        {
            AnchorOwnerIP = ip;
            AnchorPort = port;
            AnchorName = name;

            SetAnchorData(ip, port, name);
        }

        public void ForceSVAnchorReconnect()
        {
            if (!ForceAnchorReconnect())
            {
                Invoke("ForceSVAnchorReconnect", 5);
            }
        }

        // Periodically run this to ensure pose provider has valid data.
        private void SendAnchorInformationToPoseProvider()
        {
            if (AnchorOwnerIP != string.Empty &&
                AnchorName != string.Empty)
            {
                SetAnchorData(AnchorOwnerIP, AnchorPort, AnchorName);
            }
        }

        private void OnValidate()
        {
            if (ShaderManager.Instance != null &&
                ShaderManager.Instance.AllShadersAndTexturesReady())
            {
                if (Alpha != prevAlpha)
                {
                    ShaderManager.Instance.alphaBlendPreviewMat.SetFloat("_Alpha", Alpha);
                }
                prevAlpha = Alpha;
            }

            if (ResetHologramSynchronization)
            {
                Debug.Log("Resetting hologram synchronization.");
                ResetHologramSynchronization = false;
                ResetPoseCache();
            }

            if (SpatialMappingMaterial != prevSpatialMaterial)
            {
                if (SpatialMappingMaterial == null)
                {
                    SpatialMappingMaterial = Resources.Load<Material>("Materials/SpatialMapping/Default");
                }

                prevSpatialMaterial = SpatialMappingMaterial;
                if (spatialMapping != null)
                {
                    foreach (GameObject go in spatialMapping.spatialMappingMeshes)
                    {
                        go.GetComponent<Renderer>().material = SpatialMappingMaterial;
                    }
                }
            }
        }

        private void OnDestroy()
        {
            ResetCompositor();
            CancelInvoke("SendAnchorInformationToPoseProvider");
        }

        public void ResetCompositor()
        {
            StopFrameProvider();

            if (IsRecording())
            {
                StopRecording();
            }

            if (ShaderManager.Instance != null)
            {
                ShaderManager.Instance.Reset();
            }

            ResetSV();
        }

        private float UpdateSVTransformFromParentScale(float inputComponent, float scale)
        {
            // Undefined, so just return the input component.
            if (scale == 0)
            {
                return inputComponent;
            }

            return inputComponent / scale;
        }

        void Update()
        {
            GetPose(out pos, out rot, FrameOffset);
            TransformValidation.ValidatePose(ref pos, ref rot);

            // Update local transform with pose data from the network.
            // Use local transform, so we can child this gameobject to the scene anchor.
            try
            {
                if (gameObject.transform.parent != null &&
                    gameObject.transform.parent.lossyScale != Vector3.one)
                {
                    // If the object we are anchored to has a non-identity scale, 
                    // the anchor we get from the pose provider will be off by the scale factor.
                    Vector3 scale = gameObject.transform.parent.lossyScale;
                    gameObject.transform.localPosition = new Vector3(
                        UpdateSVTransformFromParentScale(pos.x, scale.x),
                        UpdateSVTransformFromParentScale(pos.y, scale.y),
                        UpdateSVTransformFromParentScale(pos.z, scale.z)
                        );
                }
                else
                {
                    // Otherwise, our scale is one and we can set the position as we got it.
                    gameObject.transform.localPosition = pos;
                }

                gameObject.transform.localRotation = rot;
            }
            catch(Exception ex)
            {
                Debug.LogWarning("Error getting position and rotation from SV: " + ex.Message);
            }

            if (!frameProviderInitialized)
            {
                frameProviderInitialized = InitializeFrameProvider();
            }

            UpdateCompositor();
            if (spatialMapping == null)
            {
                spatialMapping = new SpatialMapping();
            }
            spatialMapping.UpdateSpatialMapping(transform.parent, SpatialMappingMaterial);
        }
#endif
    }
}
