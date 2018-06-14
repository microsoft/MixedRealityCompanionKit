// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using System.Collections;
using HoloToolkit.Unity;
using System.Runtime.InteropServices;
using System;

namespace SpectatorView
{
    public class ShaderManager : SpectatorView.SV_Singleton<ShaderManager>
    {
#if UNITY_EDITOR
        #region DLLImports
        [DllImport("UnityCompositorInterface")]
        private static extern int GetFrameWidth();

        [DllImport("UnityCompositorInterface")]
        private static extern int GetFrameHeight();

        [DllImport("UnityCompositorInterface")]
        private static extern int GetFrameWidthHiRes();

        [DllImport("UnityCompositorInterface")]
        private static extern int GetFrameHeightHiRes();

        [DllImport("UnityCompositorInterface")]
        private static extern bool SetHoloTexture(IntPtr holoTexture);

        [DllImport("UnityCompositorInterface")]
        private static extern bool SetHoloTextureHiRes(IntPtr holoTexture);

        [DllImport("UnityCompositorInterface")]
        private static extern void SetFrameOffset(float frameOffset);

        [DllImport("UnityCompositorInterface")]
        private static extern void SetAlpha(float alpha);

        [DllImport("UnityCompositorInterface")]
        private static extern float GetAlpha();

        [DllImport("UnityCompositorInterface")]
        private static extern bool CreateUnityColorTexture(out IntPtr srv);

        [DllImport("UnityCompositorInterface")]
        private static extern bool CreateUnityHoloTexture(out IntPtr srv);

        [DllImport("UnityCompositorInterface")]
        private static extern bool SetMergedRenderTexture(IntPtr texturePtr);

        [DllImport("UnityCompositorInterface")]
        private static extern bool SetVideoRenderTexture(IntPtr texturePtr);

        [DllImport("UnityCompositorInterface")]
        private static extern bool SetOutputRenderTexture(IntPtr texturePtr);

        [DllImport("UnityCompositorInterface")]
        private static extern bool IsRecording();

        [DllImport("UnityCompositorInterface")]
        private static extern bool OutputYUV();

        [DllImport("UnityCompositorInterface")]
        private static extern bool CaptureHiResHolograms();

        [DllImport("UnityCompositorInterface")]
        private static extern bool QueueingHoloFrames();

        [DllImport("UnityCompositorInterface")]
        private static extern bool HardwareEncodeVideo();
        #endregion

        [HideInInspector]
        public Material alphaMat;
        [HideInInspector]
        public Material alphaBlendVideoMat;
        [HideInInspector]
        public Material alphaBlendPreviewMat;
        [HideInInspector]
        public Material flipHorizontallyMat;
        [HideInInspector]
        public Material alphaBlendOutputMat;

        [HideInInspector]
        public Texture2D holoTexture = null;
        [HideInInspector]
        public Texture2D alphaTexture = null;
        [HideInInspector]
        public RenderTexture videoTexRT = null;
        [HideInInspector]
        public RenderTexture outputTexRT = null;

        [HideInInspector]
        public Texture2D colorTexture = null;
        [HideInInspector]
        public bool setHoloTexture = false;
        [HideInInspector]
        public bool setHiResHoloTexture = false;
        [HideInInspector]
        public bool setVideoTexture = false;
        [HideInInspector]
        public bool setAlphaBlendPreviewHoloTex = false;
        [HideInInspector]
        public bool setOutputTex = false;
        bool setShaderTextures = false;

        public static IntPtr colorSRV;
        public static IntPtr holoSRV;

        private bool prevOutputYUV;

        Camera _camera;
        Camera _hiResCamera;
        [HideInInspector]
        public RenderTexture renderTexture;
        [HideInInspector]
        public RenderTexture _hiResRenderTexture;

