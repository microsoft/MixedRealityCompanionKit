// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections;
using UnityEngine;
using UnityEngine.Networking;

public class MyNetworkDiscovery : NetworkDiscovery
{
    public delegate void ServerDiscoveredDelegate(string fromAddress, string data);

    public event ServerDiscoveredDelegate OnServerDiscovered;
    public event EventHandler OnDiscoveryTimeout;

    public float DiscoveryTimeout = 5.0f;

    private bool foundServer = false;

    private IEnumerator timeoutEnum = null;

    public void StartBroadcastDiscovery()
    {
        if (this.running)
        {
            StopBroadcast();
        }

        this.foundServer = this.isClient = this.isServer = false;

        if (this.Initialize())
        {
            this.StartAsClient();

            if (timeoutEnum != null)
            {
                StopCoroutine(timeoutEnum);

                timeoutEnum = null;
            }

            timeoutEnum = Timeout(DiscoveryTimeout);
            StartCoroutine(timeoutEnum);
        }
    }

    public void StartBroadcastServer()
    {
        if (this.running)
        {
            StopBroadcast();
        }

        this.foundServer = this.isClient = this.isServer = false;

        if (this.Initialize())
        {
            if (this.StartAsServer())
            {
                Debug.Log("Broadcasting as server: " + this.hostId);
            }
            else
            {
                Debug.LogError("Not able to start broadcast server.");
            };
        }
    }

    IEnumerator Timeout(float delay)
    {
        yield return new WaitForSeconds(delay);

        if (!foundServer)
        {
            if (this.running)
            {
                StopBroadcast();
            }

            OnDiscoveryTimeout(this, null);
        }
    }

    public override void OnReceivedBroadcast(string fromAddress, string data)
    {
        Debug.Log("Broadcast mesasge received");

        string broadcastServer = fromAddress.Replace("::ffff:", "");
        //bool acceptBroadcast = data.Trim().Equals(this.broadcastData.Trim());

        if (!foundServer)//&& acceptBroadcast)
        {
            foundServer = true;

            if (this.running)
            {
                StopBroadcast();
            }

            if (timeoutEnum != null)
            {
                StopCoroutine(timeoutEnum);
            }

            // prevents message about hostId != -1
            StartCoroutine("DelayStartConnection", broadcastServer);
        }
    }

    IEnumerator DelayStartConnection(string fromAddress)
    {
        yield return new WaitForEndOfFrame();

        OnServerDiscovered(fromAddress, this.broadcastData);
    }
}
