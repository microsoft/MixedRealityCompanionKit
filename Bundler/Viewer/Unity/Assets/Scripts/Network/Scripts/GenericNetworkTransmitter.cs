// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using HoloToolkit.Unity;
using System;
using System.Collections.Generic;

#if !UNITY_EDITOR
using Windows.Networking.Sockets;
using Windows.Storage.Streams;
using Windows.Networking;
using Windows.Foundation;
using System.Threading;
using System.Threading.Tasks;
#endif

public class GenericNetworkTransmitter
{
    /// <summary>
    /// The connection port on the machine to use..
    /// </summary>
    public int SendConnectionPort = 11000;

    /// <summary>
    /// When data arrives, this event is raised.
    /// </summary>
    /// <param name="data">The data that arrived.</param>
    public delegate void OnRevievedData(byte[] data);
    public event OnRevievedData RevievedData;

    /// <summary>
    /// Keeps the most recent data buffer.
    /// </summary>
    private byte[] mostRecentDataBuffer;

    /// <summary>
    /// If someone connects to us, this is the data we will send them.
    /// </summary>
    /// <param name="data"></param>
    public void SendData(byte[] data)
    {
        StartAsServer(data);
    }

    /// <summary>
    /// Requests data from the server and handles getting the data and firing
    /// the dataReadyEvent.
    /// </summary>
    public void RequestData(string serverIp)
    {
        StartAsClient(serverIp);
    }

    // A lot of the work done in this class can only be done in UWP. The editor is not a UWP app.
#if !UNITY_EDITOR
    /// <summary>
    /// Tracks if we have an outstanding start request
    /// </summary>
    private bool pendingStartRequest = false;

    /// <summary>
    /// If we are running as the server, this is the listener the server will use.
    /// </summary>
    private StreamSocketListener networkListener;

    /// <summary>
    /// If we cannot connect to the server, this is how long we will wait before retrying.
    /// </summary>
    private float timeToDeferFailedConnections = 1.0f;

    private async Task StopServer()
    {
        if (networkListener != null)
        {
            networkListener.ConnectionReceived -= ConnectionReceived;
            await networkListener.CancelIOAsync();

            networkListener.Dispose();
            networkListener = null;
        }
    }

    /// <summary>
    /// Configures the network transmitter as the source.
    /// </summary>
    public async void StartAsServer(byte[] data)
    {
        if (pendingStartRequest)
        {
            Debug.Log("Unable to start as server, as there's a pending request.");
            return;
        }

        pendingStartRequest = true;
        Debug.Log("Starting as server.");

        try
        {
            await StopServer();
            mostRecentDataBuffer = data;
            networkListener = new StreamSocketListener();
            networkListener.ConnectionReceived += ConnectionReceived;
            await networkListener.BindServiceNameAsync(SendConnectionPort.ToString());
        }
        catch
        {
            Debug.LogError("Failed to start as server.");
        }

        pendingStartRequest = false;
    }

    /// <summary>
    /// When a connection is made to us, this call back gets called and
    /// we send our data.
    /// </summary>
    /// <param name="sender">The listener that was connected to.</param>
    /// <param name="args">some args that we don't use.</param>
    private void ConnectionReceived(
        StreamSocketListener sender, 
        StreamSocketListenerConnectionReceivedEventArgs args)
    {
        if (sender != networkListener)
        {
            Debug.LogWarning("Connection recieved on an old server. Not handling request.");
            return;
        }

        // If we have data, send it. 
        if (mostRecentDataBuffer != null)
        {
            Debug.Log("Sending the data " + mostRecentDataBuffer.Length);
            IOutputStream stream = args.Socket.OutputStream;
            using (DataWriter writer = new DataWriter(stream))
            {
                writer.WriteInt32(mostRecentDataBuffer.Length);
                writer.WriteBytes(mostRecentDataBuffer);
                writer.StoreAsync().AsTask().Wait();
                writer.FlushAsync().AsTask().Wait();
            }
        }
        else
        {
            Debug.LogError("No data to send but we've been connected to. This is unexpected.");
        }
    }

    /// <summary>
    /// Connects to the server and requests data.
    /// </summary>
    private async void StartAsClient(string serverIp)
    {
        if (pendingStartRequest)
        {
            Debug.Log("Unable to start as client, as there's a pending request: " + serverIp);
            return;
        }

        Debug.Log("Starting as client: " + serverIp);
        byte[] result = null;
        pendingStartRequest = true;

        try
        {
            await StopServer();
            HostName networkHost = new HostName(serverIp);
            using (StreamSocket networkConnection = new StreamSocket())
            {
                await networkConnection.ConnectAsync(networkHost, SendConnectionPort.ToString());
                result = await ReadInputStream(networkConnection);
            }
        }
        catch
        {
            Debug.LogError("Failed to start as client: " + serverIp);
        }

        mostRecentDataBuffer = result;
        pendingStartRequest = false;
        Debug.Log("Stopping client: " + serverIp);

        if (result != null && this.RevievedData != null)
        {
            Debug.Log("Invoking RevievedData from server: " + serverIp);
            this.RevievedData(result);
        }
    }

    /// <summary>
    /// When a connection to the server is established and we can start reading the data, this will be called.
    /// </summary>
    private async Task<byte[]> ReadInputStream(StreamSocket networkConnection)
    {
        byte[] result = null;
        Debug.Log("Attemping to readh input stream: " + networkConnection.Information.RemoteAddress.ToString());

        DataReader networkDataReader;
        // Since we are connected, we can read the data being sent to us.
        using (networkDataReader = new DataReader(networkConnection.InputStream))
        {
            // read four bytes to get the size.
            DataReaderLoadOperation loadOperation = networkDataReader.LoadAsync(4);
            while (loadOperation.Status == AsyncStatus.Started)
            {
                // just waiting.
            }

            int dataSize = networkDataReader.ReadInt32();
            if (dataSize < 0)
            {
                Debug.LogError("Super bad super big datasize");
            }

            // Need to allocate a new buffer with the dataSize.
            result = new byte[dataSize];

            // Read the data.
            await networkDataReader.LoadAsync((uint)dataSize);
            networkDataReader.ReadBytes(result);
            Debug.Log("Finished to reading input stream (" + dataSize + " bytes): " + networkConnection.Information.RemoteAddress.ToString());
        }

        return result;
    }

#else
    public void StartAsServer(byte[] data)
    {
        mostRecentDataBuffer = data;
    }

    private bool StartAsClient(string serverIp)
    {
        return true;
    }
#endif
}
