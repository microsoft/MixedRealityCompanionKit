// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;

namespace SpectatorView
{
    public class SV_SpatialMappingManager : NetworkBehaviour
    {
        private static SV_SpatialMappingManager _Instance;
        public static SV_SpatialMappingManager Instance
        {
            get
            {
                if (_Instance == null)
                {
                    _Instance = FindObjectOfType<SV_SpatialMappingManager>();
                }
                return _Instance;
            }
        }

        [SyncVar]
        public bool requestSpatialMappingFlag = false;

        private bool _createSpatialMapInEditor = false;

        [SyncVar(hook="OnSpatialMappingSerialized")]
        public int spatialMappingNumChunks = 0;

        public void OnSpatialMappingSerialized(int value)
        {
            spatialMappingNumChunks = value;
            _createSpatialMapInEditor = true;
        }

        public struct SpatialMappingChunk
        {
            public SpatialMappingChunk(int meshIndex, int chunkIndex, int totalMeshBytes, int chunkLength, byte[] bytes)
            {
                this.meshIndex = meshIndex;
                this.chunkIndex = chunkIndex;
                this.totalMeshBytes = totalMeshBytes;
                this.chunkLength = chunkLength;
                this.bytes = bytes;
            }

            public int meshIndex;
            public int chunkIndex;
            public int totalMeshBytes;
            public int chunkLength;
            public byte[] bytes;
        }

        public class SyncListSpatialMappingChunk : SyncListStruct<SpatialMappingChunk> { }

        [SyncVar]
        public SyncListSpatialMappingChunk smBytes = new SyncListSpatialMappingChunk();

        [HideInInspector]
        public int NetworkedChunkSize = 4096;

        System.Collections.IEnumerator SendSpatialMappingData(int currentIndex, int numChunks, int meshIndex, int chunkIndex, int totalMeshBytes, int chunkLength, byte[] chunkBytes)
        {
            // Introduce some latency so we do not bottleneck the network.
            yield return new WaitForSeconds(currentIndex * 0.03f);

            PlayerController.Instance.SendSpatialMappingData(meshIndex, chunkIndex, totalMeshBytes, chunkLength, chunkBytes);
        }

        public void GetSpatialMappingData()
        {
            Debug.Log("Finding spatial mapping data.");
            if (!SpectatorView.SpectatorViewManager.Instance.IsSpectatorView())
            {
                return;
            }

            if (HoloToolkit.Unity.SpatialMapping.SpatialMappingManager.Instance == null)
            {
                return;
            }

            if (UNetAnchorManager.Instance == null)
            {
                return;
            }

            PlayerController.Instance.ClearSpatialMappingData();

            int currentIndex = 0;
            List<MeshFilter> MeshFilters = HoloToolkit.Unity.SpatialMapping.SpatialMappingManager.Instance.GetMeshFilters();
            int totalChunks = 0;
            for (int index = 0; index < MeshFilters.Count; index++)
            {
                List<Mesh> meshesToSend = new List<Mesh>();
                MeshFilter filter = MeshFilters[index];
                Mesh source = filter.sharedMesh;
                Mesh clone = new Mesh();
                List<Vector3> verts = new List<Vector3>();
                verts.AddRange(source.vertices);

                for (int vertIndex = 0; vertIndex < verts.Count; vertIndex++)
                {
                    verts[vertIndex] = UNetAnchorManager.Instance.gameObject.transform.InverseTransformPoint(
                        filter.transform.TransformPoint(verts[vertIndex]));
                }

                clone.SetVertices(verts);
                clone.SetTriangles(source.triangles, 0);
                meshesToSend.Add(clone);
                byte[] serialized = SpectatorView.SV_SimpleMeshSerializer.Serialize(meshesToSend);

                // Chunk data
                int numChunks = (int)Mathf.Ceil(serialized.Length / NetworkedChunkSize);
                totalChunks += numChunks;

                for (int chunkIndex = 0; chunkIndex < numChunks; chunkIndex++)
                {
                    byte[] chunkBytes = new byte[NetworkedChunkSize];
                    int length = NetworkedChunkSize;

                    if (chunkIndex * NetworkedChunkSize + length >= serialized.Length)
                    {
                        length = serialized.Length - chunkIndex * NetworkedChunkSize;
                    }

                    if ((chunkIndex * NetworkedChunkSize) + length >= serialized.Length)
                    {
                        Debug.LogWarning("Copying too much data");
                        Debug.Log("Length: " + length + " Index " + chunkIndex + " out of " + numChunks);
                        Debug.Log("Total length: " + serialized.Length);
                    }

                    System.Array.Copy(serialized, chunkIndex * NetworkedChunkSize, chunkBytes, 0, length);

                    StartCoroutine(SendSpatialMappingData(currentIndex, numChunks, index, chunkIndex, serialized.Length, length, chunkBytes));
                    currentIndex++;
                }

                PlayerController.Instance.SetSpatialMappingDataCompleted(totalChunks);
            }
        }

        void Update()
        {
#if UNITY_EDITOR
            if (_createSpatialMapInEditor && smBytes.Count >= spatialMappingNumChunks)
            {
                _createSpatialMapInEditor = false;

                int meshIndex = -1;
                byte[] bytes = new byte[0];

                for (int i = 0; i < smBytes.Count; i++)
                {
                    if (smBytes[i].meshIndex != meshIndex)
                    {
                        meshIndex = smBytes[i].meshIndex;
                        if (bytes.Length != 0)
                        {
                            // Deserialize the old bytes.
                            IEnumerable<Mesh> meshes = SpectatorView.SV_SimpleMeshSerializer.Deserialize(bytes);
                            foreach (Mesh mesh in meshes)
                            {
                                GameObject sm = HoloToolkit.Unity.SpatialMapping.SpatialMappingManager.Instance.gameObject;
                                RemoteSpatialMappingSource src = sm.GetComponent<RemoteSpatialMappingSource>();
                                List<Vector3> v = new List<Vector3>(mesh.vertices);
                                List<Vector3> n = new List<Vector3>(mesh.normals);
                                List<int> t = new List<int>(mesh.triangles);

                                src.AddSurface(v, n, t, UNetAnchorManager.Instance.gameObject.transform);
                            }
                        }

                        // Update byte array with new mesh's size.
                        bytes = new byte[smBytes[i].totalMeshBytes];
                    }

                    // Copy network chunk to appropriate spot in the destination buffer.
                    System.Array.Copy(smBytes[i].bytes, 0, bytes, NetworkedChunkSize * smBytes[i].chunkIndex, smBytes[i].chunkLength);
                }
            }
#endif
        }
    }
}
