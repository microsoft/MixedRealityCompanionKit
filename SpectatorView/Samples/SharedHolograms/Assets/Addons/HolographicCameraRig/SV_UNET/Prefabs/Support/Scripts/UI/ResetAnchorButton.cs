// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using HoloToolkit.Unity.InputModule;
using HoloToolkit.Unity;
using System;

namespace SpectatorView
{
    public class ResetAnchorButton : MonoBehaviour, IInputClickHandler
    {

        public void OnInputClicked(InputClickedEventData eventData)
        {
            if (NetworkDiscoveryWithAnchors.Instance.isServer)
            {
                UNetAnchorManager.Instance.MakeNewAnchor();
            }
            else
            {
                Debug.Log("Only the server for now");
            }
        }

        // Use this for initialization
        void Start()
        {

        }

        // Update is called once per frame
        void Update()
        {

        }
    }
}
