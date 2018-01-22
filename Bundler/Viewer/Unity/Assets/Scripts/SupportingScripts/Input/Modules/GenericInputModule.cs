// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

namespace Showcases.Input.Modules
{
    public class GenericInputModule : BaseInputModule
    {
        public bool forceModuleActive = false;

        private Dictionary<int, WorldPointerEventData> pointerEventData 
            = new Dictionary<int, WorldPointerEventData>();

        // must set any canvas worldCamera to the eventCamera otherwise Camera.main will be used
        private Dictionary<Canvas, Camera> worldCanvases
            = new Dictionary<Canvas, Camera>();
        public bool RegisterWorldCanvas(Canvas canvas)
        {
            if (canvas.renderMode == RenderMode.WorldSpace)
            {
                if (!worldCanvases.ContainsKey(canvas))
                {
                    worldCanvases.Add(canvas, canvas.worldCamera);

                    return true;
                }
            }

            return false;
        }

        public bool UnRegisterWorldCanvas(Canvas canvas)
        {
            if (canvas.renderMode == RenderMode.WorldSpace)
            {
                if (worldCanvases.ContainsKey(canvas))
                {
                    worldCanvases.Remove(canvas);

                    return true;
                }
            }

            return false;
        }

        private bool GetPointerEventData(int id, out WorldPointerEventData data, bool create)
        {
            if (!this.pointerEventData.TryGetValue(id, out data) && create)
            {
                data = new WorldPointerEventData(this.eventSystem)
                {
                    pointerId = id,
                };
                this.pointerEventData.Add(id, data);

                return true;
            }

            return false;
        }
        private void RemovePointerData(PointerEventData data)
        {
            this.pointerEventData.Remove(data.pointerId);
        }

        protected override void OnEnable()
        {
            base.OnEnable();

            foreach (var canvas in FindObjectsOfType<Canvas>())
            {
                RegisterWorldCanvas(canvas);
            }
        }

        public override bool IsModuleSupported()
        {
            return this.forceModuleActive || InputStateManager.Instance.IsModuleSupported();
        }

        public override bool ShouldActivateModule()
        {
            if (!base.ShouldActivateModule())
                return false;

            return forceModuleActive || InputStateManager.Instance.ShouldActivate();
        }

        public override void ActivateModule()
        {
            base.ActivateModule();

            var toSelect = this.eventSystem.currentSelectedGameObject;
            if (toSelect == null)
                toSelect = this.eventSystem.firstSelectedGameObject;

            this.eventSystem.SetSelectedGameObject(toSelect, this.GetBaseEventData());
        }

        public override void DeactivateModule()
        {
            base.DeactivateModule();

            var baseEventData = GetBaseEventData();

            this.eventSystem.SetSelectedGameObject(null, baseEventData);
        }

        public override void UpdateModule()
        {
        }

        public override void Process()
        {
            bool usedEvent = SendUpdateEventToSelectedObject(this.eventSystem, GetBaseEventData());

            if (eventSystem.sendNavigationEvents)
            {
                if (!usedEvent)
                {
                    Vector2 movement;
                    if (InputStateManager.Instance.ShouldMove(out movement))
                    {
                        // convert event data to axisEventData
                        // movement shold be normalized between -1.0f to 1.0f
                        var axisEventData = GetAxisEventData(movement.x, movement.y, 0.6f);

                        usedEvent |= SendMoveEventToSelectedObject(axisEventData, this.eventSystem, GetBaseEventData());
                    }
                }

                if (!usedEvent)
                    SendSubmitEventToSelectedObject(this.eventSystem, GetBaseEventData());
            }

            ProcessStateData();
        }

