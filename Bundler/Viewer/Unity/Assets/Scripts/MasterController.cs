// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using HoloToolkit.Unity.SpatialMapping;
using Persistence;
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

#if UNITY_WSA
using UnityEngine.XR.WSA.Input;
#endif

public class MasterController : ImprovedSingletonBehavior<MasterController>
{
    public AssetManager AssetManager;
    public ListManager ListManager;
    public ServerManager ServerManager;
    public StageManager StageManager;
    public StageManipulator StageManipulator;
    public StageMenu StageMenu;
    public MenuManager MenuManager;
    public ZoneCalibrationManager ZoneCalibrationManager;
    public NetworkStateManager NetworkStateManager;
    public SpatialMappingManager SpatialMappingManager;

    public GameObject DebugInfo;

    public GameObject ModelInfoContainer;

    public GameObject Finger;

    private bool isPlacingStage;

    private bool hasSource;
    private bool delayHiding;
    private bool delayShowing;

    private bool isManipulating;
    public bool IsManipulating
    {
        get { return isManipulating; }
        set { isManipulating = value; DelayHideMenu(0, true); }
    }

    public string CurrentAssetInfoID { get { return AssetManager.CurrentAssetInfoID; } }
    public AssetInfo CurrentAssetInfo { get { return AssetManager.CurrentAssetInfo; } }
    public float CurrentStageScale { get { return NetworkStateManager.CurrentState.StageScale; } }
    public float CurrentVerticalOffset { get { return NetworkStateManager.CurrentState.StageOffset; } }
    public bool CurrentExplode { get { return NetworkStateManager.CurrentState.Explode; } }
    public bool CurrentAutoRotate { get { return StageManager.AutoRotate; } }

    void Start()
    {
        AssetManager.AssetLoaded += AssetManager_AssetLoaded;
        AssetManager.AssetLoadFailed += AssetManager_AssetLoadFailed;
        AssetManager.AssetLoadUpdated += AssetManager_AssetLoadUpdated;

#if UNITY_WSA
        InteractionManager.InteractionSourcePressed += InteractionManager_SourcePressed;
#endif
        ToggleModelInfo(false);
        StageManager.SetPreviewMode(false);
        StageManager.ToggleLoading(false);

        if (!HololensHelper.IsDeviceHololens())
        {
            QualitySettings.SetQualityLevel(2, true);
        }
    }
    
#if UNITY_WSA && !UNITY_EDITOR
    private async void OnApplicationQuit()
    {
        await System.Threading.Tasks.Task.Delay(2000);
}
#endif

    Vector3 GazeTargetPosition()
    {
        RaycastHit hitInfo;
        if (Physics.Raycast(Camera.main.transform.position, Camera.main.transform.forward, out hitInfo, 10))
        {
            return hitInfo.point;
        }
        return Vector3.zero;
    }

    private void Update()
    {
        if (isPlacingStage)
        {
            var target = GazeTargetPosition();
            if (target != Vector3.zero)
            {
                MoveStageOrigin((Vector3)target, true);
            }
        }
    }

#if UNITY_WSA
    private void InteractionManager_SourcePressed(InteractionSourcePressedEventArgs args)
    {
        if (isPlacingStage)
        {
            SpatialMappingManager.StopObserver();
            isPlacingStage = false;
            StageManager.SetPreviewMode(false);
            var target = GazeTargetPosition();
            if (target != Vector3.zero)
            {
                SetStageOrigin(target);
            }
            SpatialMappingManager.gameObject.SetActive(false);
        }
    }
#endif

    private IEnumerator DelayHideMenu(float delay = 1.0f, bool force = false)
    {
        if (!delayHiding || delay == 0 || force)
        {
            delayHiding = true;
            yield return new WaitForSeconds(delay);
            delayHiding = false;
            if (!hasSource && !delayShowing || force)
            {
                Finger.SetActive(false);
                ShowMenu(MenuManager.MenuType.None);
                Debug.Log("DelayHideMenu");
            }
        }
    }

    private void OnGUI()
    {
        if (!HololensHelper.IsDeviceHololens())
        {
            GUILayout.BeginArea(new Rect(10, 0, 100, 200));
            var names = QualitySettings.names;
            GUILayout.BeginVertical();
            GUILayout.Label("Render Quality");
            int i = 0;
            while (i < names.Length)
            {
                if (GUILayout.Toggle(QualitySettings.GetQualityLevel() == i, names[i]))
                {
                    QualitySettings.SetQualityLevel(i, true);
                }

                ++i;
            }
            GUILayout.EndVertical();
            GUILayout.EndArea();
        }

        if (CurrentAssetInfo.IsEmpty())
        {
            return;
        }
    }

    public void StartStagePlacement()
    {
        ShowMenu(MenuManager.MenuType.None);
#if UNITY_WSA
        SpatialMappingManager.gameObject.SetActive(true);
        SpatialMappingManager.StartObserver();
        ZoneCalibrationManager.Zones[0].ClearAnchor(true);
        StartCoroutine(DelayStartStagePlacement());
#endif
    }

    private IEnumerator DelayStartStagePlacement()
    {
        yield return new WaitForSeconds(.5f);

        isPlacingStage = true;
        StageManager.SetPreviewMode(true);
    }

    public void SetDefaultStagePosition()
    {
        var direction = new Vector3(Camera.main.transform.forward.x, 0, Camera.main.transform.forward.z);
        direction.Normalize();
        direction *= 1.00f;
        direction.y = -.5f;
        var newPosition = Camera.main.transform.position + direction;
        SetStageOrigin(newPosition);
    }

