// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using System;
using System.Collections.Generic;
using UnityEngine;

namespace Showcases.Input
{
    #if UNITY_EDITOR
    using UnityEditor;

    [CustomPropertyDrawer(typeof(MouseButton))]
    public class MouseButtonDrawer : ButtonDrawer { }
    #endif

    [Serializable]
    public class MouseButton : Button<MouseStateData.Button> { }

    [Serializable]
    public class MouseButtonList : DataList<MouseButton> { }

    public class MouseStateData : StateData
    {
        public override RaycastOrigin raycastOrigin { get { return RaycastOrigin.screen; } }

        public enum Button : int { Left = 0, Middle = 2, Right = 1, }

        public Button selectionButton = Button.Left;

        public MouseButtonList trackedButtons;

        private void Update()
        {
            var position = UnityEngine.Input.mousePosition;
            var viewportPosition = new Vector3(position.x / Screen.width, position.y / Screen.height, Camera.main.nearClipPlane);

            var lastPoint = this.ScreenPoint;
            var lastPosition = this.WorldPosition;

            this.ScreenPoint = Camera.main.ViewportToScreenPoint(viewportPosition);
            this.WorldPosition = Camera.main.ViewportToWorldPoint(viewportPosition);

            this.ScreenPointDelta = this.ScreenPoint - lastPoint;
            this.WorldPositionDelta = this.WorldPosition - lastPosition;

            if (this.trackedButtons == null)
                return;

            foreach (var button in this.trackedButtons.items)
            {
                button.state.pressedThisFrame = UnityEngine.Input.GetMouseButtonDown((int)button.type);
                button.state.releasedThisFrame = UnityEngine.Input.GetMouseButtonUp((int)button.type);
                button.state.pressed = UnityEngine.Input.GetMouseButton((int)button.type);

                // set pressed and released state for this input
                if (button.type == selectionButton)
                {
                    this.PressedThisFrame = button.state.pressedThisFrame;
                    this.ReleasedThisFrame = button.state.releasedThisFrame;
                }
            }
        }

        public override bool UpdateState()
        {
            return UnityEngine.Input.mousePresent;
        }

        public override bool AnyButtonsDownThisFrame()
        {
            var buttons = this.trackedButtons.items.ToArray();

            return AnyButtonsDownThisFrame<MouseStateData.Button>(buttons);
        }
    }
}
