// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using Showcases.Input;
using UnityEngine;

namespace HUD
{
    [RequireComponent(typeof(MeshRenderer))]
    public class GazeCursor : MonoBehaviour
    {
        public float defaultDistance = 2.0f;
        public float fadeDuration = 1.0f;

        private MeshRenderer objectRenderer;

        private GestureStateData gestureStateData;

        public float distance = 1.0f;

        private void Awake()
        {
            this.objectRenderer = GetComponent<MeshRenderer>();
            this.gestureStateData = InputStateManager.Instance.GetModuleOfType(typeof(GestureStateData)) as GestureStateData;
        }

        private void Update()
        {
            objectRenderer.enabled = this.gestureStateData != null;
            if (!objectRenderer.enabled)
                return;

            Camera gazeCamera = InputStateManager.Instance.gazeCamera;

            UnityEngine.EventSystems.RaycastResult result = this.gestureStateData.RaycastResult;

            var lastPosition = this.transform.position;
            var forward = -gazeCamera.transform.forward;
            var position = InputStateManager.Instance.gazeWorldPosition + (-forward * distance);

            if (result.gameObject != null)
            {
                position = result.worldPosition;
                forward = result.worldNormal;
                this.distance = (result.distance > 1.0f) ? result.distance : 1.0f;
            }

            var t = Time.deltaTime * 20.0f;

            gameObject.transform.position = Vector3.Lerp(this.transform.position, position, t);
            gameObject.transform.rotation = Quaternion.Slerp(this.transform.rotation, Quaternion.LookRotation(forward), t);
        }
    }
}
