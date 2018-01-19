// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MenuManager : MonoBehaviour
{
    public MenuBase TopMenu;
    public MenuBase AssetMenu;
    public GameObject MenuContainer;
    public MenuType CurrentMenu;
    public enum MenuType
    {
        None,
        Top,
        AssetList,
    }

    void Start()
    {
        ShowMenu(MenuType.None);
        TopMenu.ToggleMenu(false);
        AssetMenu.ToggleMenu(false);
    }

    public void ShowMenu(MenuType menu)
    {
        if (CurrentMenu == menu)
        {
            menu = MenuType.None;
        }

        CurrentMenu = menu;
        StartCoroutine(ShowMenuAsync(menu));
    }

    private IEnumerator ShowMenuAsync(MenuType menu)
    {
        if (menu != MenuType.None)
        {
            yield return new WaitForEndOfFrame();
        }

        switch (menu)
        {
            case MenuType.None:
                {
                    TopMenu.ToggleMenu(false);
                    AssetMenu.ToggleMenu(false);
                    yield return new WaitForSeconds(2);
                }
                break;
            case MenuType.Top:
                {
                    AssetMenu.ToggleMenu(false);
                    TopMenu.ToggleMenu(true);
                }
                break;
            case MenuType.AssetList:
                {
                    TopMenu.ToggleMenu(false);
                    AssetMenu.ToggleMenu(true);
                }
                break;
        }
    }
}
