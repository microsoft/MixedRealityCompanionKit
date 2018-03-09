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
    private SharedAnchorData sharedAnchor;
    public SharedAnchorData SharedAnchor
    {
        get
        {
            return sharedAnchor;
        }

        set
        {
            if (sharedAnchor.AnchorId != value.AnchorId ||
                sharedAnchor.SourceIp != value.SourceIp)
            {
                sharedAnchor = value;
                SharedAnchorChanged();
            }
        }
    }

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
            return SharedAnchor.IsValid;
        }
    }

    /// <summary>
    /// Test is the LocalIp is the owner of the anchor source
    /// </summary>
    public bool IsSharedAnchorOwner
    {
        get
        {
            return SharedAnchor.SourceIp == LocalIp;
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
    /// The last known binary anchor data to import
    /// </summary>
    private byte[] pendingAnchorData;

    /// <summary>
    /// Initialization the Network Anchor. Note, this will search of an Anchor Persistence behavior. If not present,
    /// then this behavior will not function correctly.
    /// </summary>
    private void Start()
    {
        anchorTransmitter = new GenericNetworkTransmitter();
        anchorTransmitter.RevievedData += RevievedAnchorData;

        anchorPersistence = gameObject.GetComponent<AnchorPersistence>();
        if (anchorPersistence != null)
        {
            anchorPersistence.PersistenceEvent += OnPersistenceEvent;
        }
        else
        {
            Debug.LogError("Network Anchor can't function correctly when there isn't a Anchor Persistence behavior also applied.");
        }

        networkAnchorManager = MyNetworkAnchorManager.Instance;
        if (networkAnchorManager != null)
        {
            networkAnchorManager.AnchorSourceChanged += OnAnchorSourceChanged;
        }
        else
        {
            Debug.LogError("Network Anchor can't function correctly when there isn't a Network Anchor Manager.");
        }

#if !UNITY_EDITOR
        foreach (Windows.Networking.HostName hostName in Windows.Networking.Connectivity.NetworkInformation.GetHostNames())
        {
            if (hostName.DisplayName.Split(".".ToCharArray()).Length == 4)
            {
                Debug.Log("Local IP " + hostName.DisplayName);
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
        ImportAnchorData();
    }

    /// <summary>
    /// Handle load and save persistence events. During these events attempt to share an anchor if the current 
    /// conditions make sense for the given event.
    /// </summary>
    private void OnPersistenceEvent(AnchorPersistence source, PersistenceEventArgs args)
    {
        if (args.AnchorOwner != gameObject)
        {
            Debug.LogError("Unexcepted presistence event, anchor owener is not expected game object: " + args.AnchorId);
            return;
        }

        if (args.Type == PersistenceEventType.Loaded)
        {
            Debug.Log("Anchor Presistence behavior has loaded an anchor for storage: " + args.AnchorId);
            if (!IsSharedAnchorOwned)
            {
                ShareAnchor(args.AnchorId);
            }
        }
        else if (args.Type == PersistenceEventType.Saved)
        {
            Debug.Log("Anchor Presistence behavior has saved a new anchor: " + args.AnchorId);
            if (!IsSharedAnchorOwned || SharedAnchor.AnchorId != args.AnchorId)
            {
                ShareAnchor(args.AnchorId);
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
        SharedAnchor = args;
    }

    /// <summary>
    /// Take anchor ownership and share anchor data with other players.
    /// </summary>
    private void ShareAnchor(String anchorId)
    {
        WorldAnchor worldAnchor = gameObject.GetComponent<WorldAnchor>();
        if (worldAnchor == null)
        {
            Debug.LogError("Unable to share anchor with other players. Game object is missing an anchor: " + anchorId);
            return;
        }

        Debug.Log("Attempting to share anchor with other players: " + anchorId);
        List<byte> buffer = new List<byte>();
        WorldAnchorTransferBatch batch = new WorldAnchorTransferBatch();
        batch.AddWorldAnchor(anchorId, worldAnchor);
        WorldAnchorTransferBatch.ExportAsync(
            batch,
            (byte[] data) => { buffer.AddRange(data); },
            (SerializationCompletionReason status) => { ExportAnchorDataComplete(status, buffer.ToArray(), anchorId); });
    }

    /// <summary>
    /// Called we have recieved new anchor binary data
    /// </summary>
    /// <param name="data"></param>
    private void RevievedAnchorData(byte[] data)
    {
        // If we were the source of the anchor data ignore
        if (IsSharedAnchorOwner)
        {
            return;
        }

        pendingAnchorData = data;
    }

    /// <summary>
    /// This is invoke by Unity when the networking SyncVar "AnchorSourceJson" changes.
    /// </summary>
    /// <param name="value">The new anchor name</param>
    private void SharedAnchorChanged()
    {
        if (!SharedAnchor.IsValid)
        {
            Debug.Log("Attempting to share an invalid anchor");
            return;
        }

        if (!IsSharedAnchorOwner)
        {
            Debug.Log("Requesting anchor from anchor source: " + sharedAnchor.AnchorId);
            anchorTransmitter.RequestData(SharedAnchor.SourceIp);
        }
        else if (networkAnchorManager != null)
        {
            Debug.Log("Setting anchor manager's anchor source: " + sharedAnchor.AnchorId);
            networkAnchorManager.SetAnchorSource(SharedAnchor);
        }
        else
        {
            Debug.Log("Unable to handle shareAnchor change: " + sharedAnchor.AnchorId);
        }
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
            Debug.Log("Exporting anchor succeeded: " + anchorId);

            SharedAnchorData anchorSource;
            anchorSource.SourceIp = LocalIp;
            anchorSource.AnchorId = anchorId;
            anchorTransmitter.SendData(data);
            SharedAnchor = anchorSource;
        }
        else
        {
            Debug.Log("Exporting anchor failed: " + anchorId + "(" + status + ") (" + data.Length + ")");
        }
    }

    /// <summary>
    /// Call to begin the process of importing pending anchor data
    /// </summary>
    private void ImportAnchorData()
    {
        if (ImportingSharedAnchorData || pendingAnchorData == null || pendingAnchorData.Length == 0)
        {
            return;
        }

        ImportingSharedAnchorData = true;
        WorldAnchorTransferBatch.ImportAsync(pendingAnchorData, ImportAnchorDataComplete);
        pendingAnchorData = null;
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
            Debug.Log("Anchor import has failed");
        }
        else if (anchorPersistence != null)
        { 
            Debug.Log("Anchor import has complete, applying achor via Anchor Persistence behavior");
            anchorPersistence.ApplyAnchor(batch, true);
        }
        else
        {
            Debug.Log("Anchor import has complete, but can't apply since Anchor Persistence behavior is missing");
        }

        ImportingSharedAnchorData = false;
    }
    private void OnTrackingChanged(WorldAnchor self, bool located)
    {
        if (located)
        {
            self.OnTrackingChanged -= OnTrackingChanged;

#if !UNITY_EDITOR
            //AnchorEstablished = true;
            //WorldAnchorManager.Instance.AnchorStore.Save(AnchorName, self);
#endif
        }
    }
}
