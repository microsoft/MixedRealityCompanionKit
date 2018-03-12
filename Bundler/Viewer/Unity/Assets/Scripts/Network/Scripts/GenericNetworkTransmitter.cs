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

public class RequestDataCompletedArgs
{
    public RequestDataCompletedArgs(bool successful, byte[] data = null)
    {
        Successful = successful;
        Data = data;
    }

    /// <summary>
    /// Get if data request was successful
    /// </summary>
    public bool Successful { get; private set; }

    /// <summary>
    /// The received data.
    /// </summary>
    public byte[] Data { get; private set; }
}

public class GenericNetworkTransmitter
{

    /// <summary>
    /// The various states this transmitter can be in
    /// </summary>
    private enum TransmitterState
    {
        None,
        Server,
        Client
    }

    /// <summary>
    /// When data arrives, this event is raised.
    /// </summary>
    /// <param name="data">The data that arrived.</param>
    public delegate void OnRequestDataCompleted(GenericNetworkTransmitter sender, RequestDataCompletedArgs args);
    public event OnRequestDataCompleted RequestDataCompleted;

    /// <summary>
    /// Keeps the most recent data buffer.
    /// </summary>
    private byte[] serverBuffer;

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
    /// The connection port on the machine to use..
    /// </summary>
    private const int sendConnectionPort = 11000;

    /// <summary>
    /// If we are running as the server, this is the listener the server will use.
    /// </summary>
    private StreamSocketListener networkListener;

    /// <summary>
    /// The current state of the transmitter.
    /// </summary>
    private TransmitterState state = TransmitterState.None;

    /// <summary>
    /// A lock to allow safe changing state on different threads
    /// </summary>
    private System.Object stateLock = new System.Object();

    /// <summary>
    /// If we cannot connect to the server, this is how long we will wait before retrying.
    /// </summary>
    private const int timeToDeferFailedConnectionsMS = 3000;

    /// <summary>
    /// Stop the current server
    /// </summary>
    /// <returns></returns>
    private async Task StopServer()
    {
        StreamSocketListener lastNetworkListener = null; 
        lock (stateLock)
        {
            lastNetworkListener = networkListener;
            networkListener = null;
            serverBuffer = null;
        }

        if (lastNetworkListener != null)
        {
            lastNetworkListener.ConnectionReceived -= ConnectionReceived;
            await lastNetworkListener.CancelIOAsync();
            lastNetworkListener.Dispose();
        }
    }

    /// <summary>
    /// Configures the network transmitter as the source.
    /// </summary>
    public async void StartAsServer(byte[] data)
    {
        Debug.Log("[GenericNetworkTransmitter] Starting as server.");

        StreamSocketListener newNetworkListener = null;
        lock (stateLock)
        {
            state = TransmitterState.Server;
            newNetworkListener = networkListener;
            serverBuffer = data;
        }
 
        try
        {
            if (newNetworkListener == null)
            {
                newNetworkListener = new StreamSocketListener();
                newNetworkListener.ConnectionReceived += ConnectionReceived;
                await newNetworkListener.BindServiceNameAsync(sendConnectionPort.ToString());
            }
        }
        catch
        {
            Debug.LogError("[GenericNetworkTransmitter] Failed to start as server.");
        }

        lock (stateLock)
        {
            if (networkListener == null && state == TransmitterState.Server)
            {
                networkListener = newNetworkListener;
            }
            else
            {
                newNetworkListener.ConnectionReceived -= ConnectionReceived;
                newNetworkListener.Dispose();
            }
        }
    }

    /// <summary>
    /// When a connection is made to us, this call back gets called and
    /// we send our data.
    /// </summary>
    /// <param name="sender">The listener that was connected to.</param>
    /// <param name="args">some arguments that we don't use.</param>
    private async void ConnectionReceived(
        StreamSocketListener sender, 
        StreamSocketListenerConnectionReceivedEventArgs args)
    {
        byte[] buffer = null;
        lock (stateLock)
        {
            buffer = serverBuffer;
        }

        // If we have data, send it. 
        if (buffer != null)
        {
            Debug.LogFormat("[GenericNetworkTransmitter] Sending data ({0} bytes)", serverBuffer.Length);
            IOutputStream stream = args.Socket.OutputStream;
            using (DataWriter writer = new DataWriter(stream))
            {
                writer.WriteInt32(buffer.Length);
                writer.WriteBytes(buffer);
                await writer.StoreAsync();
                await writer.FlushAsync();
            }
        }
        else
        {
            Debug.LogError("[GenericNetworkTransmitter] No data to send but we've been connected to. This is unexpected.");
        }
    }

