// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class MenuBase : MonoBehaviour
{
    protected bool isVisible = true;
    bool delayHiding;

    public virtual void UpdateMenu()
    {
    }

    public virtual void ShowMenu()
    {
        gameObject.SetActive(true);
        if (isVisible)
        {
            return;
        }
        isVisible = true;

        if (GetComponent<Animator>() != null)
        {
            GetComponent<Animator>().Play("PopIn");
        }
    }

    public virtual void HideMenu()
    {
        if (!isVisible)
        {
            return;
        }

        isVisible = false;
        if (isActiveAndEnabled)
        {
            if (GetComponent<Animator>() != null)
            {
                GetComponent<Animator>().Play("PopOut");
            }

            StartCoroutine(DelayHideMenu());
        }
    }

    private IEnumerator DelayHideMenu(float delay = 0.4f)
    {
        if (!delayHiding || delay == 0)
        {
            delayHiding = true;
            yield return new WaitForSeconds(delay);
            delayHiding = false;
            if (!isVisible)
            {
                gameObject.SetActive(false);
            }
        }
    }

    public virtual void ToggleMenu(bool? force = null)
    {
        if (isVisible || (force != null && !(bool)force))
        {
            HideMenu();
        }
        else
        {
            ShowMenu();
        }
    }
}

public class TopMenu : MenuBase
{
    public List<MenuButtonInfo> MenuButtons;
    public Transform ItemContainer;
    public MenuButton MenuButtonPrefab;
    public Text InfoText;
    private Dictionary<ButtonCommands, MenuButton> buttonInstances = new Dictionary<ButtonCommands, MenuButton>();

    void Start()
    {
        foreach (var buttonInfo in MenuButtons)
        {
            var newButton = (MenuButton)Instantiate(MenuButtonPrefab);
            newButton.ButtonClicked += Button_ButtonClicked;
            newButton.transform.SetParent(ItemContainer, false);
            newButton.SetInfo(buttonInfo);
            buttonInstances.Add(buttonInfo.Command, newButton);
        }
    }

    private void Button_ButtonClicked(Clickable source)
    {
        switch (source.Command)
        {
            case ButtonCommands.ShowAssetMenu:
                MasterController.Instance.ShowMenu(MenuManager.MenuType.AssetList);
                break;
            case ButtonCommands.RecenterStage:
                MasterController.Instance.StartStagePlacement();
                break;
            case ButtonCommands.ScaleUp:
                MasterController.Instance.SetStageScaleRelative(.25f);
                break;
            case ButtonCommands.ScaleDown:
                MasterController.Instance.SetStageScaleRelative(-.25f);
                break;
            case ButtonCommands.MoveStageUp:
                MasterController.Instance.SetVerticalOffsetRelative(.25f);
                break;
            case ButtonCommands.MoveStageDown:
                MasterController.Instance.SetVerticalOffsetRelative(-.25f);
                break;
            case ButtonCommands.AutoRotate:
                MasterController.Instance.ToggleAutoRotate();
                break;
            case ButtonCommands.Explode:
                MasterController.Instance.ToggleExplode();
                break;
            case ButtonCommands.ClearCache:
                Caching.ClearCache();
                break;
            case ButtonCommands.Info:
                MasterController.Instance.ToggleModelInfo();
                break;
            case ButtonCommands.LocalMode:
                MasterController.Instance.ToggleLocalMode();
                break;
            case ButtonCommands.Exit:
#if NETFX_CORE
            Windows.ApplicationModel.Core.CoreApplication.Exit();
#else
                UnityEngine.Application.Quit();
#endif
                break;
        }
        RefreshButtons();
    }

    void RefreshButtons()
    {
        if (buttonInstances.Count != 0)
        {
            buttonInstances[ButtonCommands.LocalMode].SetToggle(MasterController.Instance.NetworkStateManager.BypassNetwork);
            buttonInstances[ButtonCommands.Explode].SetToggle(MasterController.Instance.CurrentExplode);
            buttonInstances[ButtonCommands.AutoRotate].SetToggle(MasterController.Instance.CurrentAutoRotate);
            buttonInstances[ButtonCommands.Info].SetToggle(MasterController.Instance.ModelInfoVisible);
        }
    }

    private void OnEnable()
    {
        if (MasterController.Instance.ServerManager.UserInfo.DisplayableId != string.Empty)
        {
            InfoText.text = MasterController.Instance.ServerManager.UserInfo.GivenName + " (" + MasterController.Instance.ServerManager.UserInfo.DisplayableId + ")";
        }
        else
        {
            InfoText.text = "";
        }
        RefreshButtons();
    }
}
