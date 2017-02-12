// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using HoloToolkit.Sharing;
using HoloToolkit.Unity;
using System.Collections.Generic;
using UnityEngine;

namespace SpectatorView
{
    public class SV_CustomMessages : SpectatorView.SV_Singleton<SV_CustomMessages>
    {
        // Add an additional offset for compatibility with apps that are already using Sharing.
        // If you have > 20 custom messages you will need to reduce this offset.
        public enum TestMessageID : byte
        {
            HeadTransform = MessageID.UserMessageIDStart + 50,
            SceneTransform,
            EditorUser,
            TimeOffset,
            NetworkRoundTripTime,
            SetNetworkTime,
            SetColorDuration,
            SpatialMapping,
            SpatialMappingRequest,
            UpdateSpectatorViewIP,
            Max
        }

        public enum UserMessageChannels
        {
            Anchors = MessageChannel.UserMessageChannelStart,
        }

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

        [HideInInspector]
        public bool Initialized = false;

        void Start()
        {
            InitializeMessageHandlers();
        }

        void InitializeMessageHandlers()
        {
            sharingStage = SharingStage.Instance;

            if (sharingStage == null || sharingStage.Manager == null)
            {
                Debug.Log("Cannot Initialize CustomMessages. No SharingStage instance found.");
                return;
            }

            if (serverConnection == null)
            {
                serverConnection = sharingStage.Manager.GetServerConnection();
            }
            if (serverConnection == null)
            {
                Debug.Log("Cannot initialize CustomMessages. Cannot get a server connection.");
                return;
            }

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

        public void SendHeadTransform(Vector3 position, Quaternion rotation)
        {
            // If we are connected to a session, broadcast our head info
            if (this.serverConnection != null && this.serverConnection.IsConnected() && HolographicCameraManager.Instance != null)
            {
                // Create an outgoing network message to contain all the info we want to send
                NetworkOutMessage msg = CreateMessage((byte)TestMessageID.HeadTransform);
                uint byteSize = (uint)HolographicCameraManager.Instance.localIPBytes.Length;
                msg.Write(byteSize);
                msg.WriteArray(HolographicCameraManager.Instance.localIPBytes, byteSize);
                AppendTransform(msg, position, rotation);

                if (HolographicCameraManager.Instance == null ||
                    HolographicCameraManager.Instance.editorUser == null ||
                    !HolographicCameraManager.Instance.editorUser.IsValid())
                {
                    // Send the message as a broadcast, which will cause the server to forward it to all other users in the session.
                    this.serverConnection.Broadcast(
                    msg,
                    MessagePriority.Immediate,
                    MessageReliability.UnreliableSequenced,
                    MessageChannel.Avatar);
                }
                else
                {
                    // We have a known editor user, only send the message to the editor.
                    serverConnection.SendTo(
                        HolographicCameraManager.Instance.editorUser,
                        ClientRole.Primary,
                        msg,
                        MessagePriority.Immediate,
                        MessageReliability.UnreliableSequenced,
                        MessageChannel.Avatar);
                }
            }
        }

        public void SendUnityUserID(long localUserID)
        {
            // If we are connected to a session, broadcast our head info
            if (this.serverConnection != null && this.serverConnection.IsConnected() && HolographicCameraManager.Instance != null)
            {
                // Create an outgoing network message to contain all the info we want to send
                NetworkOutMessage msg = CreateMessage((byte)TestMessageID.EditorUser);
                uint byteSize = (uint)HolographicCameraManager.Instance.localIPBytes.Length;
                msg.Write(byteSize);
                msg.WriteArray(HolographicCameraManager.Instance.localIPBytes, byteSize);
                msg.Write(localUserID);

                // Send the message as a broadcast, which will cause the server to forward it to all other users in the session.
                this.serverConnection.Broadcast(
                    msg,
                    MessagePriority.Immediate,
                    MessageReliability.Reliable,
                    MessageChannel.Avatar);
            }
        }

        public void SendUpdatedIPs(string spectatorViewIP)
        {
            // If we are connected to a session, broadcast our head info
            if (this.serverConnection != null && this.serverConnection.IsConnected() && HolographicCameraManager.Instance != null)
            {
                // Create an outgoing network message to contain all the info we want to send
                NetworkOutMessage msg = CreateMessage((byte)TestMessageID.UpdateSpectatorViewIP);
                AppendIP(msg, spectatorViewIP);

                // Send the message as a broadcast, which will cause the server to forward it to all other users in the session.
                this.serverConnection.Broadcast(
                    msg,
                    MessagePriority.Immediate,
                    MessageReliability.ReliableSequenced,
                    MessageChannel.Avatar);
            }
        }

        public void SendTPPCColorDuration(long duration)
        {
#if UNITY_EDITOR
            if (this.serverConnection != null && this.serverConnection.IsConnected() && HolographicCameraManager.Instance != null && SharingStage.Instance.SessionUsersTracker != null)
            {
                if (SharingStage.Instance == null || SharingStage.Instance.SessionUsersTracker.CurrentUsers.Count <= 1)
                {
                    return;
                }

                if (HolographicCameraManager.Instance != null &&
                    HolographicCameraManager.Instance.tppcUser != null &&
                    HolographicCameraManager.Instance.tppcUser.IsValid())
                {
                    // Create an outgoing network message to contain all the info we want to send
                    NetworkOutMessage msg = CreateMessage((byte)TestMessageID.SetColorDuration);
                    msg.Write(duration);

                    // We have a known tppc user, only send the offset time to that device.
                    serverConnection.SendTo(
                        HolographicCameraManager.Instance.tppcUser,
                        ClientRole.Primary,
                        msg,
                        MessagePriority.Immediate,
                        MessageReliability.UnreliableSequenced,
                        MessageChannel.Avatar);
                }
            }
#endif
        }

        public void SendTPPCNetworkTime(long time)
        {
            if (this.serverConnection != null && this.serverConnection.IsConnected() && HolographicCameraManager.Instance != null)
            {
                // Create an outgoing network message to contain all the info we want to send
                NetworkOutMessage msg = CreateMessage((byte)TestMessageID.SetNetworkTime);
                msg.Write(time);

#if UNITY_EDITOR
                if (HolographicCameraManager.Instance == null ||
                    HolographicCameraManager.Instance.tppcUser == null ||
                    !HolographicCameraManager.Instance.tppcUser.IsValid())
                {
                    // Send the message as a broadcast, which will cause the server to forward it to all other users in the session.
                    serverConnection.Broadcast(
                        msg,
                        MessagePriority.Immediate,
                        MessageReliability.UnreliableSequenced,
                        MessageChannel.Avatar);
                }
                else
                {
                    // We have a known tppc user, only send the offset time to that device.
                    serverConnection.SendTo(
                        HolographicCameraManager.Instance.tppcUser,
                        ClientRole.Primary,
                        msg,
                        MessagePriority.Immediate,
                        MessageReliability.UnreliableSequenced,
                        MessageChannel.Avatar);
                }
#endif
            }
        }

        public void SendNetworkRoundTripTime(long time)
        {
            if (this.serverConnection != null && this.serverConnection.IsConnected() && HolographicCameraManager.Instance != null)
            {
#if UNITY_EDITOR
                if (HolographicCameraManager.Instance != null &&
                    HolographicCameraManager.Instance.tppcUser != null &&
                    HolographicCameraManager.Instance.tppcUser.IsValid())
                {
                    // Create an outgoing network message to contain all the info we want to send
                    NetworkOutMessage msg = CreateMessage((byte)TestMessageID.NetworkRoundTripTime);
                    msg.Write(time);

                    // We have a known tppc user, only send the offset time to that device.
                    serverConnection.SendTo(
                        HolographicCameraManager.Instance.tppcUser,
                        ClientRole.Primary,
                        msg,
                        MessagePriority.Immediate,
                        MessageReliability.UnreliableSequenced,
                        MessageChannel.Avatar);
                }
#else
                if (HolographicCameraManager.Instance != null &&
                    HolographicCameraManager.Instance.editorUser != null &&
                    HolographicCameraManager.Instance.editorUser.IsValid())
                {
                    // Create an outgoing network message to contain all the info we want to send
                    NetworkOutMessage msg = CreateMessage((byte)TestMessageID.NetworkRoundTripTime);
                    msg.Write(time);

                    // We have a known editor user, only send the message to the editor.
                    serverConnection.SendTo(
                        HolographicCameraManager.Instance.editorUser,
                        ClientRole.Primary,
                        msg,
                        MessagePriority.Immediate,
                        MessageReliability.UnreliableSequenced,
                        MessageChannel.Avatar);
                }
#endif
            }
        }

        public void SendSceneTransform(Vector3 position, Quaternion rotation, Vector3 scale)
        {
            // If we are connected to a session, broadcast the scene info
            if (serverConnection != null && serverConnection.IsConnected())
            {
                // Create an outgoing network message to contain all the info we want to send
                NetworkOutMessage msg = CreateMessage((byte)TestMessageID.SceneTransform);

                AppendSceneTransform(msg, position, rotation, scale);

                // Send the message as a broadcast, which will cause the server to forward it to all other users in the session.
                serverConnection.Broadcast(
                    msg,
                    MessagePriority.Immediate,
                    MessageReliability.Reliable,
                    MessageChannel.Avatar);
            }
        }

        public void SendSpatialMappingRequest()
        {
            // If we are connected to a session, broadcast the scene info
            if (serverConnection != null && serverConnection.IsConnected())
            {
                // Create an outgoing network message to contain all the info we want to send
                NetworkOutMessage msg = CreateMessage((byte)TestMessageID.SpatialMappingRequest);

                if (HolographicCameraManager.Instance == null ||
                    HolographicCameraManager.Instance.tppcUser == null ||
                    !HolographicCameraManager.Instance.tppcUser.IsValid())
                {
                    // Send the message as a broadcast, which will cause the server to forward it to all other users in the session.
                    serverConnection.Broadcast(
                        msg,
                        MessagePriority.Immediate,
                        MessageReliability.Reliable,
                        MessageChannel.Avatar);
                }
                else
                {
                    // We have a known tppc user, only send the offset time to that device.
                    serverConnection.SendTo(
                        HolographicCameraManager.Instance.tppcUser,
                        ClientRole.Primary,
                        msg,
                        MessagePriority.Immediate,
                        MessageReliability.Reliable,
                        MessageChannel.Avatar);
                }
            }
        }

        public void SendSpatialMapping(Vector3[] vertices, Vector3[] normals, int[] triangles)
        {
            // If we are connected to a session, broadcast the scene info
            if (serverConnection != null && serverConnection.IsConnected())
            {
                // Create an outgoing network message to contain all the info we want to send
                NetworkOutMessage msg = CreateMessage((byte)TestMessageID.SpatialMapping);

                msg.Write(vertices.Length);
                msg.Write(normals.Length);
                msg.Write(triangles.Length);

                for (int i = 0; i < vertices.Length; i++)
                {
                    //NOTE: this makes the assumption that this script is attached to the anchor.
                    AppendVector3(msg, this.transform.InverseTransformPoint(vertices[i]));
                }

                for (int i = 0; i < normals.Length; i++)
                {
                    AppendVector3(msg, normals[i]);
                }

                for (int i = 0; i < triangles.Length; i++)
                {
                    msg.Write(triangles[i]);
                }

                // Send the message as a broadcast, which will cause the server to forward it to all other users in the session.
                serverConnection.Broadcast(
                    msg,
                    MessagePriority.Immediate,
                    MessageReliability.Reliable,
                    MessageChannel.Avatar);
            }
        }

        public void SendTimeOffset(int newTime)
        {
            // If we are connected to a session, broadcast the scene info
            if (serverConnection != null && serverConnection.IsConnected())
            {
                if (HolographicCameraManager.Instance == null ||
                    HolographicCameraManager.Instance.tppcUser == null ||
                    !HolographicCameraManager.Instance.tppcUser.IsValid())
                {
                    // Create an outgoing network message to contain all the info we want to send
                    NetworkOutMessage msg = CreateMessage((byte)TestMessageID.TimeOffset);
                    AppendTimeOffset(msg, newTime);

                    // Send the message as a broadcast, which will cause the server to forward it to all other users in the session.
                    serverConnection.Broadcast(
                        msg,
                        MessagePriority.Immediate,
                        MessageReliability.UnreliableSequenced,
                        MessageChannel.Avatar);
                }
                else
                {
                    // Create an outgoing network message to contain all the info we want to send
                    NetworkOutMessage msg = CreateMessage((byte)TestMessageID.TimeOffset);
                    AppendTimeOffset(msg, newTime);

                    // We have a known tppc user, only send the offset time to that device.
                    serverConnection.SendTo(
                        HolographicCameraManager.Instance.tppcUser,
                        ClientRole.Primary,
                        msg,
                        MessagePriority.Immediate,
                        MessageReliability.UnreliableSequenced,
                        MessageChannel.Avatar);
                }
            }
        }

        protected override void OnDestroy()
        {
            Initialized = false;

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

        void AppendSceneTransform(NetworkOutMessage msg, Vector3 position, Quaternion rotation, Vector3 scale)
        {
            AppendVector3(msg, position);
            AppendQuaternion(msg, rotation);
            AppendVector3(msg, scale);
        }

        void AppendTimeOffset(NetworkOutMessage msg, int newTime)
        {
            msg.Write(newTime);
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

        // Append a network message with IP Byte Size and Byte array for IP.
        void AppendIP(NetworkOutMessage msg, string IP)
        {
            byte[] ipBytes = System.Text.Encoding.ASCII.GetBytes(IP);
            long byteSize = (long)ipBytes.Length;
            msg.Write(byteSize);
            msg.WriteArray(ipBytes, (uint)byteSize);
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

        public string ReadIP(NetworkInMessage msg)
        {
            long ipSize = msg.ReadInt64();
            byte[] ipData = new byte[(uint)ipSize];
            msg.ReadArray(ipData, (uint)ipSize);

            return System.Text.Encoding.ASCII.GetString(ipData);
        }

#endregion HelperFunctionsForReading
    }
}