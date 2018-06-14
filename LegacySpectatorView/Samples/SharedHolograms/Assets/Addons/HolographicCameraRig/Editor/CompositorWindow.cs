// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using UnityEditor;
using System;
using System.Collections;
using System.Runtime.InteropServices;
using System.Diagnostics;
using System.IO;

namespace SpectatorView
{
    public class CompositorWindow : EditorWindow
    {
        #region DLLImports
        [DllImport("UnityCompositorInterface")]
        private static extern void StopFrameProvider();

        [DllImport("UnityCompositorInterface")]
        private static extern int GetFrameWidth();

        [DllImport("UnityCompositorInterface")]
        private static extern int GetFrameHeight();

        [DllImport("UnityCompositorInterface")]
        private static extern void SetFrameOffset(float frameOffset);

        [DllImport("UnityCompositorInterface")]
        private static extern float GetFrameOffset();

        [DllImport("UnityCompositorInterface")]
        private static extern void SetAlpha(float alpha);

        [DllImport("UnityCompositorInterface")]
        private static extern float GetAlpha();

        [DllImport("UnityCompositorInterface")]
        private static extern void TakePicture();

        [DllImport("UnityCompositorInterface")]
        private static extern void TakeCanonPicture();

        [DllImport("UnityCompositorInterface")]
        private static extern bool CaptureHiResHolograms();

        [DllImport("UnityCompositorInterface")]
        private static extern void StartRecording();

        [DllImport("UnityCompositorInterface")]
        private static extern void StopRecording();

        [DllImport("UnityCompositorInterface")]
        private static extern bool IsRecording();

        [DllImport("UnityCompositorInterface")]
        private static extern void InitializeFrameProvider();

        [DllImport("UnityCompositorInterface")]
        private static extern void PollForColorFrames();

        [DllImport("UnityCompositorInterface")]
        private static extern void Reset();

        [DllImport("UnityCompositorInterface")]
        private static extern bool QueueingHoloFrames();
        #endregion

        bool showAllTextures = false;

        private static CompositorWindow window = null;

        static float padding = 10;

        static float frameWidth = 100;
        static float frameHeight = 100;
        static float aspect = (float)GetFrameWidth() / (float)GetFrameHeight();

        static int fullScreenWidth = 1980;
        static int fullScreenHeight = 1080;

        float maxFrameOffset = 1;

        float brightness = 1;
        float maxBrightness = 5;

        bool showVideo = true;

        void OnEnable()
        {
            if (SpectatorView.SpectatorViewManager.Instance != null)
            {
                SpectatorView.SpectatorViewManager.Instance.frameProviderInitialized = false;
                SetFrameOffset(SpectatorView.SpectatorViewManager.Instance.DefaultFrameOffset);
                SetAlpha(SpectatorView.SpectatorViewManager.Instance.DefaultAlpha);
            }
        }

        void OnDestroy()
        {
            if (SpectatorView.SpectatorViewManager.Instance != null)
            {
                SpectatorView.SpectatorViewManager.Instance.ResetCompositor();
            }
        }

        [MenuItem("Spectator View/Compositor", false, 0)]
        public static void ShowWindow()
        {
            fullScreenWidth = Screen.currentResolution.width;
            fullScreenHeight = Screen.currentResolution.height;

            window = (CompositorWindow)GetWindow(typeof(CompositorWindow), false, "Compositor", true);
            Vector2 minDimensons = new Vector2(315, 400);
            window.minSize = minDimensons;

            //TODO: If window has gotten stuck partially out of your monitor view bounds, 
            // uncomment this line to restore position to the center of your primary monitor.
            //ResetPosition();

            window.Show();
        }

        private static void ResetPosition()
        {
            float defaultWidth = 400;
            float defaultHeight = 400;
            window.position = new Rect(Screen.currentResolution.width / 2.0f - defaultWidth / 2.0f,
                Screen.currentResolution.height / 2.0f - defaultHeight / 2.0f, defaultWidth, defaultHeight);
        }

        private void RenderTitle(string title)
        {
            Color oldColor = GUI.backgroundColor;
            GUI.backgroundColor = Color.green;
            try
            {
                EditorGUILayout.BeginVertical("Box");
                {
                    EditorGUILayout.LabelField(title);
                }
                EditorGUILayout.EndVertical();
            }
            finally
            {
                GUI.backgroundColor = oldColor;
            }
        }

