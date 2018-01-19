// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using System;
using System.Collections.Generic;
using UnityEngine;

#if UNITY_WSA || UNITY_EDITOR
#if UNITY_5
using MR = UnityEngine.VR;
using MRWSA = UnityEngine.VR.WSA;
using MRWSAInput = UnityEngine.VR.WSA.Input;
#else
using MR = UnityEngine.XR;
using MRWSA = UnityEngine.XR.WSA;
using MRWSAInput = UnityEngine.XR.WSA.Input;
#endif

#if UNITY_5 || UNITY_EDITOR
namespace UnityEngine
{
    namespace VR
    {
        namespace WSA
        {
            namespace Showcases.Input
            {
                public enum InteractionSourceNode
                {
                    Grip = 0,
                    Pointer = 1
                }
            }
        }
    }
}
#endif
#endif



namespace Showcases.Input
{
    public enum SourceKind
    {
        Other = 0,
        Hand = 1,
        Voice = 2,
        Controller = 3
    }

    public enum SourceHandedness
    {
        Unknown = 0,
        Left = 1,
        Right = 2
    }

    public enum SourceNode
    {
        Grip = 0,
        Pointer = 1
    }

    public abstract class InteractionSourceData : StateData
    {
        public enum Button : int { Select, Grip, Thumbstick, TouchPad, TouchPadTouched, Menu, }
        public abstract SourceKind kind { get; }

        public Button selectionButton = Button.Select;

        public SourceHandedness activeSelectionHand { get; protected set; }
        public SourceNode activeSourceNode = SourceNode.Grip;

#if UNITY_WSA || UNITY_EDITOR
        private MRWSAInput.InteractionSourceState[] readings;

        public MRWSAInput.InteractionSourceState? leftHandState { get; protected set; }
        public MRWSAInput.InteractionSourceState? prevLeftHandState { get; protected set; }

        public MRWSAInput.InteractionSourceState? rightHandState { get; protected set; }
        public MRWSAInput.InteractionSourceState? prevRightHandState { get; protected set; }

        public MRWSAInput.InteractionSourceState? unspecifiedHandState { get; protected set; }
        public MRWSAInput.InteractionSourceState? prevUnspecifiedHandState { get; protected set; }
#endif

        private void Update()
        {
#if UNITY_WSA || UNITY_EDITOR
            this.prevLeftHandState = this.leftHandState;
            this.leftHandState = null;

            this.prevRightHandState = this.rightHandState;
            this.rightHandState = null;

            this.prevUnspecifiedHandState = this.unspecifiedHandState;
            this.unspecifiedHandState = null;

            this.readings = MRWSAInput.InteractionManager.GetCurrentReading();
            foreach (var reading in this.readings)
            {
                SourceKind sourceKind = SourceKind.Other;
#if UNITY_5
                sourceKind = (SourceKind)Enum.ToObject(typeof(SourceKind), reading.source.sourceKind);
#else
                sourceKind = (SourceKind)Enum.ToObject(typeof(SourceKind), reading.source.kind);
#endif
                if (sourceKind == kind)
                {
                    if (reading.source.handedness == MRWSAInput.InteractionSourceHandedness.Left)
                    {
                        this.leftHandState = reading;
                    }
                    else if (reading.source.handedness == MRWSAInput.InteractionSourceHandedness.Right)
                    {
                        this.rightHandState = reading;
                    }
#if UNITY_5
                    else if (reading.source.handedness == MRWSAInput.InteractionSourceHandedness.Unspecified)
#else
                    else if (reading.source.handedness == MRWSAInput.InteractionSourceHandedness.Unknown)
#endif
                    {
                        this.unspecifiedHandState = reading;
                    }

                    SetPressedPointerState(reading.source.handedness);
                }
            }
#endif
        }

        // event helper
        public override bool UpdateState()
        {
            SetStateData();

            return true;
        }

