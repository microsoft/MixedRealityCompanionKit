// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;

namespace HoloToolkit.Unity
{
    public enum PivotAxis
    {
        // Rotate about all axes.
        Free,
        // Rotate about an individual axis.
        Y
    }
    [RequireComponent(typeof(Interpolator))]
    /// <summary>
    /// The Billboard class implements the behaviors needed to keep a GameObject 
    /// oriented towards the user.
    /// </summary>
    public class Billboard : MonoBehaviour
    {
        /// <summary>
        /// The axis about which the object will rotate.
        /// </summary>
        [Tooltip("Specifies the axis about which the object will rotate.")]
        public PivotAxis PivotAxis = PivotAxis.Free;
        [Tooltip("When true, the motion is smoothed.")]
        public bool SmoothMotion = true;
        [Range(0.0f, 1.0f), Tooltip("The factor applied to the smoothing algorithm. 1.0f is super smooth. But slows things down a lot.")]
        public float SmoothingFactor = 0.75f;

        // The rotation speed, in degrees per second
        public float RotationDegreesPerSecond = 10.0f;

        // Adjusts rotation speed based on angular distance
        public float RotationSpeedScaler = 0.0f;
        
        protected Interpolator interpolator;

        private void Start()
        {
            interpolator = gameObject.GetComponent<Interpolator>();
            if (interpolator != null)
            {
                interpolator.SmoothLerpToTarget = SmoothMotion;
                interpolator.SmoothRotationLerpRatio = SmoothingFactor * .1f;
                interpolator.RotationDegreesPerSecond = RotationDegreesPerSecond;
                interpolator.RotationSpeedScaler = RotationSpeedScaler;
            }
        }

        private void OnEnable()
        {
            Update();
        }

        /// <summary>
        /// Keeps the object facing the camera.
        /// </summary>
        private void Update()
        {
            if (!Camera.main)
            {
                return;
            }

            // Get a Vector that points from the target to the main camera.
            Vector3 directionToTarget = Camera.main.transform.position - transform.position;

            // Adjust for the pivot axis.
            switch (PivotAxis)
            {
                case PivotAxis.Y:
                    directionToTarget.y = 0.0f;
                    break;
                case PivotAxis.Free:
                default:
                    // No changes needed.
                    break;
            }

            // If we are right next to the camera the rotation is undefined. 
            if (directionToTarget.sqrMagnitude < 0.001f)
            {
                return;
            }

            if (interpolator == null)
            {
                transform.rotation = Quaternion.LookRotation(-directionToTarget);
            }
            else
            {
                interpolator.SetTargetRotation(Quaternion.LookRotation(-directionToTarget));
            }            
        }
    }
}