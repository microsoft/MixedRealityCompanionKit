// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using System.Collections;

public class RaycastController : MonoBehaviour
{
    public float MaxDistance = 5.0f;

    public LayerMask LayerMask;

    public static bool HitThisFrame { get { return hitThisFrame; } }
    private static bool hitThisFrame;

    public static RaycastHit RaycastHit { get { return raycastHit; } }
    private static RaycastHit raycastHit;

    private Transform cameraTransform = null;

    private void Awake()
    {
        if(this.LayerMask == 0)
        {
            this.LayerMask 
                = LayerMask.NameToLayer("Everything") 
                & ~(1 << LayerMask.NameToLayer("Ignore Raycast"));
        }

        this.cameraTransform = Camera.main.transform;
    }

    private void Update()
    {
        hitThisFrame = Physics.Raycast(this.cameraTransform.position, this.cameraTransform.forward, out raycastHit, this.MaxDistance, this.LayerMask);
        if (!hitThisFrame)
        {
            raycastHit.point = this.cameraTransform.position + (this.cameraTransform.forward * this.MaxDistance);
            raycastHit.normal = -this.cameraTransform.forward;
        }
    }
}
