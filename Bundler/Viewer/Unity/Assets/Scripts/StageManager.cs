// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class StageManager : MonoBehaviour
{
    public Transform ScaleRootContainer;
    public Transform RotationRootContainer;
    public Transform ObjectStageContainer;
    public Transform MenuRootContainer;
    public GameObject LoadingStage;
    public GameObject PreviewStage;
    public Text LoadingText;
    public List<Clickable> RotateButtons;

    public bool AutoRotate { get; private set; }
    private GameObject currentModelGameObject;

    private Vector3[] originalMeshPositions;

    private bool isScaling;
    private bool isOffsetting;

	private bool shouldExplode;
    private bool isExploding;
    private bool isExploded;

    void Start()
    {
        SetStageScale(.75f);
        SetVerticalOffset(0);
        foreach (var button in RotateButtons)
        {
            button.ButtonClicked += Button_ButtonClicked;
        }
    }

    private void Button_ButtonClicked(Clickable source)
    {
        Debug.Assert(RotateButtons.Count >= 3);

        if (source == RotateButtons[0])
        {
            SetStageRotation(0, true);
        }
        else if (source == RotateButtons[1])
        {
            SetStageRotation(90, true);
        }
        else if (source == RotateButtons[2])
        {
            SetStageRotation(180, true);
        }
        else if (source == RotateButtons[3])
        {
            SetStageRotation(270, true);
        }
    }

    void Update()
    {
        if (AutoRotate)
        {
            RotationRootContainer.transform.Rotate(0, 3 * Time.deltaTime, 0);
        }
    }

    public void SetPreviewMode(bool on)
    {
        PreviewStage.SetActive(on);
        RotationRootContainer.gameObject.SetActive(!on);
        MenuRootContainer.gameObject.SetActive(!on);
    }

    public void SetExplode(bool explode, bool animate = true)
    {
        if (animate)
        {
            StartCoroutine(DoExplode(explode));
        }
    }

    struct ExplodeInfo
    {
        public Transform transform;
        public Vector3 targetPos;
    }

    private IEnumerator DoExplode(bool explode)
    {
        if (currentModelGameObject != null)
        {
            var offsetDist = .5f;
            if (!isExploding)
            {
                var meshes = currentModelGameObject.GetComponentsInChildren<MeshFilter>();
                var explodeInfos = new List<ExplodeInfo>(meshes.Length);
                var lowestTargetY = ObjectStageContainer.transform.position.y;
                bool hasWork = false;
                isExploding = true;
                if (!isExploded && explode)
                {
                    hasWork = true;
                    isExploded = true;
                    Bounds bounds = new Bounds();
                    yield return StartCoroutine(getMeshBounds(currentModelGameObject, value => bounds = value));

                    Vector3 center = bounds.center;
                    float maxDist = (bounds.max - center).magnitude;

                    foreach (var mesh in meshes)
                    {
                        Vector3 maxWorld = mesh.transform.TransformPoint(mesh.mesh.bounds.max);
                        Vector3 minWorld = mesh.transform.TransformPoint(mesh.mesh.bounds.min);

                        Bounds partBounds = new Bounds((maxWorld + minWorld) / 2, //center
                                                        maxWorld - minWorld);     //extent

                        var directionFromCenter = partBounds.center - center;
                        var distanceToCenter = (partBounds.center - center).magnitude;
                        var ratio = distanceToCenter / maxDist;

                        var targetDist = offsetDist * ratio;
                        directionFromCenter.Normalize();

                        var targetPos = mesh.transform.position + directionFromCenter * targetDist;

                        var newPartBounds = partBounds;
                        newPartBounds.center = partBounds.center + directionFromCenter * targetDist;
                        lowestTargetY = Math.Min(lowestTargetY, newPartBounds.min.y);
                        explodeInfos.Add(new ExplodeInfo { transform = mesh.transform, targetPos = targetPos });
                    }
                }
                else if (isExploded && !explode)
                {
                    isExploded = false;
                    hasWork = true;
                    for (var i = 0; i < meshes.Length; i++)
                    {
                        var mesh = meshes[i];
                        Vector3 tp = ObjectStageContainer.transform.TransformPoint(originalMeshPositions[i]);
                        explodeInfos.Add(new ExplodeInfo { transform = mesh.transform, targetPos = tp });
                    }
                }

                if (hasWork)
                {
                    var lowestOffset = ObjectStageContainer.transform.position.y - lowestTargetY;
                    foreach (var info in explodeInfos)
                    {
                        StartCoroutine(DoExplodeOffsetForPart(info.transform, info.targetPos, lowestOffset));
                    }

                    yield return new WaitForSeconds(1);
                }
                isExploding = false;
            }
        }
        else
        {
            shouldExplode = explode;
        }
    }

    private IEnumerator DoExplodeOffsetForPart(Transform part, Vector3 targetPos, float offsetY = 0)
    {
        float smoothTime = 0.3F;
        Vector3 velocity = Vector3.zero;
        var newTarget = targetPos + Vector3.up * offsetY;
        while (Vector3.Distance(part.transform.position, newTarget) > 0.01f)
        {
            part.transform.position = Vector3.SmoothDamp(part.transform.position, newTarget, ref velocity, smoothTime);
            yield return null;
        }
    }

    public void SetVerticalOffset(float height, bool animate = false)
    {
        if (animate)
        {
            StartCoroutine(DoSetVerticalOffset(height));
        }
        else
        {
            ObjectStageContainer.transform.localPosition = new Vector3(0, height, 0);
        }
    }

    private IEnumerator DoSetVerticalOffset(float targetVal)
    {
        if (!isOffsetting)
        {
            isOffsetting = true;
            var currentVal = ObjectStageContainer.transform.localPosition.y;

            float smoothTime = 0.3F;
            Vector3 velocity = Vector3.zero;

            while (Math.Abs(currentVal - targetVal) > 0.01f)
            {
                ObjectStageContainer.transform.localPosition = Vector3.SmoothDamp(ObjectStageContainer.transform.localPosition, new Vector3(0, targetVal, 0), ref velocity, smoothTime);
                currentVal = ObjectStageContainer.transform.localPosition.y;
                yield return null;
            }
            isOffsetting = false;
        }
    }

    public void SetStageScale(float scale, bool animate = false)
    {
        if (scale <= 0) scale = .25f;

        if (animate)
        {
            StartCoroutine(DoScale(scale));
        }
        else
        {
            ScaleRootContainer.localScale = new Vector3(scale, scale, scale);
        }
    }

    private IEnumerator DoScale(float targetVal)
    {
        if (!isScaling)
        {
            isScaling = true;
            var currentVal = ScaleRootContainer.localScale.x;

            float smoothTime = 0.3F;
            Vector3 velocity = Vector3.zero;

            while (Math.Abs(currentVal - targetVal) > 0.01f)
            {
                ScaleRootContainer.localScale = Vector3.SmoothDamp(ScaleRootContainer.localScale, new Vector3(targetVal, targetVal, targetVal), ref velocity, smoothTime);
                currentVal = ScaleRootContainer.localScale.x;
                yield return new WaitForEndOfFrame();
            }
            isScaling = false;
        }
    }

    public void ToggleAutoRotate(bool? force = null)
    {
        AutoRotate = (force == null)? !AutoRotate : (bool)force;
    }

    public void SetStageRotation(float val, bool animate = false)
    {
        if (animate)
        {
            StartCoroutine(DoRotation(val));
        }
        else
        {
            RotationRootContainer.transform.localRotation = Quaternion.Euler(0, val, 0);
        }
    }

    private IEnumerator DoRotation(float val)
    {
        Vector3 targetDirInLocalSpace = (Camera.main.transform.transform.position - RotationRootContainer.transform.position);

        Quaternion targetRot = Quaternion.LookRotation(targetDirInLocalSpace);
        targetRot = Quaternion.Euler(0, targetRot.eulerAngles.y - val, 0);
        float targetVal = targetRot.eulerAngles.y;
        var currentVal = RotationRootContainer.transform.localEulerAngles.y;
        float direction = Mathf.Sign(Mathf.Repeat(targetVal - currentVal + 180, 360) - 180);

        while (Math.Abs(currentVal - targetVal) > 1.0f)
        {
            yield return new WaitForEndOfFrame();
            RotationRootContainer.transform.localEulerAngles = new Vector3(0, currentVal + direction * 1.0f, 0);
            currentVal = RotationRootContainer.transform.localEulerAngles.y;
        }
    }

    public void ToggleLoading(bool isLoading)
    {
        if (isLoading)
        {
            LoadingText.text = "Loading Model";
        }

        LoadingStage.SetActive(isLoading);
    }

    public void ClearStage()
    {
        currentModelGameObject = null;
        ObjectStageContainer.DestroyChildren();
    }

    public void ShowMessage(string message)
    {
        LoadingText.text = message;
    }

    public void AddObjectToStage(GameObject newObject)
    {
        StartCoroutine(DoAddObjectToStage(newObject));
    }

    private IEnumerator DoAddObjectToStage(GameObject newObject)
    {
        ShowMessage("Loading Model: \nSetting positions...");
        yield return null;

        isExploded = false;
        currentModelGameObject = newObject;
        var root = new GameObject("ObjectRoot");

        newObject.transform.SetParent(root.transform, false);

        var bounds = new Bounds();
        yield return StartCoroutine(getMeshBounds(root, value => bounds = value));

        // Fit into a 1x1.5x1 box
        var x = bounds.extents.x * 2.0f;
        var y = bounds.extents.y * 1.5f;
        var z = bounds.extents.z * 2.0f;

        var maxVal = Math.Max(x, Math.Max(y, z));

        var scale = 1 / maxVal;
        root.transform.localScale = new Vector3(scale, scale, scale);
        var recenter = new Vector3(bounds.center.x * -1.0f * scale, -bounds.min.y * scale, bounds.center.z * -1.0f * scale);
        root.transform.Translate(recenter);
        root.transform.SetParent(ObjectStageContainer, false);
#if UNITY_EDITOR
        yield return StartCoroutine(getMeshBounds(root, value => bounds = value));
        Extensions.DrawBox(bounds, Color.green);
#endif
        
        yield return null;
        var lastYieldTime = DateTime.Now;
        var meshes = newObject.GetComponentsInChildren<MeshFilter>();
        originalMeshPositions = new Vector3[meshes.Length];
        for (var i = 0; i < meshes.Length; ++i)
        {           
            var mesh = meshes[i];
            originalMeshPositions[i] = ObjectStageContainer.transform.InverseTransformPoint(mesh.transform.position);
            var renderer = mesh.GetComponent<Renderer>();
            renderer.enabled = true;
            if ((DateTime.Now - lastYieldTime).TotalMilliseconds > 30)
            {
                lastYieldTime = DateTime.Now;
                yield return null;
            }
        }

        ToggleLoading(false);
        SetExplode(shouldExplode);
        shouldExplode = false;
    }

    public static IEnumerator getMeshBounds(GameObject root, System.Action<Bounds> result)
    {
        //Get all the mesh filters in the tree.
        var filters = root.GetComponentsInChildren<MeshFilter>();

        //Construct an empty bounds object w/o an extant.
        var bounds = new Bounds(Vector3.zero, Vector3.zero);
        bool firstTime = true;
        var lastYieldTime = DateTime.Now;
        //For each mesh filter...
        foreach (MeshFilter mf in filters)
        {
            //Pull its bounds into the overall bounds.  Bounds are given in the local space of the mesh, 
            //but we want them in world space,
            //so tranform the max and min points by the xform of the object containing the mesh.
            Vector3 maxWorld = mf.transform.TransformPoint(mf.mesh.bounds.max);
            Vector3 minWorld = mf.transform.TransformPoint(mf.mesh.bounds.min);

            //If no bounds have been set yet...
            if (firstTime)
            {
                firstTime = false;
                //Set the bounding box to encompass the current mesh and bounds in world coordinates.
                bounds = new Bounds((maxWorld + minWorld) / 2, //center
                                     maxWorld - minWorld);     //extent
            }
            else    
            {
                //We've started a bounding box.  Make sure it ecapsulates the current mesh extrema.
                bounds.Encapsulate(maxWorld);
                bounds.Encapsulate(minWorld);
            }

            if ((DateTime.Now - lastYieldTime).TotalMilliseconds > 15)
            {
                lastYieldTime = DateTime.Now;
                yield return null;
            }
        }
        bounds.extents = new Vector3(Math.Abs(bounds.extents.x), Math.Abs(bounds.extents.y), Math.Abs(bounds.extents.z));

        result(bounds);
    }
}
