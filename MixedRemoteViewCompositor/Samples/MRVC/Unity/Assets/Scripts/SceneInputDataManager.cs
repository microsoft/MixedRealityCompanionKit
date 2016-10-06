// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using System;
using MixedRemoteViewCompositor;
using SpatialTranformHelper;

public enum DataChannelState
{
    Idle,
    ListenerStarted,
    ListenerStopped,
    ConnectorStarted,
    ConnectorStopped
}

[Serializable]
public class InputState : StateDataSerializer<InputState>
{
    public SpatialTranformHelper.Float3 WorldPosition;
    public SpatialTranformHelper.Float3 WorldNormal;
    public string HitObjectName;
}

[Serializable]
public class SceneState : StateDataSerializer<SceneState>
{
    public SpatialTranformHelper.Matrix4x4 SceneRoot;
}

public class SceneInputDataManager : MonoBehaviour
{
    public MrvcManager Manager;

    public ushort Port = 27773;

    public CursorManager CursorManager;
    public Transform SceneRootTransform;

    public ManagerState ManagerState
    {
        get { return this.mgrState; }
        private set
        {
            if (this.mgrState != value)
            {
                this.previousMgrState = this.mgrState;
                this.mgrState = value;
            }
        }
    }
    private ManagerState mgrState = ManagerState.Idle;
    private ManagerState previousMgrState = ManagerState.Idle;

    private Listener listener;
    private Connection listenerConnection;
    private Connector connector;
    private Connection connectorConnection;

    [Range(1, 120)]
    public int SceneStateSendRate = 4; // every x frames
    [Range(1, 120)]
    public int InputStateSendRate = 2; // every x frames

    private SceneState sceneState = null;
    private InputState inputState = null;

    private bool isListener = false;
    private bool isPaused = false;

    private void Awake()
    {
        if (this.Manager == null)
        {
            this.Manager = FindObjectOfType<MrvcManager>();
        }

        if (this.CursorManager == null)
        {
            this.CursorManager = FindObjectOfType<CursorManager>();
        }

        if (this.Manager == null || this.CursorManager == null)
        {
            return;
        }

        this.Manager.ManagerStateChanged += OnManagerStateChanged;
    }

    private void OnDestroy()
    {
        if (this.Manager != null)
        {
            this.Manager.ManagerStateChanged -= OnManagerStateChanged;
        }

        inputState = null;
        sceneState = null;
    }

    private void OnDisable()
    {
        Shutdown();
    }

    private void OnApplicationPause(bool pauseStatus)
    {
        this.isPaused = pauseStatus;
    }

    private void Update()
    {
        // todo: don't send data if remote client is not in a state to receive
        if (this.Manager.EnableMRC || !this.isListener || this.isPaused)
        {
            return;
        }

        // if we have a scene root send that transform data
        if ((Time.frameCount % this.SceneStateSendRate) == 0)
        {
            // create a sceneState data object
            if (this.sceneState == null)
            {
                this.sceneState = new SceneState();
            }

            if (this.SceneRootTransform != null)
            {
                this.sceneState.SceneRoot =
                    SpatialTranformHelper.Matrix4x4.FromUnity(UnityEngine.Matrix4x4.TRS(
                        this.SceneRootTransform.localPosition,
                        this.SceneRootTransform.localRotation,
                        this.SceneRootTransform.localScale));

                var buffer = SceneState.ToByteArray(this.sceneState);

                // send the buffer
                this.SendStateData(DataType.SceneState, ref buffer);
            }
        }

        if ((Time.frameCount % this.InputStateSendRate) == 0)
        {
            if (this.inputState == null)
            {
                this.inputState = new InputState();
            }

            this.inputState.WorldPosition = Float3.FromUnity(RaycastController.RaycastHit.point);
            this.inputState.WorldNormal = Float3.FromUnity(RaycastController.RaycastHit.normal);
            this.inputState.HitObjectName = RaycastController.HitThisFrame ? RaycastController.RaycastHit.collider.gameObject.name : string.Empty;

            var buffer = InputState.ToByteArray(this.inputState);

            this.SendStateData(DataType.InputState, ref buffer);
        }
    }

    private void SendStateData(DataType type, ref byte[] buffer)
    {
        if (this.listenerConnection == null)
        {
            return;
        }

        this.listenerConnection.SendData(type, buffer, buffer.Length);
    }

    private void OnManagerStateChanged(object sender, StateChangedEventArgs<ManagerState> args)
    {
        switch (args.CurrentState)
        {
            case ManagerState.Listening:
                this.StartListener(this.Port);
                break;
            case ManagerState.ConnectorConnected:
                this.StartConnector(string.Format(Plugin.DataUrlFormat, this.Manager.ConnectTo, this.Port));
                break;
            case ManagerState.ListenerFailed:
            case ManagerState.ConnectorFailed:
            case ManagerState.CaptureStop:
            case ManagerState.PlaybackStop:
                this.Shutdown();
                break;
        }
    }


    private void StartListener(ushort port)
    {
        if (this.Manager.EnableMRC)
        {
            return;
        }

        StopListener();

        this.ManagerState = ManagerState.ListenerStarting;

        // create listener
        this.listener = new Listener { Port = port };
        if (this.listener != null)
        {
            this.listener.Started += this.OnListenerStarted;
            this.listener.Connected += this.OnListenerConnected;
            this.listener.Failed += this.OnListenerFailed;
            this.listener.StartAsync();
        }
    }

    private void StopListener()
    {
        if (this.listener == null)
        {
            return;
        }

        this.listener.Started -= this.OnListenerStarted;
        this.listener.Connected -= this.OnListenerConnected;
        this.listener.Failed -= this.OnListenerFailed;
        this.listener.Close();
        this.listener.Dispose();
        this.listener = null;
    }


