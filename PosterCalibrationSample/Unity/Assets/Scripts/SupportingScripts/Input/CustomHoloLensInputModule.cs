// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

namespace Showcases.Input
{
    [RequireComponent(typeof(MouseStateManager))]
    [RequireComponent(typeof(JoystickStateManager))]
    [RequireComponent(typeof(GestureStateManager))]
    [AddComponentMenu("Event/Custom HoloLens Input Module")]
    public class CustomHoloLensInputModule : PointerInputModule
    {
        public StateManager.InputType InputType = StateManager.InputType.Gesture;

        public bool CopyToMiddleButton = false;
        public bool CopyToRightButton = false;
        

        private readonly static MouseState inputState = new MouseState();

        private MouseStateManager mouseManager;
        private JoystickStateManager joystickManager;
        private GestureStateManager gestureManager;

        private static Dictionary<int, WorldPointerEventData> worldPointerEventData
            = new Dictionary<int, WorldPointerEventData>();

        private static RaycastResult emptyResult;

        public static RaycastResult GetRaycastResult(int buttonId = -1)
        {
            WorldPointerEventData data = null;
            if (worldPointerEventData.TryGetValue(buttonId, out data))
            {
                return data.pointerCurrentRaycast;
            }

            return emptyResult;
        }

        public static MouseButtonEventData GetInputButtonState(PointerEventData.InputButton button = PointerEventData.InputButton.Left)
        {
            return inputState.GetButtonState(button).eventData;
        }

        private bool forceModuleActive;
        public bool ForceModuleActive
        {
            get { return this.forceModuleActive; }
            set { this.forceModuleActive = value; }
        }

        protected override void Awake()
        {
            base.Awake();

            emptyResult = new RaycastResult();

            if (null == Camera.main)
            {
                return;
            }

            // main camera needs a 3d raycaster
            var raycaster = Camera.main.gameObject.GetComponentInParent<PhysicsRaycaster>();
            if (null == raycaster)
            {
                Camera.main.gameObject.AddComponent<PhysicsRaycaster>();

                // remove Ignore Raycast layer
                raycaster = Camera.main.gameObject.GetComponent<PhysicsRaycaster>();
                if (null != raycaster)
                {
                    raycaster.eventMask = raycaster.eventMask & ~(1 << LayerMask.NameToLayer("Ignore Raycast"));
                }
            }
        }

        public override bool IsModuleSupported()
        {
            var supported = this.forceModuleActive
                || this.gestureManager.IsSupported()
                || this.mouseManager.IsSupported()
                || this.joystickManager.IsSupported();

            return supported;
        }

        public override bool ShouldActivateModule()
        {
            if (!base.ShouldActivateModule())
            {
                return false;
            }

            var shouldActivate = this.forceModuleActive;
            shouldActivate |= this.gestureManager.ShouldActivate();
            shouldActivate |= this.mouseManager.ShouldActivate();
            shouldActivate |= this.joystickManager.ShouldActivate();

            return shouldActivate;
        }

        public override void ActivateModule()
        {
            base.ActivateModule();

            var toSelect = this.eventSystem.currentSelectedGameObject;
            if (toSelect == null)
            {
                toSelect = this.eventSystem.firstSelectedGameObject;
            }
            this.eventSystem.SetSelectedGameObject(toSelect, this.GetBaseEventData());

            if (!this.EnsureModules())
            {
                return;
            }

            this.gestureManager.ActivateModule();
            this.mouseManager.ActivateModule();
            this.joystickManager.ActivateModule();
        }

        public override void DeactivateModule()
        {
            base.DeactivateModule();

            // clear selection
            var baseEventData = GetBaseEventData();
            foreach (var pointer in worldPointerEventData.Values)
            {
                HandlePointerExitAndEnter(pointer, null);
            }
            worldPointerEventData.Clear();
            eventSystem.SetSelectedGameObject(null, baseEventData);

            if (!this.EnsureModules())
            {
                return;
            }

            this.gestureManager.DeactivateModule();
            this.mouseManager.DeactivateModule();
            this.joystickManager.DeactivateModule();
        }

