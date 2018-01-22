// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using System;
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
#endif

namespace Showcases.Input
{
    [Serializable]
    public class GestureButton : Button<GestureStateData.Button> { }

    public class GestureStateData : StateData
    {
        public enum Mode
        {
            Navigation,
            Manipulation
        }

        public bool IsCancelled { get; private set; }

        public bool IsCapturingGestures
        {
            get
            {
#if UNITY_WSA || UNITY_EDITOR
                return (this.activeRecognizer != null) ? this.activeRecognizer.IsCapturingGestures() : false;
#else
                return false;
#endif
            }
        }

        public Mode GestureMode
        {
            get { return this.recognizerType; }
            set { this.ResetGestureRecognizer(value); }
        }

        public Mode recognizerType = Mode.Manipulation;
#if UNITY_WSA || UNITY_EDITOR
        private MRWSAInput.GestureRecognizer activeRecognizer = null;
        private MRWSAInput.GestureRecognizer navigationRecognizer = null;
        private MRWSAInput.GestureRecognizer manipulationRecognizer = null;
#endif
        private Vector3 navigationPosition = Vector3.zero;
        private Vector3 manipulationPosition = Vector3.zero;
        private Vector3 manipulationPrevious = Vector3.zero;

        public enum Button : int { Tapped }

        public GestureButton trackedButton;

#if UNITY_EDITOR
        private MouseStateData mouseStateData;
#endif

        private void ResetGestureRecognizer(Mode type)
        {
#if UNITY_WSA || UNITY_EDITOR
            switch (type)
            {
                case Mode.Navigation:
                    this.Transition(this.navigationRecognizer);
                    break;
                case Mode.Manipulation:
                    this.Transition(this.manipulationRecognizer);
                    break;
            }
#endif
            this.recognizerType = type;
        }

#if UNITY_WSA || UNITY_EDITOR
        private void Awake()
        {
            // Instantiate the Navigation recognizer
            this.navigationRecognizer = new MRWSAInput.GestureRecognizer();
            this.navigationRecognizer.SetRecognizableGestures(
                MRWSAInput.GestureSettings.Tap |
                MRWSAInput.GestureSettings.DoubleTap |
                MRWSAInput.GestureSettings.NavigationX | MRWSAInput.GestureSettings.NavigationY | MRWSAInput.GestureSettings.NavigationZ);

            // Instantiate the Manipulation Recognizer.
            this.manipulationRecognizer = new MRWSAInput.GestureRecognizer();
            this.manipulationRecognizer.SetRecognizableGestures(
                MRWSAInput.GestureSettings.Tap |
                MRWSAInput.GestureSettings.DoubleTap |
                MRWSAInput.GestureSettings.ManipulationTranslate);

#if UNITY_5
            this.navigationRecognizer.TappedEvent += this.Recognizer_TappedEvent;
            this.navigationRecognizer.NavigationStartedEvent += this.NavigationRecognizer_NavigationStartedEvent;
            this.navigationRecognizer.NavigationUpdatedEvent += this.NavigationRecognizer_NavigationUpdatedEvent;
            this.navigationRecognizer.NavigationCompletedEvent += this.NavigationRecognizer_NavigationCompletedEvent;
            this.navigationRecognizer.NavigationCanceledEvent += this.NavigationRecognizer_NavigationCanceledEvent;

            this.manipulationRecognizer.TappedEvent += this.Recognizer_TappedEvent;
            this.manipulationRecognizer.ManipulationStartedEvent += this.ManipulationRecognizer_ManipulationStartedEvent;
            this.manipulationRecognizer.ManipulationUpdatedEvent += this.ManipulationRecognizer_ManipulationUpdatedEvent;
            this.manipulationRecognizer.ManipulationCompletedEvent += this.ManipulationRecognizer_ManipulationCompletedEvent;
            this.manipulationRecognizer.ManipulationCanceledEvent += this.ManipulationRecognizer_ManipulationCanceledEvent;

            // interaction manager state handling
            MRWSAInput.InteractionManager.SourcePressed += InteractionManager_SourcePressed;
            MRWSAInput.InteractionManager.SourceReleased += InteractionManager_SourceReleased;
#else
            this.navigationRecognizer.Tapped += this.Recognizer_TappedEvent;
            this.navigationRecognizer.NavigationStarted += this.NavigationRecognizer_NavigationStartedEvent;
            this.navigationRecognizer.NavigationUpdated += this.NavigationRecognizer_NavigationUpdatedEvent;
            this.navigationRecognizer.NavigationCompleted += this.NavigationRecognizer_NavigationCompletedEvent;
            this.navigationRecognizer.NavigationCanceled += this.NavigationRecognizer_NavigationCanceledEvent;
            this.manipulationRecognizer.Tapped += this.Recognizer_TappedEvent;
            this.manipulationRecognizer.ManipulationStarted += this.ManipulationRecognizer_ManipulationStartedEvent;
            this.manipulationRecognizer.ManipulationUpdated += this.ManipulationRecognizer_ManipulationUpdatedEvent;
            this.manipulationRecognizer.ManipulationCompleted += this.ManipulationRecognizer_ManipulationCompletedEvent;
            this.manipulationRecognizer.ManipulationCanceled += this.ManipulationRecognizer_ManipulationCanceledEvent;

            // interaction manager state handling
            MRWSAInput.InteractionManager.InteractionSourcePressed += InteractionManager_SourcePressed;
            MRWSAInput.InteractionManager.InteractionSourceReleased += InteractionManager_SourceReleased;
#endif
        }

