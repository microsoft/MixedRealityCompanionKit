// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using System.Collections;

public class GuiButtons : MonoBehaviour
{
    public bool showUI = false;

    public MixedRemoteViewCompositor.MrvcManager manager = null;

    void Awake()
    {
        this.manager = FindObjectOfType<MixedRemoteViewCompositor.MrvcManager>();
    }

    void OnGUI()
    {
#if true//UNITY_EDITOR
        if (this.showUI)
        {
            var y = 10;

            if (GUI.Button(new Rect(10, y, 150, 30), string.Format("Start Capture")))
            {
                this.manager.StartCapture();
            }

            if (GUI.Button(new Rect(170, y, 150, 30), string.Format("Stop Capture")))
            {
                this.manager.StopCapture();
            }

            y += 35;

            if (GUI.Button(new Rect(10, y, 150, 30), string.Format("Start Playback")))
            {
                this.manager.StartPlayback();
            }

            if (GUI.Button(new Rect(170, y, 150, 30), string.Format("Stop Playback")))
            {
                this.manager.StopPlayback();
            }
        }
#endif
    }

}
