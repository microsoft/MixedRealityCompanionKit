// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

namespace SpectatorView
{
    public enum AntiAliasLevel
    {
        One = 1,
        Two = 2,
        Four = 4,
        Eight = 8
    };

    public class SpectatorViewCamera : SV_Singleton<SpectatorViewCamera>
    {
#if UNITY_EDITOR
        #region DLLImports
        [DllImport("UnityCompositorInterface")]
        private static extern IntPtr GetRenderEventFunc();

        [DllImport("UnityCompositorInterface")]
        private static extern int GetFrameWidth();

        [DllImport("UnityCompositorInterface")]
        private static extern int GetFrameHeight();
        #endregion

        [Header("Hologram resolution settings.")]
        public AntiAliasLevel antiAliasLevel = AntiAliasLevel.Four;
        [Range(1, 16)]
        public int anisotropicFilteringLevel = 16;
        public FilterMode textureFilterMode = FilterMode.Bilinear;

        [HideInInspector]
        public RenderTexture renderTexture;

        void OnEnable()
        {
            StartCoroutine("CallPluginAtEndOfFrames");
        }

        private void OnDisable()
        {
        }

        void Awake()
        {
            Camera camera = GetComponent<Camera>();
            if (camera != null)
            {
                SetupCamera(camera, GetFrameWidth(), GetFrameHeight(), out renderTexture);
            }
        }

        private void SetupCamera(Camera camera, int rtWidth, int rtHeight, out RenderTexture renderTexture)
        {
            // Setup camera and render texture.
            if (camera == null)
            {
                renderTexture = null;
                return;
            }

            renderTexture = new RenderTexture(rtWidth, rtHeight, 0);
            renderTexture.antiAliasing = (int)antiAliasLevel;
            renderTexture.anisoLevel = (int)anisotropicFilteringLevel;
            renderTexture.filterMode = textureFilterMode;
            camera.targetTexture = renderTexture;

            camera.nearClipPlane = 0.01f;
            camera.clearFlags = CameraClearFlags.SolidColor;
            camera.backgroundColor = new Color(0, 0, 0, 0);

            camera.enabled = true;
        }

        private IEnumerator CallPluginAtEndOfFrames()
        {
            while (true)
            {
                // Wait until all frame rendering is done
                yield return new WaitForEndOfFrame();

                // Issue a plugin event with arbitrary integer identifier.
                // The plugin can distinguish between different
                // things it needs to do based on this ID.
                // For our simple plugin, it does not matter which ID we pass here.
                GL.IssuePluginEvent(GetRenderEventFunc(), 1);
            }
        }
#endif
    }
}