        public override void UpdateModule()
        {
            StateManager.CheckForScreenUpdates();

            if (!this.EnsureModules())
            {
                return;
            }

            this.gestureManager.UpdateModule();
            this.mouseManager.UpdateModule();
            this.joystickManager.UpdateModule();
        }

        public override void Process()
        {
            bool usedEvent = this.SendUpdateEventToSelectedObject();

            if (this.eventSystem.sendNavigationEvents)
            {
                if (!usedEvent)
                {
                    usedEvent |= this.SendMoveEvent();
                }

                if (!usedEvent)
                {
                    this.SendSubmitCancelEvent();
                }
            }

            this.ProcessInputEventData();
        }

        public override string ToString()
        {
            var sb = new System.Text.StringBuilder("<b>Pointer Input Module of type: </b>" + GetType());
            sb.AppendLine();
            foreach (var pointer in worldPointerEventData)
            {
                if (pointer.Value == null)
                    continue;
                sb.AppendLine("<B>Pointer:</b> " + pointer.Key);
                sb.AppendLine(pointer.Value.ToString());
            }
            return sb.ToString();
        }

        // todo: refactor to be a generic list
        private bool EnsureModules()
        {
            if (this.mouseManager == null)
            {
                this.mouseManager = this.gameObject.GetComponent<MouseStateManager>();
            }

            if (this.joystickManager == null)
            {
                this.joystickManager = this.gameObject.GetComponent<JoystickStateManager>();
            }

            if (this.gestureManager == null)
            {
                this.gestureManager = this.gameObject.GetComponent<GestureStateManager>();
            }

            return (this.mouseManager != null && this.joystickManager != null && this.gestureManager != null);
        }


        // Send update to selected object
        private bool SendUpdateEventToSelectedObject()
        {
            if (this.eventSystem.currentSelectedGameObject == null)
            {
                return false;
            }

            var data = this.GetBaseEventData();

            ExecuteEvents.Execute(this.eventSystem.currentSelectedGameObject, data, ExecuteEvents.updateSelectedHandler);

            return data.used;
        }

        // sends submit/cancel action
        private bool SendSubmitCancelEvent()
        {
            // eventSystem must have a selected object
            if (this.eventSystem.currentSelectedGameObject == null)
            {
                return false;
            }

            var data = this.GetBaseEventData();

            // Submit, only send once
            bool doSubmit = this.joystickManager.ShouldSubmit();
            if (!doSubmit)
            {
                doSubmit |= this.mouseManager.ShouldSubmit(); // should be false, but may want to support multi-button type mouse
            }
            if (!doSubmit)
            {
                doSubmit |= this.gestureManager.ShouldSubmit(); // could be true based on voice
            }

            if (doSubmit)
            {
                ExecuteEvents.Execute(this.eventSystem.currentSelectedGameObject, data, ExecuteEvents.submitHandler);
            }

            // Cancel, only send once
            bool doCancel = this.joystickManager.ShouldCancel();
            if (!doCancel)
            {
                doCancel |= this.mouseManager.ShouldCancel();
            }
            if (!doCancel)
            {
                doCancel |= this.gestureManager.ShouldCancel();
            }

            if (doCancel)
            {
                ExecuteEvents.Execute(this.eventSystem.currentSelectedGameObject, data, ExecuteEvents.cancelHandler);
            }

            return data.used;
        }

        // sends navigation event within a canvas
        private bool SendMoveEvent()
        {
            // eventSystem must have a selected object
            if (this.eventSystem.currentSelectedGameObject == null)
            {
                return false;
            }

            Vector2 movement = Vector2.zero;

            bool doMove = this.joystickManager.ShouldMove(out movement);
            if (!doMove)
            {
                doMove |= this.mouseManager.ShouldMove(out movement);
            }
            if (!doMove)
            {
                doMove |= this.gestureManager.ShouldMove(out movement);
            }

            // convert event data to axisEventData
            // movement shold be normalized between -1.0f to 1.0f
            var axisEventData = this.GetAxisEventData(movement.x, movement.y, 0.6f);

            if (doMove)
            {
                ExecuteEvents.Execute(this.eventSystem.currentSelectedGameObject, axisEventData, ExecuteEvents.moveHandler);
            }

            return axisEventData.used;
        }

