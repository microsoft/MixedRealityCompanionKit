// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

namespace SpectatorView
{
    public class SpatialMapping
    {
        #region DLLImports
        [DllImport("UnityCompositorInterface")]
        private static extern bool IsSpatialMappingDataReady(out int numMeshes);

        [DllImport("UnityCompositorInterface")]
        private static extern bool GetSpatialMappingDataBufferLengths(int index, out int numVertices, out int numIndices);

        [DllImport("UnityCompositorInterface")]
        private static extern bool GetSpatialMappingData(int index, out Vector3 translation, out Quaternion rotation, out Vector3 scale, out IntPtr vertices, out IntPtr indices);

        [DllImport("UnityCompositorInterface")]
        private static extern void ResetPoseCache();
        #endregion

        GameObject spatialMappingParent;
        public List<GameObject> spatialMappingMeshes = new List<GameObject>();

        public SpatialMapping()
        {
            spatialMappingParent = GameObject.CreatePrimitive(PrimitiveType.Cube);
            GameObject.DestroyImmediate(spatialMappingParent.GetComponent<Renderer>());
            GameObject.DestroyImmediate(spatialMappingParent.GetComponent<Collider>());
            spatialMappingParent.gameObject.name = "SV_SpatialMapping";
        }

        public void UpdateSpatialMapping(Transform parent, Material SpatialMappingMaterial)
        {
            int numSpatialMappingMeshes = 0;
            if (IsSpatialMappingDataReady(out numSpatialMappingMeshes))
            {
                // Set parent transform and remove previous meshes.
                spatialMappingParent.transform.SetParent(parent);
                spatialMappingParent.transform.localPosition = Vector3.zero;
                spatialMappingParent.transform.localRotation = Quaternion.identity;

                foreach (GameObject go in spatialMappingMeshes)
                {
                    GameObject.DestroyImmediate(go);
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
                            TransformValidation.ValidateVector(ref meshTrans);
                            TransformValidation.ValidateVector(ref meshScale);
                            meshRot = TransformValidation.GetNormalizedQuaternion(meshRot);

                            Marshal.Copy(nativeVertices, vertexElements, 0, numVertices * 3);
                            Marshal.Copy(nativeIndices, indices, 0, numIndices);

                            List<Vector3> vertices = new List<Vector3>();
                            for (int v = 0; v < vertexElements.Length; v += 3)
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

                            GameObject.DestroyImmediate(go.GetComponent<Collider>());
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

                // When receiving spatial mapping data, some amount of time will pass without receiving any pose data.
                // To ensure our poses remain synced, force a reset of our pose cache.
                ResetPoseCache();
            }
        }
    }
}