    /// <summary>
    /// Connects to the server and requests data.
    /// </summary>
    private async void StartAsClient(string serverIp)
    {
        Debug.LogFormat("[GenericNetworkTransmitter] Attempting to connect to server (server IP: {0})", serverIp);

        lock (stateLock)
        {
            state = TransmitterState.Client;
        }

        await StopServer();

        byte[] result = null;
        try
        {
            HostName networkHost = new HostName(serverIp);
            using (StreamSocket networkConnection = new StreamSocket())
            {
                await networkConnection.ConnectAsync(networkHost, sendConnectionPort.ToString());
                result = await ReadInputStream(networkConnection);
            }
        }
        catch
        {
            Debug.LogErrorFormat("[GenericNetworkTransmitter] Failed to receive data from server (server IP: {0})", serverIp);
        }

        lock (stateLock)
        {
            if (state != TransmitterState.Client)
            {
                Debug.LogWarningFormat("[GenericNetworkTransmitter] No longer a client, releasing received data (server IP: {0})", serverIp);
                result = null;
            }
        }

        bool success = result != null && result.Length > 0;
        if (!success && RetryAsClient(serverIp))
        {
            Debug.LogWarningFormat("[GenericNetworkTransmitter] Failed to received data, but retrying (server IP: {0})", serverIp);
        }
        else if (this.RequestDataCompleted != null)
        {
            Debug.LogFormat(
                "[GenericNetworkTransmitter] Invoking RequestDataCompleted from server (host IP: {0}) (success: {1}) (bytes: {2})",
                serverIp, 
                success,
                result != null ? result.Length : 0);

            this.RequestDataCompleted(this, new RequestDataCompletedArgs(success, result));
        }
    }

    /// <summary>
    /// Retry starting as a client, after a delay
    /// </summary>
    private bool RetryAsClient(string serverIp)
    {
        lock (stateLock)
        {
            if (state != TransmitterState.Client)
            {
                return false;
            }
        }

        return true;
    }

    /// <summary>
    /// Retry starting as a client, after a delay. This does the actual work of waiting
    /// </summary>
    private async void RetryAsClientWorker(string serverIp)
    {
        await Task.Delay(timeToDeferFailedConnectionsMS);
        lock (stateLock)
        {
            if (state != TransmitterState.Client)
            {
                return;
            }
        }

        StartAsClient(serverIp);
    }

    /// <summary>
    /// When a connection to the server is established and we can start reading the data, this will be called.
    /// </summary>
    private async Task<byte[]> ReadInputStream(StreamSocket networkConnection)
    {
        byte[] result = null;
        Debug.LogFormat("[GenericNetworkTransmitter] Attempting to read input stream (host IP: {0})",
            networkConnection.Information.RemoteAddress.ToString());
        
        // Since we are connected, we can read the data being sent to us.
        using (DataReader networkDataReader = new DataReader(networkConnection.InputStream))
        {
            int dataSize = await ReadInputStreamSize(networkDataReader);
            if (dataSize < 0)
            {
                Debug.LogError("[GenericNetworkTransmitter] data size was too big.");
            }
            else
            {
                // Need to allocate a new buffer with the dataSize.
                result = new byte[dataSize];

                // Read the data.
                await networkDataReader.LoadAsync((uint)dataSize);
                networkDataReader.ReadBytes(result);
                Debug.LogFormat("[GenericNetworkTransmitter] Finished to reading input stream ({0} bytes) (host IP: {1})",
                    dataSize,
                    networkConnection.Information.RemoteAddress.ToString());
            }
        }

        return result;
    }

    /// <summary>
    /// Obtain the total length of data
    /// </summary>
    private async Task<int> ReadInputStreamSize(DataReader networkDataReader)
    {
        Task<int> task = new Task<int>(() => 
        {
            // read four bytes to get the size.
            DataReaderLoadOperation loadOperation = networkDataReader.LoadAsync(4);
            while (loadOperation.Status == AsyncStatus.Started)
            {
                // just waiting.
            }

            return networkDataReader.ReadInt32();
        });

        task.Start();
        return await task;
    }
#else
    public void StartAsServer(byte[] data)
    {
    }

    private bool StartAsClient(string serverIp)
    {
        return true;
    }
#endif
}