        // Process the pointer event data
        private void ProcessInputEventData()
        {
            GetPointerEventData();

            var buttonEventData = GetInputButtonState(PointerEventData.InputButton.Left);

            // Process primary input
            this.ProcessPress(buttonEventData.buttonData, buttonEventData.PressedThisFrame(), buttonEventData.ReleasedThisFrame());
            this.ProcessMove(buttonEventData.buttonData);
            this.ProcessDrag(buttonEventData.buttonData);

            // Now process right / middle input
            buttonEventData = GetInputButtonState(PointerEventData.InputButton.Right);
            this.ProcessPress(buttonEventData.buttonData, buttonEventData.PressedThisFrame(), buttonEventData.ReleasedThisFrame());
            this.ProcessDrag(buttonEventData.buttonData);

            buttonEventData = GetInputButtonState(PointerEventData.InputButton.Middle);
            this.ProcessPress(buttonEventData.buttonData, buttonEventData.PressedThisFrame(), buttonEventData.ReleasedThisFrame());
            this.ProcessDrag(buttonEventData.buttonData);

            // process scroll event
            buttonEventData = GetInputButtonState(PointerEventData.InputButton.Left);
            if (!Mathf.Approximately(buttonEventData.buttonData.scrollDelta.sqrMagnitude, 0.0f))
            {
                var scrollHandler = ExecuteEvents.GetEventHandler<IScrollHandler>(buttonEventData.buttonData.pointerCurrentRaycast.gameObject);
                ExecuteEvents.ExecuteHierarchy(scrollHandler, buttonEventData.buttonData, ExecuteEvents.scrollHandler);
            }
        }

        // Process the press
        private void ProcessPress(PointerEventData pointerEvent, bool pressedThisFrame, bool releasedThisFrame)
        {
            var currentOverGo = pointerEvent.pointerCurrentRaycast.gameObject;

            // PointerDown/PointerClick event
            if (pressedThisFrame)
            {
                pointerEvent.eligibleForClick = true;
                pointerEvent.delta = Vector2.zero;
                pointerEvent.dragging = false;
                pointerEvent.useDragThreshold = true;
                pointerEvent.pressPosition = pointerEvent.position;
                pointerEvent.pointerPressRaycast = pointerEvent.pointerCurrentRaycast;

                this.DeselectIfSelectionChanged(currentOverGo, pointerEvent);

                // search for the control that will receive the press
                var newPressed = ExecuteEvents.ExecuteHierarchy(currentOverGo, pointerEvent, ExecuteEvents.pointerDownHandler);

                // didnt find a press handler... search for a click handler
                if (newPressed == null)
                {
                    newPressed = ExecuteEvents.GetEventHandler<IPointerClickHandler>(currentOverGo);
                }

                float time = Time.unscaledTime;

                if (newPressed == pointerEvent.lastPress)
                {
                    var diffTime = time - pointerEvent.clickTime;
                    if (diffTime < 0.3f)
                    {
                        ++pointerEvent.clickCount;
                    }
                    else
                    {
                        pointerEvent.clickCount = 1;
                    }

                    pointerEvent.clickTime = time;
                }
                else
                {
                    pointerEvent.clickCount = 1;
                }

                pointerEvent.pointerPress = newPressed;
                pointerEvent.rawPointerPress = currentOverGo;

                pointerEvent.clickTime = time;

                // Save the drag handler as well
                pointerEvent.pointerDrag = ExecuteEvents.GetEventHandler<IDragHandler>(currentOverGo);

                if (pointerEvent.pointerDrag != null)
                {
                    ExecuteEvents.Execute(pointerEvent.pointerDrag, pointerEvent, ExecuteEvents.initializePotentialDrag);
                }
            }

            // PointerUp notification
            if (releasedThisFrame)
            {
                ExecuteEvents.Execute(pointerEvent.pointerPress, pointerEvent, ExecuteEvents.pointerUpHandler);

                // see if we released on the same element that was clicked before
                var pointerUpHandler = ExecuteEvents.GetEventHandler<IPointerClickHandler>(currentOverGo);

                // PointerClick and Drop events
                if (pointerEvent.pointerPress == pointerUpHandler && pointerEvent.eligibleForClick)
                {
                    ExecuteEvents.Execute(pointerEvent.pointerPress, pointerEvent, ExecuteEvents.pointerClickHandler);
                }
                else if (pointerEvent.pointerDrag != null && pointerEvent.dragging)
                {
                    ExecuteEvents.ExecuteHierarchy(currentOverGo, pointerEvent, ExecuteEvents.dropHandler);
                }

                // reset pressed state
                pointerEvent.eligibleForClick = false;
                pointerEvent.pointerPress = null;
                pointerEvent.rawPointerPress = null;

                // send endDrag event
                if (pointerEvent.pointerDrag != null && pointerEvent.dragging)
                {
                    ExecuteEvents.Execute(pointerEvent.pointerDrag, pointerEvent, ExecuteEvents.endDragHandler);
                }

                // reset dragging state
                pointerEvent.dragging = false;
                pointerEvent.pointerDrag = null;

                // if the current object is not the previous entered object, then set that now
                if (currentOverGo != pointerEvent.pointerEnter)
                {
                    this.HandlePointerExitAndEnter(pointerEvent, null);
                    this.HandlePointerExitAndEnter(pointerEvent, currentOverGo);
                }
            }
        }

