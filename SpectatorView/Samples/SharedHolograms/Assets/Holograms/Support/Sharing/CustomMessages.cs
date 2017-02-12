using HoloToolkit.Sharing;
using HoloToolkit.Unity;
using System.Collections.Generic;
using UnityEngine;

public class CustomMessages : SpectatorView.SV_Singleton<CustomMessages>
{
    /// <summary>
    /// Message enum containing our information bytes to share.
    /// The first message type has to start with UserMessageIDStart
    /// so as not to conflict with HoloToolkit internal messages.
    /// </summary>
    public enum TestMessageID : byte
    {
        HeadTransform = MessageID.UserMessageIDStart,
        UserAvatar,
        UserHit,
        ShootProjectile,
        StageTransform,
        ResetStage,
        ExplodeTarget,
        Max
    }

    public enum UserMessageChannels
    {
        Anchors = MessageChannel.UserMessageChannelStart,
    }

    [HideInInspector]
    public bool Initialized = false;

    /// <summary>
    /// Cache the local user's ID to use when sending messages
    /// </summary>
    public long localUserID
    {
        get; set;
    }

    public delegate void MessageCallback(NetworkInMessage msg);
    private Dictionary<TestMessageID, MessageCallback> _MessageHandlers = new Dictionary<TestMessageID, MessageCallback>();
    public Dictionary<TestMessageID, MessageCallback> MessageHandlers
    {
        get
        {
            return _MessageHandlers;
        }
    }

    /// <summary>
    /// Helper object that we use to route incoming message callbacks to the member
    /// functions of this class
    /// </summary>
    NetworkConnectionAdapter connectionAdapter;

    /// <summary>
    /// Cache the connection object for the sharing service
    /// </summary>
    NetworkConnection serverConnection;

    SharingStage sharingStage;

    void Start()
    {
        InitializeMessageHandlers();
    }

    void OnDisable()
    {
        Initialized = false;
    }

    void InitializeMessageHandlers()
    {
        sharingStage = SharingStage.Instance;
        if (sharingStage == null || sharingStage.Manager == null)
        {
            return;
        }

        serverConnection = sharingStage.Manager.GetServerConnection();
        connectionAdapter = new NetworkConnectionAdapter();

        connectionAdapter.MessageReceivedCallback += OnMessageReceived;

        // Cache the local user ID
        this.localUserID = SharingStage.Instance.Manager.GetLocalUser().GetID();

        for (byte index = (byte)TestMessageID.HeadTransform; index < (byte)TestMessageID.Max; index++)
        {
            if (MessageHandlers.ContainsKey((TestMessageID)index) == false)
            {
                MessageHandlers.Add((TestMessageID)index, null);
            }

            serverConnection.AddListener(index, connectionAdapter);
        }

        Initialized = true;
    }

    void Update()
    {
        if (sharingStage == null)
        {
            InitializeMessageHandlers();
        }
    }

    private NetworkOutMessage CreateMessage(byte MessageType)
    {
        NetworkOutMessage msg = serverConnection.CreateMessage(MessageType);
        msg.Write(MessageType);
        // Add the local userID so that the remote clients know whose message they are receiving
        msg.Write(localUserID);
        return msg;
    }

    public void SendHeadTransform(Vector3 position, Quaternion rotation, byte HasAnchor)
    {
        // If we are connected to a session, broadcast our head info
        if (this.serverConnection != null && this.serverConnection.IsConnected())
        {
            // Create an outgoing network message to contain all the info we want to send
            NetworkOutMessage msg = CreateMessage((byte)TestMessageID.HeadTransform);

            AppendTransform(msg, position, rotation);

            msg.Write(HasAnchor);

            // Send the message as a broadcast, which will cause the server to forward it to all other users in the session.
            this.serverConnection.Broadcast(
                msg,
                MessagePriority.Immediate,
                MessageReliability.UnreliableSequenced,
                MessageChannel.Avatar);
        }
    }

    public void SendShootProjectile(Vector3 position, Vector3 direction)
    {
        // If we are connected to a session, broadcast our head info
        if (this.serverConnection != null && this.serverConnection.IsConnected())
        {
            // Create an outgoing network message to contain all the info we want to send
            NetworkOutMessage msg = CreateMessage((byte)TestMessageID.ShootProjectile);

            AppendVector3(msg, position + (direction * 0.016f));
            AppendVector3(msg, direction);

            // Send the message as a broadcast, which will cause the server to forward it to all other users in the session.
            this.serverConnection.Broadcast(
                msg,
                MessagePriority.Immediate,
                MessageReliability.Reliable,
                MessageChannel.Avatar);
        }
    }