        private void OnDestroy()
        {
#if UNITY_5
            if (this.navigationRecognizer != null)
            {
                this.navigationRecognizer.TappedEvent -= this.Recognizer_TappedEvent;
                this.navigationRecognizer.NavigationStartedEvent -= this.NavigationRecognizer_NavigationStartedEvent;
                this.navigationRecognizer.NavigationUpdatedEvent -= this.NavigationRecognizer_NavigationUpdatedEvent;
                this.navigationRecognizer.NavigationCompletedEvent -= this.NavigationRecognizer_NavigationCompletedEvent;
                this.navigationRecognizer.NavigationCanceledEvent -= this.NavigationRecognizer_NavigationCanceledEvent;
            }

            if (this.manipulationRecognizer != null)
            {
                this.manipulationRecognizer.TappedEvent -= this.Recognizer_TappedEvent;
                this.manipulationRecognizer.ManipulationStartedEvent -= this.ManipulationRecognizer_ManipulationStartedEvent;
                this.manipulationRecognizer.ManipulationUpdatedEvent -= this.ManipulationRecognizer_ManipulationUpdatedEvent;
                this.manipulationRecognizer.ManipulationCompletedEvent -=
                    this.ManipulationRecognizer_ManipulationCompletedEvent;
                this.manipulationRecognizer.ManipulationCanceledEvent -=
                    this.ManipulationRecognizer_ManipulationCanceledEvent;
            }

            // interaction manager state handling
            MRWSAInput.InteractionManager.SourcePressed -= InteractionManager_SourcePressed;
            MRWSAInput.InteractionManager.SourceReleased -= InteractionManager_SourceReleased;
#else
            if (this.navigationRecognizer != null)
            {
                this.navigationRecognizer.Tapped -= this.Recognizer_TappedEvent;
                this.navigationRecognizer.NavigationStarted -= this.NavigationRecognizer_NavigationStartedEvent;
                this.navigationRecognizer.NavigationUpdated -= this.NavigationRecognizer_NavigationUpdatedEvent;
                this.navigationRecognizer.NavigationCompleted -= this.NavigationRecognizer_NavigationCompletedEvent;
                this.navigationRecognizer.NavigationCanceled -= this.NavigationRecognizer_NavigationCanceledEvent;
            }

            if (this.manipulationRecognizer != null)
            {
                this.manipulationRecognizer.Tapped -= this.Recognizer_TappedEvent;
                this.manipulationRecognizer.ManipulationStarted -= this.ManipulationRecognizer_ManipulationStartedEvent;
                this.manipulationRecognizer.ManipulationUpdated -= this.ManipulationRecognizer_ManipulationUpdatedEvent;
                this.manipulationRecognizer.ManipulationCompleted -= this.ManipulationRecognizer_ManipulationCompletedEvent;
                this.manipulationRecognizer.ManipulationCanceled -=
                    this.ManipulationRecognizer_ManipulationCanceledEvent;
            }

            // interaction manager state handling
            MRWSAInput.InteractionManager.InteractionSourcePressed -= InteractionManager_SourcePressed;
            MRWSAInput.InteractionManager.InteractionSourceReleased -= InteractionManager_SourceReleased;
#endif
        }

        private void OnEnable()
        {
            this.ResetGestureRecognizer(this.GestureMode);
        }

        private void OnDisable()
        {
            this.Transition(null);
        }