        private void GetPointerEventData()
        {
            // get eventData for left button first
            WorldPointerEventData leftData;

            bool created = this.GetWorldPointerData(kMouseLeftId, out leftData, true);
            if(!created)
            {
                // first time through
            }
            leftData.Reset();

            // set the event data for screenPosition
            SetPositionEventData(ref leftData);

            // set the raycast result
            SetRaycastResult(ref leftData);

            // set scrollDelta event data
            SetScrollDeltaEventData(ref leftData);

            // update the delta and scroll for the appropriate input for drag events
            leftData.button = PointerEventData.InputButton.Left;
            SetDeltaEventData(ref leftData);

            // copy the apropriate data into right and middle slots
            WorldPointerEventData rightData;
            this.GetWorldPointerData(kMouseRightId, out rightData, true);
            if (CopyToRightButton)
            {
                CopyFromTo(leftData, rightData);
            }
            rightData.button = PointerEventData.InputButton.Right;
            //SetDeltaEventData(ref rightData);

            WorldPointerEventData middleData;
            this.GetWorldPointerData(kMouseMiddleId, out middleData, true);
            if (CopyToMiddleButton)
            {
                CopyFromTo(leftData, middleData);
            }
            middleData.button = PointerEventData.InputButton.Middle;
            //SetDeltaEventData(ref middleData);

            inputState.SetButtonState(PointerEventData.InputButton.Left, this.GetStateforButton(leftData.button), leftData);
            inputState.SetButtonState(PointerEventData.InputButton.Right, this.GetStateforButton(rightData.button), rightData);
            inputState.SetButtonState(PointerEventData.InputButton.Middle, this.GetStateforButton(middleData.button), middleData);
        }

        private bool GetWorldPointerData(int buttonId, out WorldPointerEventData data, bool create)
        {
            if (!worldPointerEventData.TryGetValue(buttonId, out data) && create)
            {
                data = new WorldPointerEventData(eventSystem)
                {
                    pointerId = buttonId,
                };
                worldPointerEventData.Add(buttonId, data);

                return true;
            }
            return false;
        }

