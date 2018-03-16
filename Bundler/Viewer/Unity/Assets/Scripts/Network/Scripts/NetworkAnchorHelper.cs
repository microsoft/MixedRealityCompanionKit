// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using UnityEngine.Networking;

public class NetworkAnchorHelper : MonoBehaviour
{
    [Tooltip("A prefab which contains all the \"network anchor manager\" behaviours. Only one of these will be spawned at launch.")]
    public GameObject NetworkAnchorManagerPrefab;

    [Tooltip("A prefab which contains all the \"network anchor player\" behaviours. The server will be spawn one of these for each connected player.")]
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
        else
        {
            Debug.LogError("[NetworkAnchorHelper] For the helper to function correctly, please set a NetworkAnchorManagerPrefab.");
        }

        if (NetworkAnchorPlayerPrefab != null)
        {
            manager.spawnPrefabs.Add(NetworkAnchorPlayerPrefab);
        }
        else
        {
            Debug.LogError("[NetworkAnchorHelper] For the helper to function correctly, please set a NetworkAnchorPlayerPrefab.");
        }
    }

    /// <summary>
    /// Spawn a network anchor manager across clients
    /// </summary>
    public void SpawnNetworkAnchorManager()
    {
        if (spawnedNetworkAnchorManager != null)
        {
            Debug.LogWarning("[NetworkAnchorHelper] Ignoring request to spawn a network anchor manager, since one was already created.");
            return;
        }

        if (NetworkAnchorManagerPrefab != null)
        {
            spawnedNetworkAnchorManager = Instantiate(NetworkAnchorManagerPrefab);
            spawnedNetworkAnchorManager.name = "NetworkAnchorManager";
            NetworkServer.Spawn(spawnedNetworkAnchorManager);
        }
        else
        {
            Debug.LogError("[NetworkAnchorHelper] Can't spawn a network anchor manager, since NetworkAnchorManagerPrefab is null.");
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
        else
        {
            Debug.LogError("[NetworkAnchorHelper] Can't spawn a network anchor player, since NetworkAnchorPlayerPrefab is null.");
        }
    }
}
