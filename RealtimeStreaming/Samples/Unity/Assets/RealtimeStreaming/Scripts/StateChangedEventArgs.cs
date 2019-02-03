// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;

namespace RealtimeStreaming
{
    public class StateChangedEventArgs<T> : EventArgs
    {
        public T LastState { get; private set; }
        public T CurrentState { get; private set; }

        public ConnectionState LastConnectionState { get; private set; }
        public ConnectionState ConnectionState { get; private set; }

        public StateChangedEventArgs(T lastState, T currentState)
        {
            this.LastState = lastState;
            this.CurrentState = currentState;
            this.LastConnectionState = ConnectionState.Idle;
            this.ConnectionState = ConnectionState.Idle;
        }

        public StateChangedEventArgs(T lastState, T currentState, ConnectionState lastConnectionState, ConnectionState connectionState)
        {
            this.LastState = lastState;
            this.CurrentState = currentState;
            this.LastConnectionState = lastConnectionState;
            this.ConnectionState = connectionState;
        }
    }
}