        private void SetPositionEventData(ref WorldPointerEventData eventData)
        {
            // get position for raycast
            Vector2 pointerPos = this.gestureManager.ScreenPosition;
            Vector3 worldPos = this.gestureManager.WorldScreenPosition;

            switch (this.InputType)
            {
                case StateManager.InputType.Mouse:
                    if(this.mouseManager.IsSupported() || this.mouseManager.ShouldActivate())
                    {
                        pointerPos = this.mouseManager.ScreenPosition;
                        worldPos = this.mouseManager.WorldScreenPosition;
                    }
                    break;
                case StateManager.InputType.Joystick:
                    if (this.joystickManager.IsSupported() || this.joystickManager.ShouldActivate())
                    {
                        pointerPos = this.joystickManager.ScreenPosition;
                        worldPos = this.joystickManager.WorldScreenPosition;
                    }
                    break;
            }

            // set the event position
            eventData.inputType = this.InputType;
            eventData.position = pointerPos;    // screenPosition
            eventData.worldPosition = worldPos; // position on near plane
        }

        private void SetRaycastResult(ref WorldPointerEventData eventData)
        {
            // raycast the position for any results
            this.eventSystem.RaycastAll(eventData, this.m_RaycastResultCache);

            // use first object in cache
            var raycast = FindFirstRaycast(this.m_RaycastResultCache);

            // clear the cache
            this.m_RaycastResultCache.Clear();

            // correct raycast result
            if (raycast.isValid)
            {
                // for UI objects raycast.worldPosition == Vector3.zero, have to raycast the screenPosition
                if (raycast.worldPosition == Vector3.zero)
                {
                    Ray ray = Camera.main.ScreenPointToRay(raycast.screenPosition);
                    raycast.worldPosition = eventData.worldPosition + (ray.direction * raycast.distance);
                }

                if (raycast.worldNormal == Vector3.zero)
                {
                    raycast.worldNormal = -raycast.gameObject.transform.forward;
                }
            }
            else
            {
                Ray ray = Camera.main.ScreenPointToRay(eventData.position);
                raycast.worldPosition = Camera.main.transform.position + ray.direction;
                raycast.worldNormal = -ray.direction;
            }

            // set the raycast data
            eventData.pointerCurrentRaycast = raycast;
        }

        private void SetScrollDeltaEventData(ref WorldPointerEventData eventData)
        {
            eventData.scrollDelta = this.gestureManager.ScrollDelta;

            if (this.joystickManager.ShouldActivate())
            {
                eventData.scrollDelta = this.joystickManager.ScrollDelta;
            }

            if (this.mouseManager.ShouldActivate())
            {
                eventData.scrollDelta = this.mouseManager.ScrollDelta;
            }
        }

        private void SetDeltaEventData(ref WorldPointerEventData eventData)
        {
            eventData.useDragThreshold = false;

            eventData.delta = this.gestureManager.ScreenDelta;
            eventData.worldDelta = this.gestureManager.WorldDelta;

            if (this.joystickManager.ShouldActivate())
            {
                eventData.delta = this.joystickManager.ScreenDelta;
                eventData.worldDelta = this.joystickManager.WorldDelta;
            }

            if (this.mouseManager.ShouldActivate())
            {
                eventData.delta = this.mouseManager.ScreenDelta;
                eventData.worldDelta = this.mouseManager.WorldDelta;
            }
        }

        private PointerEventData.FramePressState GetStateforButton(PointerEventData.InputButton button)
        {
            var pressed = this.joystickManager.IsPressed(button) || this.mouseManager.IsPressed(button) || this.gestureManager.IsPressed(button);
            var released = this.joystickManager.IsReleased(button) || this.mouseManager.IsReleased(button) || this.gestureManager.IsReleased(button);

            this.joystickManager.ResetButtonState(button);
            this.mouseManager.ResetButtonState(button);
            this.gestureManager.ResetButtonState(button);

            if (pressed && released)
            {
                return PointerEventData.FramePressState.PressedAndReleased;
            }
            if (pressed)
            {
                return PointerEventData.FramePressState.Pressed;
            }
            if (released)
            {
                return PointerEventData.FramePressState.Released;
            }

            return PointerEventData.FramePressState.NotChanged;
        }
    }
}
