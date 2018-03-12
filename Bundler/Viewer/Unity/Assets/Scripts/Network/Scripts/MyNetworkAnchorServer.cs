// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using UnityEngine.Networking;
using UnityEngine.XR.WSA;
using UnityEngine.XR.WSA.Sharing;
using System;
using System.Collections.Generic;


/// <summary>
/// Data describing the source of the anchor data.
/// </summary>
[Serializable]
public struct SharedAnchorData
{
    public static SharedAnchorData Create(string anchorId)
    {
        SharedAnchorData result;
        result.SourceIp = MyNetworkAnchorServer.Instance == null ? "unknown" : MyNetworkAnchorServer.Instance.LocalAddress;
        result.AnchorId = anchorId;
        return result;
    }

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

public class ReceivedAnchorArgs
{
    public ReceivedAnchorArgs(WorldAnchorTransferBatch transferBatch)
    {
        TransferBatch = transferBatch;
    }

    /// <summary>
    /// The transfer batch containing the anchor
    /// </summary>
    public WorldAnchorTransferBatch TransferBatch { get; private set; }
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
    /// This event is raised When a new anchor arrives from a different player.
    /// </summary>
    /// <param name="data">The data that arrived.</param>
    public delegate void OnReceivedAnchor(MyNetworkAnchorServer sender, ReceivedAnchorArgs args);
    public event OnReceivedAnchor ReceivedAnchor;

    /// <summary>
    /// Get if the local anchor player is in the process of receiving a shared anchor.
    /// </summary>
    public bool ReceivingAnchor
    {
        get;
        private set;
    }

    /// <summary>
    /// Get the last received remote anchor
    /// </summary>
    public ReceivedAnchorArgs LastRecievedAnchor
    {
        get;
        private set;
    }

    /// <summary>
    /// Get the local IP address.
    /// </summary>
    public String LocalAddress
    {
        get;
        private set;
    }

    /// <summary>
    /// The current source of the shared anchor 
    /// </summary>
    [SyncVar(hook = "ImportAnchorData")]
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
    /// This will send or receive the binary anchor data on the local instance
    /// </summary>
    private GenericNetworkTransmitter anchorTransmitter;

    #region Exporting Anchor Methods
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
    /// Try exportong the anchor data stored in game object, and broadcast anchor data with other players.
    /// </summary>
    /// <summary>
    /// Return true if exporting was able to occur.
    /// </summary>
    [Client]
    public bool TrySharingAnchor(String anchorId, GameObject gameObject)
    {
        if (AnchorSource.AnchorId == anchorId)
        {
            Debug.LogFormat("[MyNetworkAnchorServer] Ignoreing share anchor request, as anchor is already being shared. (anchor id: {0})", anchorId);
            return false;
        }

        WorldAnchor worldAnchor = gameObject.GetComponent<WorldAnchor>();
        if (worldAnchor == null)
        {
            Debug.LogErrorFormat("[MyNetworkAnchorServer] Unable to acquire anchor ownership. Game object is missing an anchor. (anchor id: {0})", anchorId);
            return false;
        }

        Debug.LogFormat("[MyNetworkAnchorServer] Attempting to acquire anchor ownership and share anchor with other players. (anchor id: {0})", anchorId);

        // The last received anchor will no longer be relevant since we're taking ownership
        LastRecievedAnchor = null;

        // Stop all pending work on the anchor transmitter
        anchorTransmitter.StopAll();

        // Export binary data
        List<byte> buffer = new List<byte>();
        WorldAnchorTransferBatch batch = new WorldAnchorTransferBatch();
        batch.AddWorldAnchor(anchorId, worldAnchor);
        WorldAnchorTransferBatch.ExportAsync(
            batch,
            (byte[] data) => { buffer.AddRange(data); },
            (SerializationCompletionReason status) => { ExportAnchorDataComplete(status, buffer.ToArray(), anchorId); });

        return true;
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
            Debug.LogFormat("[MyNetworkAnchorServer] Exporting anchor succeeded: {0} ({1} bytes)", anchorId, data.Length);
            anchorTransmitter.SendData(data);
        }
        else
        {
            Debug.LogFormat("[MyNetworkAnchorServer] Exporting anchor failed: (anchor id: {0}) (status: {1}) ({2} bytes)", anchorId, status, data.Length);
        }
    }
    #endregion Exporting Anchor Methods

    #region Importing Anchor Methods
    /// <summary>
    /// When client starts, check if an anchor needs to be imported
    /// </summary>
    public override void OnStartClient()
    {
        base.OnStartClient();
        ImportAnchorData(AnchorSource);
    }

