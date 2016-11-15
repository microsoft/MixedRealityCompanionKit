// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using System;
using UnityEngine;
using UnityEngine.EventSystems;

namespace PosterAlignment.InputUtilities
{
    public class StateManager : MonoBehaviour
    {
        public enum InputType { Gesture, Mouse, Joystick }

        public static float ScreenWidth { get; private set; }
        public static float ScreenHeight { get; private set; }
        public static float OneOverWidth { get; private set; }
        public static float OneOverHeight { get; private set; }

        public static Vector2 ScreenCenter { get { return screenCenter; } }
        private static Vector2 screenCenter = Vector2.zero;

        public static Vector3 WorldScreenCenter { get { return worldScreenCenter; } }
        private static Vector3 worldScreenCenter = Vector3.zero;

        public static Vector3 GazePoint { get; private set; }
        public static Vector3 GazeDelta { get; private set; }

        internal static void CheckForScreenUpdates()
        {
            bool changed = false;

            if ((float)Screen.width != ScreenWidth)
            {
                ScreenWidth = (float)Screen.width;
                OneOverWidth = 1.0f / ScreenWidth;
                changed = true;
            }

            if ((float)Screen.height != ScreenHeight)
            {
                ScreenHeight = (float)Screen.height;
                OneOverHeight = 1.0f / ScreenHeight;
                changed = true;
            }

            if (changed)
            {
                screenCenter.x = ScreenWidth * 0.5f;
                screenCenter.y = ScreenHeight * 0.5f;
            }

            // camera is always moving
            worldScreenCenter.x = ScreenCenter.x;
            worldScreenCenter.y = ScreenCenter.y;
            worldScreenCenter.z = Camera.main.nearClipPlane;
            worldScreenCenter = Camera.main.ScreenToWorldPoint(worldScreenCenter);

            var lastGazePosition = GazePoint;
            GazePoint = Camera.main.transform.position + Camera.main.transform.forward;
            GazeDelta = GazePoint - lastGazePosition;
        }


        public virtual Vector2 ScreenPosition { get { return Vector2.zero; } }
        public virtual Vector2 ScreenDelta { get { return Vector2.zero; } }

        public virtual Vector3 WorldScreenPosition { get { return Vector3.zero; } }
        public virtual Vector3 WorldDelta { get { return Vector3.zero; } }

        public virtual Vector2 ScrollDelta { get { return Vector2.zero; } }


        public virtual bool IsSupported() { return false; }

        public virtual bool ShouldActivate() { return false; }

        public virtual bool ShouldSubmit() { return false; }

        public virtual bool ShouldCancel() { return false; }

        public virtual bool ShouldMove(out Vector2 movement)
        {
            movement = Vector2.zero;
            return false;
        }

        public virtual bool IsPressed(PointerEventData.InputButton button)
        {
            return false;
        }

        public virtual bool IsReleased(PointerEventData.InputButton button)
        {
            return false;
        }

        public virtual void ResetButtonState(PointerEventData.InputButton button)
        {
        }

        // called from input manager
        public virtual void ActivateModule()
        {
        }

        public virtual void DeactivateModule()
        {
        }

        public virtual void UpdateModule()
        {
        }
    }
}