// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using UnityEngine;

namespace PosterAlignment.InputUtilities
{
    public class MouseStateManager : StateManager
    {
        public override Vector2 ScreenPosition { get { return this.screenPosition; } }
        private Vector2 screenPosition = Vector2.zero;

        public override Vector3 WorldScreenPosition { get { return this.worldScreenPosition; } }
        private Vector3 worldScreenPosition = Vector3.zero;

        public override Vector2 ScreenDelta { get { return this.screenDelta; } }
        private Vector2 screenDelta = Vector2.zero;

        public override Vector3 WorldDelta { get { return this.worldDelta; } }
        private Vector3 worldDelta = Vector3.zero;

        public override Vector2 ScrollDelta { get { return UnityEngine.Input.mouseScrollDelta; } }

        public string LeftButton = "Fire1";

        public override bool IsSupported()
        {
            return UnityEngine.Input.mousePresent;
        }

        public override bool ShouldActivate()
        {
            bool shouldActivate = false;

            shouldActivate |= this.screenDelta.sqrMagnitude > 0.0f;
            shouldActivate |= UnityEngine.Input.GetMouseButtonDown(0);
            shouldActivate |= UnityEngine.Input.GetMouseButtonDown(1);
            shouldActivate |= UnityEngine.Input.GetMouseButtonDown(2);

            return shouldActivate;
        }

        public override bool IsPressed(UnityEngine.EventSystems.PointerEventData.InputButton button)
        {
            return UnityEngine.Input.GetMouseButtonDown((int)button);
        }

        public override bool IsReleased(UnityEngine.EventSystems.PointerEventData.InputButton button)
        {
            return UnityEngine.Input.GetMouseButtonUp((int)button);
        }

        public override void ResetButtonState(UnityEngine.EventSystems.PointerEventData.InputButton button)
        {

        }

        public override void ActivateModule()
        {
            this.screenPosition
                = UnityEngine.Input.mousePosition;
            this.worldScreenPosition.x = this.ScreenPosition.x;
            this.worldScreenPosition.y = this.ScreenPosition.y;
            this.worldScreenPosition.z = Camera.main.nearClipPlane;
            this.worldScreenPosition
                = Camera.main.ScreenToWorldPoint(this.worldScreenPosition);
        }

        public override void UpdateModule()
        {
            float screenZ = Camera.main.nearClipPlane;

            var lastPosition = this.ScreenPosition;
            var worldLastPosition = this.WorldScreenPosition;

            this.screenPosition 
                = UnityEngine.Input.mousePosition;
            this.worldScreenPosition.x = this.ScreenPosition.x;
            this.worldScreenPosition.y = this.ScreenPosition.y;
            this.worldScreenPosition.z = Camera.main.nearClipPlane;
            this.worldScreenPosition
                = Camera.main.ScreenToWorldPoint(this.worldScreenPosition);

            if (UnityEngine.Input.GetButton(this.LeftButton))
            {
                this.screenDelta
                    = this.ScreenPosition - lastPosition;

                this.worldDelta
                    = this.WorldScreenPosition - worldLastPosition;
            }
            else
            {
                this.screenDelta = Vector2.zero;
                this.worldDelta = Vector3.zero;
            }
        }
    }
}
