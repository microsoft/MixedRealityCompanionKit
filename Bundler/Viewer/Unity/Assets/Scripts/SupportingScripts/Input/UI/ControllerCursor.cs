// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using System;
using Showcases.Input;
using UnityEngine;

namespace HUD
{
    [RequireComponent(typeof(Renderer))]
    public class ControllerCursor : MonoBehaviour
    {
        public SourceHandedness handedness;
        public SourceNode direction;
        public SourceNode lastDirection;

        public LineRenderer gazeRay;
        public Vector2 gazeRaySize = new Vector2(0.005f, 0.001f);
        public Material gazeMaterial;

        private Renderer meshRenderer;

        private ControllerStateData controller;

        private void Awake()
        {
            this.meshRenderer = GetComponent<Renderer>();

            var gazeGO = new GameObject();
            gazeGO.name = gameObject.name + "Gaze";
            gazeGO.transform.parent = this.transform;

            this.gazeRay = gazeGO.AddComponent<LineRenderer>();
            this.gazeRay.material = gazeMaterial;
            this.gazeRay.useWorldSpace = true;
            this.gazeRay.enabled = false;

            this.controller = InputStateManager.Instance.GetModuleOfType(typeof(ControllerStateData)) as ControllerStateData;
        }

        void Update()
        {
            if (this.controller == null)
            {
                meshRenderer.enabled = false;
                this.gazeRay.enabled = false;

                return;
            }

            if (this.controller.activeSelectionHand == handedness)
            {
                if (direction != this.controller.activeSourceNode)
                {
                    lastDirection = direction;
                }

                direction = this.controller.activeSourceNode;
            }
            else
            {
                direction = lastDirection;
            }

            meshRenderer.enabled = this.controller.HasData(this.handedness, false); ;
            if (!meshRenderer.enabled)
                return;

            var position = this.controller.GetPosition(this.handedness, direction);
            if (position != null && position.HasValue)
            {
                transform.position = position.Value;
            }

            var rotation = this.controller.GetRotation(this.handedness, direction);
            if (rotation != null && rotation.HasValue)
            {
                transform.rotation = rotation.Value;
            }

            this.gazeRay.enabled = this.controller.IsActiveHand(this.handedness);
            if (this.gazeRay.enabled)
            {
                var result = this.controller.RaycastResult;
                var distance = result.gameObject != null ? result.distance : 1.0f;
                var targetPosition = transform.position + (transform.forward * distance);

                this.gazeRay.startWidth = gazeRaySize.x;
                this.gazeRay.endWidth = gazeRaySize.y;
                this.gazeRay.SetPosition(0, transform.position);
                this.gazeRay.SetPosition(1, targetPosition);
            }
        }
    }

}