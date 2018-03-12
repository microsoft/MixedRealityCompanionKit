// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using Persistence;

public class MyNetworkAnchor : MonoBehaviour
{
    /// <summary>
    /// For this to function, the game object must also have the AnchorPersistence behavior applied.
    /// </summary>
    private AnchorPersistence anchorPersistence;

    /// <summary>
    /// For this to function, the game object must also have a local instance of an MyNetworkAnchorClient.
    /// </summary>
    private MyNetworkAnchorClient anchorPlayer;

    private PersistenceEventArgs pendingPersistenceEventArgs = null;

    /// <summary>
    /// Initialization the Network Anchor. Note, this will search of an Anchor Persistence behavior. If not present,
    /// then this behavior will not function correctly.
    /// </summary>
    private void Start()
    {
        anchorPersistence = gameObject.GetComponent<AnchorPersistence>();
        if (anchorPersistence != null)
        {
            anchorPersistence.PersistenceEvent += OnPersistenceEvent;
        }
        else
        {
            Debug.LogError("[MyNetworkAnchor] Network Anchor can't function correctly when there isn't a Anchor Persistence behavior also applied.");
        }
    }

    /// <summary>
    /// Update anchor state.
    /// </summary>
    private void Update()
    {
        InitializeAnchorPlayerOnce();
    }

    /// <summary>
    /// Check for a local instance of an anchor player. The local player may not exist at start.
    /// </summary>
    private void InitializeAnchorPlayerOnce()
    {
        // Check if already initialized
        if (anchorPlayer != null)
        {
            return;
        }

        // Check if can initialize
        anchorPlayer = MyNetworkAnchorClient.LocalInstance;
        if (anchorPlayer == null)
        {
            return;
        }

        var networkAnchorManager = MyNetworkAnchorServer.Instance;
        if (networkAnchorManager != null)
        {
            networkAnchorManager.ReceivedAnchor += OnReceivedRemoteAnchor;
            OnReceivedRemoteAnchor(networkAnchorManager, networkAnchorManager.LastRecievedAnchor);
        }
        else
        {
            Debug.LogError("[MyNetworkAnchor] Network Anchor can't function correctly when there isn't a Network Anchor Manager.");
        }

        OnPersistenceEvent(anchorPersistence, pendingPersistenceEventArgs);


    }

    /// <summary>
    /// When receiving a remote anchor, apply it to this game object.
    /// </summary>
    private void OnReceivedRemoteAnchor(MyNetworkAnchorServer sender, ReceivedAnchorArgs args)
    {
        if (args != null && anchorPersistence != null)
        {
            Debug.LogFormat("[MyNetworkAnchor] Applying remote anchor via anchor persistence");  
            anchorPersistence.ApplyAnchor(args.TransferBatch, true);
            pendingPersistenceEventArgs = null;
        }
    }

    /// <summary>
    /// Handle load and save persistence events. During these events attempt to share an anchor if the current 
    /// conditions make sense for the given event.
    /// </summary>
    private void OnPersistenceEvent(AnchorPersistence source, PersistenceEventArgs args)
    {
        if (args == null)
        {
            return;
        }

        if (args.AnchorOwner != gameObject)
        {
            Debug.LogErrorFormat("[MyNetworkAnchor] Unexpected persistence event, anchor owner is not the expected game object (anchor id: {0})", args.AnchorId);
            return;
        }

        pendingPersistenceEventArgs = args;
        if (anchorPlayer == null)
        {
            Debug.LogErrorFormat("[MyNetworkAnchor] Unable to process persistence event without a local instance of the MyNetworkAnchorClient (anchor id: {0})", args.AnchorId);
            return;
        }

        if (pendingPersistenceEventArgs.Type == PersistenceEventType.Loaded)
        {
            Debug.LogFormat("[MyNetworkAnchor] Anchor persistence behavior has loaded an anchor from storage: {0}", pendingPersistenceEventArgs.AnchorId);
            anchorPlayer.DefaultNetworkAnchor(pendingPersistenceEventArgs.AnchorId, gameObject);
        }
        else if (pendingPersistenceEventArgs.Type == PersistenceEventType.Saved)
        {
            Debug.LogFormat("[MyNetworkAnchor] Anchor persistence behavior has saved a new anchor: {0}", pendingPersistenceEventArgs.AnchorId);
            anchorPlayer.ShareNetworkAnchor(pendingPersistenceEventArgs.AnchorId, gameObject);
        }

        pendingPersistenceEventArgs = null;
    }
}
