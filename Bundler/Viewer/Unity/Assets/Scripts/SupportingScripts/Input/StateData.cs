// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using System;
using UnityEngine;
using UnityEngine.EventSystems;

namespace Showcases.Input
{
    public abstract class StateData : MonoBehaviour
    {
        public enum RaycastOrigin { gaze, screen, world }

        public int id;

        public float MoveAmountThreshold = 0.0f;

        public RaycastResult RaycastResult = new RaycastResult();

        public virtual RaycastOrigin raycastOrigin { get { return RaycastOrigin.gaze; } }

        public virtual Vector2 ScreenPoint { get; protected set; }

        public virtual Vector2 ScreenPointDelta { get; protected set; }

        public virtual Vector2 ScrollDelta { get { return Vector2.zero; } }

        public virtual Vector2 MoveDelta { get { return Vector2.zero; } }

        public virtual Vector3 WorldPosition { get; protected set; }

        public virtual Vector3 WorldPositionDelta { get; protected set; }

        public virtual Quaternion Rotation { get; protected set; }

        public virtual Quaternion RotationDelta { get; protected set; }

        public virtual bool PressedThisFrame { get; protected set; }

        public virtual bool ReleasedThisFrame { get; protected set; }

        public virtual bool Canceled { get; protected set; }

        public virtual bool ShouldActivate()
        {
            var shouldActivate = false;

            shouldActivate |= ShouldSubmit();
            shouldActivate |= ShouldMove();
            shouldActivate |= ScreenPointDelta.sqrMagnitude > MoveAmountThreshold;
            shouldActivate |= AnyButtonsDownThisFrame();

            return shouldActivate;
        }

        public virtual bool ShouldSubmit() { return false; }

        public virtual bool ShouldMove() { return !Mathf.Approximately(this.MoveDelta.sqrMagnitude, 0.0f); }

        public virtual bool ShouldScroll() { return !Mathf.Approximately(this.ScrollDelta.sqrMagnitude, 0.0f); }

        public virtual bool AnyButtonsDownThisFrame() { return false; }

        public virtual bool UpdateState()
        {
            return false;
        }

        protected static ButtonState GetButtonState<T>(T type, Button<T>[] trackedButtons) where T : struct
        {
            ButtonState buttonState = null;

            for (int i = 0; i < trackedButtons.Length; ++i)
            {
                if (type.ToString() == trackedButtons[i].type.ToString())
                {
                    buttonState = trackedButtons[i].state;
                    break;
                }
            }

            return buttonState;
        }

        protected static bool AnyButtonsDownThisFrame<T>(Button<T>[] trackedButtons) where T : struct
        {
            for (int i = 0; i < trackedButtons.Length; ++i)
            {
                if (trackedButtons[i].state.pressed)
                    return true;
            }
            return false;
        }

        protected static bool AnyButtonsPressedThisFrame<T>(Button<T>[] trackedButtons) where T : struct
        {
            for (int i = 0; i < trackedButtons.Length; ++i)
            {
                if (trackedButtons[i].state.pressedThisFrame)
                    return true;
            }
            return false;
        }

        protected static bool AnyButtonsReleasedThisFrame<T>(Button<T>[] trackedButtons) where T : struct
        {
            for (int i = 0; i < trackedButtons.Length; ++i)
            {
                if (trackedButtons[i].state.releasedThisFrame)
                    return true;
            }
            return false;
        }

    }
}
