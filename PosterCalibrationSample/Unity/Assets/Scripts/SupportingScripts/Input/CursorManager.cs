// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

using Showcases.Input;

public class CursorManager : ImprovedSingletonBehavior<CursorManager>
{
    public enum CursorState
    {
        Idle,
        PrimaryIn,
        PrimaryOut,
        SecondaryIn,
        SecondaryOut,
        LongTap,
        Scaling,
        ScalingInactive
    };

    public enum CursorType
    {
        Normal,
        Annotation,
        Pressed
    }

    public CursorType CurrentCursorType
    {
        get; private set;
    }

    public GameObject CursorPrefab;
    public GameObject PressedCursorPrefab;

    public float DefaultCursorDistance = 3.0f;
    public float MoveRate = 25f;
    public float TurnRate = 10f;

    public float ModelScale = .8f;
    public float MinScale = 0.025f;
    public float MaxScale = 2.5f;
    public float ScaleRate = 0.5f;
    public GameObject HitObject;
    public bool AutoCursorState;
    public bool CursorTextFacesCamera;
    public bool AutoCursorStateWhenInteracting;

    private CursorState cursorState;
    private GameObject currentCursorObj;
    private GameObject normalCursorObj;
    private GameObject pressedCursorObj;
    private Animator cursorAnimator;
    
    public Vector3 CursorLocation;
    public Quaternion CursorRotation;

    public bool UseLerp = true;
    public float LerpAmount = .35f;
    public float Threshold = .001f;
    public float CursorObjectOffset = .01f;

    public Transform CursorTransform
    {
        get { return currentCursorObj.transform; }
    }

    private void Awake()
    {
        if (CursorPrefab)
        {
            normalCursorObj = Instantiate(CursorPrefab, Camera.main.transform.position + Camera.main.transform.forward * DefaultCursorDistance, Quaternion.identity) as GameObject;
        }

        if (normalCursorObj == null)
        {
            throw new InvalidOperationException("Unable to instantiate CursorPrefab");
        }
        
        if(PressedCursorPrefab)
        {
            pressedCursorObj = Instantiate(PressedCursorPrefab, Camera.main.transform.position + Camera.main.transform.forward * DefaultCursorDistance, Quaternion.identity) as GameObject;
        }

        if (pressedCursorObj == null)
        {
            throw new InvalidOperationException("Unable to instantiate PressedCursorPrefab");
        }
        
        normalCursorObj.SetActive(false);
        pressedCursorObj.SetActive(false);

        SetCursorType(CursorType.Normal);
        
        Initialize();
    }
    
    void Update()
    {
        RaycastResult raycast = CustomHoloLensInputModule.GetRaycastResult();
        
        // set active hit object
        SetActiveObject(raycast.gameObject);

        // play sound and animate cursor
        OnClick(raycast.gameObject);

        var position = (raycast.gameObject != null) ? raycast.worldPosition : raycast.worldPosition - (raycast.worldNormal * DefaultCursorDistance);
        this.CursorLocation = position;
        this.CursorRotation = Quaternion.LookRotation(-raycast.worldNormal);
        
        Vector3 av = currentCursorObj.transform.position;
        Quaternion aq = currentCursorObj.transform.rotation;

        Vector3 bv = this.CursorLocation;
        Quaternion bq = this.CursorRotation;

        var totalDistance = ((bv - Camera.main.transform.position) - (av - Camera.main.transform.position)).sqrMagnitude;
        if (totalDistance < Threshold && totalDistance > -Threshold)
        {
            av = bv = this.CursorLocation;
            aq = bq = this.CursorRotation;
        }

        // position & rotation to match normal
        currentCursorObj.transform.position = UseLerp ? Vector3.Lerp(av, bv, LerpAmount) : this.CursorLocation;
        currentCursorObj.transform.position += CursorObjectOffset * raycast.worldNormal;// Vector3.Normalize(currentCursorObj.transform.position - Camera.main.transform.position);
        currentCursorObj.transform.rotation = UseLerp ? Quaternion.Slerp(aq, bq, LerpAmount) : this.CursorRotation;

        if(pressedCursorObj.activeInHierarchy)
        {
            pressedCursorObj.transform.position = currentCursorObj.transform.position;
            pressedCursorObj.transform.forward = Vector3.ProjectOnPlane(currentCursorObj.transform.forward, Vector3.up);
        }
    }

    public void SetActiveObject(GameObject obj)
    {
        bool hitChanged = false;

        if (obj)
        {
            if (HitObject != obj)
            {
                HitObject = obj;
                hitChanged = true;
            }
        }
        else
        {
            if (HitObject)
            {
                HitObject = null;
                hitChanged = true;
            }
        }

        if (AutoCursorState && hitChanged)
        {
            if (HitObject)
            {
                if ((HitObject.GetComponentInParent<Selectable>() != null
                    && HitObject.GetComponentInParent<Selectable>().interactable) ||
                    (HitObject.GetComponentInChildren<IEventSystemHandler>() != null))
                {
                    SetCursorState(CursorState.PrimaryIn);
                }
                else
                {
                    SetCursorState(CursorState.Idle);
                }
            }
            else
            {
                SetCursorState(CursorState.Idle);
            }
        }
    }

    public void SetCursorType(CursorType cursorType)
    {
        if (currentCursorObj != null)
            currentCursorObj.SetActive(false);
        CurrentCursorType = cursorType;
        switch (cursorType)
        {
            case CursorType.Pressed:
                pressedCursorObj.SetActive(true);
                break;
            default:
                pressedCursorObj.SetActive(false);
                currentCursorObj = normalCursorObj;
                break;
        }
        cursorAnimator = currentCursorObj.GetComponentInChildren<Animator>();
        currentCursorObj.SetActive(true);
    }

    public void SetCursorState(CursorState state)
    {
        cursorState = state;
    }

    private void OnClick(GameObject hitObject)
    {
        if (hitObject == null)
        {
            return;
        }

        var buttonState = CustomHoloLensInputModule.GetInputButtonState();
        if (buttonState.PressedThisFrame() && buttonState.ReleasedThisFrame())
        {
            if ((hitObject.GetComponentInParent<Selectable>() != null) ||
                (hitObject.GetComponentInChildren<IEventSystemHandler>() != null))
            {
                bool playSoundAndAnimate = false;
                
                if (playSoundAndAnimate)
                {
                    if (this.currentCursorObj != null)
                    {
                        var animator = this.currentCursorObj.GetComponentInChildren<Animator>();
                        if (animator != null)
                        {
                            animator.Play("Click");
                        }
                    }
                }
            }
        }
    }
}