        private void ProcessStateData()
        {
            foreach (var stateData in InputStateManager.Instance.stateDataCollectors.items)
            {
                // if inputState should be ignored go to next one
                if (!stateData.UpdateState())
                    continue;

                Camera eventCamera = InputStateManager.Instance.gazeCamera;
                if (stateData.raycastOrigin == StateData.RaycastOrigin.world)
                {
                    eventCamera = InputStateManager.Instance.eventCamera;

                    // set the canvas camera
                    foreach (var key in worldCanvases.Keys)
                    {
                        if (key == null || worldCanvases[key] == null)
                        {
                            worldCanvases.Remove(key);
                            break;
                        }
                        key.worldCamera = eventCamera;
                    }

                    // activate the correct raycaster
                    InputStateManager.Instance.gazeCameraRaycaster.enabled = false;
                    InputStateManager.Instance.eventCameraRaycaster.enabled = true;
                }
                else
                {
                    InputStateManager.Instance.gazeCameraRaycaster.enabled = true;
                    InputStateManager.Instance.eventCameraRaycaster.enabled = false;
                }

                var eventData = GetWorldPointerEventData(eventCamera, stateData);

                // get button state and process the 
                bool released = stateData.Canceled || stateData.ReleasedThisFrame;
                bool pressed = stateData.PressedThisFrame;

                this.ProcessEventData(eventData, pressed, released);

                // reset raycaster
                InputStateManager.Instance.gazeCameraRaycaster.enabled = false;
                InputStateManager.Instance.eventCameraRaycaster.enabled = false;

                // if raycast was based on world origin position
                if (stateData.raycastOrigin == StateData.RaycastOrigin.world)
                {
                    // reset the world canvas back to original camera
                    foreach (var key in worldCanvases.Keys)
                    {
                        if (key == null || worldCanvases[key] == null)
                        {
                            worldCanvases.Remove(key);
                            break;
                        }
                        key.worldCamera = worldCanvases[key];
                    }
                }
            }
        }

        private WorldPointerEventData GetWorldPointerEventData(Camera eventCamera, StateData stateData)
        {
            WorldPointerEventData eventData;
            GetPointerEventData(stateData.id, out eventData, true);

            var lastScreenPoint = eventData.position;
            var lastWorldPosition = eventData.worldPosition;

            eventData.Reset();

            eventData.position = stateData.ScreenPoint;
            eventData.worldPosition = stateData.WorldPosition;

            if (stateData.PressedThisFrame)
            {
                eventData.delta = Vector2.zero;
                eventData.worldDelta = Vector3.zero;
            }
            else
            {
                eventData.useDragThreshold = false;

                eventData.delta = stateData.ScreenPointDelta;
                eventData.worldDelta = stateData.WorldPositionDelta;
            }

            Debug.DrawLine(eventCamera.transform.position, eventData.worldPosition, Color.green);

            eventData.button = PointerEventData.InputButton.Left; // arbitrary

            // set the raycast result
            this.eventSystem.RaycastAll(eventData, this.m_RaycastResultCache);

            // use first object in cache
            var raycast = FindFirstRaycast(this.m_RaycastResultCache);
            
            // correct raycast result
            if (raycast.isValid)
            {
                // for UI objects raycast.worldPosition == Vector3.zero, correct it world position value
                if (raycast.worldPosition == Vector3.zero)
                {
                    var position = eventData.worldPosition;
                    if (stateData.raycastOrigin == StateData.RaycastOrigin.screen)
                    {
                        var direction = (position - eventCamera.transform.position).normalized;
                        position = eventCamera.transform.position + (direction * raycast.distance);
                    }
                    else
                    {
                        // camera center based raycast
                        position = eventCamera.transform.position + (eventCamera.transform.forward * raycast.distance);
                    }
                    raycast.worldPosition = position;

                    Debug.DrawLine(eventCamera.transform.position, raycast.worldPosition, Color.yellow);
                }
                else if (stateData.raycastOrigin == StateData.RaycastOrigin.world && eventData.pointerDrag != null && eventData.pointerPressRaycast.isValid)
                {
                    raycast.worldPosition = eventCamera.transform.position + (eventCamera.transform.forward * eventData.pointerPressRaycast.distance) + eventData.worldDelta;
                }


                if (raycast.worldNormal == Vector3.zero)
                {
                    raycast.worldNormal = -raycast.gameObject.transform.forward;
                }
            }
            else
            {
                if (eventData.pointerDrag != null && eventData.pointerPressRaycast.isValid)
                {
                    raycast.worldPosition = eventCamera.transform.position + (eventCamera.transform.forward * eventData.pointerPressRaycast.distance);
                }
                else
                {
                    raycast.worldPosition = eventCamera.transform.position + (eventCamera.transform.forward * eventCamera.nearClipPlane);
                    raycast.worldNormal = -InputStateManager.Instance.gazeCamera.transform.forward; // always face the gaze camera;
                }
            }

            // if raycast was based on world origin position
            if (stateData.raycastOrigin == StateData.RaycastOrigin.world)
            {
                // reset event data raycast point with respect to the main camera
                //eventData.position = InputStateManager.Instance.gazeCamera.WorldToScreenPoint(eventData.pointerCurrentRaycast.worldPosition);
                eventData.worldPosition = eventData.pointerCurrentRaycast.worldPosition;
            //    eventData.delta = eventData.position - lastScreenPoint;
                eventData.worldDelta = eventData.worldPosition - lastWorldPosition;
            }

            // set the raycast data
            eventData.pointerCurrentRaycast = raycast;

            // clear the cache
            this.m_RaycastResultCache.Clear();

            // store the raycast for others to grab
            stateData.RaycastResult = eventData.pointerCurrentRaycast;

            return eventData;
        }

