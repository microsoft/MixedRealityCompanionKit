// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections;
using UnityEngine;
using UnityEngine.Networking;
using UnityEngine.Networking.NetworkSystem;
using UnityEngine.UI;

[RequireComponent(typeof(MyNetworkDiscovery))]
public class MyNetworkManager : NetworkManager
{
    public bool AutoStartDiscovery = true;
    public PlayerList PlayerList;
    public Text StatusText;
    bool checking;
    bool paused;
    bool isInDelayedStart = false;

    private MyNetworkDiscovery discovery;

    void Start()
    {
        StartCoroutine(Startup(0));
        GetComponent<NetworkAnchorHelper>().PrepareNetworkManager(this);
    }

    void OnDestroy()
    {
        Cleanup();
    }

    IEnumerator Startup(float delay)
    {
        if (!isInDelayedStart)
        {
            isInDelayedStart = true;
            yield return new WaitForSeconds(delay);
            if (isInDelayedStart)
            {
                discovery = GetComponent<MyNetworkDiscovery>();
                if (PlayerList == null)
                {
                    PlayerList = FindObjectOfType<PlayerList>();
                }

                if (AutoStartDiscovery)
                {
                    StartBroadcastDiscovery();
                }
            }
            isInDelayedStart = false;
        }
    }

    void Cleanup()
    {
        isInDelayedStart = false;
        if (discovery != null && discovery.running && discovery.isActiveAndEnabled)
        {
            discovery.StopBroadcast();
        }

        if (IsClientConnected())
        {
            StopClient();
        }

        StopHost();
    }

    void OnApplicationPause(bool isPausing)
    {
        Debug.Log("OnApplicationPause: isPausing = " + isPausing + " - paused = " + paused);
        if (isPausing && !paused)
        {
            Cleanup();
        }
        else if (!isPausing && paused)
        {
            Cleanup();
            StartCoroutine(Startup(2.0f));
        }
        paused = isPausing;
    }

    void Update()
    {
        StartCoroutine(CheckForMigration());
    }

    public void StartBroadcastDiscovery()
    {
        Debug.Log("StartBroadcastDiscovery... discovery==null:" + (discovery == null) + " - StatusText==null:" + (StatusText == null));
        if (StatusText != null) StatusText.text = "Looking for shared experience server...";

        discovery.OnDiscoveryTimeout += NetworkDiscovery_OnDiscoveryTimeout;
        discovery.OnServerDiscovered += NetworkDiscovery_OnServerDiscovered;

        discovery.StartBroadcastDiscovery();
    }

    private void NetworkDiscovery_OnDiscoveryTimeout(object sender, EventArgs e)
    {
        Debug.Log("  ...DiscoveryTimeout, start server on local machine");

        if (StatusText != null)
        {
            StatusText.text = "...Not found...Starting local server...";
        }

        discovery.OnDiscoveryTimeout -= NetworkDiscovery_OnDiscoveryTimeout;
        discovery.OnServerDiscovered -= NetworkDiscovery_OnServerDiscovered;

        networkAddress = "localhost";

        var network = StartHost();
        if (network != null)
        {
            Debug.Log("Local server has started.");

            discovery.StartBroadcastServer();
        }
        else
        {
            Debug.LogWarning("Not able to create the server, try to connect to localhost as client...");

            StartClient();
        }
    }

    private void NetworkDiscovery_OnServerDiscovered(string fromAddress, string data)
    {
        Debug.Log("NetworkDiscovery_OnServerDiscovered: " + fromAddress + " - " + data);

        if (StatusText != null)
        {
            StatusText.text = "...found server.";
        }

        discovery.OnDiscoveryTimeout -= NetworkDiscovery_OnDiscoveryTimeout;
        discovery.OnServerDiscovered -= NetworkDiscovery_OnServerDiscovered;

        networkAddress = fromAddress;

        StartClient();
    }

    public override void OnStartServer()
    {
        base.OnStartServer();

        Shared_NetHandler.Server_Register();
    }

    public override void OnStopServer()
    {
        Shared_NetHandler.Server_UnRegister();

        base.OnStopServer();
    }

