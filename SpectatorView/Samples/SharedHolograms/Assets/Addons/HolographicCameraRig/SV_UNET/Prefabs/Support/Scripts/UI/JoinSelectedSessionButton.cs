// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using HoloToolkit.Unity.InputModule;
using System;

namespace SpectatorView
{
    public class JoinSelectedSessionButton : MonoBehaviour, IInputClickHandler
    {
        TextMesh textMesh;
        Material textMaterial;
        int textColorId;
        ScrollingSessionListUIController scrollingUIControl;
        NetworkDiscoveryWithAnchors networkDiscovery;

        private void Start()
        {
            scrollingUIControl = ScrollingSessionListUIController.Instance;
            textMesh = transform.parent.GetComponentInChildren<TextMesh>();
            textMaterial = textMesh.GetComponent<MeshRenderer>().material;
            textColorId = Shader.PropertyToID("_Color");
            textMaterial.SetColor(textColorId, Color.grey);
            networkDiscovery = NetworkDiscoveryWithAnchors.Instance;
        }

        private void Update()
        {
            if (networkDiscovery.running && networkDiscovery.isClient)
            {
                if (scrollingUIControl.SelectedSession != null)
                {
                    textMaterial.SetColor(textColorId, Color.blue);
                }
                else
                {
                    textMaterial.SetColor(textColorId, Color.grey);
                }
            }
            else
            {
                textMaterial.SetColor(textColorId, Color.grey);
            }
        }

        public void OnInputClicked(InputClickedEventData eventData)
        {
            ScrollingSessionListUIController.Instance.JoinSelectedSession();
        }
    }
}
