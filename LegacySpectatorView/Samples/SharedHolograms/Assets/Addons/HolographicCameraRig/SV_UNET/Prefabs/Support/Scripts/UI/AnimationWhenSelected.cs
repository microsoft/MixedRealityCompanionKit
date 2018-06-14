// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace SpectatorView
{
    public class AnimationWhenSelected : MonoBehaviour
    {

        private int StartFrame = 0;
        private int TotalFrames = 30;
        Vector3 startScale;
        Vector3 halfScale;

        // Use this for initialization
        void Start()
        {
            startScale = transform.localScale;
            halfScale = startScale * 0.5f;
            StartFrame = Time.frameCount;
        }

        // Update is called once per frame
        void Update()
        {
            float rat = (float)(Time.frameCount - StartFrame) / (float)TotalFrames;

            if (rat < 0.5f)
            {
                transform.localScale = Vector3.Lerp(startScale, halfScale, rat * 2);
            }
            else if (rat < 1.0f)
            {
                transform.localScale = Vector3.Lerp(halfScale, startScale, (rat - 0.5f) * 2);
            }
            else
            {
                transform.localScale = startScale;
                Destroy(this);
            }
        }
    }
}