        void OnEnable()
        {
            prevOutputYUV = OutputYUV();

            alphaMat = (Material)Resources.Load<Material>("Materials/ExtractAlpha");
            alphaBlendPreviewMat = (Material)Resources.Load<Material>("Materials/AlphaBlendPreview");
            alphaBlendVideoMat = (Material)Resources.Load<Material>("Materials/AlphaBlendVideo");
            flipHorizontallyMat = (Material)Resources.Load<Material>("Materials/FlipHorizontally");
            alphaBlendOutputMat = (Material)Resources.Load<Material>("Materials/AlphaBlendOutput");

            if (alphaMat == null)
            {
                Debug.LogWarning("ExtractAlpha material was not found.");
            }

            if (alphaBlendVideoMat == null)
            {
                Debug.LogWarning("alphaBlendVideo material was not found.");
            }

            if (alphaBlendPreviewMat == null)
            {
                Debug.LogWarning("alphaBlendPreview material was not found.");
            }

            if (flipHorizontallyMat == null)
            {
                Debug.LogWarning("FlipHorizontally material was not found.");
            }

            if (alphaBlendOutputMat == null)
            {
                Debug.LogWarning("alphaBlendOutputMat material was not found.");
            }

            alphaBlendVideoMat.SetFloat("_Alpha", SpectatorViewManager.Instance.DefaultAlpha);
            alphaBlendPreviewMat.SetFloat("_Alpha", SpectatorViewManager.Instance.DefaultAlpha);
            alphaBlendOutputMat.SetFloat("_Alpha", SpectatorViewManager.Instance.DefaultAlpha);

            SetFrameOffset(SpectatorViewManager.Instance.DefaultFrameOffset);
            SetAlpha(SpectatorViewManager.Instance.DefaultAlpha);

            alphaBlendVideoMat.SetFloat("_Brightness", 1.0f);
            alphaBlendPreviewMat.SetFloat("_Brightness", 1.0f);
            alphaBlendOutputMat.SetFloat("_Brightness", 1.0f);
            flipHorizontallyMat.SetFloat("_Brightness", 1.0f);
        }

        public void Reset()
        {
            colorTexture = null;
            holoTexture = null;
            alphaTexture = null;

            setHoloTexture = false;
            setHiResHoloTexture = false;
            setVideoTexture = false;
            setOutputTex = false;
            setAlphaBlendPreviewHoloTex = false;

            setShaderTextures = false;
        }

        public void EnableHolographicCamera(Transform parent)
        {
            // Enable the holographic camera, so we can start sending its frame buffer data to the compositor.
            Camera c = gameObject.GetComponent<Camera>();
            if (c == null)
            {
                c = gameObject.AddComponent<Camera>();
            }

            if (CaptureHiResHolograms())
            {
                Camera[] cameras = gameObject.GetComponentsInChildren<Camera>();
                if (cameras.Length >= 2)
                {
                    _hiResCamera = cameras[1];
                }

                if (_hiResCamera == null)
                {
                    GameObject hiResCamObject = GameObject.CreatePrimitive(PrimitiveType.Cube);
                    hiResCamObject.transform.position = Vector3.zero;
                    hiResCamObject.transform.localScale = Vector3.one;
                    hiResCamObject.transform.rotation = Quaternion.identity;

                    DestroyImmediate(hiResCamObject.GetComponent<Renderer>());
                    DestroyImmediate(hiResCamObject.GetComponent<Collider>());
                    _hiResCamera = hiResCamObject.AddComponent<Camera>();

                    hiResCamObject.transform.SetParent(gameObject.transform);
                }

                SetupCameraValues(_hiResCamera);
                _hiResCamera.enabled = true;
            }
            else
            {
                // Disable the hi-res camera game object.
                Camera[] cameras = gameObject.GetComponentsInChildren<Camera>();
                if (cameras.Length >= 2)
                {
                    cameras[1].gameObject.SetActive(false);
                }
            }

            SetupCameraValues(c);
        }

        private void SetupCamera(Camera camera, int rtWidth, int rtHeight, out RenderTexture renderTexture)
        {
            // Setup camera and render texture.
            if (camera == null)
            {
                renderTexture = null;
                return;
            }

            camera.enabled = true;

            renderTexture = new RenderTexture(rtWidth, rtHeight, (int)SpectatorViewManager.Instance.TextureDepth);
            renderTexture.antiAliasing = (int)SpectatorViewManager.Instance.AntiAliasing;
            renderTexture.anisoLevel = 0;
            renderTexture.filterMode = SpectatorViewManager.Instance.Filter;
            camera.targetTexture = renderTexture;
        }

        private void SetupCameraValues(Camera camera)
        {
            if (camera == null)
            {
                Debug.LogError("Camera is null.");
                return;
            }

            camera.nearClipPlane = 0.01f;
            camera.clearFlags = CameraClearFlags.SolidColor;
            camera.backgroundColor = new Color(0, 0, 0, 0);

            camera.fieldOfView = Calibration.Instance.DSLR_fov.y;
            camera.enabled = true;
        }