        void OnGUI()
        {
            EditorGUILayout.BeginVertical("Box");
            {
                RenderTitle("Recording");

                if (!IsRecording())
                {
                    EditorGUILayout.BeginVertical("Box");
                    {
                        if (GUILayout.Button("Start Recording"))
                        {
                            StartRecording();
                        }
                    }
                    EditorGUILayout.EndVertical();
                }
                else
                {
                    EditorGUILayout.BeginVertical("Box");
                    {
                        if (GUILayout.Button("Stop Recording"))
                        {
                            StopRecording();
                        }
                    }
                    EditorGUILayout.EndVertical();
                }
            }

            if (GUILayout.Button("Take Picture"))
            {
                TakePicture();
            }

            if (CaptureHiResHolograms() && GUILayout.Button("Take Canon Picture"))
            {
                TakeCanonPicture();
            }

            EditorGUILayout.Space();

            // Open Folder
            if (GUILayout.Button("Open Folder"))
            {
                Process.Start(Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments), "HologramCapture"));
            }

            EditorGUILayout.EndVertical();

            EditorGUILayout.BeginVertical("Box");
            {
                RenderTitle("Hologram Settings");

                float oldAlpha = GetAlpha();
                float newAlpha = EditorGUILayout.Slider("Alpha", oldAlpha, 0, 1);
                if (newAlpha != oldAlpha)
                {
                    SetAlpha(newAlpha);
                }

                if (SpectatorView.SpectatorViewManager.Instance != null &&
                    SpectatorView.ShaderManager.Instance != null &&
                    SpectatorView.ShaderManager.Instance.colorTexture != null &&
                    SpectatorView.ShaderManager.Instance.renderTexture != null &&
                    SpectatorView.ShaderManager.Instance.holoTexture != null &&
                    SpectatorView.ShaderManager.Instance.alphaBlendVideoMat != null &&
                    SpectatorView.ShaderManager.Instance.alphaBlendOutputMat != null &&
                    SpectatorView.ShaderManager.Instance.alphaBlendPreviewMat != null)
                {
                    SpectatorView.SpectatorViewManager.Instance.DefaultAlpha = newAlpha;

                    SpectatorView.ShaderManager.Instance.alphaBlendVideoMat.SetFloat("_Alpha", newAlpha);
                    SpectatorView.ShaderManager.Instance.alphaBlendOutputMat.SetFloat("_Alpha", newAlpha);
                    SpectatorView.ShaderManager.Instance.alphaBlendPreviewMat.SetFloat("_Alpha", newAlpha);
                }

                EditorGUILayout.Space();

                float previousFrameOffset = GetFrameOffset();
                float newFrameOffset = EditorGUILayout.Slider("Frame Offset", previousFrameOffset, -1 * maxFrameOffset, maxFrameOffset);

                if (SpectatorView.SpectatorViewManager.Instance)
                {
                    SpectatorView.SpectatorViewManager.Instance.DefaultFrameOffset = newFrameOffset;
                }

                if (newFrameOffset != previousFrameOffset)
                {
                    SetFrameOffset(newFrameOffset);
                }

                EditorGUILayout.Space();

                brightness = EditorGUILayout.Slider("Brightness", brightness, 0, maxBrightness);

                if (SpectatorView.SpectatorViewManager.Instance != null &&
                    SpectatorView.ShaderManager.Instance != null &&
                    SpectatorView.ShaderManager.Instance.colorTexture != null &&
                    SpectatorView.ShaderManager.Instance.renderTexture != null &&
                    SpectatorView.ShaderManager.Instance.holoTexture != null &&
                    SpectatorView.ShaderManager.Instance.alphaBlendVideoMat != null &&
                    SpectatorView.ShaderManager.Instance.alphaBlendOutputMat != null &&
                    SpectatorView.ShaderManager.Instance.alphaBlendPreviewMat != null)
                {
                    SpectatorView.SpectatorViewManager.Instance.DefaultAlpha = newAlpha;

                    SpectatorView.ShaderManager.Instance.alphaBlendVideoMat.SetFloat("_Brightness", brightness);
                    SpectatorView.ShaderManager.Instance.alphaBlendOutputMat.SetFloat("_Brightness", brightness);
                    SpectatorView.ShaderManager.Instance.alphaBlendPreviewMat.SetFloat("_Brightness", brightness);
                }

            }
            EditorGUILayout.EndVertical();

