// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using UnityEngine.Networking;
using System;


/// <summary>
/// Data describing the source of the anchor data.
/// </summary>
[Serializable]
public struct SharedAnchorData
{
    /// <summary>
    /// The IP address of the player that is serving the most recent anchor
    /// </summary>
    public string SourceIp;

    /// <summary>
    /// The id (or name) of the anchor.
    /// </summary>
    public string AnchorId;

    /// <summary>
    /// Test if this meta data object hold valid information
    /// </summary>
    public bool IsValid
    {
        get
        {
            return !String.IsNullOrEmpty(SourceIp) && !String.IsNullOrEmpty(AnchorId);
        }
    }

    public override string ToString()
    {
        return String.Format("(anchor source ip: {0}) (anchor id: {1})", SourceIp, AnchorId);
    }
}

public class MyNetworkAnchorServer : NetworkBehaviour
{
    private static MyNetworkAnchorServer instance;
    public static MyNetworkAnchorServer Instance
    {
        get
        {
            return instance;
        }
    }

    /// <summary>
    /// When data arrives, this event is raised.
    /// </summary>
    public delegate void OnAnchorSourceChanged(MyNetworkAnchorServer source, SharedAnchorData args);
    public event OnAnchorSourceChanged AnchorSourceChanged;

    /// <summary>
    /// The current source of the shared anchor 
    /// </summary>
    [SyncVar(hook = "AnchorSourceHook")]
    public SharedAnchorData AnchorSource;

    /// <summary>
    /// Get if the anchor is currently owned by a player.
    /// </summary>
    public bool IsSharedAnchorOwned
    {
        get
        {
            return AnchorSource.IsValid;
        }
    }

    /// <summary>
    /// Set the anchor source.
    /// </summary>
    [Server]
    public void SetAnchorSource(SharedAnchorData anchorSource)
    {
        Debug.Log("[MyNetworkAnchorServer] Server is setting the anchor source: " + anchorSource.ToString());
        AnchorSource = anchorSource;
    }

    /// <summary>
    /// On creation save this an the static instance. There should be only one manager.
    /// </summary>
    private void Awake()
    {
        Debug.Log("[MyNetworkAnchorServer] Awoke");
        DontDestroyOnLoad(gameObject);
        instance = this;
    }

    /// <summary>
    /// Raise a change event so to notify listeners
    /// </summary>
    private void AnchorSourceHook(SharedAnchorData anchorSource)
    {
        Debug.Log("[MyNetworkAnchorServer] AnchorSourceHook was invoked: " + anchorSource.ToString());
        if (AnchorSourceChanged != null)
        {
            AnchorSourceChanged(this, anchorSource);
        }
    }
}
