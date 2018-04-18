// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace SimpleSharing
{
    // Note: In this simple sharing implementation, network messages are only being sent from HoloLens clients to the Unity server.
    // This means that only Unity will have a complete knowledge of the application state.
    // 
    // If you want all clients to know about all other clients' states, you will need to 
    // relay all network traffic coming into this class to all other clients in the remotePlayers dictionary.
    public class RemotePlayerManager : SimpleSharing.Singleton<RemotePlayerManager>
    {
        Dictionary<int, GameObject> remotePlayers = new Dictionary<int, GameObject>();

        private void AddRemotePlayer(int connectionID)
        {
            // In this simple example, this will add a cube to the remote player's head.
            // For more robust remote player behavior, consider adding a RemotePlayer class to this GO and destroying the cube.
            GameObject go = GameObject.CreatePrimitive(PrimitiveType.Cube);
            go.transform.localScale = Vector3.one * 0.2f;
            go.name = "RemotePlayer_" + connectionID.ToString();

            // Child remote players to the shared anchored object if available.
            if (AnchorManager.Instance != null && AnchorManager.Instance.objectToAnchor != null)
            {
                go.transform.SetParent(AnchorManager.Instance.objectToAnchor.transform);
            }
            else
            {
                // Otherwise child remote players to this gameObject, which shares a transform to the AnchorManager.
                go.transform.SetParent(gameObject.transform);
            }

            // All remote interaction will be keyed off of the connectionID.
            remotePlayers.Add(connectionID, go);
        }

        public void UpdateRemotePlayer(int connectionID, Vector3 position, Quaternion rotation)
        {
            if (!remotePlayers.ContainsKey(connectionID))
            {
                AddRemotePlayer(connectionID);
            }

            remotePlayers[connectionID].transform.localPosition = position;
            remotePlayers[connectionID].transform.localRotation = rotation;
        }

        public void RemoveRemotePlayer(int connectionID)
        {
            if (remotePlayers.ContainsKey(connectionID))
            {
                GameObject go = remotePlayers[connectionID];
                remotePlayers.Remove(connectionID);
                DestroyImmediate(go);
            }
        }

        // Respond to remote player input here.
        // This will be called if a remote player's airtap successfully collided with any geometry.
        public void PerformRemoteAirTap(int connectionID, Vector3 airTapLocation, Vector3 airTapDirection, Vector3 airTapHitLocation)
        {
            Debug.Log(String.Format("Client {0} performed air tap at location {1}, which hit location {2}", connectionID, airTapLocation.ToString(), airTapHitLocation.ToString()));
        }
    }
}