        private void Transition(MRWSAInput.GestureRecognizer newRecognizer)
        {
            if (newRecognizer != null && this.activeRecognizer == newRecognizer && this.IsCapturingGestures)
            {
                return;
            }

            if (this.activeRecognizer != null)
            {
                if (this.activeRecognizer == newRecognizer)
                {
                    return;
                }

                this.activeRecognizer.CancelGestures();
                this.activeRecognizer.StopCapturingGestures();
            }

            if (newRecognizer != null)
            {
                newRecognizer.StartCapturingGestures();
            }

            this.activeRecognizer = newRecognizer;
        }

        // only used when called via Unity UI
        // use ResetGestureRecognizer
        public void SetGestureMode(int type)
        {
            this.GestureMode = (Mode)Enum.ToObject(typeof(Mode), type);
        }

        private bool IsSupportedSource(MRWSAInput.InteractionSource source)
        {
#if UNITY_5
            if (source.sourceKind == MRWSAInput.InteractionSourceKind.Hand
                ||
                source.sourceKind == MRWSAInput.InteractionSourceKind.Voice)
#else
            if (source.kind == MRWSAInput.InteractionSourceKind.Hand
                ||
                source.kind == MRWSAInput.InteractionSourceKind.Voice
                ||
                // Work around for Unity bug 986569
                // https://fogbugz.unity3d.com/default.asp?986569_ocqv0s170krpen7g
                source.kind == MRWSAInput.InteractionSourceKind.Other
                )
#endif
            {
                return true;
            }

            return false;
        }

        private void Recognizer_TappedEvent(MRWSAInput.TappedEventArgs args)
        {
            if (!IsSupportedSource(args.source))
                return;

            this.trackedButton.state.pressed = false;
            this.trackedButton.state.pressedThisFrame = true;
            this.trackedButton.state.releasedThisFrame = true;
        }

        private void NavigationRecognizer_NavigationStartedEvent(MRWSAInput.NavigationStartedEventArgs args)
        {
            if (!IsSupportedSource(args.source))
                return;

            this.trackedButton.state.pressed = true;
            this.trackedButton.state.pressedThisFrame = true;
            this.trackedButton.state.releasedThisFrame = false;
            this.IsCancelled = false;

#if UNITY_5
            this.navigationPosition = args.normalizedOffset;
#else
            args.sourcePose.TryGetPosition(out this.navigationPosition);
#endif
        }

        private void NavigationRecognizer_NavigationUpdatedEvent(MRWSAInput.NavigationUpdatedEventArgs args)
        {
            if (!IsSupportedSource(args.source))
                return;

            this.trackedButton.state.pressed = true;
            this.trackedButton.state.releasedThisFrame = false;
            this.IsCancelled = false;

#if UNITY_5
            this.navigationPosition = args.normalizedOffset;
#else
            args.sourcePose.TryGetPosition(out this.navigationPosition);
#endif
        }

        private void NavigationRecognizer_NavigationCompletedEvent(MRWSAInput.NavigationCompletedEventArgs args)
        {
            if (!IsSupportedSource(args.source))
                return;

            this.trackedButton.state.pressed = false;
            this.trackedButton.state.releasedThisFrame = true;
            this.IsCancelled = false;

#if UNITY_5
            this.navigationPosition = args.normalizedOffset;
#else
            args.sourcePose.TryGetPosition(out this.navigationPosition);
#endif
        }

        private void NavigationRecognizer_NavigationCanceledEvent(MRWSAInput.NavigationCanceledEventArgs args)
        {
            if (!IsSupportedSource(args.source))
                return;

            this.trackedButton.state.pressed = false;
            this.trackedButton.state.releasedThisFrame = true;
            this.IsCancelled = true;

            this.navigationPosition = Vector3.zero;
        }

        private void ManipulationRecognizer_ManipulationStartedEvent(MRWSAInput.ManipulationStartedEventArgs args)
        {
            if (!IsSupportedSource(args.source))
                return;

            this.trackedButton.state.pressed = true;
            this.trackedButton.state.pressedThisFrame = true;
            this.trackedButton.state.releasedThisFrame = false;
            this.IsCancelled = false;

#if UNITY_5
            this.manipulationPosition = this.manipulationPrevious = args.cumulativeDelta;
#else
            args.sourcePose.TryGetPosition(out this.manipulationPosition);
            this.manipulationPrevious = this.manipulationPosition;
#endif
        }

        private void ManipulationRecognizer_ManipulationUpdatedEvent(MRWSAInput.ManipulationUpdatedEventArgs args)
        {
            if (!IsSupportedSource(args.source))
                return;

            this.trackedButton.state.pressed = true;
            this.trackedButton.state.releasedThisFrame = false;
            this.IsCancelled = false;
            this.manipulationPrevious = this.manipulationPosition;
            this.manipulationPosition = args.cumulativeDelta;
        }

