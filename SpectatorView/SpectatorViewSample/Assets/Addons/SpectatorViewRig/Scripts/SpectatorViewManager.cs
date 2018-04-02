// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

namespace SpectatorView
{
    public class SpectatorViewManager : SV_Singleton<SpectatorViewManager>
    {
#if UNITY_EDITOR
        #region DLLImports
        [DllImport("UnityCompositorInterface")]
        private static extern void GetPose(int nsPast, out Vector3 pos, out Quaternion rot);

        [DllImport("UnityCompositorInterface")]
        private static extern void SetSpectatorViewIP(string ip);

        [DllImport("UnityCompositorInterface")]
        private static extern bool InitializeFrameProvider();

        [DllImport("UnityCompositorInterface")]
        private static extern void UpdateCompositor();

        [DllImport("UnityCompositorInterface")]
        private static extern void StopFrameProvider();

        [DllImport("UnityCompositorInterface")]
        private static extern void ResetSV();

        [DllImport("UnityCompositorInterface")]
        private static extern void SetAudioData(byte[] audioData);

        [DllImport("UnityCompositorInterface")]
        private static extern bool IsRecording();

        [DllImport("UnityCompositorInterface")]
        private static extern void StopRecording();
        #endregion

        [Tooltip("Hologram transparency.")]
        [Range(0, 1)]
        public float Alpha = 0.95f;
        private float prevAlpha;

        [Tooltip("Manual additional time offset to compensate for any latency in the capture pipeline.")]
        [Range(0, 1)]
        public float AdditionalTimeOffsetSeconds = 0;

        [Tooltip("IP of the spectator view device.")]
        public string SpectatorViewHoloLensIP;

        [Header("DEBUG")]
        [Tooltip("Debug flag to simulate low app framerate to check that holograms remain stable in the compositor even under load.")]
        public bool SimulateLowFramerate = false;

        const int S2NS = 1000000000;

        Vector3 pos = Vector3.zero;
        Quaternion rot = Quaternion.identity;

        [HideInInspector]
        public bool frameProviderInitialized = false;

        private void Awake()
        {
            // Remove the default audio listener, there can only be one at a time.
            if (Camera.main != null)
            {
                AudioListener listener = Camera.main.GetComponent<AudioListener>();
                if (listener != null)
                {
                    listener.enabled = false;
                }
            }
        }

        private void OnEnable()
        {
            SetSpectatorViewIP(SpectatorViewHoloLensIP.Trim());
            prevAlpha = Alpha;
        }

        private void OnValidate()
        {
            if (ShaderManager.Instance != null &&
                ShaderManager.Instance.AllShadersAndTexturesReady())
            {
                if (Alpha != prevAlpha)
                {
                    ShaderManager.Instance.alphaBlendPreviewMat.SetFloat("_Alpha", Alpha);
                }
                prevAlpha = Alpha;
            }
        }

        private void OnDestroy()
        {
            ResetCompositor();
        }

        public void ResetCompositor()
        {
            StopFrameProvider();

            if (IsRecording())
            {
                StopRecording();
            }

            if (ShaderManager.Instance != null)
            {
                ShaderManager.Instance.Reset();
            }

            ResetSV();
        }

        void Update()
        {
            //DEBUG: Simulate low frame rate.
            if (SimulateLowFramerate)
            {
                System.Threading.Thread.Sleep(40);
            }

            GetPose((int)(AdditionalTimeOffsetSeconds * S2NS), out pos, out rot);

            gameObject.transform.position = pos;
            gameObject.transform.rotation = rot;

            if (!frameProviderInitialized)
            {
                frameProviderInitialized = InitializeFrameProvider();
            }

            UpdateCompositor();
        }

        // Send audio data to Compositor.
        void OnAudioFilterRead(float[] data, int channels)
        {
            Byte[] audioBytes = new Byte[data.Length * 2];

            for (int i = 0; i < data.Length; i++)
            {
                // Rescale float to short range for encoding.
                short audioEntry = (short)(data[i] * short.MaxValue);
                BitConverter.GetBytes(audioEntry).CopyTo(audioBytes, i * 2);
            }

            SetAudioData(audioBytes);
        }
#endif
    }
}
