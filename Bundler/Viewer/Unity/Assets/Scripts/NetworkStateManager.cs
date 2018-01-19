// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using UnityEngine;
using UnityEngine.Networking;
using UnityEngine.Networking.NetworkSystem;

[NetworkSettings(sendInterval = .05f, channel = 0)]
public class NetworkStateManager : NetworkBehaviour
{
    [Flags]
    public enum NetworkStateChanges
    {
        None = 0,
        Scale = 1,
        Offset = 1 << 1,
        Rotation = 1 << 2,
        Explode = 1 << 3,
        Asset = 1 << 4,

        All = ~None
    }

    public class NetworkStateMessage : MessageBase
    {
        public NetworkState State;
        public NetworkStateChanges StatesChanged;

        public NetworkStateMessage(NetworkState state, NetworkStateChanges changes = NetworkStateChanges.All)
        {
            State = state;
            StatesChanged = changes;
        }
        public NetworkStateMessage()
        {
        }

        public override string ToString()
        {
            string ret = "Changes: " + StatesChanged + "\n";
            if (StatesChanged.HasFlag(NetworkStateChanges.Scale))
            {
                ret += "StageScale: " + State.StageScale.ToString("0.00") + "\n";
            }
            if (StatesChanged.HasFlag(NetworkStateChanges.Offset))
            {
                ret += "StageOffset: " + State.StageOffset.ToString("0.00") + "\n";
            }
            if (StatesChanged.HasFlag(NetworkStateChanges.Rotation))
            {
                ret += "StageRotation: " + State.StageRotation.ToString("0.00") + "\n";
            }
            if (StatesChanged.HasFlag(NetworkStateChanges.Explode))
            {
                ret += "Explode: " + State.Explode + "\n";
            }

            if (StatesChanged.HasFlag(NetworkStateChanges.Asset))
            {
                ret += "AssetInfoID: " + State.AssetInfo.Id.ToString() + "\n";
                ret += "AssetInfoOwnerId: " + State.AssetInfo.OwnerId.ToString() + "\n";
            }
            return ret;
        }

        public override void Serialize(NetworkWriter writer)
        {
            writer.Write((Int16)StatesChanged);

            writer.Write(State.MessageId);
            if (StatesChanged.HasFlag(NetworkStateChanges.Scale))
            {
                writer.Write(State.StageScale);
            }
            if (StatesChanged.HasFlag(NetworkStateChanges.Offset))
            {
                writer.Write(State.StageOffset);
            }
            if (StatesChanged.HasFlag(NetworkStateChanges.Explode))
            {
                writer.Write(State.Explode);
            }
            if (StatesChanged.HasFlag(NetworkStateChanges.Rotation))
            {
                writer.Write(State.StageRotation);
            }
            if (StatesChanged.HasFlag(NetworkStateChanges.Asset))
            {
                State.AssetInfo.Serialize(writer);
            }
        }

        public override void Deserialize(NetworkReader reader)
        {
            StatesChanged = (NetworkStateChanges)reader.ReadInt16();

            State = new NetworkState();
            State.MessageId = reader.ReadInt32();
            if (StatesChanged.HasFlag(NetworkStateChanges.Scale))
            {
                State.StageScale = reader.ReadSingle();
            }
            if (StatesChanged.HasFlag(NetworkStateChanges.Offset))
            {
                State.StageOffset = reader.ReadSingle();
            }
            if (StatesChanged.HasFlag(NetworkStateChanges.Explode))
            {
                State.Explode = reader.ReadBoolean();
            }
            if (StatesChanged.HasFlag(NetworkStateChanges.Rotation))
            {
                State.StageRotation = reader.ReadSingle();
            }
            if (StatesChanged.HasFlag(NetworkStateChanges.Asset))
            {
                State.AssetInfo = AssetInfo.Deserialize(reader);
            }
        }
    }

    public struct NetworkState
    {
        public int MessageId;
        public float StageScale;
        public float StageOffset;
        public float StageRotation;
        public bool Explode;
        public AssetInfo AssetInfo;

