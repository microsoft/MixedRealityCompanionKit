// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using UnityEngine.Networking;

public class NetworkAnchorHelper : MonoBehaviour
{
    public GameObject NetworkAnchorManagerPrefab;
    public GameObject NetworkAnchorPlayerPrefab;

    /// <summary>
    /// Track the anchor manager spawned by this class. Only one manager per session.
    /// </summary>
    private static GameObject spawnedNetworkAnchorManager;

    /// <summary>
    /// Prepare the given network manager to host an anchor manager and anchor players
    /// </summary>
    public void PrepareNetworkManager(NetworkManager manager)
    {
        if (manager == null)
        {
            return;
        }

        if (NetworkAnchorManagerPrefab != null)
        {
            manager.spawnPrefabs.Add(NetworkAnchorManagerPrefab);
        }

        if (NetworkAnchorPlayerPrefab != null)
        {
            manager.spawnPrefabs.Add(NetworkAnchorPlayerPrefab);
        }
    }

    /// <summary>
    /// Spawn a network anchor manager across clients
    /// </summary>
    public void SpawnNetworkAnchorManager()
    {
        if (NetworkAnchorManagerPrefab != null && spawnedNetworkAnchorManager == null)
        {
            spawnedNetworkAnchorManager = Instantiate(NetworkAnchorManagerPrefab);
            spawnedNetworkAnchorManager.name = "NetworkAnchorManager";
            NetworkServer.Spawn(spawnedNetworkAnchorManager);
        }
    }

    /// <summary>
    /// Spawn a network anchor player for the given player game object
    /// </summary>
    public void SpawnNetworkAnchorPlayer(GameObject player)
    {
        if (NetworkAnchorPlayerPrefab != null)
        {
            GameObject spawnedNetworkAnchorPlayer = Instantiate(NetworkAnchorPlayerPrefab);
            spawnedNetworkAnchorPlayer.name = "NetworkAnchorPlayer";
            NetworkServer.SpawnWithClientAuthority(spawnedNetworkAnchorPlayer, player);
        }
    }
}
