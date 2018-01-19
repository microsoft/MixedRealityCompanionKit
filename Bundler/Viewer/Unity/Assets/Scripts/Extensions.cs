// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;

public static class Extensions
{
    public static bool HasFlag(this NetworkStateManager.NetworkStateChanges changes, NetworkStateManager.NetworkStateChanges change)
    {
        return (changes & change) == change;
    }

    public static void DestroyChildren(this Transform transform)
    {
        for (int i = transform.childCount - 1; i >= 0; --i)
        {
            GameObject.DestroyImmediate(transform.GetChild(i).gameObject);
        }
        transform.DetachChildren();
    }

    public static void DrawBox(Bounds bounds, Color color)
    {
        Vector3 v3Center = bounds.center;
        Vector3 v3Extents = bounds.extents;

        var v3FrontTopLeft = new Vector3(v3Center.x - v3Extents.x, v3Center.y + v3Extents.y, v3Center.z - v3Extents.z);  // Front top left corner
        var v3FrontTopRight = new Vector3(v3Center.x + v3Extents.x, v3Center.y + v3Extents.y, v3Center.z - v3Extents.z);  // Front top right corner
        var v3FrontBottomLeft = new Vector3(v3Center.x - v3Extents.x, v3Center.y - v3Extents.y, v3Center.z - v3Extents.z);  // Front bottom left corner
        var v3FrontBottomRight = new Vector3(v3Center.x + v3Extents.x, v3Center.y - v3Extents.y, v3Center.z - v3Extents.z);  // Front bottom right corner
        var v3BackTopLeft = new Vector3(v3Center.x - v3Extents.x, v3Center.y + v3Extents.y, v3Center.z + v3Extents.z);  // Back top left corner
        var v3BackTopRight = new Vector3(v3Center.x + v3Extents.x, v3Center.y + v3Extents.y, v3Center.z + v3Extents.z);  // Back top right corner
        var v3BackBottomLeft = new Vector3(v3Center.x - v3Extents.x, v3Center.y - v3Extents.y, v3Center.z + v3Extents.z);  // Back bottom left corner
        var v3BackBottomRight = new Vector3(v3Center.x + v3Extents.x, v3Center.y - v3Extents.y, v3Center.z + v3Extents.z);  // Back bottom right corner

        Debug.DrawLine(v3FrontTopLeft, v3FrontTopRight, color, 60.0f);
        Debug.DrawLine(v3FrontTopRight, v3FrontBottomRight, color, 60.0f);
        Debug.DrawLine(v3FrontBottomRight, v3FrontBottomLeft, color, 60.0f);
        Debug.DrawLine(v3FrontBottomLeft, v3FrontTopLeft, color, 60.0f);

        Debug.DrawLine(v3BackTopLeft, v3BackTopRight, color, 60.0f);
        Debug.DrawLine(v3BackTopRight, v3BackBottomRight, color, 60.0f);
        Debug.DrawLine(v3BackBottomRight, v3BackBottomLeft, color, 60.0f);
        Debug.DrawLine(v3BackBottomLeft, v3BackTopLeft, color, 60.0f);

        Debug.DrawLine(v3FrontTopLeft, v3BackTopLeft, color, 60.0f);
        Debug.DrawLine(v3FrontTopRight, v3BackTopRight, color, 60.0f);
        Debug.DrawLine(v3FrontBottomRight, v3BackBottomRight, color, 60.0f);
        Debug.DrawLine(v3FrontBottomLeft, v3BackBottomLeft, color, 60.0f);
    }
}