    public override void OnClientConnect(NetworkConnection conn)
    {
        base.OnClientConnect(conn);

        Shared_NetHandler.Client_Register();
        Shared_NetHandler.Server_Register();
    }

    public override void OnClientDisconnect(NetworkConnection conn)
    {
        Shared_NetHandler.Client_UnRegister();
        Shared_NetHandler.Server_UnRegister();

        base.OnClientDisconnect(conn);

        StartBroadcastDiscovery();
    }

    public override void OnServerReady(NetworkConnection conn)
    {
        base.OnServerReady(conn);
        GetComponent<NetworkAnchorHelper>().SpawnNetworkAnchorManager();
    }

    public override void OnServerAddPlayer(NetworkConnection conn, short playerControllerId)
    {
        base.OnServerAddPlayer(conn, playerControllerId);

        var newPlayer = conn.playerControllers[0].gameObject;
        PlayerList.AddPlayer(newPlayer.gameObject.GetComponent<Player>());
    }

    public override void OnServerRemovePlayer(NetworkConnection conn, PlayerController player)
    {
        PlayerList.RemovePlayer(player.gameObject.GetComponent<Player>());

        base.OnServerRemovePlayer(conn, player);
    }

    public override void OnServerDisconnect(NetworkConnection conn)
    {
        foreach (var p in conn.playerControllers)
        {
            PlayerList.RemovePlayer(p.gameObject.GetComponent<Player>());
        }

        base.OnServerDisconnect(conn);
    }

    private IEnumerator CheckForMigration()
    {
        if (!checking)
        {
            checking = true;
            // add in a random delay so that all clients don't try to do the same thing immediately on disconnect
            yield return new WaitForSeconds((float)new System.Random().NextDouble() * 2.0f);
            if (migrationManager != null)
            {
                if (migrationManager.hostWasShutdown)
                {
                    if (migrationManager.waitingReconnectToNewHost)
                    {
                        migrationManager.Reset(ClientScene.ReconnectIdHost);

                        networkAddress = migrationManager.newHostAddress;

                        StartClient();
                    }
                    else
                    {
                        PeerInfoMessage m_NewHostInfo = new PeerInfoMessage();

                        bool youAreNewHost;
                        if (migrationManager.FindNewHost(out m_NewHostInfo, out youAreNewHost))
                        {
                            migrationManager.newHostAddress = m_NewHostInfo.address;
                            if (youAreNewHost)
                            {
                                // you cannot be the new host.. you were the old host..?
                                Debug.LogWarning("MigrationManager FindNewHost - new host is self?");
                            }
                            else
                            {
                                migrationManager.waitingReconnectToNewHost = true;
                            }
                        }
                    }

                }
                else if (migrationManager.disconnectedFromHost &&
                    migrationManager.oldServerConnectionId != -1)
                {
                    if (migrationManager.waitingToBecomeNewHost)
                    {
                        migrationManager.BecomeNewHost(NetworkManager.singleton.networkPort);

                        Debug.Log("Starting discovery server broadcast");
                        discovery.StartBroadcastServer();
                    }
                    else if (migrationManager.waitingReconnectToNewHost)
                    {
                        migrationManager.Reset(migrationManager.oldServerConnectionId);
                        NetworkManager.singleton.networkAddress = migrationManager.newHostAddress;
                        NetworkManager.singleton.client.ReconnectToNewHost(migrationManager.newHostAddress, NetworkManager.singleton.networkPort);
                    }
                    else
                    {
                        PeerInfoMessage m_NewHostInfo = new PeerInfoMessage();

                        bool youAreNewHost;
                        if (migrationManager.FindNewHost(out m_NewHostInfo, out youAreNewHost))
                        {
                            migrationManager.newHostAddress = m_NewHostInfo.address.Replace("::ffff:", ""); ;
                            if (youAreNewHost)
                            {
                                migrationManager.waitingToBecomeNewHost = true;
                            }
                            else
                            {
                                migrationManager.waitingReconnectToNewHost = true;
                            }
                        }
                    }
                }
            }
        }
        checking = false;
    }
}
