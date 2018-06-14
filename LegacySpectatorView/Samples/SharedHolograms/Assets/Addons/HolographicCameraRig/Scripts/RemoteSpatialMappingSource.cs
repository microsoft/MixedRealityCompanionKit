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

        public void AddSurface(List<Vector3> vertices, List<Vector3> normals, List<int> triangles, Transform parent = null)
        {
            Mesh mesh = new Mesh();
            mesh.Clear();

            mesh.SetVertices(vertices);
            mesh.SetNormals(normals);
            mesh.SetTriangles(triangles, 0);

            mesh.RecalculateNormals();

            if (parent == null)
            {
                parent = transform;
            }

            SurfaceObject surfaceObject = CreateSurfaceObject(
                        mesh: mesh,
                        objectName: "RemoteSpatialMapping",
                        parentObject: parent,
                        drawVisualMeshesOverride: SpatialMappingManager.Instance.DrawVisualMeshes,
                        castShadowsOverride: SpatialMappingManager.Instance.CastShadows
                        );

            surfaceObject.Object.transform.localPosition = Vector3.zero;
            surfaceObject.Object.transform.localRotation = Quaternion.identity;

            AddSurfaceObject(surfaceObject);

        }

        public void ClearMeshes()
        {
            Cleanup();
        }
#endif
    }
}
