// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using UnityEngine;
using System.Collections.Generic;
using System;

namespace PosterAlignment
{
    public class ZoneCalibrationManager : MonoBehaviour
    {
        [Tooltip("Multiple calibration zones, or set to 0 if this manager is on the zone to calibrate already.")]
        public List<CalibrationZone> Zones;
        [Tooltip("The overlay that shows the current camera feed and the active poster.")]
        public GameObject CalibrationOverlay;

        [HideInInspector]
        public CalibrationZone currentZone;

        void Start()
        {
            if (Zones.Count == 0)
            {
                /// If we don't have any zones, check to see if it is on this node.
                /// This is helpful for single zone scenes, but where you still want the manager running.
                if (GetComponent<CalibrationZone>() != null)
                {
                    Zones.Add(GetComponent<CalibrationZone>());
                }
            }
            foreach (var zone in Zones)
            {
                if (zone != null)
                {
                    zone.CalibrationStarted += Zone_CalibrationStarted;
                    zone.CalibrationComplete += Zone_CalibrationComplete;
                }
            }

            ToggleOverlay(false);
        }

        void OnDestroy()
        {
            foreach (var zone in Zones)
            {
                if (zone != null)
                {
                    zone.CalibrationStarted -= Zone_CalibrationStarted;
                    zone.CalibrationComplete -= Zone_CalibrationComplete;
                }
            }
        }

        private void Zone_CalibrationComplete(CalibrationZone zone)
        {
            currentZone = null;
            ToggleOverlay(false);
        }

        private void Zone_CalibrationStarted(CalibrationZone zone)
        {
            if (zone != currentZone)
            {
                // lock the previous zone but don't write an anchor if they didn't say "lock"
                LockZone(false);
            }
            currentZone = zone;
            ToggleOverlay(true);
        }

        public void ToggleOverlay(bool? force = null)
        {
            if (CalibrationOverlay == null)
                return;

            if (force != null)
                CalibrationOverlay.SetActive((bool)force);
            else
                CalibrationOverlay.SetActive(!CalibrationOverlay.activeSelf);
        }

        public void AlignZone(int whichZoneIndex)
        {
            whichZoneIndex = Math.Max(Math.Min(whichZoneIndex, Zones.Count - 1), 0);
            AlignZone(Zones[whichZoneIndex]);
        }

        public void AlignZone(CalibrationZone whichZone)
        {
            // Check to see if they are currently calibrating another zone and skip this, or
            // if they are calibrating the same zone, send the command down so the zone can
            // start over or some other desired behavior.
            if (currentZone == null || !currentZone.IsCalibrating || whichZone == currentZone)
            {
                whichZone.AlignZone();
            }
        }

        public void LockZone(bool placeAnchor)
        {
            if (currentZone == null)
                return;

            currentZone.LockZone(placeAnchor);
            ToggleOverlay(false);
        }
    }
}