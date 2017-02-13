// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using System.Collections.Generic;
using HoloToolkit.Unity;
using HoloToolkit.Unity.SpatialMapping;

namespace SpectatorView
{
    public class RemoteSpatialMappingSource : SpatialMappingSource
    {
#if UNITY_EDITOR
        void Start()
        {
            SpatialMappingManager.Instance.SetSpatialMappingSource(this);
        }

        public void AddSurface(List<Vector3> vertices, List<Vector3> normals, List<int> triangles)
        {
            GameObject meshObject = GameObject.CreatePrimitive(PrimitiveType.Cube);
            meshObject.layer = SpatialMappingManager.Instance.PhysicsLayer;
            meshObject.transform.localPosition = Vector3.zero;
            meshObject.transform.localRotation = Quaternion.identity;

            Mesh mesh = new Mesh();
            meshObject.GetComponent<MeshFilter>().mesh.Clear();
            meshObject.GetComponent<MeshFilter>().mesh = mesh;
            mesh.SetVertices(vertices);
            mesh.SetNormals(normals);
            mesh.SetTriangles(triangles, 0);

            mesh.RecalculateNormals();

            Collider collider = meshObject.GetComponent<Collider>();
            if (collider != null)
            {
                GameObject.DestroyImmediate(collider);
            }
            MeshCollider meshCollider = meshObject.AddComponent<MeshCollider>();
            meshCollider.sharedMesh = null;
            meshCollider.sharedMesh = mesh;

            if (!SpatialMappingManager.Instance.DrawVisualMeshes)
            {
                meshObject.GetComponent<Renderer>().enabled = false;
            }
            if (!SpatialMappingManager.Instance.CastShadows)
            {
                meshObject.GetComponent<Renderer>().shadowCastingMode = UnityEngine.Rendering.ShadowCastingMode.Off;
            }

            GameObject surfaceObject = AddSurfaceObject(mesh, "RemoteSpatialMapping", transform);
            surfaceObject.transform.localPosition = Vector3.zero;
            surfaceObject.transform.localRotation = Quaternion.identity;

            meshCollider = surfaceObject.GetComponent<MeshCollider>();
            meshCollider.sharedMesh = null;
            meshCollider.sharedMesh = mesh;

            // Destroy original game object.
            GameObject.DestroyImmediate(meshObject);
        }

        public void ClearMeshes()
        {
            Cleanup();
        }
#endif
    }
}
