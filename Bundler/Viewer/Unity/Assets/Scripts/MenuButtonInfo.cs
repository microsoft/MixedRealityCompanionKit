// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using UnityEngine;

[Serializable]
public class MenuButtonInfo
{
    public ButtonCommands Command;
    public string CommandParam;
    public string NormalIcon = "-";
    public string EnabledIcon = "+";
    public string TooltipText = "ToolTip";
    public bool AlwaysShowToolTip = true;
    public bool IsToggleButton;
    public bool InitialToggleState;
    public Vector3 OffsetPosition;
    public bool VisibleOnPC = true;
}