        public void Merge(NetworkState other, NetworkStateChanges changes)
        {
            MessageId = other.MessageId;
            if (changes.HasFlag(NetworkStateChanges.Scale))
            {
                StageScale = other.StageScale;
            }
            if (changes.HasFlag(NetworkStateChanges.Offset))
            {
                StageOffset = other.StageOffset;
            }
            if (changes.HasFlag(NetworkStateChanges.Rotation))
            {
                StageRotation = other.StageRotation;
            }
            if (changes.HasFlag(NetworkStateChanges.Explode))
            {
                Explode = other.Explode;
            }
            if (changes.HasFlag(NetworkStateChanges.Asset))
            {
                AssetInfo = other.AssetInfo;
            }
        }
    }

    public NetworkState CurrentState
    {
        get { return localState; }
    }

    private NetworkState localState = new NetworkState()
    {
        MessageId = 0,
        StageScale = .66f,
        StageOffset = 0,
        StageRotation = 0,
        AssetInfo = new AssetInfo(),
        Explode = false
    };

    private NetworkState serverState = new NetworkState()
    {
        MessageId = 0,
        StageScale = .66f,
        StageOffset = 0,
        StageRotation = 0,
        AssetInfo = new AssetInfo(),
        Explode = false
    };

    private bool bypassNetwork;
    public bool BypassNetwork
    {
        get { return bypassNetwork; }
        set
        {
            bypassNetwork = value;
            if (isClient && !bypassNetwork)
            {
                NetworkManager.singleton.client.Send(JoinedMsg, new EmptyMessage());
            }
        }
    }

    const short JoinedMsg = 1001;
    const short SetStateRequestMsg = 1002;
    const short SetStateResponseMsg = 1003;
    const short HeartbeatRequestMsg = 1004;
    const short HeartbeatResponseMsg = 1005;

    private int myId;

    float lastHeartBeatSend = 0;
    float lastHeartBeatReceive = 0;
    float lastTick = 0;

    private void Start()
    {
        myId = (int)UnityEngine.Random.Range(0, int.MaxValue);
    }

    public override void OnStartClient()
    {
        base.OnStartClient();
        if (NetworkManager.singleton.client != null)
        {
            NetworkManager.singleton.client.RegisterHandler(SetStateResponseMsg, OnSetStateResponseMessage);
            NetworkManager.singleton.client.RegisterHandler(HeartbeatResponseMsg, OnHeartbeatResponseMessage);
            NetworkManager.singleton.client.Send(JoinedMsg, new EmptyMessage());
        }
        else
        {
            throw new Exception("Network client not created");
        }
    }

    public override void OnStartServer()
    {
        base.OnStartServer();
        NetworkServer.RegisterHandler(SetStateRequestMsg, OnSetStateRequestMessage);
        NetworkServer.RegisterHandler(SetStateResponseMsg, OnSetStateResponseMessage);
        NetworkServer.RegisterHandler(HeartbeatRequestMsg, OnHeartbeatRequestMessage);
        NetworkServer.RegisterHandler(JoinedMsg, OnJoinedMessage);
    }

    void OnJoinedMessage(NetworkMessage netMsg)
    {
        Debug.Log("received OnJoinedMessage");
        NetworkServer.SendToClient(netMsg.conn.connectionId, SetStateResponseMsg, new NetworkStateMessage(serverState, NetworkStateChanges.All));
    }

    void OnSetStateRequestMessage(NetworkMessage netMsg)
    {
        var message = netMsg.ReadMessage<NetworkStateMessage>();

        NetworkServer.SendToAll(SetStateResponseMsg, message);
        serverState.Merge(message.State, message.StatesChanged);
        if (!BypassNetwork)
        {
            OnStateChange(message.State, true, message.StatesChanged);
        }
    }

    private void Update()
    {
        if (Time.realtimeSinceStartup - lastTick > 5)
        {
            lastTick = Time.realtimeSinceStartup;
            if (!isServer && Time.realtimeSinceStartup - lastHeartBeatReceive > 30)
            {
                var lastTime = Time.realtimeSinceStartup - lastHeartBeatReceive;
                Debug.LogWarning("No heartbeat response in: " + lastTime + " sec");
            }

            if (!isServer && Time.realtimeSinceStartup - lastHeartBeatSend > 10)
            {
                lastHeartBeatSend = Time.realtimeSinceStartup;
                SendHeartbeatToServer();
            }
        }
    }