        // Process the pointer event data
        private void ProcessEventData(PointerEventData eventData, bool pressed, bool released)
        {
            ProcessPress(eventData, pressed, released);

            if (!released)
            {
                ProcessMove(eventData);
                ProcessDrag(eventData);
            }
            else
                RemovePointerData(eventData);
        }

        private void ProcessPress(PointerEventData eventData, bool pressed, bool released)
        {
            var currentOverGo = eventData.pointerCurrentRaycast.gameObject;

            // PointerDown/PointerClick event
            if (pressed)
            {
                eventData.eligibleForClick = true;
                eventData.delta = Vector2.zero;
                eventData.dragging = false;
                eventData.useDragThreshold = true;
                eventData.pressPosition = eventData.position;
                eventData.pointerPressRaycast = eventData.pointerCurrentRaycast;

                DeselectIfSelectionChanged(currentOverGo, eventData);

                if (eventData.pointerEnter != currentOverGo)
                {
                    // send a pointer enter to the touched element if it isn't the one to select...
                    HandlePointerExitAndEnter(eventData, currentOverGo);
                    eventData.pointerEnter = currentOverGo;
                }

                // search for the control that will receive the press
                var newPressed = ExecuteEvents.ExecuteHierarchy(currentOverGo, eventData, ExecuteEvents.pointerDownHandler);

                // didnt find a press handler... search for a click handler
                if (newPressed == null)
                    newPressed = ExecuteEvents.GetEventHandler<IPointerClickHandler>(currentOverGo);

                float time = Time.unscaledTime;

                if (newPressed == eventData.lastPress)
                {
                    var diffTime = time - eventData.clickTime;
                    if (diffTime < 0.3f)
                        ++eventData.clickCount;
                    else
                        eventData.clickCount = 1;

                    eventData.clickTime = time;
                }
                else
                {
                    eventData.clickCount = 1;
                }

                eventData.pointerPress = newPressed;
                eventData.rawPointerPress = currentOverGo;

                eventData.clickTime = time;

                // Save the drag handler as well
                eventData.pointerDrag = ExecuteEvents.GetEventHandler<IDragHandler>(currentOverGo);

                if (eventData.pointerDrag != null)
                    ExecuteEvents.Execute(eventData.pointerDrag, eventData, ExecuteEvents.initializePotentialDrag);
            }

            // PointerUp notification
            if (released)
            {
                ExecuteEvents.Execute(eventData.pointerPress, eventData, ExecuteEvents.pointerUpHandler);

                // see if we released on the same element that was clicked before
                var pointerUpHandler = ExecuteEvents.GetEventHandler<IPointerClickHandler>(currentOverGo);

                // PointerClick and Drop events
                if (eventData.pointerPress == pointerUpHandler && eventData.eligibleForClick)
                {
                    ExecuteEvents.Execute(eventData.pointerPress, eventData, ExecuteEvents.pointerClickHandler);
                }
                else if (eventData.pointerDrag != null && eventData.dragging)
                {
                    ExecuteEvents.ExecuteHierarchy(currentOverGo, eventData, ExecuteEvents.dropHandler);
                }

                eventData.eligibleForClick = false;
                eventData.pointerPress = null;
                eventData.rawPointerPress = null;

                if (eventData.pointerDrag != null && eventData.dragging)
                    ExecuteEvents.Execute(eventData.pointerDrag, eventData, ExecuteEvents.endDragHandler);

                eventData.dragging = false;
                eventData.pointerDrag = null;

                // send exit events as we need to simulate this on touch up on touch device
                ExecuteEvents.ExecuteHierarchy(eventData.pointerEnter, eventData, ExecuteEvents.pointerExitHandler);
                eventData.pointerEnter = null;
            }
        }