        protected void SetStateData()
        {
            var position = GetPosition(this.activeSelectionHand, this.activeSourceNode);
            var rotation = GetRotation(this.activeSelectionHand, this.activeSourceNode);

            this.WorldPosition = position.HasValue ? position.Value : Vector3.zero;
            this.Rotation = rotation.HasValue ? rotation.Value : Quaternion.identity;
            this.WorldPositionDelta = GetPositionDelta(this.activeSelectionHand, this.activeSourceNode);
            this.RotationDelta = GetRotationDelta(this.activeSelectionHand, this.activeSourceNode);
        }

        protected void SetButtonStateData<T>(T button) where T : Button<InteractionSourceData.Button>
        {
            button.state.pressedThisFrame = GetPressedThisFrame(this.activeSelectionHand, button.type);
            button.state.releasedThisFrame = GetReleasedThisFrame(this.activeSelectionHand, button.type);
            button.state.pressed = IsPressed(this.activeSelectionHand, button.type);
        }

#if UNITY_WSA || UNITY_EDITOR
        private void SetPressedPointerState(MRWSAInput.InteractionSourceHandedness handedness)
        {
            var state = GetState(handedness, false);
            var lastState = GetState(handedness, true);

            var hand = (SourceHandedness)Enum.ToObject(typeof(SourceHandedness), state.Value.source.handedness);


            var isPressedThisFrame = IsPressed(state, selectionButton);
            var isPressedLastFrame = IsPressed(lastState, selectionButton);
            if (isPressedThisFrame && !isPressedLastFrame)
            {
                if (activeSelectionHand != hand)
                    activeSelectionHand = hand;
            }

            if (state.HasValue && activeSelectionHand == hand)
            {
                PressedThisFrame = (isPressedThisFrame && !isPressedLastFrame);
                ReleasedThisFrame = (!isPressedThisFrame && isPressedLastFrame);
            }
        }

        // helper methods
        private bool IsPressed(MRWSAInput.InteractionSourceState? state, Button btn)
        {
            if (state == null || !state.HasValue)
                return false;

            switch (btn)
            {
                case Button.Select:
                    return state.Value.selectPressed;
                case Button.Grip:
                    return state.Value.grasped;
                case Button.Thumbstick:
#if UNITY_5
                    return state.Value.controllerProperties.thumbstickPressed;
#else
                    return state.Value.thumbstickPressed;
#endif
                case Button.TouchPad:
#if UNITY_5
                    return state.Value.controllerProperties.touchpadPressed;
#else
                    return state.Value.touchpadPressed;
#endif
                case Button.TouchPadTouched:
#if UNITY_5
                    return state.Value.controllerProperties.touchpadTouched;
#else
                    return state.Value.touchpadTouched;
#endif
                case Button.Menu:
                    return state.Value.menuPressed;
                default:
                    return false;
            }
        }

        private Vector3? GetPosition(MRWSAInput.InteractionSourceState? state, MRWSAInput.InteractionSourceNode node)
        {
            if (state == null || !state.HasValue)
                return null;

            Vector3? position = null;

            Vector3 pos = Vector3.zero;
#if UNITY_5
            bool succeeded = false;
            if (node == MRWSAInput.InteractionSourceNode.Grip)
            {
                succeeded = state.Value.sourcePose.TryGetPosition(out pos);
            }
            else if (node == MRWSAInput.InteractionSourceNode.Pointer)
            {
                succeeded = state.Value.sourcePose.TryGetPointerPosition(out pos);
            }

            if (succeeded)
#else
            if (state.Value.sourcePose.TryGetPosition(out pos, node))
#endif
            {
                position = pos;
            }

            return position;
        }