            EditorGUILayout.BeginVertical("Box");
            {
                RenderTitle("Composite");

                if (GUILayout.Button("Toggle View"))
                {
                    showAllTextures = !showAllTextures;
                }

                EditorGUILayout.BeginHorizontal("Box");
                {
                    if (GUILayout.Button("Toggle Full Screen"))
                    {
                        PreviewWindow.ShowWindow();
                    }

                    GUILayout.Label("Width: ");
                    string w = GUILayout.TextField(fullScreenWidth.ToString());
                    fullScreenWidth = Convert.ToInt32(w);

                    GUILayout.Label("Height: ");
                    string h = GUILayout.TextField(fullScreenHeight.ToString());
                    fullScreenHeight = Convert.ToInt32(h);

                    PreviewWindow.SetDimensions(Convert.ToInt32(w), Convert.ToInt32(h));
                }
                EditorGUILayout.EndHorizontal();

                if (HoloToolkit.Unity.SpatialMapping.SpatialMappingManager.Instance != null &&
                    PlayerController.Instance != null)
                {
                    // Request Spatial Mapping
                    if (GUILayout.Button("Spatial Mapping"))
                    {
                        // Clear any old spatial mapping meshes.
                        if (HoloToolkit.Unity.SpatialMapping.SpatialMappingManager.Instance)
                        {
                            SpectatorView.RemoteSpatialMappingSource rsms =
                                HoloToolkit.Unity.SpatialMapping.SpatialMappingManager.Instance.GetComponent<SpectatorView.RemoteSpatialMappingSource>();
                            if (rsms != null)
                            {
                                rsms.ClearMeshes();
                            }
                            else
                            {
                                rsms = HoloToolkit.Unity.SpatialMapping.SpatialMappingManager.Instance.gameObject.AddComponent<SpectatorView.RemoteSpatialMappingSource>();
                                if (rsms != null)
                                {
                                    rsms.ClearMeshes();
                                }
                            }
                        }

                        // Set a flag on the server to send a new mesh.
                        PlayerController.Instance.RequestSpatialMapping();
                    }
                }

                // Rendering
                if (showVideo)
                {
                    Rect spacing = GUILayoutUtility.GetRect(1, 2);
                    float left = 0;
                    UpdateFrameDimensions(spacing.y, ref left);

                    Rect framesRect = GUILayoutUtility.GetRect(frameWidth * 2 + padding, frameHeight * 2 + padding);
                    framesRect.x += left;

                    if (Event.current != null && Event.current.type == EventType.Repaint)
                    {
                        if (SpectatorView.SpectatorViewManager.Instance != null &&
                            SpectatorView.ShaderManager.Instance != null)
                        {
                            if (
                                    SpectatorView.ShaderManager.Instance.colorTexture != null &&
                                    SpectatorView.ShaderManager.Instance.renderTexture != null &&
                                    SpectatorView.ShaderManager.Instance.holoTexture != null &&
                                    SpectatorView.ShaderManager.Instance.alphaBlendVideoMat != null &&
                                    SpectatorView.ShaderManager.Instance.alphaBlendOutputMat != null &&
                                    SpectatorView.ShaderManager.Instance.alphaBlendPreviewMat != null
                                )
                            {
                                if (!SpectatorView.ShaderManager.Instance.setAlphaBlendPreviewHoloTex)
                                {
                                    SpectatorView.ShaderManager.Instance.setAlphaBlendPreviewHoloTex = true;
                                    if (QueueingHoloFrames())
                                    {
                                        SpectatorView.ShaderManager.Instance.alphaBlendPreviewMat.SetTexture("_FrontTex", SpectatorView.ShaderManager.Instance.holoTexture);
                                        SpectatorView.ShaderManager.Instance.alphaBlendVideoMat.SetTexture("_FrontTex", SpectatorView.ShaderManager.Instance.holoTexture);
                                        SpectatorView.ShaderManager.Instance.alphaBlendOutputMat.SetTexture("_FrontTex", SpectatorView.ShaderManager.Instance.holoTexture);
                                    }
                                    else
                                    {
                                        SpectatorView.ShaderManager.Instance.alphaBlendPreviewMat.SetTexture("_FrontTex", SpectatorView.ShaderManager.Instance.renderTexture);
                                        SpectatorView.ShaderManager.Instance.alphaBlendVideoMat.SetTexture("_FrontTex", SpectatorView.ShaderManager.Instance.renderTexture);
                                        SpectatorView.ShaderManager.Instance.alphaBlendOutputMat.SetTexture("_FrontTex", SpectatorView.ShaderManager.Instance.renderTexture);
                                    }
                                }

                                Graphics.DrawTexture(
                                    new Rect(framesRect.x, framesRect.y, frameWidth, frameHeight),
                                    SpectatorView.ShaderManager.Instance.colorTexture,
                                    SpectatorView.ShaderManager.Instance.alphaBlendPreviewMat);
                            }

                            if (showAllTextures)
                            {
                                if (QueueingHoloFrames())
                                {
                                    if (SpectatorView.ShaderManager.Instance.holoTexture != null)
                                    {
                                        Graphics.DrawTexture
                                            (
                                                new Rect(framesRect.x, framesRect.y + frameHeight + padding, frameWidth, frameHeight),
                                                SpectatorView.ShaderManager.Instance.holoTexture
                                            );
                                    }
                                }
                                else
                                {
                                    if (SpectatorView.ShaderManager.Instance.renderTexture != null)
                                    {
                                        Graphics.DrawTexture
                                            (
                                                new Rect(framesRect.x, framesRect.y + frameHeight + padding, frameWidth, frameHeight),
                                                SpectatorView.ShaderManager.Instance.renderTexture
                                            );
                                    }
                                }

                                if (SpectatorView.ShaderManager.Instance.flipHorizontallyMat != null &&
                                    SpectatorView.ShaderManager.Instance.colorTexture != null)
                                {
                                    Graphics.DrawTexture
                                        (
                                            new Rect(framesRect.x + frameWidth + padding, framesRect.y + 0, frameWidth, frameHeight),
                                            SpectatorView.ShaderManager.Instance.colorTexture,
                                            SpectatorView.ShaderManager.Instance.flipHorizontallyMat
                                        );
                                }

                                if (QueueingHoloFrames())
                                {
                                    if (SpectatorView.ShaderManager.Instance.holoTexture != null && SpectatorView.ShaderManager.Instance.alphaMat != null)
                                    {
                                        Graphics.DrawTexture
                                            (
                                                new Rect(framesRect.x + frameWidth + padding, framesRect.y + frameHeight + padding, frameWidth, frameHeight),
                                                SpectatorView.ShaderManager.Instance.holoTexture,
                                                SpectatorView.ShaderManager.Instance.alphaMat
                                            );
                                    }
                                }
                                else
                                {
                                    if (SpectatorView.ShaderManager.Instance.renderTexture != null && SpectatorView.ShaderManager.Instance.alphaMat != null)
                                    {
                                        Graphics.DrawTexture
                                            (
                                                new Rect(framesRect.x + frameWidth + padding, framesRect.y + frameHeight + padding, frameWidth, frameHeight),
                                                SpectatorView.ShaderManager.Instance.renderTexture,
                                                SpectatorView.ShaderManager.Instance.alphaMat
                                            );
                                    }
                                }
                            }
                        }
                    }
                }
            }
            EditorGUILayout.EndVertical();
        }

        void UpdateFrameDimensions(float currentTop, ref float left)
        {
            frameWidth = position.width;
            frameHeight = position.height - currentTop;

            if (showAllTextures)
            {
                frameWidth /= 2.0f;
                frameHeight /= 2.0f;

                frameWidth -= padding;
                frameHeight -= padding;
            }

            if (frameWidth <= frameHeight * aspect)
            {
                frameHeight = frameWidth / aspect;
                left = 0;
            }
            else
            {
                frameWidth = frameHeight * aspect;

                if (showAllTextures)
                {
                    left = (position.width / 2.0f) - 2 * (frameWidth / 2.0f);
                }
                else
                {
                    left = (position.width / 2.0f) - (frameWidth / 2.0f);
                }
            }

            frameWidth -= padding;
        }

        void Update()
        {
            CheckEditorWindowStatus();
        }

        /// <summary>
        /// Ensure the window is still active.  When entering or leaving Play mode, the window can be set to null.
        /// </summary>
        private void CheckEditorWindowStatus()
        {
            if (window != null)
            {
                window.Repaint();
            }
            else
            {
                // Window has been destroyed, recreate it.
                // This might happen when transitioning to play mode in Unity.
                ShowWindow();
            }
        }
    }
}