    public void SetStageOrigin(Vector3 position)
    {
        ZoneCalibrationManager.Zones[0].ClearAnchor(true);
        MoveStageOrigin(position);
        ZoneCalibrationManager.Zones[0].PlaceAnchor(true);
    }

    private void MoveStageOrigin(Vector3 position, bool smooth = false)
    {
        if (smooth)
        {
            float smoothTime = 0.1F;
            Vector3 velocity = Vector3.zero;
            ZoneCalibrationManager.Zones[0].TargetTransform.position = Vector3.SmoothDamp(ZoneCalibrationManager.Zones[0].TargetTransform.position, position, ref velocity, smoothTime);
            float velocityF = 0.0f;
            var newRotY = Mathf.SmoothDampAngle(ZoneCalibrationManager.Zones[0].TargetTransform.rotation.eulerAngles.y, Camera.main.transform.rotation.eulerAngles.y, ref velocityF, smoothTime);
            ZoneCalibrationManager.Zones[0].TargetTransform.rotation = Quaternion.Euler(0, newRotY, 0);
        }
        else
        {
            ZoneCalibrationManager.Zones[0].TargetTransform.position = position;
            var newRot = Quaternion.Euler(0, Camera.main.transform.rotation.eulerAngles.y, 0);
            ZoneCalibrationManager.Zones[0].TargetTransform.rotation = newRot;
        }
    }

    public void ToggleManipulationMode(bool? force = null)
    {
        if (force == null)
        {
            StageManipulator.IsEnabled = !StageManipulator.IsEnabled;
        }
        else
        {
            StageManipulator.IsEnabled = (bool)force;
        }
    }

    public void LoadBundle(AssetInfo assetInfo)
    {
        //TODO: throw exception?
        if (assetInfo.IsEmpty())
        {
            //Debug.LogWarning("LoadBundle: assetInfo is null");
            return;
        }

        NetworkStateManager.LoadAsset(assetInfo);
    }

    public void DoLoadBundle(AssetInfo assetInfo)
    {
        //TODO: throw exception?
        if (assetInfo.IsEmpty())
        {
            //Debug.LogWarning("DoLoadBundle: assetInfo is null");
            return;
        }

        if (assetInfo.Equals(AssetManager.CurrentAssetInfo))
        {
            return;
        }

        HideMenu();
        StageManager.ClearStage();
        StageManager.ToggleLoading(true);
        AssetManager.LoadBundle(assetInfo);
    }

    private void AssetManager_AssetLoaded(AssetInfo assetInfo, int lodIndex, GameObject gameObject)
    {
        StageManager.AddObjectToStage(gameObject);
    }

    private void AssetManager_AssetLoadUpdated(AssetInfo assetInfo, int lodIndex, string message)
    {
        StageManager.ShowMessage(message);
    }

    private void AssetManager_AssetLoadFailed(AssetInfo assetInfo, int lodIndex, string message)
    {
        StageManager.ShowMessage(message);
    }

    public void ToggleAutoRotate(bool? force = null)
    {
        StageManager.ToggleAutoRotate(force);
    }

    public bool ToggleLocalMode()
    {
        NetworkStateManager.BypassNetwork = !NetworkStateManager.BypassNetwork;
        return NetworkStateManager.BypassNetwork;
    }

    public bool ModelInfoVisible { get { return ModelInfoContainer.activeSelf; } }

    public void ToggleModelInfo(bool? force = null)
    {
        HideMenu();
        if (force == null)
        {
            ModelInfoContainer.SetActive(!ModelInfoContainer.activeSelf);
        }
        else
        {
            ModelInfoContainer.SetActive((bool)force);
        }
    }

    public void SetStageRotationRelative(float value)
    {
        SetStageRotation(NetworkStateManager.CurrentState.StageRotation + value);
    }

    public void SetStageRotation(float value)
    {
        NetworkStateManager.SetRotation(value);
    }

    public void DoSetStageRotation(float value)
    {
        StageManager.SetStageRotation(value);
    }

    public void SetStageScaleRelative(float scale)
    {
        SetStageScale(NetworkStateManager.CurrentState.StageScale + scale);
    }

    public void SetStageScale(float scale)
    {
        if (scale <= 0)
        {
            scale = .25f;
        } 
        NetworkStateManager.SetScale(scale);
    }

    public void DoSetStageScale(float scale)
    {
        StageManager.SetStageScale(scale);
    }

    public void ToggleExplode()
    {
        SetExplode(!CurrentExplode);
    }

    public void SetExplode(bool explode)
    {
        NetworkStateManager.SetExplode(explode);
    }

    public void DoSetExplode(bool explode)
    {
        HideMenu();
        StageManager.SetExplode(explode);
    }

    public void SetVerticalOffsetRelative(float height)
    {
        SetVerticalOffset(CurrentVerticalOffset + height);
    }

    public void SetVerticalOffset(float height)
    {
        NetworkStateManager.SetOffset(height);
    }

    public void DoSetVerticalOffset(float height)
    {
        StageManager.SetVerticalOffset(height);
    }

    public void HideMenu()
    {
        ShowMenu(MenuManager.MenuType.None);
        StageMenu.RefreshButtons();
    }

    public void ShowMenu(MenuManager.MenuType menu)
    {
        MenuManager.ShowMenu(menu);
    }

    public void ToggleDebugInfo(bool show)
    {
        DebugInfo.SetActive(show);
    }
}