        private void ManipulationRecognizer_ManipulationCompletedEvent(MRWSAInput.ManipulationCompletedEventArgs args)
        {
            if (!IsSupportedSource(args.source))
                return;

            this.trackedButton.state.pressed = true;
            this.trackedButton.state.releasedThisFrame = true;
            this.IsCancelled = false;
            this.manipulationPrevious = this.manipulationPosition;
            this.manipulationPosition = args.cumulativeDelta;
        }

        private void ManipulationRecognizer_ManipulationCanceledEvent(MRWSAInput.ManipulationCanceledEventArgs args)
        {
            if (!IsSupportedSource(args.source))
                return;

            this.trackedButton.state.pressed = false;
            this.trackedButton.state.releasedThisFrame = true;
            this.IsCancelled = true;
            this.manipulationPrevious = Vector3.zero;
            this.manipulationPosition = Vector3.zero;
        }

#if UNITY_5
        private void InteractionManager_SourcePressed(MRWSAInput.InteractionManager.SourceEventArgs args)
#else
        private void InteractionManager_SourcePressed(MRWSAInput.InteractionSourcePressedEventArgs args)
#endif
        {
            if (IsSupportedSource(args.state.source) 
                &&
#if UNITY_5
                args.pressKind == MRWSAInput.InteractionPressKind.Select
#else
                // Work around for Unity bug 986569
                // https://fogbugz.unity3d.com/default.asp?986569_ocqv0s170krpen7g
                true //  args.pressType == MRWSAInput.InteractionSourcePressType.Select
#endif
                )
            {
                    this.trackedButton.state.pressed = true;
                    this.trackedButton.state.pressedThisFrame = true;
            }
        }

#if UNITY_5
        private void InteractionManager_SourceReleased(MRWSAInput.InteractionManager.SourceEventArgs args)
#else
        private void InteractionManager_SourceReleased(MRWSAInput.InteractionSourceReleasedEventArgs args)
#endif
        {
            if (IsSupportedSource(args.state.source)
                &&
#if UNITY_5
                args.pressKind == MRWSAInput.InteractionPressKind.Select
#else
                // Work around for Unity bug 986569
                // https://fogbugz.unity3d.com/default.asp?986569_ocqv0s170krpen7g
                true // args.pressType == MRWSAInput.InteractionSourcePressType.Select
#endif
                )
            {
                this.trackedButton.state.pressed = false;
                this.trackedButton.state.pressedThisFrame = false;
                this.trackedButton.state.releasedThisFrame = true;
            }
        }
#endif

        public override bool AnyButtonsDownThisFrame()
        {
            return trackedButton.state.pressed;
        }

        public override bool UpdateState()
        {
            var lastScreenPoint = this.ScreenPoint;
            var lastPosition = this.WorldPosition;

            // always center of screen
            this.ScreenPoint = InputStateManager.Instance.gazeScreenPoint;
            this.ScreenPointDelta = InputStateManager.Instance.gazeScreenPointDelta;
            this.WorldPosition = InputStateManager.Instance.gazeWorldPosition;

            this.PressedThisFrame = this.trackedButton.state.pressedThisFrame;
            this.ReleasedThisFrame = this.trackedButton.state.releasedThisFrame;

            Vector3 delta = Vector3.zero;
            if (this.IsCapturingGestures)
            {
                if (this.GestureMode == Mode.Navigation)
                {
                    delta = this.navigationPosition;
                }
                else
                {
                    delta = this.manipulationPosition - this.manipulationPrevious;
                }
                
                this.WorldPositionDelta = delta;
            }

            // reset button
            this.trackedButton.state.pressed = false;
            this.trackedButton.state.pressedThisFrame = false;
            this.trackedButton.state.releasedThisFrame = false;

#if UNITY_EDITOR
            if (this.mouseStateData == null)
            {
                this.mouseStateData = InputStateManager.Instance.GetModuleOfType(typeof(MouseStateData)) as MouseStateData;
            }

            if (this.mouseStateData != null && this.mouseStateData.AnyButtonsDownThisFrame())
            {
                this.ScreenPointDelta = this.mouseStateData.ScreenPointDelta;
                this.WorldPositionDelta = this.mouseStateData.WorldPositionDelta;

                this.PressedThisFrame = this.mouseStateData.PressedThisFrame;
                this.ReleasedThisFrame = this.mouseStateData.ReleasedThisFrame;
            }
#endif

            return true;
        }
    }
}
