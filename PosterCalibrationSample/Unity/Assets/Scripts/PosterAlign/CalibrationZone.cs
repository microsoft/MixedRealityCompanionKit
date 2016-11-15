// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using UnityEngine;
using UnityEngine.VR.WSA;
using System;
namespace PosterAlignment
{
    [RequireComponent(typeof(AutoAlignToPoster))]
    [RequireComponent(typeof(AnchorPersistence))]
    public class CalibrationZone : MonoBehaviour
    {
        public event Action<CalibrationZone> CalibrationStarted;
        public event Action<CalibrationZone> CalibrationComplete;

        [HideInInspector]
        public bool IsCalibrated = false;
        [HideInInspector]
        public bool IsCalibrating = false;
        public bool HidePosterWhenCalibrated = false;

        private Vector3 origPosition;
        private Quaternion origRotation;
        private Vector3 origScale;

        private AutoAlignToPoster aligner;
        private AnchorPersistence persistence;

        void Awake()
        {
            origPosition = gameObject.transform.localPosition;
            origRotation = gameObject.transform.localRotation;
            origScale = gameObject.transform.localScale;
        }

        void Start()
        {
            persistence = GetComponent<AnchorPersistence>();
            persistence.AnchorLoaded += Persistence_AnchorLoaded;
            Persistence_AnchorLoaded();
            aligner = GetComponent<AutoAlignToPoster>();
            aligner.enabled = false;
        }

        private void Persistence_AnchorLoaded()
        {
            if (persistence.IsAnchored && HidePosterWhenCalibrated)
                aligner.Poster.SetActive(false);
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
            aligner.Poster.SetActive(true);
            aligner.enabled = true;
            aligner.StartProcessing();
            ClearAnchor(false);
            if (this.CalibrationStarted != null)
            {
                this.CalibrationStarted.Invoke(this);
            }
        }

        public void LockZone(bool placeAnchor)
        {
            if (!IsCalibrating)
            {
                Debug.Log("LockZone failed - not calibrating: " + this.name);
                return;
            }

            Debug.Log("LockZone: " + this.name);
            if (placeAnchor)
            {
                SaveAnchor();
            }
            IsCalibrating = false;
            IsCalibrated = true;
            aligner.StopProcessing();
            aligner.enabled = false;
            if (HidePosterWhenCalibrated)
                aligner.Poster.SetActive(false);

            if (this.CalibrationComplete != null)
            {
                this.CalibrationComplete.Invoke(this);
            }
        }

        public bool SaveAnchor()
        {
            return persistence.AnchorAndSave();
        }

        public bool ClearAnchor(bool removeSavedLocation)
        {
            if (removeSavedLocation)
                return persistence.ClearAnchor();

            if (GetComponent<WorldAnchor>() != null)
                DestroyImmediate(GetComponent<WorldAnchor>());
            return true;
        }


        public void ResetTransform(bool removeSavedLocation = false)
        {
            ClearAnchor(removeSavedLocation);
            gameObject.transform.localPosition = origPosition;
            gameObject.transform.localRotation = origRotation;
            gameObject.transform.localScale = origScale;
            IsCalibrated = false;
        }
    }
}