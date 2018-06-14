// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Windows.Speech;
using HoloToolkit.Unity.InputModule;
using System;

namespace SpectatorView
{
    public class ToggleDebugWindow : MonoBehaviour, IInputClickHandler
    {
        bool debugEnabled = false;
        public GameObject DebugWindow;

        // Use this for initialization
        void Start()
        {
            UpdateChildren();
        }

        public void OnInputClicked(InputClickedEventData eventData)
        {
            debugEnabled = !debugEnabled;
            UpdateChildren();
        }

        private void UpdateChildren()
        {
            DebugWindow.SetActive(debugEnabled);
        }
    }
}
