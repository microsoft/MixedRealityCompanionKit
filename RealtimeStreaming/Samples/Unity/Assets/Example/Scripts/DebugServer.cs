// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using RealtimeStreaming;
using UnityEngine;

public class DebugServer : MonoBehaviour
{
    public RealtimeVideoServer server;
    public RealtimeServerSource streamSource;
    public TMPro.TextMeshPro debugLabel;

    void Update()
    {
        if (server != null && streamSource != null)
        {
            debugLabel.text = "Server Status: " + server.CurrentState + " - Connection Status: " + server.NetworkState + " - Streaming FPS: " + streamSource.FrameRate.ToString("F2");
        }
    }
}
