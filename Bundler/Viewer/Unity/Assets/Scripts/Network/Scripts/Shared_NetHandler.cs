// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine.Networking;

public class Shared_NetHandler
{
    public static void Client_Register()
    {
        NetworkClient client = NetworkManager.singleton.client;
        client.UnregisterHandler(99);
        client.RegisterHandler((short)99, LocalNetworkTransform.HandleTransform);
    }

    public static void Client_UnRegister()
    {
        NetworkClient client = NetworkManager.singleton.client;
        client.UnregisterHandler(99);
    }

    public static void Server_Register()
    {
        Server_UnRegister();
        NetworkServer.RegisterHandler((short)99, LocalNetworkTransform.HandleTransform);
    }

    public static void Server_UnRegister()
    {
        NetworkServer.UnregisterHandler(99);
    }
}
