// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using UnityEngine.Networking;
using System;
using System.Collections.Generic;

public class MyNetworkAnchorClient : NetworkBehaviour
{
    /// <summary>
    /// The local anchor client.
    /// </summary>
    private static MyNetworkAnchorClient localInstance;
    public static MyNetworkAnchorClient LocalInstance
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
        Debug.LogFormat("[MyNetworkAnchorClient] Awoke. isLocalPlayer: {0} isServer: {1} isClient: {2}", isLocalPlayer, isServer, isClient);
        DontDestroyOnLoad(gameObject);
    }

    private string DebugInfo()
    {
        string clientConnectionIp = connectionToClient == null ? "not server" : connectionToClient.address;
        return string.Format("(netId: {0}) (isLocalPlayer: {1}) (isServer: {2}) (isClient: {3}) (hasAuthority: {4}) (connectionToClient address: {5})",
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
        Debug.LogFormat("[MyNetworkAnchorClient] OnStartAuthority. {0}", DebugInfo());

        if (hasAuthority)
        {
            Debug.LogFormat("[MyNetworkAnchorClient] OnStartAuthority. Setting instance. {0}", DebugInfo());
            localInstance = this;
        }
    }
    /// <summary>
    /// If anchor is unowned, export the anchor data stored in game object, take anchor ownership of the shared anchor,
    /// and share anchor with other players.
    /// </summary>
    public void DefaultNetworkAnchor(String anchorId, GameObject gameObject)
    {
        if (MyNetworkAnchorServer.Instance != null && 
            !MyNetworkAnchorServer.Instance.IsSharedAnchorOwned)
        {
            ShareNetworkAnchor(anchorId, gameObject);
        }
    }

    /// <summary>
    /// Export the anchor data stored in game object, take anchor ownership of the shared anchor, and share anchor with 
    /// other players.
    /// </summary>
    public void ShareNetworkAnchor(String anchorId, GameObject gameObject)
    {
        if (MyNetworkAnchorServer.Instance != null)
        {
            Debug.LogFormat("[MyNetworkAnchorClient] Ignoreing share anchor request, as there is no anchor server. (anchor id: {0})", anchorId);
            return;
        }

        if (MyNetworkAnchorServer.Instance.TrySharingAnchor(anchorId, gameObject))
        {
            // Start taking ownership of the anchor
            CmdShareAnchor(SharedAnchorData.Create(anchorId));
        }
    }

    /// <summary>
    /// A network command to allow clients to change the anchor source.
    /// </summary>
    [Command]
    private void CmdShareAnchor(SharedAnchorData anchorSource)
    {
        if (MyNetworkAnchorServer.Instance != null)
        {
            Debug.LogFormat("[MyNetworkAnchorClient] Command is setting the anchor source {0} {1}", anchorSource.ToString(), DebugInfo());
            MyNetworkAnchorServer.Instance.SetAnchorSource(anchorSource);
        }
        else
        {
            Debug.LogErrorFormat("[MyNetworkAnchorClient] Can't set anchor source, network anchor server is missing. {0} {1}", anchorSource.ToString(), DebugInfo());
        }
    }
}
