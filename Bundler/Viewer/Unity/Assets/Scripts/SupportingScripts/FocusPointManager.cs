// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using UnityEngine;

#if UNITY_5
using UnityEngine.VR.WSA;
#else
using UnityEngine.XR.WSA;
#endif

public class FocusPointManager : SingletonBehavior<FocusPointManager>
{
    public float DefaultStartDistance = 2.0f;

    public float MinDistance = 0.01f;

    public bool Snap = false;

    public float FocusPointDistance { get; private set; }

    public Vector3 FocusPointPosition { get; private set; }

    public Vector3 FocusPointNormal { get { return -Camera.main.transform.forward; } }

    private const float LerpPowerCloser = 7.0f;
    private const float LerpPowerFarther = 10.0f;

    protected override void InternalAwake()
    {
        // uses the main camera for focus plane adjustmensts
        if (Camera.main == null)
        {
            Debug.LogError("Tag a MainCamera that will be used for the scene.");
            return;
        }

        this.MinDistance = Camera.main.nearClipPlane + this.MinDistance;
        this.FocusPointDistance = this.DefaultStartDistance;
        this.FocusPointPosition = Camera.main.transform.position + (Camera.main.transform.forward * this.FocusPointDistance);
    }

    protected override void InternalOnDestroy()
    {
    }

    void LateUpdate()
    {
        if (HolographicSettings.IsDisplayOpaque)
            return;

        // use the last location
        var newFocusPos = Camera.main.transform.position + (Camera.main.transform.forward * this.FocusPointDistance);

        // if we hit an object, set this as the new position
        RaycastHit hitInfo;
        if (Physics.Raycast(Camera.main.transform.position, Camera.main.transform.forward, out hitInfo, 10))
        {
            newFocusPos = hitInfo.transform.position;
        }

        // determine its distance to that point
        var newFocusPointDistance = (Camera.main.transform.position - newFocusPos).magnitude;

        // if we want to set it immediately, set snap mode
        if (this.Snap)
        {
            this.FocusPointDistance = newFocusPointDistance;
        }
        else
        {
            // otherwise, lerp based on going away or getting closer to the camera
            if (newFocusPointDistance > this.FocusPointDistance)
            {
                this.FocusPointDistance = Mathf.Lerp(this.FocusPointDistance, newFocusPointDistance, LerpPowerFarther * Time.deltaTime);
            }
            else
            {
                this.FocusPointDistance = Mathf.Lerp(newFocusPointDistance, this.FocusPointDistance, LerpPowerCloser * Time.deltaTime);
            }
        }

        if (this.FocusPointDistance <= this.MinDistance)
        {
            this.FocusPointDistance = this.MinDistance;
        }

        // set the position
        this.FocusPointPosition
            = Camera.main.transform.position + (Camera.main.transform.forward * this.FocusPointDistance);

        HolographicSettings.SetFocusPointForFrame(this.FocusPointPosition, this.FocusPointNormal);
    }
}
