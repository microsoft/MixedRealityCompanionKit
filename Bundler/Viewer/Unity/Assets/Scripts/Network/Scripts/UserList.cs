// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using UnityEngine.UI;
using UnityEngine.Networking;
using System;

public class UserList : MonoBehaviour
{
    public RectTransform Content;
    public GameObject ItemPrefab;

    private PlayerList playerList;

    void Awake()
    {
        if (Content == null)
        {
            Debug.LogError("Set the Content object on the UserList.");
            return;
        }

        if (ItemPrefab == null)
        {
            Debug.LogError("UserList requires a Prefab that will represent the user in the list.");
            return;
        }

        if (ItemPrefab.GetComponentInChildren<Text>() == null)
        {
            Debug.LogError("Prefab object requires a Text UI element.");
            return;
        }
    }

    private void Update()
    {
        if (this.playerList == null)
        {
            var networkManager = NetworkManager.singleton as MyNetworkManager;
            if (networkManager == null)
            {
                Debug.LogError("Could not find Custom Network Manager");
                return;
            }

            this.playerList = networkManager.PlayerList;
            if (this.playerList == null)
            {
                Debug.LogError("PlayerList not attached to Custom Network Manager");
                return;
            }

            this.playerList.PlayerListChanged += OnPlayerListChanged;

            ClearAllChildren();

            PopulateList();
        }
    }

    private void OnDisable()
    {
        if (this.playerList != null)
        {
            this.playerList.PlayerListChanged -= OnPlayerListChanged;
        }
    }

    private void OnPlayerListChanged(PlayerList sender, GameObject gameObject)
    {
        ClearAllChildren();
        PopulateList();
    }

    private void ClearAllChildren()
    {
        foreach (Transform child in Content.gameObject.transform)
        {
            GameObject.Destroy(child.gameObject);
        }
    }

    private void PopulateList()
    {
        foreach (var obj in this.playerList.CurrentPlayerList)
        {
            // create item
            GameObject newItem = (GameObject)Instantiate(ItemPrefab);
            newItem.transform.SetParent(Content.gameObject.transform);
            newItem.transform.localPosition = Vector3.zero;
            newItem.transform.localRotation = Quaternion.identity;
            newItem.transform.localScale = Vector3.one;

            string name = obj.name;
            newItem.name = name;
            newItem.GetComponentInChildren<Text>().text = string.Format("{0}: {1}", obj.netId, name);
        }
    }
}
