// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using UnityEngine.Networking;

public class PlayerList : NetworkBehaviour
{
    public struct PlayerInfo
    {
        public NetworkInstanceId netId;
        public string name;
        public float added;

        public override string ToString()
        {
            return string.Format("{0}: {1}", netId, name);
        }
    };

    public class SyncPlayerList : SyncListStruct<PlayerInfo> { }
    public SyncPlayerList CurrentPlayerList = new SyncPlayerList();

    public delegate void OnPlayerListChanged(PlayerList sender, GameObject playerObj);
    public event OnPlayerListChanged PlayerListChanged;

    public override void OnStartClient()
    {
        CurrentPlayerList.Callback = OnSyncListChanged;

        if (PlayerListChanged != null)
        {
            PlayerListChanged(this, null);
        }
    }

    public override void OnStartServer()
    {
        if (PlayerListChanged != null)
        {
            PlayerListChanged(this, null);
        }
    }

    void OnSyncListChanged(SyncList<PlayerInfo>.Operation op, int itemIndex)
    {
        Debug.Log("Players.OnSyncListChanged changed:" + op);

        GameObject playerObj = null;
        if (itemIndex < CurrentPlayerList.Count)
        {
            playerObj = ClientScene.FindLocalObject(CurrentPlayerList[itemIndex].netId);
        }

        if (PlayerListChanged != null)
        {
            PlayerListChanged(this, playerObj);
        }
    }

    private PlayerInfo? FindPlayerInfo(NetworkInstanceId netId, out int index)
    {
        PlayerInfo? playerInfo = null;

        index = -1;

        for (var i = 0; i < CurrentPlayerList.Count; ++i)
        {
            var player = CurrentPlayerList[i];
            if (netId == player.netId)
            {
                playerInfo = player;
                index = i;
                break;
            }
        }

        return playerInfo;
    }

    public bool AddPlayer(Player player)
    {
        if (player == null)
        {
            Debug.LogError("AddPlayer() player is null");
            return false;
        }

        var ni = player.GetComponent<NetworkIdentity>();
        if (ni == null)
        {
            Debug.LogError("AddPlayer no network identity");
            return false;
        }

        int index = -1;
        var pi = FindPlayerInfo(ni.netId, out index);
        if (pi == null)
        {
            var playerInfo = new PlayerInfo();
            playerInfo.netId = ni.netId;
            playerInfo.name = player.myName;
            playerInfo.added = Time.time;

            CurrentPlayerList.Add(playerInfo);

            return true;
        }

        return false;
    }

    public PlayerInfo? RemovePlayer(Player player)
    {
        if (player == null)
        {
            Debug.LogError("RemovePlayer null");
            return null;
        }

        var uv = player.GetComponent<NetworkIdentity>();
        if (uv == null)
        {
            Debug.LogError("RemovePlayer no network identity");
            return null;
        }

        PlayerInfo? pi = null;
        foreach (var playerInfo in CurrentPlayerList)
        {
            if (uv.netId == playerInfo.netId)
            {
                pi = playerInfo;
                CurrentPlayerList.Remove(playerInfo);
                break;
            }
        }

        return pi;
    }

    public void UpdatePlayer(Player player)
    {
        if (player == null)
        {
            Debug.LogError("UpdatePlayer null");
            return;
        }

        int index = -1;
        var playerInfo = FindPlayerInfo(player.netId, out index);
        if (playerInfo != null && playerInfo.HasValue && playerInfo.Value.name != player.myName)
        {
            var pi = new PlayerInfo();
            pi.netId = player.netId;
            pi.name = player.myName;
            pi.added = Time.time;

            CurrentPlayerList[index] = pi;
        }
        else if (playerInfo == null)
        {
            AddPlayer(player);
        }
    }
}