        private Quaternion? GetRotation(MRWSAInput.InteractionSourceState? state, MRWSAInput.InteractionSourceNode node)
        {
            if (state == null || !state.HasValue)
                return null;

            Quaternion? rotation = null;

            Quaternion rot = Quaternion.identity;
#if UNITY_5
            bool succeeded = false;
            if (node == MRWSAInput.InteractionSourceNode.Grip)
            {
                succeeded = state.Value.sourcePose.TryGetRotation(out rot);
            }
            else if (node == MRWSAInput.InteractionSourceNode.Pointer)
            {
                succeeded = state.Value.sourcePose.TryGetPointerRotation(out rot);
            }

            if (succeeded)
#else
            if (state.Value.sourcePose.TryGetRotation(out rot, node))
#endif
            {
                rotation = rot;
            }

            return rotation;
        }

        // state
        public MRWSAInput.InteractionSourceState? GetState(SourceHandedness handedness, bool getPreviousState)
        {
            var hand = (MRWSAInput.InteractionSourceHandedness)Enum.ToObject(typeof(MRWSAInput.InteractionSourceHandedness), handedness);

            return GetState(hand, getPreviousState);
        }

        public MRWSAInput.InteractionSourceState? GetState(MRWSAInput.InteractionSourceHandedness handedness, bool getPreviousState)
        {
            MRWSAInput.InteractionSourceState? state = null;

            if (handedness == MRWSAInput.InteractionSourceHandedness.Left)
            {
                state = getPreviousState ? this.prevLeftHandState : this.leftHandState;
            }
            else if (handedness == MRWSAInput.InteractionSourceHandedness.Right)
            {
                state = getPreviousState ? this.prevRightHandState : this.rightHandState;
            }
#if UNITY_5
            else if (handedness == MRWSAInput.InteractionSourceHandedness.Unspecified)
#else
            else if (handedness == MRWSAInput.InteractionSourceHandedness.Unknown)
#endif
            {
                state = getPreviousState ? this.prevUnspecifiedHandState : this.unspecifiedHandState;
            }

            return state;
        }

        // button states
        public bool IsPressed(MRWSAInput.InteractionSourceHandedness handedness, Button btn, bool getPreviousValue = false)
        {
            var state = GetState(handedness, getPreviousValue);

            return IsPressed(state, btn);
        }

        public bool GetPressedThisFrame(MRWSAInput.InteractionSourceHandedness handedness, Button btn)
        {
            var state = GetState(handedness, false);
            var lastState = GetState(handedness, true);

            return IsPressed(state, btn) && !IsPressed(lastState, btn);
        }

        public bool GetReleasedThisFrame(MRWSAInput.InteractionSourceHandedness handedness, Button btn)
        {
            var state = GetState(handedness, false);
            var lastState = GetState(handedness, true);

            return !IsPressed(state, btn) && IsPressed(lastState, btn);
        }

        public bool GetPressedAndReleased(MRWSAInput.InteractionSourceHandedness handedness, Button btn)
        {
            return GetPressedThisFrame(handedness, btn) && GetReleasedThisFrame(handedness, btn);
        }

        public bool GetPressedOrReleased(MRWSAInput.InteractionSourceHandedness handedness, Button btn)
        {
            return GetPressedThisFrame(handedness, btn) || GetReleasedThisFrame(handedness, btn);
        }

        // special states
        public double SelectAmount(MRWSAInput.InteractionSourceHandedness handedness, bool getPreviousValue = false)
        {
            var state = GetState(handedness, getPreviousValue);

            double amount = 0.0;
            if (state.HasValue)
            {
#if UNITY_5
                amount = state.Value.selectPressedValue;
#else
            amount = state.Value.selectPressedAmount;
#endif
            }

            return amount;
        }

        public Vector2 TouchPosition(MRWSAInput.InteractionSourceHandedness handedness, bool getPreviousValue = false)
        {
            var state = GetState(handedness, getPreviousValue);

            var position = Vector2.zero;
            if (state.HasValue)
            {
#if UNITY_5
                position = new Vector2((float)state.Value.controllerProperties.touchpadX, (float)state.Value.controllerProperties.touchpadY);
#else
            position = state.Value.touchpadPosition;
#endif
            }

            return position;
        }

