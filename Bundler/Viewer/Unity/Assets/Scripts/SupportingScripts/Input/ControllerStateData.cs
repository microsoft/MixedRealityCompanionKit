// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using System;
using UnityEngine;

namespace Showcases.Input
{
#if UNITY_EDITOR
    using UnityEditor;

    [CustomPropertyDrawer(typeof(ControllerButton))]
    public class ControllerButtonDrawer : ButtonDrawer { }
#endif

    [Serializable]
    public class ControllerButton : Button<InteractionSourceData.Button> { }

    [Serializable]
    public class ControllerButtonList : DataList<ControllerButton> { }

    public class ControllerStateData : InteractionSourceData
    {
        public override RaycastOrigin raycastOrigin { get { return RaycastOrigin.world; } }

        public override SourceKind kind { get { return SourceKind.Controller; } }

        public ControllerButtonList trackedButtons;

        public override bool AnyButtonsDownThisFrame()
        {
            var buttons = this.trackedButtons.items.ToArray();

            return AnyButtonsDownThisFrame<ControllerStateData.Button>(buttons);
        }

        public override bool UpdateState()
        {
            SetStateData();
            
            // calculate screen position/delta wrt Main camera
            var eventCamera = InputStateManager.Instance.eventCamera;
            if (eventCamera == null)
                return false;

            // move camera to current position
            eventCamera.transform.position = this.WorldPosition;
            eventCamera.transform.rotation = this.Rotation;

            this.ScreenPoint = InputStateManager.Instance.eventCameraScreenCenterPoint;

            // set screen space delta
            var lastPosition = GetPosition(this.activeSelectionHand, this.activeSourceNode, true);
            var lastRotation = GetRotation(this.activeSelectionHand, this.activeSourceNode, true);
            if ((lastPosition != null && lastPosition.HasValue) && (lastRotation != null && lastRotation != null))
            {
                // project wrt gaze
                var screenPoint = InputStateManager.Instance.gazeCamera.WorldToScreenPoint(this.WorldPosition + (this.Rotation * Vector3.forward));
                var lastScreenPoint = InputStateManager.Instance.gazeCamera.WorldToScreenPoint(lastPosition.Value + (lastRotation.Value * Vector3.forward));

                // calc the delta in screen space
                this.ScreenPointDelta = screenPoint - lastScreenPoint;
            }

            foreach (var button in this.trackedButtons.items)
            {
                SetButtonStateData(button);
            }

            return true;
        }
    }
}
