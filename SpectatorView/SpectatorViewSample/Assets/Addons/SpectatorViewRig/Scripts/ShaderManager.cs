// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;
using System;

namespace SpectatorView
{
    public class ShaderManager : SV_Singleton<ShaderManager>
    {
#if UNITY_EDITOR
        #region DLLImports
        [DllImport("UnityCompositorInterface")]
        private static extern int GetFrameWidth();

        [DllImport("UnityCompositorInterface")]
        private static extern int GetFrameHeight();

        [DllImport("UnityCompositorInterface")]
        private static extern bool CreateUnityColorTexture(out IntPtr srv);

        [DllImport("UnityCompositorInterface")]
        private static extern bool SetOutputRenderTexture(IntPtr texturePtr);

        [DllImport("UnityCompositorInterface")]
        private static extern bool SetVideoRenderTexture(IntPtr texturePtr);

        [DllImport("UnityCompositorInterface")]
        private static extern bool OutputYUV();
        #endregion

        [HideInInspector]
        public Material passthroughColorMat;
        [HideInInspector]
        public Material alphaBlendPreviewMat;
        [HideInInspector]
        public Material yuv2rgbMat;
        [HideInInspector]
        public Material compositeOutputMat;
        [HideInInspector]
        public Material compositeVideoMat;

        [HideInInspector]
        public Texture2D colorTexture = null;
        [HideInInspector]
        public RenderTexture outputTexRT = null;
        [HideInInspector]
        public RenderTexture previewTexRT = null;
        [HideInInspector]
        public RenderTexture videoTexRT = null;
        [HideInInspector]
        public RenderTexture convertedColor = null;

        public static IntPtr colorSRV;

        bool setOutputTex = false;
        bool setVideoTex = false;
        bool setShaderTextures = false;

        [HideInInspector]
        bool allShadersAndTexturesReady = false;

        bool YUV = true;
        bool prevYUV = true;

        private void Awake()
        {
            passthroughColorMat = Resources.Load<Material>("Materials/PassthroughColor");
            alphaBlendPreviewMat = Resources.Load<Material>("Materials/AlphaBlendPreview");
            yuv2rgbMat = Resources.Load<Material>("Materials/Yuv2Rgb");
            compositeOutputMat = Resources.Load<Material>("Materials/CompositeOutput");
            compositeVideoMat = Resources.Load<Material>("Materials/CompositeVideo");

            convertedColor = new RenderTexture(GetFrameWidth(), GetFrameHeight(), 0, RenderTextureFormat.Default);
            outputTexRT = new RenderTexture(GetFrameWidth(), GetFrameHeight(), 0, RenderTextureFormat.Default);
            previewTexRT = new RenderTexture(GetFrameWidth(), GetFrameHeight(), 0, RenderTextureFormat.Default);
            videoTexRT = new RenderTexture(GetFrameWidth(), GetFrameHeight(), 0, RenderTextureFormat.Default);
        }

        public void Reset()
        {
            colorTexture = null;

            setOutputTex = false;
            setVideoTex = false;

            setShaderTextures = false;
            allShadersAndTexturesReady = false;
        }

        private void Update()
        {
            SetColorFrame();
            SetOutputFrame();
            CheckYUVFlags();

            SetShaderValues();
            BlitTextures();
        }

        public bool AllShadersAndTexturesReady()
        {
            if (allShadersAndTexturesReady)
            {
                return true;
            }

            // First check singletons
            if (SpectatorViewManager.Instance == null
                || SpectatorViewCamera.Instance == null)
            {
                return false;
            }

            //TODO: Add to this list if new textures or shaders are introduced.
            allShadersAndTexturesReady =
                // Textures:
                SpectatorViewCamera.Instance.renderTexture != null
                && colorTexture != null
                && convertedColor != null
                && outputTexRT != null
                && previewTexRT != null
                && videoTexRT != null

                // Materials:
                && passthroughColorMat != null
                && alphaBlendPreviewMat != null
                && yuv2rgbMat != null
                && compositeOutputMat != null
                && compositeVideoMat != null;

            return allShadersAndTexturesReady;
        }

        public void SetYUVFlags()
        {
            if (!AllShadersAndTexturesReady())
            {
                return;
            }

            yuv2rgbMat.SetInt("_YUV", YUV ? 1 : 0);
        }

        public void CheckYUVFlags()
        {
            if (!AllShadersAndTexturesReady())
            {
                return;
            }

            prevYUV = YUV;
            YUV = OutputYUV();

            if (YUV != prevYUV)
            {
                SetYUVFlags();
            }
        }

        public void SetShaderValues()
        {
            // Shader values have already been set.
            if (setShaderTextures)
            {
                return;
            }

            // Textures have not been initialized yet.
            if (!AllShadersAndTexturesReady())
            {
                return;
            }

            // Main textures may not need to be explicitly set since Blit or DrawTexture should implicitly set them,
            // but this should guarantee that we can preview the shader in the material inspector.
            yuv2rgbMat.SetTexture("_MainTex", colorTexture);
            alphaBlendPreviewMat.SetTexture("_MainTex", convertedColor);
            compositeOutputMat.SetTexture("_MainTex", previewTexRT);
            compositeVideoMat.SetTexture("_MainTex", previewTexRT);

            // Set holo texture
            alphaBlendPreviewMat.SetTexture("_HoloTex", SpectatorViewCamera.Instance.renderTexture);

            // Set Texture dimensions
            yuv2rgbMat.SetInt("_Width", GetFrameWidth());
            compositeVideoMat.SetInt("_Width", GetFrameWidth());
            compositeVideoMat.SetInt("_Height", GetFrameHeight());

            // Set yuv flags on shaders to match input color format from capture pipeline.
            SetYUVFlags();

            setShaderTextures = true;
        }

        private void BlitTextures()
        {
            if (!AllShadersAndTexturesReady())
            {
                return;
            }

            // Blit YUV color texture to RGB destination so we do not need to do yuv conversion for every shader.
            Graphics.Blit(colorTexture, convertedColor, yuv2rgbMat);

            // Blit converted color to rgb preview texture for compositor window.
            Graphics.Blit(convertedColor, previewTexRT, alphaBlendPreviewMat);

            // Blit preview texture to output format texture (matching input color texture format).
            Graphics.Blit(previewTexRT, outputTexRT, compositeOutputMat);

            // Blit preview texture to NV12 video texture.
            Graphics.Blit(previewTexRT, videoTexRT, compositeVideoMat);
        }

        #region UnityExternalTextures
        /// <summary>
        /// Create External texture resources and poll for latest Color frame.
        /// </summary>
        private void SetColorFrame()
        {
            if (colorTexture == null)
            {
                // Unity 5.6 workaround:
                // Creating textures on the graphics device on the first frame deadlocks Unity 5.6.
                if (Time.frameCount > 1 && CreateUnityColorTexture(out colorSRV))
                {
                    colorTexture = Texture2D.CreateExternalTexture(GetFrameWidth(), GetFrameHeight(), TextureFormat.ARGB32, false, true, colorSRV);
                }
            }
        }

        private void SetOutputFrame()
        {
            if (outputTexRT != null && !setOutputTex)
            {
                setOutputTex = SetOutputRenderTexture(outputTexRT.GetNativeTexturePtr());
            }

            if (videoTexRT != null && !setVideoTex)
            {
                setVideoTex = SetVideoRenderTexture(videoTexRT.GetNativeTexturePtr());
            }
        }
        #endregion
    }
#endif
}
