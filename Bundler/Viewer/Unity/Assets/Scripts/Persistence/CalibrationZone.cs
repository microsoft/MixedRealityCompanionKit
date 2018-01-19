// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using System;

namespace Persistence
{
    [RequireComponent(typeof(AnchorPersistence))]
    public class CalibrationZone : MonoBehaviour
    {
        public event Action<CalibrationZone> CalibrationStarted;
        public event Action<CalibrationZone> CalibrationComplete;

        [Tooltip("Root transform to align. Leave null to use current transform.")]
        public Transform TargetTransform = null;

        [HideInInspector]
        public bool IsCalibrated { get { return persistence.IsAnchored; } }
        [HideInInspector]
        public bool IsCalibrating = false;

        private Vector3 origPosition;
        private Quaternion origRotation;
        private Vector3 origScale;

        private AnchorPersistence persistence;
        
        void Awake()
        {
            TargetTransform = TargetTransform == null ? gameObject.transform : TargetTransform;
            origPosition = TargetTransform.localPosition;
            origRotation = TargetTransform.localRotation;
            origScale = TargetTransform.localScale;
        }

        void Start()
        {
            persistence = GetComponent<AnchorPersistence>();
            persistence.TargetGameObject = TargetTransform.gameObject;
            persistence.AnchorLoaded += Persistence_AnchorLoaded;

            Persistence_AnchorLoaded();
        }

        private void Persistence_AnchorLoaded()
        {
        }

        public void AlignZone()
        {
            if (IsCalibrating)
            {
                Debug.Log("AlignZone failed - already calibrating: " + this.name);
                return;
            }

            IsCalibrating = true;
            Debug.Log("AlignZone: " + this.name);
            
            ClearAnchor(false);
            if (this.CalibrationStarted != null)
            {
                this.CalibrationStarted.Invoke(this);
            }
        }

        public void LockZone(bool placeAnchor, bool saveAnchor)
        {
            if (!IsCalibrating)
            {
                Debug.Log("LockZone failed - not calibrating: " + this.name);
                return;
            }

            Debug.Log("LockZone: " + this.name);

            if (placeAnchor)
            {
                PlaceAnchor(saveAnchor);
            }

            IsCalibrating = false;

            if (this.CalibrationComplete != null)
            {
                this.CalibrationComplete.Invoke(this);
            }
        }

        public void PlaceAnchor(bool saveAnchor)
        {
            persistence.PlaceAnchor(saveAnchor);
        }
        
        public bool ClearAnchor(bool removeSavedLocation)
        {
            var ret =  persistence.ClearAnchor(removeSavedLocation);
            Persistence_AnchorLoaded();
            return ret;
        }

        public void ResetTransform(bool removeSavedLocation = false)
        {
            ClearAnchor(removeSavedLocation);
            TargetTransform.localPosition = origPosition;
            TargetTransform.localRotation = origRotation;
            TargetTransform.localScale = origScale;
        }
    }
}