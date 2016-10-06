// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Text;
using MixedRemoteViewCompositor;
using UnityEngine;
using UnityEngine.UI;

public class LogStates : MonoBehaviour
{
    public Text LogText = null;
    private MrvcManager mrvcManager = null;
    private readonly StringBuilder sb = new StringBuilder();

    private void Start()
    {
        this.mrvcManager = FindObjectOfType<MrvcManager>();
        this.mrvcManager.ManagerStateChanged += MrvcManager_ManagerStateChanged;
        this.mrvcManager.ComponentStateChanged += MrvcManager_ComponentStateChanged;
        this.mrvcManager.CaptureStateChanged += MrvcManager_CaptureStateChanged;
        this.mrvcManager.PlayerStateChanged += MrvcManager_PlayerStateChanged;
    }

    private void MrvcManager_PlayerStateChanged(object sender, StateChangedEventArgs<PlayerState> args)
    {
        var updateText = string.Format(
            "Player state - lastState: {0}   currentState: {1}   connection - lastState: {2}   currentState: {3}"
            , args.LastState
            , args.CurrentState
            , args.LastConnectionState
            , args.ConnectionState);
        this.AppendDebugText(updateText);
    }

    private void MrvcManager_CaptureStateChanged(object sender, StateChangedEventArgs<CaptureState> args)
    {
        var updateText = string.Format(
            "Capture state - lastState: {0}   currentState: {1}   connection - lastState: {2}   currentState: {3}"
            , args.LastState
            , args.CurrentState
            , args.LastConnectionState
            , args.ConnectionState);
        this.AppendDebugText(updateText);
    }

    private void MrvcManager_ComponentStateChanged(object sender, StateChangedEventArgs<ComponentState> args)
    {
        var updateText = string.Format("Manager component - lastState: {0}   currentState: {1}", args.LastState, args.CurrentState);
        this.AppendDebugText(updateText);
    }

    private void MrvcManager_ManagerStateChanged(object sender, StateChangedEventArgs<ManagerState> args)
    {
        var updateText = string.Format("Manager state - lastState: {0}   currentState: {1}", args.LastState, args.CurrentState);
        this.AppendDebugText(updateText);
    }

    private void AppendDebugText(string text)
    {
        this.sb.AppendLine(text);

        this.LogText.text = this.sb.ToString();
    }
}
