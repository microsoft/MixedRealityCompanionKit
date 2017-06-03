// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using UnityEngine.Networking;
using System.Collections.Generic;
using System;
#if WINDOWS_UWP
using Windows.Networking.Connectivity;
using Windows.Networking;
#endif

namespace SpectatorView
{
    /// <summary>
    /// Inherits from UNet's NetworkDiscovery script. 
    /// Adds automatic anchor management on discovery.
    /// If the script detects that it should be the server then
    /// the script starts the anchor creation and export process.
    /// If the script detects that it should be a client then the 
    /// script kicks off the anchor ingestion process.
    /// </summary>
    public class NetworkDiscoveryWithAnchors : NetworkDiscovery
    {
        private static NetworkDiscoveryWithAnchors _Instance;
        public static NetworkDiscoveryWithAnchors Instance
        {
            get
            {
                NetworkDiscoveryWithAnchors[] objects = FindObjectsOfType<NetworkDiscoveryWithAnchors>();
                if (objects.Length != 1)
                {
                    Debug.LogFormat("Expected exactly 1 {0} but found {1}", typeof(NetworkDiscoveryWithAnchors).ToString(), objects.Length);
                }
                else
                {
                    _Instance = objects[0];
                }
                return _Instance;
            }
        }

        public class SessionInfo
        {
            public string SessionName;
            public string SessionIp;
        }

        public bool Connected
        {
            get
            {
                // we are connected if we are the server or if we aren't running discovery
                return (isServer || !running);
            }
        }


        /// <summary>
        /// Event raised when the list of sessions changes.
        /// </summary>
        public event EventHandler<EventArgs> SessionListChanged;

        public Dictionary<string, SessionInfo> remoteSessions = new Dictionary<string, SessionInfo>();

        public event EventHandler<EventArgs> ConnectionStatusChanged;

        /// <summary>
        /// Controls how often a broadcast should be sent to clients
        /// looking to join our session.
        /// </summary>
        public int BroadcastInterval = 1000;

        /// <summary>
        /// Keeps track of the IP address of the system that sent the 
        /// broadcast.  We will use this IP address to connect and 
        /// download anchor data.
        /// </summary>
        public string ServerIp { get; private set; }

        public string LocalIp;
        /// <summary>
        /// Sanity checks that our scene has everything we need to proceed.
        /// </summary>
        /// <returns>true if we have what we need, false otherwise.</returns>
        private bool CheckComponents()
        {
#if !UNITY_EDITOR
            if (GenericNetworkTransmitter.Instance == null)
            {
                Debug.Log("Need a UNetNetworkTransmitter in the scene for sending anchor data");
                return false;
            }
#endif
            if (NetworkManager.singleton == null)
            {
                Debug.Log("Need a NetworkManager in the scene");
                return false;
            }

            return true;
        }

        private void Awake()
        {
#if WINDOWS_UWP
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
            LocalIp = "editor";
#endif
        }

        private void Start()
        {
            Restart();
        }

        public void Restart()
        {
            // Initializes NetworkDiscovery.
            Initialize();

            if (!CheckComponents())
            {
                Debug.Log("Invalid configuration detected. Network Discovery disabled.");
                Destroy(this);
                return;
            }

            broadcastInterval = BroadcastInterval;
            string computerName = GetLocalComputerName();
            broadcastData = computerName.Length.ToString() + "_" + GetLocalComputerName();
            
            Debug.Log(broadcastData);
            // Start listening for broadcasts.
            StartAsClient();
        }

        private string GetLocalComputerName()
        {
#if WINDOWS_UWP
            foreach (Windows.Networking.HostName hostName in Windows.Networking.Connectivity.NetworkInformation.GetHostNames())
            {
                if (hostName.Type == HostNameType.DomainName)
                {

                    Debug.Log("My name is " + hostName.DisplayName);
                    return hostName.DisplayName;
                }
            }
            return "default_name";
#else
            return "editor";
#endif
        }

        /// <summary>
        /// Called by UnityEngine when a broadcast is received. 
        /// </summary>
        /// <param name="fromAddress">When the broadcast came from</param>
        /// <param name="data">The data in the broad cast. Not currently used, but could
        /// be used for differntiating rooms or similar.</param>
        public override void OnReceivedBroadcast(string fromAddress, string data)
        {
            ServerIp = fromAddress.Substring(fromAddress.LastIndexOf(':') + 1);
            SessionInfo sessionInfo;
            if (remoteSessions.TryGetValue(ServerIp, out sessionInfo) == false)
            {
                Debug.Log("new session: " + fromAddress);
                Debug.Log(data);

                string name = GetNameFromBroadcastData(data);

                remoteSessions.Add(ServerIp, new SessionInfo() { SessionIp = ServerIp, SessionName = name });
                SignalSessionListEvent();
            }
        }

        public string GetNameFromBroadcastData(string data)
        {
            string name = data;
            if (data.Contains("_"))
            {
                string[] nameData = data.Split('_');
                int len = Convert.ToInt32(nameData[0]);
                name = nameData[1].Substring(0, len);
            }

            return name;
        }

        /// <summary>
        /// Call to stop listening for sessions.
        /// </summary>
        public void StopListening()
        {
            StopBroadcast();
            remoteSessions.Clear();
        }

        /// <summary>
        /// Call to start listening for sessions.
        /// </summary>
        public void StartListening()
        {
            StopListening();
            StartAsClient();
        }

        public void JoinSession(SessionInfo session)
        {
            StopListening();
            // We have to parse the server IP to make the string friendly to the windows APIs.
            ServerIp = session.SessionIp;
            NetworkManager.singleton.networkAddress = ServerIp;

            // And join the networked experience as a client.
            NetworkManager.singleton.StartClient();
            SignalConnectionStatusEvent();
        }

        public void StartHosting(string SessionName)
        {
            StopListening();
            string LocalName = SessionName;

#if WINDOWS_UWP
            
            foreach (Windows.Networking.HostName hostName in Windows.Networking.Connectivity.NetworkInformation.GetHostNames())
            {
            if (string.IsNullOrEmpty(LocalName))
            {
            LocalName = hostName.DisplayName;
            }
                if (hostName.DisplayName.Split(".".ToCharArray()).Length == 4)
                {
                    Debug.Log(hostName.DisplayName);
                    NetworkManager.singleton.serverBindToIP = true;
                    NetworkManager.singleton.serverBindAddress = hostName.DisplayName;
                }
               // NetworkManager.singleton.serverBindToIP = hostName.DisplayName;
            }
#endif
            // broadcastData = LocalName;
            // Starting as a 'host' makes us both a client and a server.
            // There are nuances to this in UNet's sync system, so do make sure
            // to test behavior of your networked objects on both a host and a client 
            // device.
            NetworkManager.singleton.StartHost();
            // Start broadcasting for other clients.
            StartAsServer();

            SignalSessionListEvent();
            SignalConnectionStatusEvent();
        }

        void SignalSessionListEvent()
        {
            EventHandler<EventArgs> sessionListChanged = SessionListChanged;
            if (sessionListChanged != null)
            {
                sessionListChanged(this, EventArgs.Empty);
            }
        }

        void SignalConnectionStatusEvent()
        {
            EventHandler<EventArgs> connectionEvent = this.ConnectionStatusChanged;
            if (connectionEvent != null)
            {
                connectionEvent(this, EventArgs.Empty);
            }
        }
    }
}