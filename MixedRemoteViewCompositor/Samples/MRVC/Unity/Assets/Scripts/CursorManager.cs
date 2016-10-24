// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;

public class CursorManager : MonoBehaviour
{
    public GameObject CursorPrefab;
    public float DefaultDistance = 5.0f;
    public Color HitColor = Color.yellow;
    public Color BaseColor = Color.white;

    private Transform cameraTransform;

    private GameObject cursorGameObject;
    private GameObject hitObject;

    private bool mixedMode = false;
    private Vector3 overridePosition;
    private Vector3 overrideNormal;
    private GameObject overrideObject;

    private void Awake()
    {
        this.cameraTransform = Camera.main.transform;

        if (this.CursorPrefab)
        {
            this.cursorGameObject =
                Instantiate(this.CursorPrefab, this.cameraTransform.position + this.cameraTransform.forward * DefaultDistance,
                    Quaternion.identity) as GameObject;
        }

        if (this.cursorGameObject == null)
        {
            Debug.LogError("Not able to create Cursor");

            return;
        }

        var raycastController = FindObjectOfType<RaycastController>();
        if (raycastController == null)
        {
            Debug.LogError("Not able to find RaycastController in scene.");

            return;
        }
    }

    private void LateUpdate()
    {
        if (this.cameraTransform == null)
        {
            return;
        }

        var raycast = RaycastController.RaycastHit;
        var go = raycast.collider != null ? raycast.collider.gameObject : null;

        // position & normal
        var position = (go != null) ? raycast.point : this.cameraTransform.position + (this.cameraTransform.forward * DefaultDistance);
        var normal = (go != null) ? -raycast.normal : this.cameraTransform.forward;

        var screenPoint = Camera.main.WorldToScreenPoint(position);
        screenPoint.z = Camera.main.nearClipPlane;
        var worldPoint = Camera.main.ScreenToWorldPoint(screenPoint);

        Debug.DrawRay(worldPoint, (position - worldPoint), go != null ? Color.yellow : Color.white);

        // set active hit object
        if (this.mixedMode)
        {
            position = this.overridePosition;
            normal = this.overrideNormal;
            go = this.overrideObject;
        }

        screenPoint = Camera.main.WorldToScreenPoint(position);
        screenPoint.z = Camera.main.nearClipPlane;
        worldPoint = Camera.main.ScreenToWorldPoint(screenPoint);

        Debug.DrawRay(worldPoint, (position - worldPoint), Color.red);

        // update the cusor object properties
        SetActiveObject(go);

        this.cursorGameObject.transform.position = position;
        this.cursorGameObject.transform.forward = normal;
    }

    private void SetActiveObject(GameObject go)
    {
        bool hitChanged = false;

        if (go != null)
        {
            if (this.hitObject != go)
            {
                this.hitObject = go;
                hitChanged = true;
            }
        }
        else
        {
            if (this.hitObject != null)
            {
                this.hitObject = null;
                hitChanged = true;
            }
        }

        if (hitChanged)
        {
            var rend = this.cursorGameObject.GetComponentInChildren<Renderer>();
            if (this.hitObject)
            {
                if (rend != null)
                {
                    rend.material.color = this.HitColor;
                }
            }
            else
            {
                if (rend != null)
                {
                    rend.material.color = this.BaseColor;
                }
            }
        }
    }

    public void SetCursorData(Vector3 position, Vector3 normal, GameObject go)
    {
        mixedMode = true;

        this.overridePosition = position;
        this.overrideNormal = normal;
        this.overrideObject = go;
    }
}