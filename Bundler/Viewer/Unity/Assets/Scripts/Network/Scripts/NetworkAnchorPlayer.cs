// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using UnityEngine.Networking;
using System;
using System.Collections.Generic;

public class NetworkAnchorPlayer : NetworkBehaviour
{
    /// <summary>
    /// The local anchor player.
    /// </summary>
    private static NetworkAnchorPlayer localInstance;
    public static NetworkAnchorPlayer LocalInstance
    {
        get
        {
            return localInstance;
        }
    }

    /// <summary>
    /// Prevent client from being destroyed on scene changes.
    /// </summary>
    private void Awake()
    {
        DontDestroyOnLoad(gameObject);
    }

    private string DebugInfo()
    {
        string clientConnectionIp = connectionToClient == null ? "not server" : connectionToClient.address;
        return string.Format("(netId: {0}) (isLocalPlayer: {1}) (isServer: {2}) (isClient: {3}) (hasAuthority: {4}) (connection to client: {5})",
            netId,
            isLocalPlayer,
            isServer,
            isClient,
            hasAuthority,
            clientConnectionIp);
    }

    public override void OnStartAuthority()
    {
        base.OnStartAuthority();

        if (hasAuthority)
        {
            localInstance = this;
        }
    }
    /// <summary>
    /// If anchor is unowned, export the anchor data stored in game object, take anchor ownership of the shared anchor,
    /// and broadcast anchor data to other players.
    /// </summary>
    public void DefaultNetworkAnchor(String anchorId, GameObject gameObject)
    {
        if (NetworkAnchorManager.Instance != null && 
            !NetworkAnchorManager.Instance.IsSharedAnchorOwned)
        {
            ShareNetworkAnchor(anchorId, gameObject);
        }
    }

    /// <summary>
    /// Export the anchor data stored in game object, take anchor ownership of the shared anchor, and broadcast anchor
    /// data to other players.
    /// </summary>
    public void ShareNetworkAnchor(String anchorId, GameObject gameObject)
    {
        if (NetworkAnchorManager.Instance == null)
        {
            Debug.LogFormat("[NetworkAnchorPlayer] Ignoring share anchor request, as there is no anchor server. (anchor id: {0}) {1}", anchorId, DebugInfo());
            return;
        }

        if (NetworkAnchorManager.Instance.TrySharingAnchor(anchorId, gameObject))
        {
            // Start taking ownership of the anchor
            CmdShareAnchor(SharedAnchorData.Create(anchorId));
        }
    }

    /// <summary>
    /// A network command to allow local clients to change the anchor source.
    /// </summary>
    [Command]
    private void CmdShareAnchor(SharedAnchorData anchorSource)
    {
        if (NetworkAnchorManager.Instance != null)
        {
            Debug.LogFormat("[NetworkAnchorPlayer] Command is setting the anchor source {0} {1}", anchorSource.ToString(), DebugInfo());
            NetworkAnchorManager.Instance.SetAnchorSource(anchorSource);
        }
        else
        {
            Debug.LogErrorFormat("[NetworkAnchorPlayer] Can't set anchor source, network anchor server is missing. {0} {1}", anchorSource.ToString(), DebugInfo());
        }
    }
}
