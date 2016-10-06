// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.VR.WSA;

[RequireComponent(typeof(RaycastController))]
public class FocusPointController : MonoBehaviour
{
    public float DefaultStartDistance = 2.0f;

    public float MinDistance = 0.1f;

    public bool Snap = false;

    public float FocusPointDistance { get; private set; }

    public Vector3 FocusPointPosition { get; private set; }

    public Vector3 FocusPointNormal { get { return -Camera.main.transform.forward; } }

    private const float LerpPowerCloser = 7.0f;
    private const float LerpPowerFarther = 10.0f;

    private Transform cameraTransform = null;

    void Awake()
    {
        // uses the main camera for focus plane adjustmensts
        if (Camera.main == null)
        {
            Debug.LogError("You need to choose a main camera that will be used for the scene");
            return;
        }

        this.cameraTransform = Camera.main.transform;

        this.MinDistance = Camera.main.nearClipPlane + this.MinDistance;
        this.FocusPointDistance = this.DefaultStartDistance;
        this.FocusPointPosition = this.cameraTransform.position + (this.cameraTransform.forward * this.FocusPointDistance);
    }

    void LateUpdate()
    {
        // calculating the new position since the camera may have moved and need to maintain that distance
        var newFocusPos = this.cameraTransform.position + (this.cameraTransform.forward * this.FocusPointDistance);

        // if we hit an object, set this as the new position
        if (RaycastController.HitThisFrame)
        {
            newFocusPos = RaycastController.RaycastHit.point;
        }

        // determine its distance to that point
        var newFocusPointDistance = (cameraTransform.position - newFocusPos).magnitude;

        // if we want to set it immediately, set snap mode
        if (this.Snap)
        {
            this.FocusPointDistance = newFocusPointDistance;
        }
        else
        {
            // otherwise, lerp based on whether we are going away from the camera
            if (newFocusPointDistance > this.FocusPointDistance)
            {
                this.FocusPointDistance = Mathf.Lerp(this.FocusPointDistance, newFocusPointDistance, LerpPowerFarther * Time.deltaTime);
            }
            else
            {
                this.FocusPointDistance = Mathf.Lerp(newFocusPointDistance, this.FocusPointDistance, LerpPowerCloser * Time.deltaTime);
            }
        }

        this.FocusPointDistance = Mathf.Max(this.FocusPointDistance, this.MinDistance);

        // set the position
        this.FocusPointPosition
            = this.cameraTransform.position + (this.cameraTransform.forward * this.FocusPointDistance);

        HolographicSettings.SetFocusPointForFrame(this.FocusPointPosition, this.FocusPointNormal);
    }
}
