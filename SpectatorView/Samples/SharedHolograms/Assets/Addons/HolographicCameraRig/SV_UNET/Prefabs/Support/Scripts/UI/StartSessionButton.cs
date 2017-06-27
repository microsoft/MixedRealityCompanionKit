// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections;
using System.Collections.Generic;
using HoloToolkit.Unity.InputModule;
using UnityEngine;

namespace SpectatorView
{
    public class StartSessionButton : MonoBehaviour, IInputClickHandler
    {

        NetworkDiscoveryWithAnchors networkDiscovery;

        public void OnInputClicked(InputClickedEventData eventData)
        {
            if (networkDiscovery.running)
            {
                {
                    networkDiscovery.StartHosting("SpectatorViewSession");
                }
            }
        }

        // Use this for initialization
        void Start()
        {
            networkDiscovery = NetworkDiscoveryWithAnchors.Instance;
        }
    }
}
