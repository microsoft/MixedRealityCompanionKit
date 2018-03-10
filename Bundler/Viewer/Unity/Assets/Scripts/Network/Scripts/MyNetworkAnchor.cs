// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using System.Collections.Generic;
using UnityEngine.Networking;
using UnityEngine.XR.WSA;
using UnityEngine.XR.WSA.Sharing;
using Persistence;

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
    /// Test if this metadata object hold valid information
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
        return String.Format("( sourceIp: {0}, anchorId: {1} )", SourceIp, AnchorId);
    }
}

public class MyNetworkAnchor : MonoBehaviour
{
    /// <summary>
    /// Data on about which player the anchor is currently being served from.
    /// </summary>
    private SharedAnchorData anchorSource;

    /// <summary>
    /// Get the local ip address
    /// </summary>
    public String LocalIp
    {
        get;
        private set;
    }

    /// <summary>
    /// Incidates if binary anchor data is being imported
    /// </summary>
    public bool ImportingSharedAnchorData
    {
        get;
        private set;
    }

    /// <summary>
    /// Get if the anchor is currently owned by a player.
    /// </summary>
    public bool IsSharedAnchorOwned
    {
        get
        {
            return anchorSource.IsValid;
        }
    }

    /// <summary>
    /// Test is the LocalIp is the owner of the anchor source
    /// </summary>
    public bool IsAnchourSourceOwner
    {
        get
        {
            return anchorSource.SourceIp == LocalIp;
        }
    }

    /// <summary>
    /// This will send or recieve the binary anchor data.
    /// </summary>
    private GenericNetworkTransmitter anchorTransmitter;

    /// <summary>
    /// For this to function, the game object must also have the AnchorPersistence behavior applied.
    /// </summary>
    private AnchorPersistence anchorPersistence;

    /// <summary>
    /// For this to function, the network manager must spawn a game object with an anchor manager.
    /// The anchor manager will mange sync vars related to sharing anchors.
    /// </summary>
    private MyNetworkAnchorManager networkAnchorManager;

    /// <summary>
    /// Initialization the Network Anchor. Note, this will search of an Anchor Persistence behavior. If not present,
    /// then this behavior will not function correctly.
    /// </summary>
    private void Start()
    {
        anchorTransmitter = new GenericNetworkTransmitter();
        anchorTransmitter.RevievedData += ImportAnchorData;

        anchorPersistence = gameObject.GetComponent<AnchorPersistence>();
        if (anchorPersistence != null)
        {
            anchorPersistence.PersistenceEvent += OnPersistenceEvent;
        }
        else
        {
            Debug.LogError("[MyNetworkAnchor] Network Anchor can't function correctly when there isn't a Anchor Persistence behavior also applied.");
        }

        networkAnchorManager = MyNetworkAnchorManager.Instance;
        if (networkAnchorManager != null)
        {
            networkAnchorManager.AnchorSourceChanged += OnAnchorSourceChanged;
        }
        else
        {
            Debug.LogError("[MyNetworkAnchor] Network Anchor can't function correctly when there isn't a Network Anchor Manager.");
        }

#if !UNITY_EDITOR
        foreach (Windows.Networking.HostName hostName in Windows.Networking.Connectivity.NetworkInformation.GetHostNames())
        {
            if (hostName.DisplayName.Split(".".ToCharArray()).Length == 4)
            {
                Debug.Log("[MyNetworkAnchor] Local IP " + hostName.DisplayName);
                LocalIp = hostName.DisplayName;
                break;
            }
        }
#else
        LocalIp = "editor" + UnityEngine.Random.Range(0, 999999).ToString();
#endif
    }

    // Update is called once per frame
    private void Update()
    {
    }

    /// <summary>
    /// Handle load and save persistence events. During these events attempt to share an anchor if the current 
    /// conditions make sense for the given event.
    /// </summary>
    private void OnPersistenceEvent(AnchorPersistence source, PersistenceEventArgs args)
    {
        if (args.AnchorOwner != gameObject)
        {
            Debug.LogErrorFormat("[MyNetworkAnchor] Unexcepted presistence event, anchor owener is not expected game object: {0}", args.AnchorId);
            return;
        }

        if (args.Type == PersistenceEventType.Loaded)
        {
            Debug.LogFormat("[MyNetworkAnchor] Anchor Presistence behavior has loaded an anchor for storage: {0}", args.AnchorId);
            if (!IsSharedAnchorOwned)
            {
                ExportAnchorData(args.AnchorId);
            }
        }
        else if (args.Type == PersistenceEventType.Saved)
        {
            Debug.LogFormat("[MyNetworkAnchor] Anchor Presistence behavior has saved a new anchor: {0}", args.AnchorId);
            if (!IsSharedAnchorOwned || anchorSource.AnchorId != args.AnchorId)
            {
                ExportAnchorData(args.AnchorId);
            }
        }
    }

    /// <summary>
    /// Handle manager changes to the anchor source data.
    /// </summary>
    /// <param name="source"></param>
    /// <param name="args"></param>
    private void OnAnchorSourceChanged(MyNetworkAnchorManager source, SharedAnchorData args)
    {
        SetAnchorSource(args);
    }

