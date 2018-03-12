// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using UnityEngine.Networking;
using UnityEngine.XR.WSA;
using UnityEngine.XR.WSA.Sharing;
using System;
using System.Collections.Generic;

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
    /// This event is raised When a new anchor arrives from a different player.
    /// </summary>
    /// <param name="data">The data that arrived.</param>
    public delegate void OnReceivedAnchor(MyNetworkAnchorClient sender, ReceivedAnchorArgs args);
    public event OnReceivedAnchor ReceivedAnchor;

    /// <summary>
    /// Get the last received remote anchor
    /// </summary>
    public ReceivedAnchorArgs LastRecievedAnchor
    {
        get;
        private set;
    }

    /// <summary>
    /// Get the local IP address. If this is not local instance, this will be empty
    /// </summary>
    public String LocalAddress
    {
        get;
        private set;
    }

    /// <summary>
    /// This will send or receive the binary anchor data on the local instance
    /// </summary>
    private GenericNetworkTransmitter anchorTransmitter;

    /// <summary>
    /// For this to function, the network manager must spawn a game object with an anchor service behavior.
    /// The anchor server will manage sync vars related to sharing anchors.
    /// </summary>
    private MyNetworkAnchorServer networkAnchorServer;

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

    public override void OnStartLocalPlayer()
    {
        base.OnStartLocalPlayer();
        Debug.LogFormat("[MyNetworkAnchorClient] OnStartLocalPlayer. {0}", DebugInfo());
    }

    public override void OnStartClient()
    {
        base.OnStartClient();
        Debug.LogFormat("[MyNetworkAnchorClient] OnStartClient. {0}", DebugInfo());
    }

    public override void OnStopAuthority()
    {
        base.OnStopAuthority();
        Debug.LogFormat("[MyNetworkAnchorClient] OnStopAuthority. {0}", DebugInfo());
    }

    public override void OnStartServer()
    {
        base.OnStartServer();
        Debug.LogFormat("[MyNetworkAnchorClient] OnStartServer. {0}", DebugInfo());
    }

    public override void OnStartAuthority()
    {
        base.OnStartAuthority();
        Debug.LogFormat("[MyNetworkAnchorClient] OnStartAuthority. {0}", DebugInfo());

        if (hasAuthority)
        {
            Debug.LogFormat("[MyNetworkAnchorClient] OnStartAuthority. Setting instance. {0}", DebugInfo());
            localInstance = this;
            this.InitializeLocalAddress();
            this.InitializeAnchorTransmitterOnce();
            this.InitializeAnchorHostOnce();
        }
    }
    /// <summary>
    /// If anchor is unowned, export the anchor data stored in game object, take anchor ownership of the shared anchor,
    /// and share anchor with other players.
    /// </summary>
    public void ShareAnchorDefault(String anchorId, GameObject gameObject)
    {
        if (networkAnchorServer == null || !networkAnchorServer.AnchorSource.IsValid)
        {
            ShareAnchor(anchorId, gameObject);
        }
    }

    /// <summary>
    /// Export the anchor data stored in game object, take anchor ownership of the shared anchor, and share anchor with 
    /// other players.
    /// </summary>
    public void ShareAnchor(String anchorId, GameObject gameObject)
    {
        if (networkAnchorServer != null && networkAnchorServer.AnchorSource.AnchorId == anchorId)
        {
            Debug.LogFormat("[MyNetworkAnchorClient] Ignoreing share anchor request, as anchor is already being shared. (anchor id: {0})", anchorId);
            return;
        }

        WorldAnchor worldAnchor = gameObject.GetComponent<WorldAnchor>();
        if (worldAnchor == null)
        {
            Debug.LogErrorFormat("[MyNetworkAnchorClient] Unable to acquire anchor ownership. Game object is missing an anchor. (anchor id: {0})", anchorId);
            return;
        }

        Debug.LogFormat("[MyNetworkAnchorClient] Attempting to acquire anchor ownership and share anchor with other players. (anchor id: {0})", anchorId);

        // The last received anchor will no longer be relevant since we're taking ownership
        LastRecievedAnchor = null;

        // Start taking ownership of the anchor
        List<byte> buffer = new List<byte>();
        WorldAnchorTransferBatch batch = new WorldAnchorTransferBatch();
        batch.AddWorldAnchor(anchorId, worldAnchor);
        WorldAnchorTransferBatch.ExportAsync(
            batch,
            (byte[] data) => { buffer.AddRange(data); },
            (SerializationCompletionReason status) => { ExportAnchorDataComplete(status, buffer.ToArray(), anchorId); });
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

    /// <summary>
    /// Initialize the anchor host and start listening to new anchor owners.
    /// </summary>
    private void InitializeAnchorHostOnce()
    {
        if (networkAnchorServer == null)
        {
            networkAnchorServer = MyNetworkAnchorServer.Instance;
            if (networkAnchorServer != null)
            {
                networkAnchorServer.AnchorSourceChanged += AnchorSourceChanged;
                AnchorSourceChanged(networkAnchorServer, networkAnchorServer.AnchorSource);
            }
            else
            {
                Debug.LogErrorFormat("[MyNetworkAnchorClient] For the anchor player to function correctly it needs a networkAnchorServer. {0}", DebugInfo());
            }
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
            Debug.LogFormat("[MyNetworkAnchorClient] Exporting anchor succeeded: {0} ({1} bytes)", anchorId, data.Length);

            SharedAnchorData anchorSource;
            anchorSource.SourceIp = LocalAddress;
            anchorSource.AnchorId = anchorId;
            anchorTransmitter.SendData(data);
            CmdSetAnchorSource(anchorSource);
        }
        else
        {
            Debug.LogFormat("[MyNetworkAnchorClient] Exporting anchor failed: (anchor id: {0}) (status: {1}) ({2} bytes)", anchorId, status, data.Length);
        }
    }

    /// <summary>
    /// A network command to allow clients to change the anchor source.
    /// </summary>
    [Command]
    private void CmdSetAnchorSource(SharedAnchorData anchorSource)
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

    /// <summary>
    /// The anchor host's anchor source has changed.
    /// </summary>
    private void AnchorSourceChanged(MyNetworkAnchorServer source, SharedAnchorData args)
    {
        if (!args.IsValid)
        {
            Debug.Log("[MyNetworkAnchorClient] Ignoring anchor source since it's invalid.");
            return;
        }

        if (args.SourceIp == LocalAddress)
        {
            Debug.Log("[MyNetworkAnchorClient] Ignoring anchor source since it originated from this player.");
            return;
        }

        ImportAnchorData(args);
    }

    /// <summary>
    /// Begin import anchor data from source.
    /// </summary>
    private void ImportAnchorData(SharedAnchorData anchorSource)
    {
        Debug.LogFormat("[MyNetworkAnchorClient] Importing anchor (source IP: {0}) (local ID: {1})", anchorSource.SourceIp, LocalAddress);
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
            return;
        }

        if (args.Data == null || args.Data.Length == 0)
        {
            Debug.LogFormat("[MyNetworkAnchorClient] Binary anchor data is null or empty, ignoring request to import anchor data. {0}", DebugInfo());
            return;
        }

        Debug.LogFormat("[MyNetworkAnchorClient] Starting import of binary anchor data. ({0} bytes) {1}", args.Data.Length, DebugInfo());
        WorldAnchorTransferBatch.ImportAsync(args.Data, ImportAnchorDataComplete);
    }

    /// <summary>
    /// Called when a remote anchor has been de-serialized
    /// </summary>
    /// <param name="status">Tracks if the import worked</param>
    /// <param name="batch">The WorldAnchorTransferBatch that has the anchor information.</param>
    private void ImportAnchorDataComplete(
        SerializationCompletionReason status,
        WorldAnchorTransferBatch batch)
    {
        if (status != SerializationCompletionReason.Succeeded)
        {
            // if we failed, we can simply try again. (TODO)
            Debug.Log("[MyNetworkAnchor] Anchor import has failed");
        }
        else
        {
            Debug.Log("[MyNetworkAnchor] Anchor import has complete, and raising RevievedAnchor event");
            LastRecievedAnchor = new ReceivedAnchorArgs(batch);
            if (ReceivedAnchor != null)
            {
                ReceivedAnchor(this, LastRecievedAnchor);
            }
        }
    }
}