    public void SendUserAvatar(int UserAvatarID)
    {
        // If we are connected to a session, broadcast our head info
        if (this.serverConnection != null && this.serverConnection.IsConnected())
        {
            // Create an outgoing network message to contain all the info we want to send
            NetworkOutMessage msg = CreateMessage((byte)TestMessageID.UserAvatar);

            msg.Write(UserAvatarID);

            // Send the message as a broadcast, which will cause the server to forward it to all other users in the session.
            this.serverConnection.Broadcast(
                msg,
                MessagePriority.Medium,
                MessageReliability.Reliable,
                MessageChannel.Avatar);
        }
    }

    public void SendUserHit(long HitUserID)
    {
        // If we are connected to a session, broadcast our head info
        if (this.serverConnection != null && this.serverConnection.IsConnected())
        {
            // Create an outgoing network message to contain all the info we want to send
            NetworkOutMessage msg = CreateMessage((byte)TestMessageID.UserHit);

            msg.Write(HitUserID);

            // Send the message as a broadcast, which will cause the server to forward it to all other users in the session.
            this.serverConnection.Broadcast(
                msg,
                MessagePriority.Medium,
                MessageReliability.ReliableOrdered,
                MessageChannel.Avatar);
        }
    }

    public void SendStageTransform(Vector3 position, Quaternion rotation)
    {
        // If we are connected to a session, broadcast our head info
        if (this.serverConnection != null && this.serverConnection.IsConnected())
        {
            // Create an outgoing network message to contain all the info we want to send
            NetworkOutMessage msg = CreateMessage((byte)TestMessageID.StageTransform);

            AppendTransform(msg, position, rotation);

            // Send the message as a broadcast, which will cause the server to forward it to all other users in the session.
            this.serverConnection.Broadcast(
                msg,
                MessagePriority.Immediate,
                MessageReliability.ReliableOrdered,
                MessageChannel.Avatar);
        }
    }

    public void SendResetStage()
    {
        // If we are connected to a session, broadcast our head info
        if (this.serverConnection != null && this.serverConnection.IsConnected())
        {
            // Create an outgoing network message to contain all the info we want to send
            NetworkOutMessage msg = CreateMessage((byte)TestMessageID.ResetStage);

            // Send the message as a broadcast, which will cause the server to forward it to all other users in the session.
            this.serverConnection.Broadcast(
                msg,
                MessagePriority.Immediate,
                MessageReliability.ReliableOrdered,
                MessageChannel.Avatar);
        }
    }

    public void SendExplodeTarget()
    {
        // If we are connected to a session, broadcast that the target exploded.
        if (this.serverConnection != null && this.serverConnection.IsConnected())
        {
            // Create an outgoing network message to contain all the info we want to send.
            NetworkOutMessage msg = CreateMessage((byte)TestMessageID.ExplodeTarget);

            // Send the message as a broadcast, which will cause the server to forward it to all other users in the session.
            this.serverConnection.Broadcast(
                msg,
                MessagePriority.Immediate,
                MessageReliability.ReliableOrdered,
                MessageChannel.Avatar);
        }
    }

    protected override void OnDestroy()
    {
        if (this.serverConnection != null)
        {
            for (byte index = (byte)TestMessageID.HeadTransform; index < (byte)TestMessageID.Max; index++)
            {
                this.serverConnection.RemoveListener(index, this.connectionAdapter);
            }
            this.connectionAdapter.MessageReceivedCallback -= OnMessageReceived;
        }

        base.OnDestroy();
    }

    void OnMessageReceived(NetworkConnection connection, NetworkInMessage msg)
    {
        byte messageType = msg.ReadByte();
        MessageCallback messageHandler = MessageHandlers[(TestMessageID)messageType];
        if (messageHandler != null)
        {
            messageHandler(msg);
        }
    }

    #region HelperFunctionsForWriting

    void AppendTransform(NetworkOutMessage msg, Vector3 position, Quaternion rotation)
    {
        AppendVector3(msg, position);
        AppendQuaternion(msg, rotation);
    }

    void AppendVector3(NetworkOutMessage msg, Vector3 vector)
    {
        msg.Write(vector.x);
        msg.Write(vector.y);
        msg.Write(vector.z);
    }

    void AppendQuaternion(NetworkOutMessage msg, Quaternion rotation)
    {
        msg.Write(rotation.x);
        msg.Write(rotation.y);
        msg.Write(rotation.z);
        msg.Write(rotation.w);
    }

    #endregion HelperFunctionsForWriting

    #region HelperFunctionsForReading

    public Vector3 ReadVector3(NetworkInMessage msg)
    {
        return new Vector3(msg.ReadFloat(), msg.ReadFloat(), msg.ReadFloat());
    }

    public Quaternion ReadQuaternion(NetworkInMessage msg)
    {
        return new Quaternion(msg.ReadFloat(), msg.ReadFloat(), msg.ReadFloat(), msg.ReadFloat());
    }

    #endregion HelperFunctionsForReading
}