        public Vector2 TouchDelta(MRWSAInput.InteractionSourceHandedness handedness)
        {
            var state = GetState(handedness, false);
            var lastState = GetState(handedness, true);

            var delta = Vector2.zero;
            if (state.HasValue && lastState.HasValue)
            {
#if UNITY_5
                var pos = new Vector2((float)state.Value.controllerProperties.touchpadX, (float)state.Value.controllerProperties.touchpadY);
                var lastPos = new Vector2((float)lastState.Value.controllerProperties.touchpadX, (float)lastState.Value.controllerProperties.touchpadY);
#else
            var pos = state.Value.touchpadPosition;
            var lastPos = state.Value.touchpadPosition;
#endif
                delta = pos - lastPos;
            }

            return delta;
        }

        public Vector2 ThumbStickPosition(MRWSAInput.InteractionSourceHandedness handedness, bool getPreviousValue = false)
        {
            var state = GetState(handedness, getPreviousValue);

            var position = Vector2.zero;
            if (state.HasValue)
            {
#if UNITY_5
                position = new Vector2((float)state.Value.controllerProperties.thumbstickX, (float)state.Value.controllerProperties.thumbstickY);
#else
            position = state.Value.thumbstickPosition;
#endif
            }

            return position;
        }

        public Vector2 ThumbStickDelta(MRWSAInput.InteractionSourceHandedness handedness)
        {
            var state = GetState(handedness, false);
            var lastState = GetState(handedness, true);

            var delta = Vector2.zero;
            if (state.HasValue && lastState.HasValue)
            {
#if UNITY_5
                var pos = new Vector2((float)state.Value.controllerProperties.thumbstickX, (float)state.Value.controllerProperties.thumbstickY);
                var lastPos = new Vector2((float)lastState.Value.controllerProperties.thumbstickX, (float)lastState.Value.controllerProperties.thumbstickY);
#else
            var pos = state.Value.thumbstickPosition;
            var lastPos = lastState.Value.thumbstickPosition;
#endif
                delta = pos - lastPos;
            }

            return delta;
        }

        public Vector3? GetPosition(MRWSAInput.InteractionSourceHandedness handedness, MRWSAInput.InteractionSourceNode node, bool getPreviousValue = false)
        {
            var state = GetState(handedness, getPreviousValue);

            return GetPosition(state, node);
        }

        public Vector3 GetPositionDelta(MRWSAInput.InteractionSourceHandedness handedness, MRWSAInput.InteractionSourceNode node)
        {
            var state = GetState(handedness, false);
            var lastState = GetState(handedness, true);

            var position = GetPosition(state, node);
            var lastPosition = GetPosition(lastState, node);

            Vector3 delta = Vector3.zero;
            if (position.HasValue && lastPosition.HasValue)
            {
                delta = position.Value - lastPosition.Value;
            }

            return delta;
        }

        // rotation
        public Quaternion? GetRotation(MRWSAInput.InteractionSourceHandedness handedness, MRWSAInput.InteractionSourceNode node, bool getPreviousValue = false)
        {
            var state = GetState(handedness, getPreviousValue);

            return GetRotation(state, node);
        }

        public Quaternion GetRotationDelta(MRWSAInput.InteractionSourceHandedness handedness, MRWSAInput.InteractionSourceNode node)
        {
            var state = GetState(handedness, false);
            var lastState = GetState(handedness, true);

            var rotation = GetRotation(state, node);
            var lastRotation = GetRotation(lastState, node);

            Quaternion delta = Quaternion.identity;
            if (rotation.HasValue && lastRotation.HasValue)
            {
                delta = rotation.Value * Quaternion.Inverse(lastRotation.Value);
            }

            return delta;
        }
#endif

        public bool HasData(SourceHandedness handedness, bool getPreviousState)
        {
#if UNITY_WSA || UNITY_EDITOR
    
            var state = GetState(handedness, getPreviousState);

            return (state != null && state.HasValue);
#else
            return false;
#endif
        }

