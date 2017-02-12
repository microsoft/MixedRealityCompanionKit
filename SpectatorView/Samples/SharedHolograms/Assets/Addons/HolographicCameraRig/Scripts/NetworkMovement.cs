// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using System.Collections.Generic;

namespace SpectatorView
{
    public class NetworkMovement : MonoBehaviour
    {
        public struct NetworkTransform
        {
            public Vector3 Position;
            public Quaternion Rotation;
        }

        private const int maxNetworkTransforms = 3;
        Queue<NetworkTransform> networkTransforms = new Queue<NetworkTransform>(maxNetworkTransforms);

        public void AddTransform(NetworkTransform networkTransform)
        {
            if (networkTransforms.Count >= maxNetworkTransforms)
            {
                networkTransforms.Dequeue();
            }

            networkTransforms.Enqueue(networkTransform);
        }

        void Update()
        {
            if (networkTransforms.Count > 0)
            {
                NetworkTransform networkTransform = networkTransforms.Dequeue();
                gameObject.transform.localPosition = networkTransform.Position;
                gameObject.transform.localRotation = networkTransform.Rotation;
            }
        }
    }
}
