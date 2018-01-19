// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
#if UNITY_WSA
using UnityEngine.XR.WSA.Input;
#endif

public class StageManipulator : MonoBehaviour
{
    public enum Mode
    {
        None,
        Rotate,
        Scale,
        Move
    }

    [HideInInspector]
    public bool IsEnabled;

    private bool isManipulating = false;
    private Mode currentMode;

    private void Start()
    {
        currentMode = Mode.None;
#if UNITY_WSA
        InteractionManager.InteractionSourcePressed += InteractionManager_SourcePressed;
        InteractionManager.InteractionSourceLost += InteractionManager_SourceLost;
        InteractionManager.InteractionSourceReleased += InteractionManager_SourceReleased;
        InteractionManager.InteractionSourceUpdated += InteractionManager_SourceUpdated;
#endif
    }
    

    Vector3 lastPos = Vector3.zero;
    Vector3 targetPos = Vector3.zero;

    private void Update()
    {
        if (!IsEnabled)
            return;

        Vector3 vel = Vector3.zero;
        var newPos = Vector3.SmoothDamp(lastPos, targetPos, ref vel, .066f);

        var delta = newPos - lastPos;
        if (delta.sqrMagnitude > float.Epsilon)
        {
            HandleMotion(delta);
        }
        else
        {
            currentMode = Mode.None;
        }
        lastPos = newPos;

    }
#if UNITY_WSA
    private void InteractionManager_SourceUpdated(InteractionSourceUpdatedEventArgs args)
    {
        if (!isManipulating || !IsEnabled)
            return;

        Vector3 pos;
        if (args.state.sourcePose.TryGetPosition(out pos))
        {
            targetPos = pos;
        }
    }

    private void InteractionManager_SourceReleased(InteractionSourceReleasedEventArgs args)
    {
        isManipulating = false;
        MasterController.Instance.IsManipulating = false;
    }

    private void InteractionManager_SourcePressed(InteractionSourcePressedEventArgs args)
    {
        if (!IsEnabled)
            return;

        Vector3 pos;

        if (args.state.sourcePose.TryGetPosition(out pos))
        {
            targetPos = pos;
            lastPos = pos;
        }
        isManipulating = true;
        MasterController.Instance.IsManipulating = true;
        currentMode = Mode.None;
        cumDelta = Vector3.zero;
    }

    private void InteractionManager_SourceLost(InteractionSourceLostEventArgs state)
    {
        isManipulating = false;
        MasterController.Instance.IsManipulating = false;
    }

#endif

    private void OnGUI()
    {
        if (!HololensHelper.IsDeviceHololens())
        {
            GUILayout.BeginArea(new Rect(10, 110, 100, 200));
            GUILayout.Label("Manipulate");
            if (GUI.RepeatButton(new Rect(0, 25, 30, 30), "<"))
            {
                MasterController.Instance.SetStageRotationRelative(25f * Time.deltaTime);
            }
            if (GUI.RepeatButton(new Rect(35, 25, 30, 30), ">"))
            {
                MasterController.Instance.SetStageRotationRelative(-25f * Time.deltaTime);
            }
            if (GUI.RepeatButton(new Rect(0, 60, 30, 30), "Up"))
            {
                MasterController.Instance.SetVerticalOffsetRelative(.1f * Time.deltaTime);
            }
            if (GUI.RepeatButton(new Rect(35, 60, 30, 30), "Dn"))
            {
                MasterController.Instance.SetVerticalOffsetRelative(-.1f * Time.deltaTime);
            }
            if (GUI.RepeatButton(new Rect(0, 95, 30, 30), "+"))
            {
                MasterController.Instance.SetStageScaleRelative(.075f * Time.deltaTime);
            }
            if (GUI.RepeatButton(new Rect(35, 95, 30, 30), "-"))
            {
                MasterController.Instance.SetStageScaleRelative(-.075f * Time.deltaTime);
            }
            GUILayout.EndArea();
        }
    }

    Vector3 cumDelta = Vector3.zero;

    private void HandleMotion(Vector3 delta)
    {
        delta = Camera.main.transform.InverseTransformDirection(delta);

        if (currentMode == Mode.None)
        {
            cumDelta += delta;
            if (cumDelta.magnitude > .02f)
            {
                cumDelta.x = Mathf.Abs(cumDelta.x);
                cumDelta.y = Mathf.Abs(cumDelta.y);
                cumDelta.z = Mathf.Abs(cumDelta.z);
                if (cumDelta.x > cumDelta.y && cumDelta.x > cumDelta.z)
                    currentMode = Mode.Rotate;
                if (cumDelta.y > cumDelta.x && cumDelta.y > cumDelta.z)
                    currentMode = Mode.Move;
                if (cumDelta.z > cumDelta.x && cumDelta.z > cumDelta.y)
                    currentMode = Mode.Scale;
            }
        }

        switch (currentMode)
        {
            case Mode.Scale:
                {
                    var movementAxis = Vector3.forward;
                    var axisDelta = Vector3.Project(delta, movementAxis);

                    float distance = axisDelta.magnitude;
                    distance *= Mathf.Sign(Vector3.Dot(axisDelta, movementAxis));
                    MasterController.Instance.SetStageScaleRelative(distance * -1);
                }
                break;
            case Mode.Move:
                {
                    var movementAxis = Vector3.up;
                    var axisDelta = Vector3.Project(delta, movementAxis);

                    float distance = axisDelta.magnitude;
                    distance *= Mathf.Sign(Vector3.Dot(axisDelta, movementAxis));
                    MasterController.Instance.SetVerticalOffsetRelative(distance * 1);
                }
                break;
            case Mode.Rotate:
                {
                    var movementAxis = Vector3.right;
                    var axisDelta = Vector3.Project(delta, movementAxis);

                    float distance = axisDelta.magnitude;
                    distance *= Mathf.Sign(Vector3.Dot(axisDelta, movementAxis));
                    MasterController.Instance.SetStageRotationRelative(-distance * 150);
                }
                break;
        }
    }
}
