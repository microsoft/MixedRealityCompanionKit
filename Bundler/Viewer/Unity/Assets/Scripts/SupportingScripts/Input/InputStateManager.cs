// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using Showcases.Input.Modules;
using System;
using System.Collections;
using UnityEngine;
using UnityEngine.EventSystems;

namespace Showcases.Input
{
#if UNITY_EDITOR
    using UnityEditor;

    [CustomPropertyDrawer(typeof(StateDataList))]
    public class InputStateDataListDrawer : DataListDrawer
    {
        protected override void RefreshList(SerializedProperty property)
        {
            // will keep checking the list to ensure this updates automatically with UI changes
            InputStateManager.Instance.UpdateDataCollectors();

            // only allow re-ordering
            base.RefreshList(property, true, false, false, false);
        }
    }
#endif

    [Serializable]
    public class StateDataList : DataList<StateData> { }

    [RequireComponent(typeof(EventSystem))]
    [RequireComponent(typeof(GenericInputModule))]
    public class InputStateManager : SingletonBehavior<InputStateManager>
    {
        [SerializeField] public StateDataList stateDataCollectors;

        public float maxDistance = 10.0f;

        public Vector3 viewportSize { get; private set; }

        public Vector3 viewportCenter { get; private set; }

        public Vector2 screenSize { get; private set; }

        public Vector2 gazeScreenPoint { get; private set; }

        public Vector2 gazeScreenPointDelta { get; private set; }

        public Vector3 gazeWorldPosition { get; private set; }

        public Vector3 gazeWorldPositionDelta { get; private set; }

        public Vector3 inputMoveDelta { get; private set; }

        // periodic timer/coroutine
        public float screenCheckTimeout = 1.25f;
        private bool running = false;

        // event system
        private EventSystem eventSystem = null;

        public Camera gazeCamera { get; private set; }

        // independent event camera for world origin based raycasting
        public Camera eventCamera { get; private set; }
        public Vector3 eventCameraScreenCenterPoint { get; private set; }

        private GenericInputModule inputModule = null;

        // Raycasters - toggle based on which is needed for raycasting in input module
        public PhysicsRaycaster gazeCameraRaycaster { get; private set; }
        public PhysicsRaycaster eventCameraRaycaster { get; private set; }

        public bool IsModuleSupported() 
        { 
            return this.eventSystem != null && this.inputModule != null && this.stateDataCollectors.Length > 0; 
        }

        public bool ShouldActivate()
        {
            var shouldActivate = false;

            shouldActivate |= ShouldSubmit();
            Vector2 movement;
            shouldActivate |= ShouldMove(out movement);
            //shouldActivate |= ScreenPointDelta.sqrMagnitude > MoveAmountThreshold;
            //shouldActivate |= AnyButtonsDownThisFrame();
            shouldActivate |= IsModuleSupported();

            return shouldActivate;
        }

        public bool ShouldSubmit() { return false; }
        public bool ShouldCancel() { return false; }
        public bool ShouldMove(out Vector2 movement) 
        {
            movement = this.inputMoveDelta; // TODO: accumulate all movemnt vectors from inputs

            return !Mathf.Approximately(movement.sqrMagnitude, 0.0f);
        }