        void LateUpdate()
        {
            if (renderTexture != null && !setHoloTexture)
            {
                setHoloTexture = SetHoloTexture(renderTexture.GetNativeTexturePtr());
            }

            if (_hiResRenderTexture != null && !setHiResHoloTexture)
            {
                setHiResHoloTexture = SetHoloTextureHiRes(_hiResRenderTexture.GetNativeTexturePtr());
            }

            if (_camera == null || renderTexture == null)
            {
                _camera = GetComponent<Camera>();
                SetupCamera(_camera, GetFrameWidth(), GetFrameHeight(), out renderTexture);
            }

            if (CaptureHiResHolograms())
            {
                if (_hiResCamera == null || _hiResRenderTexture == null)
                {
                    Camera[] cameras = gameObject.GetComponentsInChildren<Camera>();
                    if (cameras.Length >= 2)
                    {
                        _hiResCamera = cameras[1];
                    }
                    SetupCamera(_hiResCamera, GetFrameWidthHiRes(), GetFrameHeightHiRes(), out _hiResRenderTexture);
                }
            }

            SetColorFrame();
            SetHoloFrame();
            SetMergedFrame();

            SetShaderValues();
            ClearAndBlitShaders();
        }

        private void SetShaderValues()
        {
            if (!setShaderTextures)
            {
                if (colorTexture != null && setHoloTexture && holoTexture != null && renderTexture != null
                    && videoTexRT != null && outputTexRT != null &&
                    alphaBlendVideoMat != null && alphaBlendOutputMat != null)
                {
                    setShaderTextures = true;

                    if (QueueingHoloFrames())
                    {
                        alphaBlendPreviewMat.SetTexture("_FrontTex", holoTexture);
                        alphaBlendVideoMat.SetTexture("_FrontTex", holoTexture);
                        alphaBlendOutputMat.SetTexture("_FrontTex", holoTexture);
                    }
                    else
                    {
                        alphaBlendPreviewMat.SetTexture("_FrontTex", renderTexture);
                        alphaBlendVideoMat.SetTexture("_FrontTex", renderTexture);
                        alphaBlendOutputMat.SetTexture("_FrontTex", renderTexture);
                    }

                    if (HardwareEncodeVideo())
                    {
                        alphaBlendVideoMat.SetInt("_NV12", 1);
                    }
                    else
                    {
                        alphaBlendVideoMat.SetInt("_NV12", 0);
                    }

                    flipHorizontallyMat.SetInt("_Width", GetFrameWidth());
                    flipHorizontallyMat.SetInt("_Height", GetFrameHeight());
                    flipHorizontallyMat.SetTexture("_MainTex", colorTexture);

                    alphaBlendPreviewMat.SetTexture("_MainTex", colorTexture);
                    alphaBlendPreviewMat.SetFloat("_Alpha", GetAlpha());
                    alphaBlendPreviewMat.SetInt("_Width", GetFrameWidth());
                    alphaBlendPreviewMat.SetInt("_Height", GetFrameHeight());

                    alphaBlendVideoMat.SetTexture("_MainTex", colorTexture);
                    alphaBlendVideoMat.SetFloat("_Alpha", GetAlpha());
                    alphaBlendVideoMat.SetInt("_Width", GetFrameWidth());
                    alphaBlendVideoMat.SetInt("_Height", GetFrameHeight());

                    alphaBlendPreviewMat.SetInt("_RGBA", 0);

                    alphaBlendVideoMat.SetInt("_FlipVideo", 1);
                    alphaBlendVideoMat.SetInt("_RGBA", 1);

                    alphaBlendOutputMat.SetTexture("_MainTex", colorTexture);
                    alphaBlendOutputMat.SetFloat("_Alpha", GetAlpha());
                    alphaBlendOutputMat.SetInt("_Width", GetFrameWidth());
                    alphaBlendOutputMat.SetInt("_Height", GetFrameHeight());

                    SetOutputShaderValues();
                }
            }

            if (prevOutputYUV != OutputYUV())
            {
                prevOutputYUV = OutputYUV();
                SetOutputShaderValues();
            }
        }

        private void ClearAndBlitShaders()
        {
            if (colorTexture != null && videoTexRT != null && outputTexRT != null &&
                alphaBlendVideoMat != null && alphaBlendOutputMat != null)
            {
                // Output texture.
                outputTexRT.DiscardContents();
                Graphics.Blit(colorTexture, outputTexRT, alphaBlendOutputMat);

                // Video texture.
                if (IsRecording() || !setVideoTexture)
                {
                    videoTexRT.DiscardContents();
                    Graphics.Blit(colorTexture, videoTexRT, alphaBlendVideoMat);
                }
            }
        }

