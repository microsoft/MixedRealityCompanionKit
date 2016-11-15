// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using System.Collections;

namespace PosterAlignment.AnimationUtilities
{
    public class AnimationSpeed : MonoBehaviour
    {
        public float speed = 1.0f;

        // Use this for initialization
        void Start()
        {
            foreach (AnimationState state in this.gameObject.GetComponent<Animation>())
            {
                state.speed = speed;
            }
        }
    }
}
