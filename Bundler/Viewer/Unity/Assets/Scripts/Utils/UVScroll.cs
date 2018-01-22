// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;

[RequireComponent(typeof(Renderer))]
[RequireComponent(typeof(MeshFilter))]
public class UVScroll : MonoBehaviour
{
    public float scrollSpeed = 0.05F;
    private Renderer rend;

    private const float boundwidth = 5.0f;

    void Start()
    {
        rend = GetComponent<Renderer>();        

        var mesh = GetComponent<MeshFilter>().mesh;
        mesh.bounds = new Bounds(new Vector3(0, 0, 0), Vector3.one * boundwidth);
    }

    void Update()
    {
        var offset = Mathf.Repeat(Time.time * scrollSpeed, 4);
        rend.sharedMaterial.SetVector("_Offset", new Vector4(offset, offset * 0.05f, 0, 0));
    }
}
