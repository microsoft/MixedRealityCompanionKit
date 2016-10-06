// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using SpatialTranformHelper;
using UnityEngine;
using Matrix4x4 = SpatialTranformHelper.Matrix4x4;

namespace MixedRemoteViewCompositor
{
    [RequireComponent(typeof(Camera))]
    public class VideoCompositor : MonoBehaviour
    {
        public Shader CompositorShader = null;

        [Range(0.0f, 1.0f)]
        public float Brightness = .85f;

        [Range(0.01f, 1.0f)]
        public float Transparency = .15f;

        public Vector2 TextureOffset = new Vector2(0.0f, 1.0f);

        public Vector2 TextureScale = new Vector2(1.0f, -1.0f);

        public Texture2D VideoTexture
        {
            get { return this.videoTexture; }
            set
            {
                if (this.videoTexture != value)
                {
                    this.enabled = true;

                    if (this.compositorMaterial == null)
                    {
                        this.CreateResources();
                    }

                    if (this.videoTexture == null)
                    {
                        this.OnResolutionChanged(value.width, value.height);
                    }
                    else if (value != null && (value.width != this.videoTexture.width || value.height != this.videoTexture.height))
                    {
                        this.OnResolutionChanged(value.width, value.height);
                    }

                    this.videoTexture = value;
                }
            }
        }
        private Texture2D videoTexture;

        public Matrix4x4 CameraTransform
        {
            set
            {
                if(this.cameraTransform != Matrix4x4.Zero)
                {
                    this.lastGoodCameraTransform = this.cameraTransform;
                }
                this.cameraTransform = value;
            }
        }
        private Matrix4x4 cameraTransform;
        private Matrix4x4 lastGoodCameraTransform;

        public Matrix4x4 CameraProjection
        {
            set
            {
                if(this.cameraProjection != Matrix4x4.Zero)
                {
                    this.lastGoodCameraProjection = this.cameraProjection;
                }
                this.cameraProjection = value;
            }
        }
        private Matrix4x4 cameraProjection;
        private Matrix4x4 lastGoodCameraProjection;

        private Material compositorMaterial = null;

        private Camera sceneCamera;

        private int lastScreenWidth, lastScreenHeight;

        private float fov, aspectRatio, nearPlane, farPlane;
        private UnityEngine.Matrix4x4 defaultCameraProjection;

        private void Awake()
        {
            this.sceneCamera = this.GetComponent<Camera>();
            if (this.sceneCamera == null)
            {
                Debug.LogError("The VideoCompositor component must be added to a camera.");

                return;
            }
        }

        private void Start()
        {
            this.fov = this.sceneCamera.fieldOfView;
            this.aspectRatio = this.sceneCamera.aspect;
            this.nearPlane = this.sceneCamera.nearClipPlane;
            this.farPlane = this.sceneCamera.farClipPlane;
            this.defaultCameraProjection =
                UnityEngine.Matrix4x4.Perspective(this.fov, this.aspectRatio, this.nearPlane, this.farPlane);

            this.sceneCamera.projectionMatrix = this.defaultCameraProjection;
        }

        private void OnDisable()
        {
            this.DestroyResources();
        }

        private void LateUpdate()
        {
            if (this.compositorMaterial == null || this.videoTexture == null)
            {
                return;
            }

            // set the camera transforms
            CameraPose? currentCameraPose
                = this.cameraTransform.ConvertWorldViewMatrix();
            if (currentCameraPose == null)
            {
                currentCameraPose = this.lastGoodCameraTransform.ConvertWorldViewMatrix();
            }

            if (currentCameraPose != null)
            {
                this.transform.position = currentCameraPose.Value.Position;
                this.transform.rotation = currentCameraPose.Value.Rotation;
            }

            // swap the projection for the one sent to us
            UnityEngine.Matrix4x4? currentCameraProjection
                = this.cameraProjection.ConvertCameraProjectionMatrix(this.nearPlane, this.farPlane);
            if (currentCameraProjection == null)
            {
                currentCameraProjection 
                    = this.lastGoodCameraProjection.ConvertCameraProjectionMatrix(this.nearPlane, this.farPlane);
            }

            if (currentCameraProjection != null)
            {
                this.sceneCamera.projectionMatrix = currentCameraProjection.Value;
            }

            // if screen dimensions have changed, adjust the viewport
            if (Screen.width != this.lastScreenWidth || Screen.height != this.lastScreenHeight)
            {
                this.OnResolutionChanged(this.videoTexture.width, this.videoTexture.height);

                this.defaultCameraProjection =
                    UnityEngine.Matrix4x4.Perspective(this.fov, this.aspectRatio, this.nearPlane, this.farPlane);
            }
        }