        private void SetOutputShaderValues()
        {
            if (OutputYUV())
            {
                flipHorizontallyMat.SetInt("_YUV", 1);
                alphaBlendVideoMat.SetInt("_YUV", 1);
                alphaBlendPreviewMat.SetInt("_YUV", 1);
                alphaBlendOutputMat.SetInt("_YUV", 1);
                alphaBlendOutputMat.SetInt("_RGBA", 0);
                alphaBlendOutputMat.SetInt("_FlipVideo", 0);
                alphaBlendPreviewMat.SetInt("_FlipVideo", 1);
                alphaBlendVideoMat.SetInt("_FlipVideo", 1);

                flipHorizontallyMat.SetInt("_RGBA", 1);
                alphaBlendOutputMat.SetInt("_SwapBackgroundRB", 0);
                alphaBlendPreviewMat.SetInt("_SwapBackgroundRB", 0);
                if (HardwareEncodeVideo())
                {
                    alphaBlendVideoMat.SetInt("_SwapBackgroundRB", 1);
                }
                else
                {
                    alphaBlendVideoMat.SetInt("_SwapBackgroundRB", 0);
                }
            }
            else
            {
                flipHorizontallyMat.SetInt("_YUV", 0);
                alphaBlendVideoMat.SetInt("_YUV", 0);
                alphaBlendPreviewMat.SetInt("_YUV", 0);
                alphaBlendOutputMat.SetInt("_YUV", 0);
                alphaBlendOutputMat.SetInt("_RGBA", 1);
                alphaBlendVideoMat.SetInt("_RGBA", 1);
                alphaBlendOutputMat.SetInt("_FlipVideo", 0);
                alphaBlendPreviewMat.SetInt("_FlipVideo", 1);
                alphaBlendVideoMat.SetInt("_FlipVideo", 0);

                flipHorizontallyMat.SetInt("_RGBA", 1);
                alphaBlendOutputMat.SetInt("_SwapBackgroundRB", 0);
                alphaBlendPreviewMat.SetInt("_SwapBackgroundRB", 0);

                if (HardwareEncodeVideo())
                {
                    alphaBlendVideoMat.SetInt("_SwapBackgroundRB", 1);
                }
                else
                {
                    alphaBlendVideoMat.SetInt("_SwapBackgroundRB", 0);
                }
            }
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
                    colorTexture = Texture2D.CreateExternalTexture(GetFrameWidth(), GetFrameHeight(), TextureFormat.ARGB32, false, false, colorSRV);
                    colorTexture.filterMode = FilterMode.Point;
                    colorTexture.anisoLevel = 0;
                }
            }
        }

        private void SetHoloFrame()
        {
            if (holoTexture == null)
            {
                // Unity 5.6 workaround:
                // Creating textures on the graphics device on the first frame deadlocks Unity 5.6.
                if (Time.frameCount > 1 && CreateUnityHoloTexture(out holoSRV))
                {
                    holoTexture = Texture2D.CreateExternalTexture(GetFrameWidth(), GetFrameHeight(), TextureFormat.ARGB32, false, false, holoSRV);
                }
            }
        }

        private void SetMergedFrame()
        {
            if (videoTexRT == null)
            {
                videoTexRT = new RenderTexture(GetFrameWidth(), GetFrameHeight(), 0, RenderTextureFormat.Default);
                videoTexRT.filterMode = FilterMode.Point;
                videoTexRT.anisoLevel = 0;
                videoTexRT.antiAliasing = 1;
                videoTexRT.depth = 0;
                videoTexRT.useMipMap = false;
            }

            if (videoTexRT != null && !setVideoTexture)
            {
                setVideoTexture = SetVideoRenderTexture(videoTexRT.GetNativeTexturePtr());
            }

            if (outputTexRT == null)
            {
                outputTexRT = new RenderTexture(GetFrameWidth(), GetFrameHeight(), 0, RenderTextureFormat.Default);
                outputTexRT.filterMode = FilterMode.Point;
                outputTexRT.anisoLevel = 0;
                outputTexRT.antiAliasing = 1;
                outputTexRT.depth = 0;
                outputTexRT.useMipMap = false;
            }

            if (outputTexRT != null && !setOutputTex)
            {
                setOutputTex = SetOutputRenderTexture(outputTexRT.GetNativeTexturePtr());
            }
        }
        #endregion
#endif
    }
}
