// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;

namespace SpectatorView
{
    public class DisableInEditor : MonoBehaviour
    {
        void Start()
        {
#if UNITY_EDITOR
            gameObject.SetActive(false);
#endif
        }
    }
}
