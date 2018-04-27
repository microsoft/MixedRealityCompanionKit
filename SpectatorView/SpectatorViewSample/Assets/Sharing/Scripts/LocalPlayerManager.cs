// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

#if UNITY_WSA
using UnityEngine.XR.WSA.Input;
using UnityEngine.Windows.Speech;
#endif

namespace SimpleSharing
{
    public class LocalPlayerManager : MonoBehaviour
    {
#if WINDOWS_UWP
        GestureRecognizer gr;

        // Simple example showing how to respond to remote user's interaction.
        // See RemotePlayerManager.
        GameObject placedObject = null;

        KeywordRecognizer kr;

        void Start()
        {
            // Send pose data less frequently than Update frequency.
            // Sending data too frequently will saturate the socket and force a disconnect.
            InvokeRepeating("SendTransform", 0, 0.1f);

            gr = new GestureRecognizer();
            gr.SetRecognizableGestures(GestureSettings.Tap);
            gr.Tapped += Tapped;
            gr.StartCapturingGestures();

            kr = new KeywordRecognizer(new string[] {"Reset Anchor."});
            kr.OnPhraseRecognized += OnPhraseRecognized;
            kr.Start();
        }

        private void OnPhraseRecognized(PhraseRecognizedEventArgs args)
        {
            if (AnchorManager.Instance != null)
            {
                Debug.Log("Reset anchors.");
                AnchorManager.Instance.ResetAnchor();
            }
        }

        private void Tapped(TappedEventArgs e)
        {
            if (SharingManager.Instance != null)
            {
                Vector3 position = Camera.main.transform.position;
                Vector3 direction = Camera.main.transform.forward;
                RaycastHit hitInfo;
                Vector3 hitPoint = Vector3.zero;

                if (Physics.Raycast(position, direction, out hitInfo))
                {
                    hitPoint = hitInfo.point;
                }
                else
                {
                    hitPoint = position + direction * 2;
                }

                SharingManager.Instance.SendAirTap(position, direction, hitPoint);
                if (placedObject == null)
                {
                    placedObject = GameObject.CreatePrimitive(PrimitiveType.Sphere);
                    placedObject.transform.localScale = Vector3.one * 0.2f;
                }

                placedObject.transform.position = hitPoint;
            }
        }

        private void OnDestroy()
        {
            CancelInvoke("SendTransform");
            gr.StopCapturingGestures();
        }

        private void SendTransform()
        {
            if (SharingManager.Instance != null)
            {
                SharingManager.Instance.SendPose();
            }
        }
#endif
    }
}
