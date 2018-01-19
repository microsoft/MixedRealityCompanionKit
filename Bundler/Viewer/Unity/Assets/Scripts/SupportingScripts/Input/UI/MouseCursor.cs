// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using Showcases.Input;
using UnityEngine;

namespace HUD
{
    [RequireComponent(typeof(MeshRenderer))]
    public class MouseCursor : MonoBehaviour
    {
        private MeshRenderer objectRenderer;
        private MouseStateData mouseStateData;
        private float distance = 0.0f;

        private void Awake()
        {
            this.objectRenderer = GetComponent<MeshRenderer>();
            this.mouseStateData = InputStateManager.Instance.GetModuleOfType(typeof(MouseStateData)) as MouseStateData;
        }

        private void Update()
        {
            objectRenderer.enabled = this.mouseStateData != null;
            if (!objectRenderer.enabled)
                return;

            Camera gazeCamera = InputStateManager.Instance.gazeCamera;

            var lastPosition = gameObject.transform.position;

            UnityEngine.EventSystems.RaycastResult result = this.mouseStateData.RaycastResult;

            var forward = -gazeCamera.transform.forward;
            var position = this.mouseStateData.WorldPosition;

            if (result.gameObject != null)
            {
                forward = result.worldNormal;
                position = result.worldPosition;

                this.distance = (result.distance > 1.0f) ? result.distance : 1.0f;
            }
            else
            {
                var direction = (position - gazeCamera.transform.position).normalized;
                position = gazeCamera.transform.position + (direction * distance);
            }

            var t = Time.deltaTime * 20.0f;

            gameObject.transform.position = Vector3.Lerp(this.transform.position, position, t);
            gameObject.transform.rotation = Quaternion.Slerp(this.transform.rotation, Quaternion.LookRotation(forward), t);
        }

        private bool IsMouseActive
        {
            get
            {
                return
                    (this.mouseStateData != null) && (this.mouseStateData.ScreenPointDelta.sqrMagnitude != 0.0f)
                    ||
                    (this.mouseStateData != null) && this.mouseStateData.AnyButtonsDownThisFrame();
            }
        }
    }
}