        protected override void InternalAwake()
        {
            this.gazeCamera = Camera.main;
            if (this.gazeCamera == null)
            {
                Debug.LogError("Could not find a camera that is tagged as \"Main Camera\"");

                return;
            }

            UpdateDataCollectors();

            this.eventSystem = GetComponent<EventSystem>();
            this.inputModule = GetComponent<GenericInputModule>();

            var eventCameraObject = new GameObject("Event System Camera");
            eventCameraObject.transform.SetParent(null);

            this.eventCamera = eventCameraObject.AddComponent<Camera>();
            this.eventCamera.orthographic = true;
            this.eventCamera.orthographicSize = 0.05f;
            this.eventCamera.aspect = 1.0f;

            // make sure this is not a stereo camera
            this.eventCamera.stereoTargetEye = StereoTargetEyeMask.None;
            this.eventCamera.clearFlags = CameraClearFlags.SolidColor;
            this.eventCamera.renderingPath = RenderingPath.Forward;
            this.eventCamera.backgroundColor = Color.magenta * 0.25f; // color that is noticeable if not set correctly
            this.eventCamera.allowMSAA = false;
            this.eventCamera.allowHDR = false;
            this.eventCamera.nearClipPlane = .001f;
            this.eventCamera.farClipPlane = this.maxDistance;
            this.eventCamera.forceIntoRenderTexture = true;
            this.eventCamera.targetTexture = new RenderTexture(1, 1, 0);

            this.eventCameraScreenCenterPoint = new Vector3(eventCamera.rect.center.x, eventCamera.rect.center.y, eventCamera.nearClipPlane);

            // associate a raycaster for this eventSystem
            this.eventCameraRaycaster = this.eventCamera.gameObject.AddComponent<PhysicsRaycaster>();
            this.eventCameraRaycaster.eventMask = this.eventCameraRaycaster.eventMask & ~(1 << LayerMask.NameToLayer("Ignore Raycast"));
            this.eventCameraRaycaster.enabled = false;

            // add physics raycaster to Main camera.
            this.gazeCameraRaycaster = this.gazeCamera.GetComponent<PhysicsRaycaster>();
            if (this.gazeCameraRaycaster == null)
            {
                Debug.LogWarning("Adding Physics Raycaster(with Ignore Raycast mask) to Main camera. To remove this warning, add a Physics Raycaster component to the main camera.");

                this.gazeCameraRaycaster = this.gazeCamera.gameObject.AddComponent<PhysicsRaycaster>();
                this.gazeCameraRaycaster.eventMask = this.eventCameraRaycaster.eventMask;
                this.gazeCameraRaycaster.enabled = false;
            }

            CheckForScreenSpaceChanges(this.gazeCamera);
        }

        protected override void InternalOnDestroy()
        {
        }

        private void OnEnable()
        {
            StartCoroutine(PeriodicUpdate());
        }

        private void OnDisable()
        {
            this.running = false;
        }

        private void Update()
        {
            // need to recalculate world position based on camera movement
            var lastPosition = this.gazeWorldPosition;

            this.gazeWorldPosition
                = this.gazeCamera.ViewportToWorldPoint(this.viewportCenter);

            this.gazeWorldPositionDelta = this.gazeWorldPosition - lastPosition;

            this.gazeScreenPointDelta =
                this.gazeCamera.WorldToScreenPoint(this.gazeWorldPositionDelta) - this.gazeCamera.WorldToScreenPoint(lastPosition);
        }

        public void UpdateDataCollectors()
        {
            var enabledCollectors = GetComponentsInChildren<StateData>(false);
            if (enabledCollectors == null)
                return;

            if (this.stateDataCollectors == null)
                this.stateDataCollectors = new StateDataList();

            if (this.stateDataCollectors.Length != enabledCollectors.Length)
                this.stateDataCollectors.items.Clear();

            int id = 0;
            foreach (var collector in enabledCollectors)
            {
                collector.id = id++;

                if (this.stateDataCollectors.items.Contains(collector))
                {
                    continue;
                }

                this.stateDataCollectors.items.Add(collector);
            }
        }

        public StateData GetModuleOfType(Type type)
        {
            StateData stateData = null;

            foreach (var item in this.stateDataCollectors.items)
            {
                if (item.GetType() == type)
                {
                    stateData = item;
                    break;
                }
            }

            return stateData;
        }

        private IEnumerator PeriodicUpdate()
        {
            this.running = true;

            var firstRun = true;

            while (this.running)
            {
                CheckForScreenSpaceChanges(this.gazeCamera);

                if (firstRun)
                {
                    yield return new WaitForEndOfFrame();

                    firstRun = false;
                }
                else
                {
                    yield return new WaitForSeconds(this.screenCheckTimeout);
                }
            }
        }

        public void CheckForScreenSpaceChanges(Camera camera)
        {
            if (camera == null)
                return;

            var rect = camera.rect;
            this.viewportSize = new Vector3(rect.width, rect.height, camera.nearClipPlane);
            this.viewportCenter = new Vector3(rect.center.x, rect.center.y, camera.nearClipPlane);

            var pixelRect = camera.pixelRect;
            if (this.screenSize.x != pixelRect.width || this.screenSize.y != pixelRect.height)
            {
                this.screenSize = new Vector2(pixelRect.width, pixelRect.height);
                this.gazeScreenPoint = this.screenSize * 0.5f;
            }
        }

        public bool RegisterWorldCanvas(Canvas canvas)
        {
            if (inputModule == null)
                return false;

            return inputModule.RegisterWorldCanvas(canvas);
        }

        public bool UnRegisterWorldCanvas(Canvas canvas)
        {
            if (inputModule == null)
                return false;

            return inputModule.UnRegisterWorldCanvas(canvas);
        }
    }
}