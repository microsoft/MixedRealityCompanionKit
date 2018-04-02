// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;

namespace SpectatorView
{
    public class HideRenderersInChildren : MonoBehaviour
    {
        void Start()
        {
#if UNITY_EDITOR

            foreach (Renderer renderer in GetComponentsInChildren<Renderer>())
            {
                renderer.enabled = false;
            }

#endif
        }
    }
}