    /// <summary>
    /// Begin import anchor data from source.
    /// </summary>
    private void ImportAnchorData(SharedAnchorData anchorSource)
    {
        Debug.Log("[MyNetworkAnchorServer] AnchorSourceChanged was invoked: " + anchorSource.ToString());
        if (!anchorSource.IsValid)
        {
            Debug.Log("[MyNetworkAnchorServer] Ignoring anchor source since it's invalid.");
            return;
        }

        if (anchorSource.SourceIp == LocalAddress)
        {
            Debug.Log("[MyNetworkAnchorServer] Ignoring anchor source since it originated from this player.");
            return;
        }

        Debug.LogFormat("[MyNetworkAnchorClient] Importing anchor (source IP: {0}) (local ID: {1})", anchorSource.SourceIp, LocalAddress);
        ReceivingAnchor = true;
        anchorTransmitter.RequestData(anchorSource.SourceIp);
    }

    /// <summary>
    /// Called to begin the process of importing pending anchor data
    /// </summary>
    private void RequestAnchorDataCompleted(GenericNetworkTransmitter sender, RequestDataCompletedArgs args)
    {
        if (!args.Successful)
        {
            Debug.LogFormat("[MyNetworkAnchorClient] Failed to receive anchor data. {0}", DebugInfo());
            ImportAnchorDataCompleted(null);
            return;
        }

        if (args.Data == null || args.Data.Length == 0)
        {
            Debug.LogFormat("[MyNetworkAnchorClient] Binary anchor data is null or empty, ignoring request to import anchor data. {0}", DebugInfo());
            ImportAnchorDataCompleted(null);
            return;
        }

        Debug.LogFormat("[MyNetworkAnchorClient] Starting import of binary anchor data. ({0} bytes) {1}", args.Data.Length, DebugInfo());
        WorldAnchorTransferBatch.ImportAsync(args.Data, BatchImportAsyncCompleted);
    }

    /// <summary>
    /// Called when a remote anchor has been de-serialized
    /// </summary>
    /// <param name="status">Tracks if the import worked</param>
    /// <param name="batch">The WorldAnchorTransferBatch that has the anchor information.</param>
    private void BatchImportAsyncCompleted(
        SerializationCompletionReason status,
        WorldAnchorTransferBatch batch)
    {
        if (status != SerializationCompletionReason.Succeeded)
        {
            Debug.Log("[MyNetworkAnchor] Anchor import has failed");
            ImportAnchorDataCompleted(null);
        }
        else
        {
            Debug.Log("[MyNetworkAnchor] Anchor import has complete, and raising RevievedAnchor event");
            ImportAnchorDataCompleted(batch);
        }
    }

    /// <summary>
    /// The final fuinction called once a network anchor has been imported.
    /// </summary>
    private void ImportAnchorDataCompleted(WorldAnchorTransferBatch batch)
    {
        ReceivingAnchor = false;
        if (batch == null)
        {
            LastRecievedAnchor = null;
        }
        else
        {
            LastRecievedAnchor = new ReceivedAnchorArgs(batch);
        }

        if (ReceivedAnchor != null)
        {
            ReceivedAnchor(this, LastRecievedAnchor);
        }
    }
    #endregion Importing Achor Methods

    #region Initialization Methods
    /// <summary>
    /// On creation save this an the static instance. There should be only one manager.
    /// </summary>
    private void Awake()
    {
        Debug.Log("[MyNetworkAnchorServer] Awoke");
        DontDestroyOnLoad(gameObject);
        instance = this;

        this.InitializeLocalAddress();
        this.InitializeAnchorTransmitterOnce();
    }

    /// <summary>
    /// Initialize the local IP address. This should only be called with authority.
    /// </summary>
    private void InitializeLocalAddress()
    {
        if (!hasAuthority)
        {
            return;
        }

#if !UNITY_EDITOR
        foreach (Windows.Networking.HostName hostName in Windows.Networking.Connectivity.NetworkInformation.GetHostNames())
        {
            if (hostName.DisplayName.Split(".".ToCharArray()).Length == 4)
            {
                Debug.Log("[MyNetworkAnchor] Local IP " + hostName.DisplayName);
                LocalAddress = hostName.DisplayName;
                break;
            }
        }
#else
        LocalAddress = "editor" + UnityEngine.Random.Range(0, 999999).ToString();
#endif
    }

    /// <summary>
    /// Initialize the anchor transmitter only once
    /// </summary>
    private void InitializeAnchorTransmitterOnce()
    {
        if (anchorTransmitter == null)
        {
            anchorTransmitter = new GenericNetworkTransmitter();
            anchorTransmitter.RequestDataCompleted += RequestAnchorDataCompleted;
        }
    }
    #endregion Initialization Methods

    #region Debug Methods
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
    #endregion Debug Methods
}
