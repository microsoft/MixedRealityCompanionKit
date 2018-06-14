// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;
using UnityEditor;

namespace SpectatorView
{
    /// <summary>
    /// Some capture cards use an automatic hardware passthrough when using hdmi in.
    /// If this is the case, you can still preview the composite by opening this window and
    /// setting it to an external monitor in Windows.
    /// </summary>
    public class PreviewWindow : EditorWindow
    {
        #region DLLImports
        [DllImport("UnityCompositorInterface")]
        private static extern int GetFrameWidth();

        [DllImport("UnityCompositorInterface")]
        private static extern int GetFrameHeight();
        #endregion

        static float aspect = (float)GetFrameWidth() / (float)GetFrameHeight();

        private static PreviewWindow window = null;

        private static Vector2 pos = Vector2.zero;
        private static Vector2? dim = null;

        int fastMove = 25;
        float slowMove = 0.25f;

        static bool killWindow = false;

        public static void SetDimensions(int width, int height)
        {
            Vector2 d;
            d.x = width;
            d.y = height;
            dim = d;
        }

        public static void ShowWindow()
        {
            killWindow = false;
            if (window == null)
            {
                pos = new Vector2(0, 0);
                if (!dim.HasValue)
                {
                    dim = new Vector2(Screen.currentResolution.width, Screen.currentResolution.height);
                }

                window = ScriptableObject.CreateInstance<PreviewWindow>();
                window.maxSize = new Vector2(6000, 4000);
                window.position = new Rect(pos, dim.Value);
                window.ShowAsDropDown(new Rect(), dim.Value);
            }
            else
            {
                killWindow = true;
                window.Close();
            }
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
            else if (!killWindow)
            {
                ShowWindow();
            }
        }

        void OnGUI()
        {
            if (Event.current != null &&
                Event.current.isKey)
            {
                if (Event.current.keyCode == KeyCode.LeftArrow)
                {
                    if (window != null)
                    {
                        pos.x -= fastMove;
                        window.position = new Rect(pos, dim.Value);
                    }
                }
                else if (Event.current.keyCode == KeyCode.RightArrow)
                {
                    if (window != null)
                    {
                        pos.x += fastMove;
                        window.position = new Rect(pos, dim.Value);
                    }
                }
                else if (Event.current.keyCode == KeyCode.UpArrow)
                {
                    if (window != null)
                    {
                        pos.y -= fastMove;
                        window.position = new Rect(pos, dim.Value);
                    }
                }
                else if (Event.current.keyCode == KeyCode.DownArrow)
                {
                    if (window != null)
                    {
                        pos.y += fastMove;
                        window.position = new Rect(pos, dim.Value);
                    }
                }
                else if (Event.current.keyCode == KeyCode.A)
                {
                    if (window != null)
                    {
                        pos.x -= slowMove;
                        window.position = new Rect(pos, dim.Value);
                    }
                }
                else if (Event.current.keyCode == KeyCode.D)
                {
                    if (window != null)
                    {
                        pos.x += slowMove;
                        window.position = new Rect(pos, dim.Value);
                    }
                }
                else if (Event.current.keyCode == KeyCode.W)
                {
                    if (window != null)
                    {
                        pos.y -= slowMove;
                        window.position = new Rect(pos, dim.Value);
                    }
                }
                else if (Event.current.keyCode == KeyCode.S)
                {
                    if (window != null)
                    {
                        pos.y += slowMove;
                        window.position = new Rect(pos, dim.Value);
                    }
                }
                else if (Event.current.keyCode != KeyCode.None)
                {
                    killWindow = true;
                    this.Close();
                    window = null;
                }
            }

            if (Event.current != null && Event.current.type == EventType.Repaint)
            {
                float frameWidth = position.width;
                float frameHeight = position.height;

                if (frameWidth <= frameHeight * aspect)
                {
                    frameHeight = frameWidth / aspect;
                }
                else
                {
                    frameWidth = frameHeight * aspect;
                }

                float left = position.width - frameWidth;
                float height = position.height - frameHeight;

                if (SpectatorView.ShaderManager.Instance != null &&
                    SpectatorView.ShaderManager.Instance.alphaBlendPreviewMat != null &&
                    SpectatorView.ShaderManager.Instance.colorTexture != null)
                {
                    Graphics.DrawTexture(
                        new Rect(left / 2.0f, height / 2.0f, frameWidth, frameHeight),
                        SpectatorView.ShaderManager.Instance.colorTexture,
                        SpectatorView.ShaderManager.Instance.alphaBlendPreviewMat);
                }
            }
        }
    }
}
