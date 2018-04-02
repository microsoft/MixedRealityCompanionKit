// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using HoloToolkit.Unity.InputModule;
using System;

namespace SpectatorView
{
    public class ScrollSessionListButton : MonoBehaviour, IInputClickHandler
    {

        public int Direction;

        public void OnInputClicked(InputClickedEventData eventData)
        {
            ScrollingSessionListUIController.Instance.ScrollSessions(Direction);
        }
    }
}
