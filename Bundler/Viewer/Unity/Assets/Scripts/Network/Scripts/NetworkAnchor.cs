// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using Persistence;

public class NetworkAnchor : MonoBehaviour
{
    [Tooltip("The game object to enable when the initial network anchor is being loaded.")]
    public GameObject LoadingAnchorRoot;

    [Tooltip("The game object to enable when the initial network anchor has been loaded.")]
    public GameObject FoundAnchorRoot;

    /// <summary>
    /// For this to function, the game object must also have the AnchorPersistence behavior applied.
    /// </summary>
    private AnchorPersistence anchorPersistence;

    /// <summary>
    /// For this to function, there must be a global NetworkAnchorManager.
    /// </summary>
    private NetworkAnchorManager networkAnchorManager;

    /// <summary>
    /// For this to function, the game object must also have a local instance of an NetworkAnchorPlayer.
    /// </summary>
    private NetworkAnchorPlayer anchorPlayer;

    private PersistenceEventArgs pendingPersistenceEventArgs = null;

    private void Awake()
    {
        if (FoundAnchorRoot != null)
        {
            FoundAnchorRoot.SetActive(false);
        }

        if (LoadingAnchorRoot != null)
        {
            LoadingAnchorRoot.SetActive(false);
        }
    }

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
            Debug.LogError("[NetworkAnchor] Network Anchor can't function correctly when there isn't an Anchor Persistence behaviour applied.");
        }
    }

    /// <summary>
    /// Update anchor state.
    /// </summary>
    private void Update()
    {
        WhenReadyInitializeAnchorManagerOnce();
        WhenReadyInitializeAnchorPlayerOnce();
        UpdateActiveGameObjects();
    }

    /// <summary>
    /// Check if we can inititialize the anchor manager usage. If we can, only do the initialization work once. Note
    /// that the anchor manager instance won't be ready at "Start".
    /// </summary>
    private void WhenReadyInitializeAnchorManagerOnce()
    {
        // Check if already initialized
        if (networkAnchorManager != null)
        {
            return;
        }

        // Check if can initialize
        networkAnchorManager = NetworkAnchorManager.Instance;
        if (networkAnchorManager == null)
        {
            return;
        }

        networkAnchorManager.LastReceivedAnchorChanged += OnReceivedRemoteAnchor;
    }

    /// <summary>
    /// Check if we can inititialize the local instance of an anchor player. If we can, only do the initialization work
    /// once. Note that the local player may not exist at "Start".
    /// </summary>
    private void WhenReadyInitializeAnchorPlayerOnce()
    {
        // Check if already initialized
        if (anchorPlayer != null)
        {
            return;
        }

        // Check if can initialize
        anchorPlayer = NetworkAnchorPlayer.LocalInstance;
        if (anchorPlayer == null)
        {
            return;
        }

        // If an anchor blob was received from another player, now's the time to handle this.
        if (networkAnchorManager != null)
        {
            OnReceivedRemoteAnchor(networkAnchorManager, networkAnchorManager.LastReceivedAnchor);
        }
        else
        {
            Debug.LogError("[NetworkAnchor] Network Anchor can't function correctly when there isn't a Network Anchor Manager.");
        }

        OnPersistenceEvent(anchorPersistence, pendingPersistenceEventArgs);
    }

    /// <summary>
    /// Update "root" object based on if anchors are being downloaded.
    /// </summary>
    private void UpdateActiveGameObjects()
    {
        // Check if we've recieved an anchor manager yet.
        if (networkAnchorManager == null)
        {
            return;
        }

        if (LoadingAnchorRoot != null && networkAnchorManager.LoadingAnchor)
        {
            if (FoundAnchorRoot != null)
            {
                FoundAnchorRoot.SetActive(false);
            }

            LoadingAnchorRoot.SetActive(true);
        }
        else
        {
            // Only show "loading anchor root" once
            if (LoadingAnchorRoot != null)
            {
                LoadingAnchorRoot.SetActive(false);
                LoadingAnchorRoot = null;
            }

            // Show "found anchor root", if not loading or there is no "loading anchor root"
            if (FoundAnchorRoot != null)
            {
                FoundAnchorRoot.SetActive(true);
            }
        }
    }

    /// <summary>
    /// When receiving a remote anchor, apply it to this game object.
    /// </summary>
    private void OnReceivedRemoteAnchor(NetworkAnchorManager sender, LastReceivedAnchorArgs args)
    {
        if (args != null && anchorPersistence != null)
        {
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
            Debug.LogErrorFormat("[NetworkAnchor] Unexpected persistence event, anchor owner is not the expected game object (anchor id: {0})", args.AnchorId);
            return;
        }

        pendingPersistenceEventArgs = args;
        if (anchorPlayer == null)
        {
            Debug.LogErrorFormat("[NetworkAnchor] Unable to process persistence event without a local instance of the Network Anchor Player (anchor id: {0})", args.AnchorId);
            return;
        }

        if (pendingPersistenceEventArgs.Type == PersistenceEventType.Loaded)
        {
            anchorPlayer.DefaultNetworkAnchor(pendingPersistenceEventArgs.AnchorId, gameObject);
        }
        else if (pendingPersistenceEventArgs.Type == PersistenceEventType.Saved)
        {
            anchorPlayer.ShareNetworkAnchor(pendingPersistenceEventArgs.AnchorId, gameObject);
        }

        pendingPersistenceEventArgs = null;
    }
}