    /// <summary>
    /// Set the anchor source, and either import external data or start exporting data.
    /// </summary>
    /// <param name="value">The new anchor name</param>
    private void SetAnchorSource(SharedAnchorData args)
    {
        if (!args.IsValid)
        {
            Debug.Log("[MyNetworkAnchor] Attempting to share an invalid anchor");
            return;
        }

        if (anchorSource.AnchorId == args.AnchorId &&
            anchorSource.SourceIp == args.SourceIp)
        {
            Debug.Log("[MyNetworkAnchor] Setting anchor with not differences, so ignoring request");
            return;
        }

        anchorSource = args;

        if (!IsAnchourSourceOwner)
        {
            Debug.LogFormat("[MyNetworkAnchor] Requesting anchor binary data from anchor source: {0}", anchorSource.AnchorId);
            anchorTransmitter.RequestData(anchorSource.SourceIp);
        }
        else if (networkAnchorManager != null)
        {
            Debug.LogFormat("[MyNetworkAnchor] Notifying listeners of a new anchor source: {0}", anchorSource.AnchorId);
            networkAnchorManager.SetAnchorSource(anchorSource);
        }
        else
        {
            Debug.LogFormat("[MyNetworkAnchor] Unable to handle shareAnchor changes: {0}", anchorSource.AnchorId);
        }
    }

    /// <summary>
    /// Take anchor ownership and share anchor data with other players.
    /// </summary>
    private void ExportAnchorData(String anchorId)
    {
        WorldAnchor worldAnchor = gameObject.GetComponent<WorldAnchor>();
        if (worldAnchor == null)
        {
            Debug.LogErrorFormat("[MyNetworkAnchor] Unable to acquire anchor ownership. Game object is missing an anchor: {0}", anchorId);
            return;
        }

        Debug.LogFormat("[MyNetworkAnchor] Attempting to acquire anchor ownership and share anchor with other players: {0}", anchorId);
        List<byte> buffer = new List<byte>();
        WorldAnchorTransferBatch batch = new WorldAnchorTransferBatch();
        batch.AddWorldAnchor(anchorId, worldAnchor);
        WorldAnchorTransferBatch.ExportAsync(
            batch,
            (byte[] data) => { buffer.AddRange(data); },
            (SerializationCompletionReason status) => { ExportAnchorDataComplete(status, buffer.ToArray(), anchorId); });
    }

    /// <summary>
    /// This is invoked once we've finished exported the binary anchor data to a byte array.
    /// </summary>
    private void ExportAnchorDataComplete(
        SerializationCompletionReason status, 
        byte[] data,
        String anchorId)
    {
        if (status == SerializationCompletionReason.Succeeded)
        {
            Debug.LogFormat("[MyNetworkAnchor] Exporting anchor succeeded: {0} ({1} bytes)", anchorId, data.Length);

            SharedAnchorData anchorSource;
            anchorSource.SourceIp = LocalIp;
            anchorSource.AnchorId = anchorId;
            anchorTransmitter.SendData(data);
            SetAnchorSource(anchorSource);
        }
        else
        {
            Debug.LogFormat("[MyNetworkAnchor] Exporting anchor failed: {0} ({1}) ({2} bytes)", anchorId, status, data.Length);
        }
    }

    /// <summary>
    /// Call to begin the process of importing pending anchor data
    /// </summary>
    private void ImportAnchorData(byte[] data)
    {
        // If we were the source of the anchor data ignore
        if (IsAnchourSourceOwner)
        {
            Debug.Log("[MyNetworkAnchor] Owner of anchor, ignoring request to import anchor data.");
            return;
        }

        if (data == null || data.Length == 0)
        {
            Debug.Log("[MyNetworkAnchor] Binary anchor data is null or empty, ignoring request to import anchor data.");
            return;
        }

        Debug.LogFormat("[MyNetworkAnchor] Starting import of binary anchor data. ({0} bytes)", data.Length);
        ImportingSharedAnchorData = true;
        WorldAnchorTransferBatch.ImportAsync(data, ImportAnchorDataComplete);
    }

    /// <summary>
    /// Called when a remote anchor has been deserialized
    /// </summary>
    /// <param name="status">Tracks if the import worked</param>
    /// <param name="wat">The WorldAnchorTransferBatch that has the anchor information.</param>
    private void ImportAnchorDataComplete(
        SerializationCompletionReason status,
        WorldAnchorTransferBatch batch)
    {
        if (status != SerializationCompletionReason.Succeeded)
        {
            // if we failed, we can simply try again. (TODO)
            Debug.Log("[MyNetworkAnchor] Anchor import has failed");
        }
        else if (anchorPersistence != null)
        { 
            Debug.Log("[MyNetworkAnchor] Anchor import has complete, applying achor via Anchor Persistence behavior");
            anchorPersistence.ApplyAnchor(batch, true);
        }
        else
        {
            Debug.Log("[MyNetworkAnchor] Anchor import has complete, but can't apply since Anchor Persistence behavior is missing");
        }

        ImportingSharedAnchorData = false;
    }
}
