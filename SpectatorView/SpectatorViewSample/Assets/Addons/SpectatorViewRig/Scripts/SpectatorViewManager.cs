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
        private static extern void GetPose(out Vector3 pos, out Quaternion rot, float UnityTimeS, int frameOffset);

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
        private static extern void SetAudioData(byte[] audioData);

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

        [DllImport("UnityCompositorInterface")]
        private static extern bool IsSpatialMappingDataReady(out int numMeshes);

        [DllImport("UnityCompositorInterface")]
        private static extern bool GetSpatialMappingDataBufferLengths(int index, out int numVertices, out int numIndices);

        [DllImport("UnityCompositorInterface")]
        private static extern bool GetSpatialMappingData(int index, out Vector3 translation, out Quaternion rotation, out Vector3 scale, out IntPtr vertices, out IntPtr indices);
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
        GameObject spatialMappingParent;
        List<GameObject> spatialMappingMeshes = new List<GameObject>();

        [Header("Spatial Mapping")]
        public Material SpatialMappingMaterial;
        private Material prevSpatialMaterial;

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

            spatialMappingParent = GameObject.CreatePrimitive(PrimitiveType.Cube);
            DestroyImmediate(spatialMappingParent.GetComponent<Renderer>());
            DestroyImmediate(spatialMappingParent.GetComponent<Collider>());
            spatialMappingParent.gameObject.name = "SV_SpatialMapping";

            prevSpatialMaterial = SpatialMappingMaterial;

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
                foreach (GameObject go in spatialMappingMeshes)
                {
                    go.GetComponent<Renderer>().material = SpatialMappingMaterial;
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

        // Unity requires quaternions to be normalized.
        Quaternion GetNormalizedQuaternion(Quaternion q)
        {
            float f = 1.0f / Mathf.Sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
            return new Quaternion(q.x * f, q.y * f, q.z * f, q.w * f);
        }

        void ValidateVector(ref Vector3 pos)
        {
            // Unity will fail if position values are too small.
            float epsilon = 0.00001f;
            if (Mathf.Abs(pos.x) < epsilon) { pos.x = 0; }
            if (Mathf.Abs(pos.y) < epsilon) { pos.y = 0; }
            if (Mathf.Abs(pos.z) < epsilon) { pos.z = 0; }
        }

        void ValidatePose(ref Vector3 pos, ref Quaternion rot)
        {
            ValidateVector(ref pos);

            // Unity will fail if a transform's rotation is not normalized.
            rot = GetNormalizedQuaternion(rot);
        }

        void Update()
        {
            GetPose(out pos, out rot, Time.time, FrameOffset);
            ValidatePose(ref pos, ref rot);

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
            UpdateSpatialMapping();
        }

        void UpdateSpatialMapping()
        {
            int numSpatialMappingMeshes = 0;
            if (IsSpatialMappingDataReady(out numSpatialMappingMeshes))
            {
                // Set parent transform and remove previous meshes.
                spatialMappingParent.transform.SetParent(transform.parent);
                spatialMappingParent.transform.localPosition = Vector3.zero;
                spatialMappingParent.transform.localRotation = Quaternion.identity;

                foreach (GameObject go in spatialMappingMeshes)
                {
                    DestroyImmediate(go);
                }
                spatialMappingMeshes.Clear();

                // Find largest sizes for index and vertex buffers.
                int largestNumVertices = int.MinValue;
                int largestNumIndices = int.MinValue;
                int numVertices;
                int numIndices;
                for (int i = 0; i < numSpatialMappingMeshes; i++)
                {
                    if (GetSpatialMappingDataBufferLengths(i, out numVertices, out numIndices))
                    {
                        if (numVertices > largestNumVertices)
                        {
                            largestNumVertices = numVertices;
                        }

                        if (numIndices > largestNumIndices)
                        {
                            largestNumIndices = numIndices;
                        }
                    }
                }

                IntPtr nativeVertices = Marshal.AllocHGlobal(largestNumVertices * 3 * sizeof(float));
                IntPtr nativeIndices = Marshal.AllocHGlobal(largestNumIndices * sizeof(short));

                for (int i = 0; i < numSpatialMappingMeshes; i++)
                {
                    if (GetSpatialMappingDataBufferLengths(i, out numVertices, out numIndices))
                    {
                        float[] vertexElements = new float[numVertices * 3];
                        short[] indices = new short[numIndices];

                        if (numVertices <= 0 || numIndices <= 0)
                        {
                            return;
                        }

                        Vector3 meshTrans;
                        Quaternion meshRot;
                        Vector3 meshScale;
                        if (GetSpatialMappingData(i, out meshTrans, out meshRot, out meshScale, out nativeVertices, out nativeIndices))
                        {
                            ValidateVector(ref meshTrans);
                            ValidateVector(ref meshScale);
                            meshRot = GetNormalizedQuaternion(meshRot);

                            Marshal.Copy(nativeVertices, vertexElements, 0, numVertices * 3);
                            Marshal.Copy(nativeIndices, indices, 0, numIndices);

                            List<Vector3> vertices = new List<Vector3>();
                            for (int v = 0; v < vertexElements.Length; v+=3)
                            {
                                Vector3 vertex = new Vector3(
                                    vertexElements[v],
                                    vertexElements[v + 1],
                                    vertexElements[v + 2]
                                );

                                vertices.Add(vertex);
                            }

                            int[] int_indices = new int[numIndices];
                            for (int idx = 0; idx < numIndices; idx++)
                            {
                                int_indices[idx] = (int)indices[idx];
                            }

                            // Create spatial mapping meshes.
                            GameObject go = GameObject.CreatePrimitive(PrimitiveType.Cube);
                            go.name = "SpatialMapping_" + i.ToString();
                            go.transform.SetParent(spatialMappingParent.transform);

                            go.transform.localPosition = meshTrans;
                            go.transform.localRotation = meshRot;

                            // Scale coming from device is larger by a factor of 5.
                            go.transform.localScale = meshScale / 5.0f;

                            if (SpatialMappingMaterial != null)
                            {
                                go.GetComponent<Renderer>().material = SpatialMappingMaterial;
                            }

                            spatialMappingMeshes.Add(go);

                            DestroyImmediate(go.GetComponent<Collider>());
                            MeshFilter filter = go.GetComponent<MeshFilter>();
                            filter.mesh = new Mesh();
                            filter.sharedMesh = filter.mesh;

                            Mesh mesh = filter.sharedMesh;
                            mesh.Clear();

                            mesh.SetVertices(vertices);
                            mesh.SetIndices(int_indices, MeshTopology.Triangles, 0);

                            mesh.RecalculateNormals();
                            mesh.RecalculateBounds();

                            Debug.Log(String.Format("Received spatial mapping mesh {0} with {1} vertices and {2} indices", i, numVertices, numIndices));
                        }
                    }
                }

                //TODO: We should be freeing the memory we allocated for the index and vertex buffers, but this was deadlocking.
                //Marshal.FreeHGlobal(nativeVertices);
                //Marshal.FreeHGlobal(nativeIndices);
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
