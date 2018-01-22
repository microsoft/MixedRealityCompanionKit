// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using UnityEngine;
using UnityEngine.UI;

public class MenuButton : Clickable
{

    public GameObject Border;
    public Color BorderNormalColor = Color.white;
    public Color BorderOverColor = Color.red;
    public Color BorderEnabledColor = Color.green;
    public GameObject Highlight;
    public Text Icon;
    public Color IconNormalColor = Color.white;
    public Color IconOverColor = Color.red;
    public Color IconEnabledColor = Color.green;
    public GameObject ToolTip = null;
    public Text TooltipLabel = null;

    public MenuButtonInfo ButtonInfo;

    public bool CurrentToggleState;

    void Start()
    {
        SetInfo(ButtonInfo);
    }

    public void SetInfo(MenuButtonInfo buttonInfo)
    {
        ButtonInfo = buttonInfo;
        SetToggle(ButtonInfo.InitialToggleState);
        gameObject.SetActive(HololensHelper.IsDeviceHololens() || buttonInfo.VisibleOnPC);
    }

    public void SetToggle(bool newVal)
    {
        if (ButtonInfo.IsToggleButton)
        {
            CurrentToggleState = newVal;
        }

        updateButton();
    }

    protected override void updateButton()
    {
        base.updateButton();
        Command = ButtonInfo.Command;
        CommandParam = ButtonInfo.CommandParam;
        Icon.text = CurrentToggleState && ButtonInfo.EnabledIcon != "" ? ButtonInfo.EnabledIcon : ButtonInfo.NormalIcon;
        Icon.color  = CurrentToggleState ? IconEnabledColor : IconNormalColor;
        Icon.color = isOver ? IconOverColor : Icon.color;
        if (ToolTip != null)
        {
            ToolTip.SetActive(isOver || ButtonInfo.AlwaysShowToolTip);
            TooltipLabel.text = ButtonInfo.TooltipText;
        }

        if (Border != null)
        {
            Border.GetComponent<Renderer>().material.color = isOver ? BorderOverColor : BorderNormalColor;
            Border.GetComponent<Renderer>().material.color = CurrentToggleState ? BorderEnabledColor : Border.GetComponent<Renderer>().material.color;            
        }

        if (Highlight != null)
        {
            Highlight.SetActive(isOver);
        }
    }
}