    void SendHeartbeatToServer()
    {
        Debug.Log("SendHeartbeatToServer: " + DateTime.Now.ToString());
        if (NetworkManager.singleton.client != null)
        {
            NetworkManager.singleton.client.Send(HeartbeatRequestMsg, new StringMessage(myId + " Heartbeat (" + DateTime.Now.ToString() + ")"));
        }
    }

    void OnHeartbeatRequestMessage(NetworkMessage netMsg)
    {
        var message = netMsg.ReadMessage<StringMessage>();
        Debug.Log(DateTime.Now.ToString() + " - Heartbeat Request: " + message.value);
        NetworkServer.SendToClient(netMsg.conn.connectionId, HeartbeatResponseMsg, new StringMessage(" Heartbeat (" + DateTime.Now.ToString() + ")"));
    }

    void OnHeartbeatResponseMessage(NetworkMessage netMsg)
    {
        lastHeartBeatReceive = Time.realtimeSinceStartup;
        var message = netMsg.ReadMessage<StringMessage>();
        var roundTripTime = lastHeartBeatReceive - lastHeartBeatSend;
        Debug.Log(DateTime.Now.ToString() + " - Heartbeat Response (" + (roundTripTime * 1000).ToString("0.0") + "ms): " + message.value);
    }

    void OnSetStateResponseMessage(NetworkMessage netMsg)
    {
        if (!BypassNetwork)
        {
            var message = netMsg.ReadMessage<NetworkStateMessage>();
            OnStateChange(message.State, false, message.StatesChanged);
        }
    }

    private void SendStateToServer(NetworkState state, NetworkStateChanges changes = NetworkStateChanges.All)
    {
        state.MessageId = myId;

        if (BypassNetwork || NetworkManager.singleton == null || NetworkManager.singleton.client == null)
        {
            OnStateChange(state, true, changes);
        }
        else
        {
            NetworkManager.singleton.client.Send(SetStateRequestMsg, new NetworkStateMessage(state, changes));

            if (!isServer)
            {
                OnStateChange(state, true, changes);
            }
        }
    }

    public void SetScale(float newValue)
    {
        var state = localState;
        state.StageScale = newValue;
        SendStateToServer(state, NetworkStateChanges.Scale);
    }

    public void SetRotation(float newValue)
    {
        var state = localState;
        state.StageRotation = newValue;
        SendStateToServer(state, NetworkStateChanges.Rotation);
    }

    public void SetOffset(float newValue)
    {
        var state = localState;
        state.StageOffset = newValue;
        SendStateToServer(state, NetworkStateChanges.Offset);
    }

    public void SetExplode(bool newValue)
    {
        var state = localState;
        state.Explode = newValue;
        SendStateToServer(state, NetworkStateChanges.Explode);
    }

    public void LoadAsset(AssetInfo newValue)
    {
        var state = localState;
        state.Explode = false;
        state.AssetInfo = newValue;
        SendStateToServer(state, NetworkStateChanges.All);
    }

    private void OnStateChange(NetworkState newState, bool force = false, NetworkStateChanges changes = NetworkStateChanges.All)
    {
        if (myId == newState.MessageId && !force)
        {
            return;
        }

        localState.Merge(newState, changes);
        if (!isServer)  // everybody keeps a local copy of the server state in case they become the server
        {
            serverState = localState;
        }

        if (changes.HasFlag(NetworkStateChanges.Scale))
        {
            MasterController.Instance.DoSetStageScale(localState.StageScale);
        }
        if (changes.HasFlag(NetworkStateChanges.Offset))
        {
            MasterController.Instance.DoSetVerticalOffset(localState.StageOffset);
        }
        if (changes.HasFlag(NetworkStateChanges.Rotation))
        {
            MasterController.Instance.DoSetStageRotation(localState.StageRotation);
        }
        if (changes.HasFlag(NetworkStateChanges.Explode))
        {
            MasterController.Instance.DoSetExplode(localState.Explode);
        }
        if (changes.HasFlag(NetworkStateChanges.Asset))
        {
            MasterController.Instance.DoLoadBundle(localState.AssetInfo);
        }
    }
}
