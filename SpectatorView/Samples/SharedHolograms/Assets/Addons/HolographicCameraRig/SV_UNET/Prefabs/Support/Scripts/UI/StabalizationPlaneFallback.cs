// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using HoloToolkit.Unity.InputModule;
using HoloToolkit.Unity;
#if UNITY_2017_2_OR_NEWER
using UnityEngine.XR.WSA;
#else
using UnityEngine.VR.WSA;
#endif

namespace SpectatorView
{
    public class StabalizationPlaneFallback : MonoBehaviour
    {

        public float VisibilitySafeFactor = 0.1f;
        // Use this for initialization
        void Start()
        {
            if (GazeManager.Instance != null && GazeManager.Instance.GetComponent<StabilizationPlaneModifier>() != null)
            {
                Debug.Log("Already setting plane, don't need the fall back");
                Destroy(this);
            }
        }

        // Update is called once per frame
        void Update()
        {
            if (IsTargetVisible())
            {
                HolographicSettings.SetFocusPointForFrame(transform.position, -Camera.main.transform.forward);
            }
        }

        private bool IsTargetVisible()
        {
            // This will return true if the target's mesh is within the Main Camera's view frustums.
            Vector3 targetViewportPosition = Camera.main.WorldToViewportPoint(gameObject.transform.position);
            return (targetViewportPosition.x > VisibilitySafeFactor && targetViewportPosition.x < 1 - VisibilitySafeFactor &&
                    targetViewportPosition.y > VisibilitySafeFactor && targetViewportPosition.y < 1 - VisibilitySafeFactor &&
                    targetViewportPosition.z > 0.1f);
        }
    }
}
