// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Collections.Generic;
using UnityEngine;

public class StageMenu : MonoBehaviour
{
    public List<MenuButtonInfo> MenuButtons;
    public MenuButton MenuButtonPrefab;
    private Dictionary<ButtonCommands, MenuButton> buttonInstances = new Dictionary<ButtonCommands, MenuButton>();

    private float[] rotations = new float[] { 16.5f, 0, -16.5f };

    void Start()
    {
        for (var i = 0; i < MenuButtons.Count; ++i)
        {
            var buttonInfo = MenuButtons[i];

            var newButton = (MenuButton)Instantiate(MenuButtonPrefab);
            newButton.ButtonClicked += Button_ButtonClicked;
            newButton.transform.rotation = Quaternion.Euler(20, rotations[i], 0);
            newButton.transform.Translate(buttonInfo.OffsetPosition);
            newButton.transform.SetParent(transform, false);
            newButton.SetInfo(buttonInfo);
            buttonInstances.Add(buttonInfo.Command, newButton);            
        }
    }

    private void Button_ButtonClicked(Clickable source)
    {
        switch (source.Command)
        {
            case ButtonCommands.ShowTopMenu:
                MasterController.Instance.ShowMenu(MenuManager.MenuType.Top);
                break;
            case ButtonCommands.ShowAssetMenu:
                MasterController.Instance.ShowMenu(MenuManager.MenuType.AssetList);
                break;

            case ButtonCommands.ManipulateStage:
                MasterController.Instance.ToggleManipulationMode();
                break;
        }

        RefreshButtons();
    }

    public void RefreshButtons()
    {
        if (buttonInstances.Count != 0)
        {
            buttonInstances[ButtonCommands.ShowTopMenu].SetToggle(MasterController.Instance.MenuManager.CurrentMenu == MenuManager.MenuType.Top);
            buttonInstances[ButtonCommands.ShowAssetMenu].SetToggle(MasterController.Instance.MenuManager.CurrentMenu == MenuManager.MenuType.AssetList);
            buttonInstances[ButtonCommands.ManipulateStage].SetToggle(MasterController.Instance.StageManipulator.IsEnabled);
        }
    }

    private void OnEnable()
    {
        RefreshButtons();
    }
}
