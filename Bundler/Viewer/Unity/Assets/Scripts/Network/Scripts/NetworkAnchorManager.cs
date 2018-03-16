// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using UnityEngine.Networking;
using UnityEngine.XR.WSA;
using System;
using System.Collections.Generic;

#if UNITY_WSA
using UnityEngine.XR.WSA.Sharing;
#else
/// <summary>
/// Mock enum to allow building on non-uwp platforms.
/// </summary>
public enum SerializationCompletionReason
{
    Succeeded = 0,
    NotSupported = 1,
    AccessDenied = 2,
    UnknownError = 3
}

/// <summary>
/// Mock class to allow building on non-uwp platforms.
/// </summary>
public class WorldAnchorTransferBatch
{
    public delegate void SerializationDataAvailableDelegate(byte[] data);
    public delegate void SerializationCompleteDelegate(SerializationCompletionReason completionReason);
    public delegate void DeserializationCompleteDelegate(SerializationCompletionReason completionReason, WorldAnchorTransferBatch deserializedTransferBatch);

    public string[] GetAllIds()
    {
        return new string[0];
    }

    public void AddWorldAnchor(string id, WorldAnchor anchor)
    {
    }

    public WorldAnchor LockObject(string id, GameObject obj)
    {
        return null;
    }

    public static void ExportAsync(
        WorldAnchorTransferBatch batch, 
        SerializationDataAvailableDelegate dataBuffer,
        SerializationCompleteDelegate completeCallback)
    {
        if (completeCallback != null)
        {
            completeCallback(SerializationCompletionReason.NotSupported);
        }
    }

    public static void ImportAsync(
        byte[] data,
        DeserializationCompleteDelegate completeCallback)
    {
        if (completeCallback != null)
        {
            completeCallback(SerializationCompletionReason.NotSupported, null);
        }
    }
}
#endif

