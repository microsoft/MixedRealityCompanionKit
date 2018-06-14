// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace SpectatorView
{
    public class WireupDebugPanel : MonoBehaviour
    {

        public bool ConnectedPosition = false;
        // Use this for initialization
        void Awake()
        {
            if (ConnectedPosition)
            {
                PositionDebugButton.Instance.ConnectedPosition = this.gameObject;
            }
            else
            {
                PositionDebugButton.Instance.DisconnectedPosition = this.gameObject;
            }
        }
    }
}
