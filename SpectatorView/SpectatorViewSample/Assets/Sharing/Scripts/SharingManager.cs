// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;

namespace SimpleSharing
{
    public class SharingManager : SimpleSharing.Singleton<SharingManager>
    {
        public int Port = 11024;

        // In this simple example of sharing, traffic is routed through Unity running on the local PC.
        // We are not using UDP multicast because some network configurations actively block that.
        // For a more robust networking stack, consider using UNET's HLAPI.  Mixed Reality 250 demonstrates how to do this:
        // https://docs.microsoft.com/en-us/windows/mixed-reality/mixed-reality-250
        [Header("IP of the local PC running this instance of Unity.")]
        public string ServerIP;

        int channelId;
        int socketId;
        int connectionId;

        bool connected = false;

        // Use a single NetworkData class for the pose data on each remote machine, so we do not need to recreate a new one each frame.
        // We are recycling this on the server to receive all data, since we get data in a sequence.
        NetworkData networkPoseData = new NetworkData();

        void Start()
        {
            NetworkTransport.Init();

            ConnectionConfig config = new ConnectionConfig();
            channelId = config.AddChannel(QosType.UnreliableSequenced);

            int maxConnections = 10;
            HostTopology topology = new HostTopology(config, maxConnections);

            socketId = NetworkTransport.AddHost(topology, Port);

            connected = Connect();
        }

        private void OnDestroy()
        {
            byte error;

#if WINDOWS_UWP
            NetworkTransport.Disconnect(socketId, connectionId, out error);
#endif

#if UNITY_EDITOR
            NetworkTransport.DisconnectNetworkHost(socketId, out error);
#endif
        }

        public bool Connect()
        {
            byte error;
            connectionId = NetworkTransport.Connect(socketId, ServerIP, Port, 0, out error);

            if (error != (byte)NetworkError.Ok)
            {
                Debug.Log("Failed to connect.");
                return false;
            }

            Debug.Log("Connected to server. ConnectionId: " + connectionId);

            return true;
        }

        private void ReceiveMessage()
        {
            int recHostId;
            int recConnectionId;
            int recChannelId;
            const int bufferSize = 1024;
            byte[] recBuffer = new byte[bufferSize];
            int dataSize;
            byte error;

            NetworkEventType recNetworkEvent = NetworkTransport.Receive(
                out recHostId, out recConnectionId, out recChannelId,
                recBuffer, bufferSize, out dataSize, out error);

            switch (recNetworkEvent)
            {
                case NetworkEventType.ConnectEvent:
                    Debug.Log("Connection Event Received.");
                    break;
                case NetworkEventType.DataEvent:
                    // Find the type of message we have received, and deserialize network data to properties in teh NetworkData class.
                    NetworkData.MessageType type = networkPoseData.DeserializeData(recBuffer, dataSize);

                    if (type == NetworkData.MessageType.Pose)
                    {
                        if (RemotePlayerManager.Instance != null)
                        {
                            RemotePlayerManager.Instance.UpdateRemotePlayer(recConnectionId, networkPoseData.Position, networkPoseData.Rotation);
                        }
                    }
                    if (type == NetworkData.MessageType.AirTap)
                    {
                        if (RemotePlayerManager.Instance != null)
                        {
                            RemotePlayerManager.Instance.PerformRemoteAirTap(recConnectionId, networkPoseData.AirTapLocation, networkPoseData.AirTapDirection, networkPoseData.AirTapHitLocation);
                        }
                    }

                    break;
                case NetworkEventType.DisconnectEvent:
                    if (RemotePlayerManager.Instance != null)
                    {
                        // Remove the remote player from our game state.
                        RemotePlayerManager.Instance.RemoveRemotePlayer(recConnectionId);
                    }

#if WINDOWS_UWP
                    // Server has disconnected, queue another connection request.
                    Connect();
#endif

                    break;
            }
        }

        void Update()
        {
            if (!connected)
            {
                Connect();
            }

            ReceiveMessage();
        }

#region Send Client Data to Server
        private Vector3 GetVectorInAnchorSpace(Vector3 input)
        {
            // Assumes anchor is attached to this gameobject.
            return transform.InverseTransformPoint(input);
        }

        private Quaternion GetQuaternionInAnchorSpace(Quaternion input)
        {
            // Assumes anchor is attached to this gameobject.
            return Quaternion.Inverse(transform.rotation) * input;
        }

        public void SendPose()
        {
            networkPoseData.SerializePose(
                GetVectorInAnchorSpace(Camera.main.transform.position),
                GetQuaternionInAnchorSpace(Camera.main.transform.rotation));

            SendMessage(networkPoseData);
        }

        public void SendAirTap(Vector3 position, Vector3 rotation, Vector3 hitLocation)
        {
            // Use a different NetworkData instance so we do not overwrite our pose data.
            NetworkData networkData = new NetworkData();
            networkData.SerializeAirTap(
                GetVectorInAnchorSpace(position),
                GetVectorInAnchorSpace(rotation),
                GetVectorInAnchorSpace(hitLocation));

            SendMessage(networkData);
        }

        private void SendMessage(NetworkData networkData)
        {
            byte error;
            NetworkTransport.Send(socketId, connectionId, channelId, networkData.buffer, NetworkData.bufSize, out error);

            if (error != (byte)NetworkError.Ok)
            {
                Debug.Log("Error sending network message.");
            }
        }
#endregion
    }
}