    private void StartConnector(string url)
    {
        if (this.Manager.EnableMRC)
        {
            return;
        }

        StopConnector();

        this.ManagerState = ManagerState.ConnectorStarting;

        this.connector = new Connector { ConnectionUrl = url };
        if (this.connector != null)
        {
            this.connector.Started += this.OnConnectorStarted;
            this.connector.Connected += this.OnConnectorConnected;
            this.connector.Failed += this.OnConnectorFailed;
            this.connector.StartAsync();
        }
    }

    private void StopConnector()
    {
        if (this.connector == null)
        {
            return;
        }

        this.connector.Started -= this.OnConnectorStarted;
        this.connector.Connected -= this.OnConnectorConnected;
        this.connector.Failed -= this.OnConnectorFailed;
        this.connector.Close();
        this.connector.Dispose();
        this.connector = null;
    }


    private void Shutdown()
    {
        this.isListener = false;

        if (this.connectorConnection != null)
        {
            this.connectorConnection.Close();
        }

        if (this.listenerConnection != null)
        {
            this.listenerConnection.Close();
        }
    }


    private void OnListenerStarted(object sender, EventArgs args)
    {
        this.ManagerState = ManagerState.Listening;
    }

    private void OnListenerFailed(object sender, FailedEventArgs args)
    {
        this.ManagerState = ManagerState.ListenerFailed;

        this.StopListener();
    }

    private void OnListenerConnected(object sender, Connection connection)
    {
        this.ManagerState = ManagerState.ListenerConnected;

        this.isListener = true;

        this.listenerConnection = connection;
        this.listenerConnection.Disconnected += this.OnListenerConnectionDisconnected;
        this.listenerConnection.Closed += this.OnListenerConnectionClosed;

        this.StopListener();
    }

    private void OnListenerConnectionDisconnected(object sender, EventArgs args)
    {
        this.Shutdown();
    }

    private void OnListenerConnectionClosed(object sender, EventArgs args)
    {
        if (this.listenerConnection == null)
        {
            return;
        }

        this.isListener = false;

        this.listenerConnection.Disconnected -= this.OnListenerConnectionDisconnected;
        this.listenerConnection.Closed -= this.OnListenerConnectionClosed;
        this.listenerConnection.Dispose();
        this.listenerConnection = null;

        this.ManagerState = ManagerState.Idle;
    }


    private void OnConnectorStarted(object sender, EventArgs args)
    {
        this.ManagerState = ManagerState.Connecting;
    }

    private void OnConnectorFailed(object sender, FailedEventArgs args)
    {
        this.ManagerState = ManagerState.ConnectorFailed;

        this.StopConnector();
    }

    private void OnConnectorConnected(object sender, Connection connection)
    {
        this.ManagerState = ManagerState.ConnectorConnected;

        this.connectorConnection = connection;
        this.connectorConnection.Disconnected += this.OnConnectorConnectionDisconnected;
        this.connectorConnection.Closed += this.OnConnectorConnectionClosed;
        this.connectorConnection.DataReceived += this.OnDataReceived;

        this.StopConnector();
    }

    private void OnConnectorConnectionDisconnected(object sender, EventArgs args)
    {
        this.Shutdown();
    }

    private void OnConnectorConnectionClosed(object sender, EventArgs args)
    {
        if (this.connectorConnection == null)
        {
            return;
        }

        this.connectorConnection.DataReceived -= this.OnDataReceived;
        this.connectorConnection.Disconnected -= this.OnConnectorConnectionDisconnected;
        this.connectorConnection.Closed -= this.OnConnectorConnectionClosed;
        this.connectorConnection.Dispose();
        this.connectorConnection = null;

        this.ManagerState = ManagerState.Idle;
    }


    private void OnDataReceived(object sender, DataReceivedArgs args)
    {
        switch (args.Type)
        {
            case DataType.SceneState:
                SceneState.FromByteArray(args.Buffer, ref this.sceneState);
                this.OnSceneData();
                break;
            case DataType.InputState:
                InputState.FromByteArray(args.Buffer, ref this.inputState);
                this.OnInputData();
                break;
        }
    }

    private void OnSceneData()
    {
        // update scene root transform
        if (this.SceneRootTransform == null || this.sceneState == null)
        {
            return;
        }

        var sceneRootMatrix = this.sceneState.SceneRoot.GetUnityMatrix();
        if (sceneRootMatrix != null)
        {
            var result = sceneRootMatrix.Value;

            // Potential perf issue:  extracting scale from a TRS matrix involves 
            // three square root operations.  Could just set scale to Vector3.one if 
            // you know it won't change.
            this.SceneRootTransform.localScale = result.Scale();
            this.SceneRootTransform.localRotation = result.Rotation();
            this.SceneRootTransform.localPosition = result.Translation();
        }
    }

    private void OnInputData()
    {
        if (this.inputState == null || this.CursorManager == null)
        {
            return;
        }

        GameObject hitObject = null;
        if (!string.IsNullOrEmpty(this.inputState.HitObjectName))
        {
            foreach (var hitCollider in Physics.OverlapSphere(this.inputState.WorldPosition.ToUnity(), 0.5f))
            {
                var gameObj = hitCollider.gameObject;
                if (gameObj != null && gameObj.name == this.inputState.HitObjectName)
                {
                    hitObject = gameObj;
                }
            }
        }

        this.CursorManager.SetCursorData(
            this.inputState.WorldPosition.ToUnity(),
            this.inputState.WorldNormal.ToUnity(),
            hitObject);
    }
}
