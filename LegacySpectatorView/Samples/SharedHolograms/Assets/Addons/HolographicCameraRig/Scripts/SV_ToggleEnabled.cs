// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;

namespace SpectatorView
{
    /// <summary>
    /// Add this component to any GameObject you wish to disable when running spectator view.
    /// If you press 'T' at runtime, these GameObjects will be toggled on and off.
    /// </summary>
    public class SV_ToggleEnabled : MonoBehaviour
    {
#if UNITY_EDITOR
        private bool visible = false;

        void Start()
        {
            gameObject.SetActive(visible);
        }

        public void ToggleVisibility()
        {
            visible = !visible;
            gameObject.SetActive(visible);
        }
#endif
    }
}