/// <summary>
/// Data describing the source of the shared anchor data.
/// </summary>
[Serializable]
public struct SharedAnchorData
{
    public static SharedAnchorData Create(string anchorId)
    {
        SharedAnchorData result;
        result.SourceIp = NetworkAnchorManager.Instance == null ? "unknown" : NetworkAnchorManager.Instance.LocalAddress;
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

public class LastReceivedAnchorArgs
{
    public LastReceivedAnchorArgs(WorldAnchorTransferBatch transferBatch)
    {
        TransferBatch = transferBatch;
    }

    /// <summary>
    /// The transfer batch containing the anchor
    /// </summary>
    public WorldAnchorTransferBatch TransferBatch { get; private set; }

    /// <summary>
    /// The first anchor id in the transfer batch
    /// </summary>
    public string AnchorId
    {
        get
        {
            if (TransferBatch == null)
            {
                return String.Empty;
            }

            var ids = TransferBatch.GetAllIds();
            if (ids == null || ids.Length == 0)
            {
                return String.Empty;
            }

            return ids[0];
        }
    }
}

public class NetworkAnchorManager : NetworkBehaviour
{
    /// <summary>
    /// Get the local instance of the anchor manager
    /// </summary>
    private static NetworkAnchorManager instance;
    public static NetworkAnchorManager Instance
    {
        get
        {
            return instance;
        }
    }

    [SyncVar(hook = "ImportAnchorData")]
    [Tooltip("The current source of the shared anchor.")]
    public SharedAnchorData AnchorSource;

    /// <summary>
    /// This event is raised when a new anchor arrives from a different player.
    /// </summary>
    /// <param name="args">Contains the data that arrived.</param>
    public delegate void OnLastReceivedAnchor(NetworkAnchorManager sender, LastReceivedAnchorArgs args);
    public event OnLastReceivedAnchor LastReceivedAnchorChanged;

    /// <summary>
    /// Get if the local anchor player is in the process of receiving a shared anchor.
    /// </summary>
    public bool LoadingAnchor { get; private set; }

    /// <summary>
    /// Get the last received remote anchor
    /// </summary>
    public LastReceivedAnchorArgs LastReceivedAnchor { get; private set; }

    /// <summary>
    /// Get the local IP address.
    /// </summary>
    public String LocalAddress { get; private set; }

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

    /// <summary>
    /// Retry failed exports every second.
    /// </summary>
    private const float retryExportDelaySeconds = 1.0f;

#region Exporting Anchor Methods
    /// <summary>
    /// Set the anchor source.
    /// </summary>
    [Server]
    public void SetAnchorSource(SharedAnchorData anchorSource)
    {
        Debug.LogFormat("[NetworkAnchorManager] Server is setting the anchor source. {0} {1}", anchorSource.ToString(), DebugInfo());
        AnchorSource = anchorSource;
    }

    /// <summary>
    /// Try exporting the anchor data stored in game object, and broadcast anchor data with other players.
    /// </summary>
    /// <summary>
    /// Return true if exporting was able to occur.
    /// </summary>
    [Client]
    public bool TrySharingAnchor(String anchorId, GameObject gameObject)
    {
        if (HolographicSettings.IsDisplayOpaque)
        {
            Debug.LogFormat("[NetworkAnchorManager] Ignoring share anchor request, as this device doesn't support anchoring. (anchor id: {0})", anchorId);
            return false;
        }

        if (AnchorSource.AnchorId == anchorId)
        {
            Debug.LogFormat("[NetworkAnchorManager] Ignoring share anchor request, as anchor is already being shared. (anchor id: {0})", anchorId);
            return false;
        }

        if (LastReceivedAnchor != null && LastReceivedAnchor.AnchorId == anchorId)
        {
            Debug.LogFormat("[NetworkAnchorManager] Ignoring share anchor request, as anchor was just received. (anchor id: {0})", anchorId);
            return false;
        }

        WorldAnchor worldAnchor = gameObject.GetComponent<WorldAnchor>();
        if (worldAnchor == null)
        {
            Debug.LogErrorFormat("[NetworkAnchorManager] Unable to acquire anchor ownership. Game object is missing an anchor. (anchor id: {0})", anchorId);
            return false;
        }

        Debug.LogFormat("[NetworkAnchorManager] Attempting to acquire anchor ownership and share anchor with other players. (new anchor id: {0}) {1} {2}", anchorId, AnchorSource.ToString(), DebugInfo());

        // The last received anchor will no longer be relevant since we're taking ownership
        LastReceivedAnchor = null;

        // Stop all pending work on the anchor transmitter
        anchorTransmitter.StopAll();

        // Export binary data
        List<byte> buffer = new List<byte>();
        WorldAnchorTransferBatch batch = new WorldAnchorTransferBatch();
        batch.AddWorldAnchor(anchorId, worldAnchor);
        WorldAnchorTransferBatch.ExportAsync(
            batch,
            (byte[] data) => { buffer.AddRange(data); },
            (SerializationCompletionReason status) => { ExportAnchorDataComplete(status, buffer.ToArray(), anchorId, gameObject); });

        return true;
    }

    /// <summary>
    /// This is invoked once we've finished exported the binary anchor data to a byte array.
    /// </summary>
    private void ExportAnchorDataComplete(
        SerializationCompletionReason status,
        byte[] data,
        String anchorId,
        GameObject gameObject)
    {
        if (status == SerializationCompletionReason.Succeeded)
        {
            Debug.LogFormat("[NetworkAnchorManager] Exporting anchor succeeded. (anchor id: {0}) (bytes: {1}) {2}", anchorId, data.Length, DebugInfo());
            anchorTransmitter.SendData(data);
        }
        else
        {
            Debug.LogErrorFormat("[NetworkAnchorManager] Exporting anchor failed, going to retrying. (anchor id: {0}) (status: {1}) (bytes: {2}) {3}", anchorId, status, data.Length, DebugInfo());
            StartCoroutine(RetrySharingAnchor(anchorId, gameObject));
        }
    }

    /// <summary>
    /// Retry sharing anchor, if it's still possible to.
    /// </summary>
    private System.Collections.IEnumerator RetrySharingAnchor(String anchorId, GameObject gameObject)
    {
        yield return new WaitForSeconds(retryExportDelaySeconds);

        // If loading and received an anchor, don't continue to try to share anchor data.
        if (!LoadingAnchor && LastReceivedAnchor == null)
        {
            TrySharingAnchor(anchorId, gameObject);
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
        if (HolographicSettings.IsDisplayOpaque)
        {
            Debug.LogFormat("[NetworkAnchorManager] Ignoring import anchor request, as this device doesn't support anchoring. {0} {1}", anchorSource.ToString(), DebugInfo());
            return;
        }

        if (!anchorSource.IsValid)
        {
            Debug.LogFormat("[NetworkAnchorManager] Ignoring anchor source since it's invalid. {0} {1}", anchorSource.ToString(), DebugInfo());
            return;
        }

        if (anchorSource.SourceIp == LocalAddress)
        {
            Debug.LogFormat("[NetworkAnchorManager] Ignoring anchor source since it originated from this player. {0} {1}", anchorSource.ToString(), DebugInfo());
            return;
        }

        Debug.LogFormat("[NetworkAnchorManager] Importing anchor. {0} {1}", anchorSource.ToString(), DebugInfo());

        LoadingAnchor = true;
        anchorTransmitter.RequestData(anchorSource.SourceIp);
    }

    /// <summary>
    /// Called to begin the process of importing pending anchor data
    /// </summary>
    private void RequestAnchorDataCompleted(GenericNetworkTransmitter sender, RequestDataCompletedArgs args)
    {
        if (!args.Successful)
        {
            Debug.LogErrorFormat("[NetworkAnchorManager] Failed to receive anchor data. {0}", DebugInfo());
            ImportAnchorDataCompleted(null);
            return;
        }

        if (args.Data == null || args.Data.Length == 0)
        {
            Debug.LogErrorFormat("[NetworkAnchorManager] Binary anchor data is null or empty, ignoring request to import anchor data. {0}", DebugInfo());
            ImportAnchorDataCompleted(null);
            return;
        }

        Debug.LogFormat("[NetworkAnchorManager] Starting import of binary anchor data. (bytes: {0}) {1}", args.Data.Length, DebugInfo());
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
            Debug.LogErrorFormat("[NetworkAnchorManager] Anchor import has failed. (status: {0})", status);
            ImportAnchorDataCompleted(null);
        }
        else
        {
            Debug.Log("[NetworkAnchorManager] Anchor import was successful.");
            ImportAnchorDataCompleted(batch);
        }
    }

    /// <summary>
    /// The final function called once a network anchor has been imported.
    /// </summary>
    private void ImportAnchorDataCompleted(WorldAnchorTransferBatch batch)
    {
        if (batch == null)
        {
            LastReceivedAnchor = null;
        }
        else
        {
            LastReceivedAnchor = new LastReceivedAnchorArgs(batch);
        }

        LoadingAnchor = false;

        if (LastReceivedAnchorChanged != null)
        {
            LastReceivedAnchorChanged(this, LastReceivedAnchor);
        }
    }
#endregion Importing Anchor Methods

#region Initialization Methods
    /// <summary>
    /// On creation save this as the static instance. There should be only one manager.
    /// </summary>
    private void Awake()
    {
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
#if !UNITY_EDITOR
        foreach (Windows.Networking.HostName hostName in Windows.Networking.Connectivity.NetworkInformation.GetHostNames())
        {
            if (hostName.DisplayName.Split(".".ToCharArray()).Length == 4)
            {
                LocalAddress = hostName.DisplayName;
                Debug.LogFormat("[NetworkAnchorManager] Found local ip address. {0}", DebugInfo());
                break;
            }
        }

        if (string.IsNullOrEmpty(LocalAddress))
        {
            Debug.LogErrorFormat("[NetworkAnchorManager] Failed to find local ip address. {0}", DebugInfo());
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
        return string.Format("(netId: {0}) (isLocalPlayer: {1}) (isServer: {2}) (isClient: {3}) (hasAuthority: {4}) (local ip: {5})",
            netId,
            isLocalPlayer,
            isServer,
            isClient,
            hasAuthority,
            LocalAddress);
    }
#endregion Debug Methods
}