        public bool IsActiveHand(SourceHandedness sourceHandedness)
        {
            return (this.activeSelectionHand == sourceHandedness);
        }

        // button states
        public bool IsPressed(SourceHandedness handedness, Button btn, bool getPreviousValue = false)
        {
#if UNITY_WSA || UNITY_EDITOR
            var hand = (MRWSAInput.InteractionSourceHandedness)Enum.ToObject(typeof(MRWSAInput.InteractionSourceHandedness), handedness);

            return IsPressed(hand, btn, getPreviousValue);
#else
            return false;
#endif
        }


        public bool GetPressedThisFrame(SourceHandedness handedness, Button btn)
        {
#if UNITY_WSA || UNITY_EDITOR

            var hand = (MRWSAInput.InteractionSourceHandedness)Enum.ToObject(typeof(MRWSAInput.InteractionSourceHandedness), handedness);

            return GetPressedThisFrame(hand, btn);
#else
            return false;
#endif
        }


        public bool GetReleasedThisFrame(SourceHandedness handedness, Button btn)
        {
#if UNITY_WSA || UNITY_EDITOR
            var hand = (MRWSAInput.InteractionSourceHandedness)Enum.ToObject(typeof(MRWSAInput.InteractionSourceHandedness), handedness);

            return GetReleasedThisFrame(hand, btn);
#else
            return false;
#endif
        }

        // position states
        public Vector3? GetPosition(SourceHandedness sourceHandedness, SourceNode sourceNode, bool getPreviousValue = false)
        {
#if UNITY_WSA || UNITY_EDITOR
            var hand = (MRWSAInput.InteractionSourceHandedness)Enum.ToObject(typeof(MRWSAInput.InteractionSourceHandedness), sourceHandedness);
            var node = (MRWSAInput.InteractionSourceNode)Enum.ToObject(typeof(MRWSAInput.InteractionSourceNode), sourceNode);

            return GetPosition(hand, node, getPreviousValue);
#else
            return null;
#endif
        }

        public Vector3 GetPositionDelta(SourceHandedness sourceHandedness, SourceNode sourceNode)
        {
#if UNITY_WSA || UNITY_EDITOR
            var hand = (MRWSAInput.InteractionSourceHandedness)Enum.ToObject(typeof(MRWSAInput.InteractionSourceHandedness), sourceHandedness);
            var node = (MRWSAInput.InteractionSourceNode)Enum.ToObject(typeof(MRWSAInput.InteractionSourceNode), sourceNode);

            return GetPositionDelta(hand, node);
#else
            return Vector3.zero;
#endif
        }
        public Quaternion? GetRotation(SourceHandedness sourceHandedness, SourceNode sourceNode, bool getPreviousValue = false)
        {
#if UNITY_WSA || UNITY_EDITOR
            var hand = (MRWSAInput.InteractionSourceHandedness)Enum.ToObject(typeof(MRWSAInput.InteractionSourceHandedness), sourceHandedness);
            var node = (MRWSAInput.InteractionSourceNode)Enum.ToObject(typeof(MRWSAInput.InteractionSourceNode), sourceNode);

            return GetRotation(hand, node, getPreviousValue);
#else
            return Quaternion.identity;
#endif
        }

        public Quaternion GetRotationDelta(SourceHandedness sourceHandedness, SourceNode sourceNode)
        {
#if UNITY_WSA || UNITY_EDITOR
            var hand = (MRWSAInput.InteractionSourceHandedness)Enum.ToObject(typeof(MRWSAInput.InteractionSourceHandedness), sourceHandedness);
            var node = (MRWSAInput.InteractionSourceNode)Enum.ToObject(typeof(MRWSAInput.InteractionSourceNode), sourceNode);

            return GetRotationDelta(hand, node);
#else
            return Quaternion.identity;
#endif
        }
    }
}