        private void OnRenderImage(RenderTexture source, RenderTexture destination)
        {
            if (this.compositorMaterial == null || this.videoTexture == null)
            {
                Graphics.Blit(source, destination);

                return;
            }

            this.compositorMaterial.SetFloat("_Brightness", this.Brightness);
            this.compositorMaterial.SetFloat("_Transparency", this.Transparency);

            this.compositorMaterial.SetTexture("_VideoTex", this.VideoTexture);
            this.compositorMaterial.SetTextureOffset("_VideoTex", this.TextureOffset);
            this.compositorMaterial.SetTextureScale("_VideoTex", this.TextureScale);

            Graphics.Blit(source, destination, this.compositorMaterial, 0);
        }

        private void CreateResources()
        {
            // check the shader is set
            if (this.CompositorShader == null)
            {
                this.CompositorShader = Shader.Find("Hidden/Compositor");
            }

            if (this.CompositorShader != null && !this.CompositorShader.isSupported)
            {
                Debug.Log("The shader " + this.CompositorShader.ToString() + " on effect " + this.ToString() + " is not supported on this platform!");

                this.enabled = false;

                return;
            }

            if (this.compositorMaterial == null)
            {
                this.compositorMaterial = new Material(this.CompositorShader);
            }

            // is shader and material created
            if (this.compositorMaterial == null ||
                (this.compositorMaterial.shader != null && this.compositorMaterial.shader != this.CompositorShader))
            {
                Debug.LogWarning("The image effect " + this.ToString() + " has been disabled as it's not supported on the current platform.");

                this.enabled = false;
            }
        }

        private void DestroyResources()
        {
            if (this.compositorMaterial != null)
            {
                Destroy(this.compositorMaterial);
                this.compositorMaterial = null;
            }
        }

        private void OnResolutionChanged(int width, int height)
        {
            this.sceneCamera.rect = this.GetViewport(width, height);
        }

        private Rect GetViewport(int imageWidth, int imageHeight)
        {
            this.lastScreenWidth = Screen.width;
            this.lastScreenHeight = Screen.height;
            float screenAspectRatio = Math.Abs((float) this.lastScreenWidth / (float) this.lastScreenHeight);

            this.fov = (float)imageWidth / (float)imageHeight;
            float imageAspectRatio = Math.Abs(this.fov);

            Rect rc;

            if (Math.Abs(screenAspectRatio - imageAspectRatio) <= 0.01f)
            {
                // Screen and video has the same aspect ratio
                rc = new Rect(0.0f, 0.0f, 1.0f, 1.0f);
            }
            else if (screenAspectRatio < imageAspectRatio)
            {
                // Horizonal letterbox
                float heightRatio = screenAspectRatio / imageAspectRatio;
                float topMargin = (1.0f - heightRatio) / 2.0f;
                rc = new Rect(0.0f, topMargin, 1.0f, heightRatio);
            }
            else
            {
                // Vertical letterbox
                float widthRatio = imageAspectRatio / screenAspectRatio;
                float leftMargin = (1.0f - widthRatio) / 2.0f;
                rc = new Rect(leftMargin, 0.0f, widthRatio, 1.0f);
            }

            return rc;
        }
    }
}
