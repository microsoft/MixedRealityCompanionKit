// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using System.Collections;
using UnityEngine.Networking;
using System.Collections.Generic;
using System;

public class Player : NetworkBehaviour
{
    public static Player Local;
    public bool AttachToCamera;

    public enum PlayerState : int
    {
        Uninitialized,
        Ready,
        NotReady
    }

    public enum PlayerAction : int
    {
        Add,
        Remove,
        Update
    }

    [SyncVar(hook = "OnNameChanged")]
    public string myName = "uninitialized";

    [SyncVar(hook = "OnColorChanged")]
    public Color myColor = Color.clear;

    [SyncVar(hook = "OnStateChanged")]
    public PlayerState myState = PlayerState.Uninitialized;

    public List<Renderer> ObjectsToColor;
    public TextMesh Label;

    public static Color[] colors = { Color.red, Color.blue, Color.green, Color.cyan, Color.yellow, Color.magenta };

    public override void OnStartClient()
    {
        
        var parentObject = GameObject.Find("CalibratedRoot") as GameObject;
        if (parentObject != null)
        {
            transform.SetParent(parentObject.transform, false);
        }
        else
        {
            Debug.LogWarning("could not find Calibrated Root.");

            transform.SetParent(null);
        }

        // make sure all properties are setup based on server properties
        OnNameChanged(myName);
        OnColorChanged(myColor);
        OnStateChanged(myState);
        UpdatePlayerList(PlayerAction.Add);
    }

    public override void OnStartServer()
    {
        // activated on the server.

        // make sure all properties are setup based on server properties
        // OnStartServer is called when migration happens
        OnNameChanged(myName);
        OnColorChanged(myColor);
        OnStateChanged(myState);

        UpdatePlayerList(PlayerAction.Add);
    }

    public override void OnStartLocalPlayer()
    {
        // Called when the local player object has been set up.
        Local = this;
        var name = MasterController.Instance.ServerManager.UserInfo.GivenName;
        if (name == string.Empty)
        {
            name = SystemInfo.deviceName;
        }

        // spawned on a new client send command to update my data for this client
        CmdUpdateMyInfo(
            name + (isServer ? " (server)" : ""),
            colors[UnityEngine.Random.Range(0, colors.Length - 1)],
            PlayerState.Ready);

        // hides our local player prefab
        if (isLocalPlayer)
        {
            for (var i = 0; i < transform.childCount; ++i)
            {
                transform.GetChild(i).gameObject.SetActive(false);
            }
        }

        // Spawn a separate player object that can alter network anchors
        CmdSpawnNetworkAnchorPlayer();
    }

    public override void OnStartAuthority()
    {
        // This is invoked on behaviours that have authority, based on context and NetworkIdentity.localPlayerAuthority.
        UpdatePlayerList(PlayerAction.Update);
    }

    public override void OnStopAuthority()
    {
        myState = PlayerState.NotReady;

        UpdatePlayerList(PlayerAction.Update);
    }

    public override void OnNetworkDestroy()
    {
        myState = PlayerState.Uninitialized;

        UpdatePlayerList(PlayerAction.Remove);
    }

    public void RequestAuthority(GameObject obj, Action callback)
    {
        StartCoroutine(RequestAuthorityAsync(obj, callback));
    }

    private IEnumerator RequestAuthorityAsync(GameObject obj, Action callback)
    {
        Cmd_AssignLocalAuthority(obj);
        while (!hasAuthority)
        {
            yield return null;
        }
        if (callback != null)
        {
            callback();
        }
    }

    public void RemoveAuthority(GameObject obj)
    {
        Cmd_RemoveLocalAuthority(obj);
    }

    [Command]
    void Cmd_AssignLocalAuthority(GameObject obj)
    {
        var nIns = obj.GetComponent<NetworkIdentity>().netId;
        var client = ClientScene.FindLocalObject(nIns);
        var ni = client.GetComponent<NetworkIdentity>();
        ni.AssignClientAuthority(connectionToClient);
    }

    [Command]
    void Cmd_RemoveLocalAuthority(GameObject obj)
    {
        var nIns = obj.GetComponent<NetworkIdentity>().netId;
        var client = NetworkServer.FindLocalObject(nIns);
        var ni = client.GetComponent<NetworkIdentity>();
        ni.RemoveClientAuthority(ni.clientAuthorityOwner);
    }

    [Command]
    void CmdUpdateMyInfo(string name, Color color, PlayerState state)
    {
        myColor = color;
        myName = name;
        myState = state;
        SetDirtyBit(1);

        UpdatePlayerList(PlayerAction.Update);
    }

    [Command]
    void CmdSpawnNetworkAnchorPlayer()
    {
        GetComponent<NetworkAnchorHelper>().SpawnNetworkAnchorPlayer(gameObject);
    }

    void OnColorChanged(Color newColor)
    {
        myColor = newColor;
        foreach (var renderer in ObjectsToColor)
        {
            renderer.material.color = myColor;
        }
    }

    void OnNameChanged(string newName)
    {
        myName = newName;
        name = newName;
        Label.text = myName;
    }

    void OnStateChanged(PlayerState newState)
    {
        myState = newState;
    }

    bool wasServer = false;
    void Update()
    {
        if (isLocalPlayer && isServer != wasServer)
        {
            CmdUpdateMyInfo(SystemInfo.deviceName + (isServer ? " (server)" : ""), myColor, myState);
        }
        wasServer = isServer;

        if (!isLocalPlayer)
        {
            return;
        }

        if (AttachToCamera)
        {
            if (transform.parent != null)
            {
                transform.localPosition = transform.parent.InverseTransformPoint(Camera.main.transform.position);
                transform.localRotation = Quaternion.Inverse(transform.parent.rotation) * Camera.main.transform.rotation;
            }
            else
            {
                Debug.LogWarning("transform.parent == null");
            }
        }
    }

    void UpdatePlayerList(PlayerAction action)
    {
        if (isServer)
        {
            var netMgr = NetworkManager.singleton as MyNetworkManager;
            if (netMgr == null)
            {
                return;
            }

            if (action != PlayerAction.Remove && !isActiveAndEnabled)
            {
                action = PlayerAction.Remove;
            }

            switch (action)
            {
                case PlayerAction.Add:
                    netMgr.PlayerList.AddPlayer(this);
                    break;
                case PlayerAction.Remove:
                    netMgr.PlayerList.RemovePlayer(this);
                    break;
                case PlayerAction.Update:
                    netMgr.PlayerList.UpdatePlayer(this);
                    break;
            }
        }
    }
}
