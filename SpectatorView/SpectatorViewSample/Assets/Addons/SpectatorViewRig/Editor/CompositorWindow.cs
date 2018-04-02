// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.InteropServices;
using UnityEngine;
using UnityEditor;

namespace SpectatorView
{
    public class CompositorWindow : EditorWindow
    {
        #region DLLImports
        [DllImport("UnityCompositorInterface")]
        private static extern void InitializeFrameProvider();

        [DllImport("UnityCompositorInterface")]
        private static extern void StopFrameProvider();

        [DllImport("UnityCompositorInterface")]
        private static extern int GetFrameWidth();

        [DllImport("UnityCompositorInterface")]
        private static extern int GetFrameHeight();

        [DllImport("UnityCompositorInterface")]
        private static extern int TakePicture();

        [DllImport("UnityCompositorInterface")]
        private static extern bool IsRecording();

        [DllImport("UnityCompositorInterface")]
        private static extern void StartRecording();

        [DllImport("UnityCompositorInterface")]
        private static extern void StopRecording();
        #endregion

        private static CompositorWindow window = null;

        static float aspect;
        static float padding = 10;
        static float frameWidth = 100;
        static float frameHeight = 100;

        bool isFullScreen = false;

        Rect cachedPosition;
        Vector2 cachedMinSize;
        Vector2 cachedMaxSize;

        private void OnEnable()
        {
            aspect = (float)GetFrameWidth() / (float)GetFrameHeight();
            if (SpectatorViewManager.Instance != null)
            {
                SpectatorViewManager.Instance.frameProviderInitialized = false;
            }
        }

        private void OnDestroy()
        {
            if (SpectatorView.SpectatorViewManager.Instance != null)
            {
                SpectatorViewManager.Instance.ResetCompositor();
            }
        }

        [MenuItem("Spectator View/Compositor", false, 0)]
        public static void ShowWindow()
        {
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

        void Update()
        {
            if (ShaderManager.Instance != null)
            {
                ShaderManager.Instance.SetShaderValues();
            }

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

        void OnGUI()
        {
            RenderRecordingButtons();

            RenderCompositeFrame();
        }

        void ToggleFullScreen()
        {
            if (Event.current != null &&
                Event.current.isKey)
            {
                if (isFullScreen && Event.current.keyCode == KeyCode.Escape)
                {
                    isFullScreen = false;
                    RestoreWindowSize();
                }
            }

            // Toggle Fullscreen.
            EditorGUILayout.BeginVertical("Box");
            {
                // This seems to work best if the compositor window is not docked.
                if (GUILayout.Button("Toggle Full Screen"))
                {
                    isFullScreen = !isFullScreen;
                    if (isFullScreen)
                    {
                        cachedPosition = window.position;
                        cachedMinSize = window.minSize;
                        cachedMaxSize = window.maxSize;

                        // Hide top menu bar
                        int topBuffer = 22;
                        // Hide left border.
                        int leftBuffer = 5 + (int)(topBuffer * aspect);

                        // These values may need to be changed manually depending on your resolution or DPI settings.
                        int width = (Screen.currentResolution.width + leftBuffer);
                        int height = (Screen.currentResolution.height + topBuffer);

                        // Render on a different monitor by changing the x and y values below:
                        Rect newPos = new Rect(0 - leftBuffer / 2, 0 - topBuffer, width, height + topBuffer);

                        window.minSize = new Vector2(width, height + topBuffer);
                        window.maxSize = window.minSize;
                        window.position = newPos;

                        Screen.fullScreen = true;
                    }
                    else
                    {
                        // This button will not be visible when full screen unless we do not have a valid texture.
                        // Use Escape to restore instead.
                        RestoreWindowSize();
                    }
                }
            }
            EditorGUILayout.EndVertical();
        }

        void RestoreWindowSize()
        {
            if (isFullScreen)
            {
                isFullScreen = false;
                Screen.fullScreen = false;
            }

            window.minSize = cachedMinSize;
            window.maxSize = cachedMaxSize;
            window.position = cachedPosition;
        }

        void RenderRecordingButtons()
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

            if (GUILayout.Button("View Captures"))
            {
                Process.Start(Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments), "HologramCapture"));
            }

            EditorGUILayout.EndVertical();
        }

        void RenderCompositeFrame()
        {
            EditorGUILayout.BeginVertical("Box");
            {
                RenderTitle("Preview");

                ToggleFullScreen();

                // Render preview.
                Rect spacing = GUILayoutUtility.GetRect(1, 1);
                float left = 0;
                UpdateFrameDimensions(spacing.y, ref left);

                Rect frameRect = GUILayoutUtility.GetRect(frameWidth, frameHeight);
                frameRect.x += left;

                if (Event.current != null && Event.current.type == EventType.Repaint)
                {
                    if (ShaderManager.Instance != null &&
                        ShaderManager.Instance.AllShadersAndTexturesReady())
                    {
                        Rect screen = new Rect(frameRect.x, frameRect.y, frameWidth, frameHeight);

                        if (isFullScreen)
                        {
                            Graphics.Blit(
                                ShaderManager.Instance.previewTexRT,
                                ShaderManager.Instance.passthroughColorMat
                            );
                        }
                        else
                        {
                            Graphics.DrawTexture(
                                screen,
                                ShaderManager.Instance.previewTexRT
                            );
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

            if (frameWidth <= frameHeight * aspect)
            {
                frameHeight = frameWidth / aspect;
                left = 0;
            }
            else
            {
                frameWidth = frameHeight * aspect;
                left = (position.width / 2.0f) - (frameWidth / 2.0f);
            }

            frameWidth -= 1.5f * padding;
        }
    }
}
