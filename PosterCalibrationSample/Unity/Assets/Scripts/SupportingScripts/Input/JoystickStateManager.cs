// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using UnityEngine;
using UnityEngine.EventSystems;

namespace PosterAlignment.InputUtilities
{
    public class JoystickStateManager : StateManager
    {
        public enum Stick { Left, Right }

        public string HorizontalAxis = "Horizontal";
        public string VerticalAxis = "Vertical";
        public string SubmitButton = "Submit";
        public string CancelButton = "Cancel";
        public string LeftButton = "Fire1";
        public string RightButton = "Fire2";
        public string MiddleButton = "Fire3";

        public bool UseKeyboard = true;
        public KeyCode SubmitKey = KeyCode.Return;
        public KeyCode CancelKey = KeyCode.Escape;
        public KeyCode LeftKey = KeyCode.LeftControl;
        public KeyCode MiddleKey = KeyCode.LeftAlt;
        public KeyCode RightKey = KeyCode.LeftWindows;

        [SerializeField]
        private float inputActionsPerSecond = 10;
        public float InputActionsPerSecond
        {
            get { return this.inputActionsPerSecond; }
            set { this.inputActionsPerSecond = value; }
        }

        [SerializeField]
        private float repeatDelay = 0.5f;
        public float RepeatDelay
        {
            get { return this.repeatDelay; }
            set { this.repeatDelay = value; }
        }

        private float prevActionTime;
        private Vector2 lastMoveVector;
        private int consecutiveMoveCount = 0;

        public override Vector2 ScreenPosition { get { return this.screenPosition; } }
        private Vector2 screenPosition;

        public override Vector3 WorldScreenPosition { get { return this.worldScreenPosition; } }
        private Vector3 worldScreenPosition;

        public override Vector2 ScreenDelta { get { return this.screenDelta; } }
        private Vector2 screenDelta;

        public override Vector3 WorldDelta { get { return this.worldDelta; } }
        private Vector3 worldDelta;
        
        private void Update()
        {
            var lastScreenPosition = this.screenPosition;
            var lastWorldScreenPosition = this.worldScreenPosition;
            float screenZ = Camera.main.nearClipPlane;

            var joyOffset = this.GetRawMoveVector(Stick.Left);
            joyOffset.x *= StateManager.ScreenWidth * 0.5f;
            joyOffset.y *= StateManager.ScreenHeight * 0.5f;

            this.screenPosition = StateManager.ScreenCenter + joyOffset;
            this.worldScreenPosition
                = Camera.main.ScreenToWorldPoint(new Vector3(this.screenPosition.x, this.screenPosition.y, screenZ));

            if (UnityEngine.Input.GetButton(this.LeftButton))
            {
                this.screenDelta = this.screenPosition - lastScreenPosition;
                this.worldDelta = this.worldScreenPosition - lastWorldScreenPosition;
            }
            else
            {
                this.screenDelta = Vector2.zero;
                this.worldDelta = Vector3.zero;
            }
        }

        public override bool IsSupported()
        {
            return (UnityEngine.Input.GetJoystickNames().Length > 0 || this.UseKeyboard);
        }

        public override bool ShouldActivate()
        {
            bool shouldActivate = false;

            shouldActivate |= UnityEngine.Input.GetButtonDown(this.SubmitButton);
            shouldActivate |= this.UseKeyboard && UnityEngine.Input.GetKeyDown(this.SubmitKey);
            shouldActivate |= UnityEngine.Input.GetButtonDown(this.CancelButton);
            shouldActivate |= this.UseKeyboard && UnityEngine.Input.GetKeyDown(this.CancelKey);
            shouldActivate |= !Mathf.Approximately(UnityEngine.Input.GetAxisRaw(this.HorizontalAxis), 0.0f);
            shouldActivate |= !Mathf.Approximately(UnityEngine.Input.GetAxisRaw(this.VerticalAxis), 0.0f);

            return shouldActivate;
        }