        private void ProcessMove(PointerEventData eventData)
        {
            var targetGO = (Cursor.lockState == CursorLockMode.Locked ? null : eventData.pointerCurrentRaycast.gameObject);
            HandlePointerExitAndEnter(eventData, targetGO);
        }

        private void ProcessDrag(PointerEventData eventData)
        {
            if (!eventData.IsPointerMoving() ||
                //Cursor.lockState == CursorLockMode.Locked ||
                eventData.pointerDrag == null)
                return;

            if (!eventData.dragging
                && ShouldStartDrag(eventData.pressPosition, eventData.position, eventSystem.pixelDragThreshold, eventData.useDragThreshold))
            {
                ExecuteEvents.Execute(eventData.pointerDrag, eventData, ExecuteEvents.beginDragHandler);
                eventData.dragging = true;
            }

            // Drag notification
            if (eventData.dragging)
            {
                // Before doing drag we should cancel any pointer down state
                // And clear selection!
                if (eventData.pointerPress != eventData.pointerDrag)
                {
                    ExecuteEvents.Execute(eventData.pointerPress, eventData, ExecuteEvents.pointerUpHandler);

                    eventData.eligibleForClick = false;
                    eventData.pointerPress = null;
                    eventData.rawPointerPress = null;
                }
                ExecuteEvents.Execute(eventData.pointerDrag, eventData, ExecuteEvents.dragHandler);
            }
        }

        private void DeselectIfSelectionChanged(GameObject currentOverGo, BaseEventData pointerEvent)
        {
            // Selection tracking
            var selectHandlerGO = ExecuteEvents.GetEventHandler<ISelectHandler>(currentOverGo);

            // if we have clicked something new, deselect the old thing
            // leave 'selection handling' up to the press event though.
            if (selectHandlerGO != eventSystem.currentSelectedGameObject)
                eventSystem.SetSelectedGameObject(null, pointerEvent);
        }

        private static bool SendUpdateEventToSelectedObject(EventSystem eventSystem, BaseEventData data)
        {
            if (eventSystem.currentSelectedGameObject == null)
                return false;

            ExecuteEvents.Execute(eventSystem.currentSelectedGameObject, data, ExecuteEvents.updateSelectedHandler);

            return data.used;
        }

        // sends submit/cancel action
        private static bool SendSubmitEventToSelectedObject(EventSystem eventSystem, BaseEventData data)
        {
            // eventSystem must have a selected object
            if (eventSystem.currentSelectedGameObject == null)
            {
                return false;
            }

            // Submit, only send once
            if (InputStateManager.Instance.ShouldSubmit())
            {
                ExecuteEvents.Execute(eventSystem.currentSelectedGameObject, data, ExecuteEvents.submitHandler);
            }

            // Cancel, only send once
            if (InputStateManager.Instance.ShouldCancel())
            {
                ExecuteEvents.Execute(eventSystem.currentSelectedGameObject, data, ExecuteEvents.cancelHandler);
            }

            return data.used;
        }

        // sends navigation event within a canvas
        private static bool SendMoveEventToSelectedObject(AxisEventData axisEventData, EventSystem eventSystem, BaseEventData data)
        {
            // eventSystem must have a selected object
            if (eventSystem.currentSelectedGameObject == null)
            {
                return false;
            }

            ExecuteEvents.Execute(eventSystem.currentSelectedGameObject, axisEventData, ExecuteEvents.moveHandler);

            return axisEventData.used;
        }

        private static bool ShouldStartDrag(Vector2 pressPos, Vector2 currentPos, float threshold, bool useDragThreshold)
        {
            if (!useDragThreshold)
                return true;

            return (pressPos - currentPos).sqrMagnitude >= threshold * threshold;
        }

        public override string ToString()
        {
            var sb = new System.Text.StringBuilder("<b>Pointer Input Module of type: </b>" + GetType());
            sb.AppendLine();
            foreach (var pointer in this.pointerEventData)
            {
                if (pointer.Value == null)
                    continue;
                sb.AppendLine("<B>Pointer:</b> " + pointer.Key);
                sb.AppendLine(pointer.Value.ToString());
            }
            return sb.ToString();
        }
    }
}