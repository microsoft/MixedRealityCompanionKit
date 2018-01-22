// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using System;
using UnityEngine;

namespace Showcases.Input
{
#if UNITY_EDITOR
using UnityEditor;

[CustomPropertyDrawer(typeof(JoystickButton))]
public class JoystickButtonDrawer : ButtonDrawer { }
#endif

    [Serializable]
    public class JoystickButton : Button<JoystickStateData.Button> { }

    [Serializable]
    public class JoystickButtonList : DataList<JoystickButton> { }

    public class JoystickStateData : StateData
    {
        public enum Button : int { X, Y, A, B, Select, Start }

        public JoystickButtonList trackedButtons;

        public override bool AnyButtonsDownThisFrame()
        {
            var buttons = this.trackedButtons.items.ToArray();

            return AnyButtonsDownThisFrame<JoystickStateData.Button>(buttons);
        }
    }
}
