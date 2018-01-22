// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using UnityEngine.UI;

[RequireComponent(typeof(Text))]
public class FPSDisplay : MonoBehaviour
{
    private float _accumDelta = 0.0f;
    private int _frames = 0;
    private Text fpsGUIText;
    private float _lastDisplayUpdate = 0;

    void Awake()
    {
        fpsGUIText = GetComponent<Text>();
    }

    void Update()
    {
        _accumDelta += Time.deltaTime;
        ++_frames;

        if (Time.time - _lastDisplayUpdate < .5)
        {
            return;
        }

        float msec = _accumDelta * 1000.0f;
        float fps = (float)_frames / _accumDelta;        
        
        if (fpsGUIText != null)
        {
            fpsGUIText.text = string.Format("{0:0.0} ms ({1:0.} fps)", msec / _frames, fps);
        }

        _lastDisplayUpdate = Time.time;
        _frames = 0;
        _accumDelta = 0;
    }
}
