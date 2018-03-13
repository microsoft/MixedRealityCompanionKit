// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using Persistence;

public class NetworkAnchor : MonoBehaviour
{
    /// <summary>
    /// The game object to enable when the initial network anchor is being loaded.
    /// </summary>
    public GameObject LoadingAnchorRoot;

    /// <summary>
    /// The game object to enable when the initial network anchor has been loaded.
    /// </summary>
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
            Debug.LogError("[NetworkAnchor] Network Anchor can't function correctly when there isn't a Anchor Persistence behavior also applied.");
        }
    }

    /// <summary>
    /// Update anchor state.
    /// </summary>
    private void Update()
    {
        InitializeAnchorManagerOnce();
        InitializeAnchorPlayerOnce();
        UpdateActiveGameObjects();
    }

    /// <summary>
    /// Inititialize the anchor manager usage. Note, the manager instance won't be ready at "Start".
    /// </summary>
    private void InitializeAnchorManagerOnce()
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
    /// Check for a local instance of an anchor player. The local player may not exist at "Start".
    /// </summary>
    private void InitializeAnchorPlayerOnce()
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
            Debug.LogFormat("[NetworkAnchor] Applying remote anchor via anchor persistence");  
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
            Debug.LogErrorFormat("[NetworkAnchor] Unable to process persistence event without a local instance of the NetworkAnchorPlayer (anchor id: {0})", args.AnchorId);
            return;
        }

        if (pendingPersistenceEventArgs.Type == PersistenceEventType.Loaded)
        {
            Debug.LogFormat("[NetworkAnchor] Anchor persistence behavior has loaded an anchor from storage: {0}", pendingPersistenceEventArgs.AnchorId);
            anchorPlayer.DefaultNetworkAnchor(pendingPersistenceEventArgs.AnchorId, gameObject);
        }
        else if (pendingPersistenceEventArgs.Type == PersistenceEventType.Saved)
        {
            Debug.LogFormat("[NetworkAnchor] Anchor persistence behavior has saved a new anchor: {0}", pendingPersistenceEventArgs.AnchorId);
            anchorPlayer.ShareNetworkAnchor(pendingPersistenceEventArgs.AnchorId, gameObject);
        }

        pendingPersistenceEventArgs = null;
    }
}