        public override bool ShouldSubmit()
        {
            return (UnityEngine.Input.GetButtonDown(this.SubmitButton) || (this.UseKeyboard && UnityEngine.Input.GetKeyDown(this.SubmitKey)));
        }

        public override bool ShouldCancel()
        {
            return (UnityEngine.Input.GetButtonDown(this.CancelButton) || (this.UseKeyboard && UnityEngine.Input.GetKeyDown(this.CancelKey)));
        }

        // Process joystick/keyboard events.
        public override bool ShouldMove(out Vector2 movement)
        {
            float time = Time.unscaledTime;

            movement = this.GetRawMoveVector(Stick.Left);
            if (Mathf.Approximately(movement.x, 0.0f)
                && Mathf.Approximately(movement.y, 0.0f))
            {
                this.consecutiveMoveCount = 0;

                return false;
            }

            // If user pressed key again, always allow event
            bool allow = UnityEngine.Input.GetButtonDown(this.HorizontalAxis) || UnityEngine.Input.GetButtonDown(this.VerticalAxis);
            bool similarDir = (Vector3.Dot(movement, this.lastMoveVector) > 0.0f);
            if (!allow)
            {
                // Otherwise, user held down key or axis.
                // If direction didn't change at least 90 degrees, wait for delay before allowing consequtive event.
                if (similarDir && this.consecutiveMoveCount == 1)
                {
                    allow = (time > this.prevActionTime + this.repeatDelay);
                }
                // If direction changed at least 90 degree, or we already had the delay, repeat at repeat rate.
                else
                {
                    allow = (time > this.prevActionTime + 1.0f / this.inputActionsPerSecond);
                }
            }

            if (!allow)
            {
                return false;
            }

            // move is satisfied, update and return
            if (!similarDir)
            {
                this.consecutiveMoveCount = 0;
            }
            this.consecutiveMoveCount++;
            this.prevActionTime = time;
            this.lastMoveVector = movement;

            return true;
        }

        public override bool IsPressed(PointerEventData.InputButton button)
        {
            bool isPressed = false;

            switch (button)
            {
                case PointerEventData.InputButton.Left:
                    isPressed = (UseKeyboard && UnityEngine.Input.GetKeyDown(this.LeftKey)) || UnityEngine.Input.GetButtonDown(this.LeftButton);
                    break;
                case PointerEventData.InputButton.Middle:
                    isPressed = (UseKeyboard && UnityEngine.Input.GetKeyDown(this.MiddleKey)) || UnityEngine.Input.GetButtonDown(this.MiddleButton);
                    break;
                case PointerEventData.InputButton.Right:
                    isPressed = (UseKeyboard && UnityEngine.Input.GetKeyDown(this.RightKey)) || UnityEngine.Input.GetButtonDown(this.RightButton);
                    break;
            }

            return isPressed; ;
        }

        public override void ResetButtonState(UnityEngine.EventSystems.PointerEventData.InputButton button)
        {

        }

        private Vector2 GetRawMoveVector(Stick whichStick)
        {
            Vector2 move = GetJoystickOffset(whichStick);

            if (UnityEngine.Input.GetButtonDown(this.HorizontalAxis))
            {
                if (move.x < 0.0f)
                {
                    move.x = -1.0f;
                }
                if (move.x > 0.0f)
                {
                    move.x = 1.0f;
                }
            }

            if (UnityEngine.Input.GetButtonDown(this.VerticalAxis))
            {
                if (move.y < 0.0f)
                {
                    move.y = -1.0f;
                }
                if (move.y > 0.0f)
                {
                    move.y = 1.0f;
                }
            }

            return move;
        }

        private Vector2 GetJoystickOffset(Stick whichStick)
        {
            Vector2 move = Vector2.zero;
            if (whichStick == Stick.Left)
            {
                move.x = UnityEngine.Input.GetAxisRaw(this.HorizontalAxis);
                move.y = UnityEngine.Input.GetAxisRaw(this.VerticalAxis);
            }
            else
            {
            }

            return move;
        }
    }
}
