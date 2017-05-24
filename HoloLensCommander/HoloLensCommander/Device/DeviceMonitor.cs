// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;
using Microsoft.Tools.WindowsDevicePortal;
using Windows.UI.Core;

namespace HoloLensCommander
{
    /// <summary>
    /// Application install status event handler.
    /// </summary>
    /// <param name="sender">The DeviceMonitor sending the event.</param>
    /// <param name="args">Install status information.</param>
    public delegate void DeviceMonitorAppInstallStatusEventHandler(
        DeviceMonitor sender, 
        ApplicationInstallStatusEventArgs args);

    /// <summary>
    /// Delegate defining the method signature for handling the HeartbeatLost event.
    /// </summary>
    /// <param name="sender">The object sending the event.</param>
    public delegate void HeartbeatLostEventHandler(
        DeviceMonitor sender);

    /// <summary>
    /// Delegate defining the method signature for handling the HeartbeatLost event.
    /// </summary>
    /// <param name="sender">The object sending the event.</param>
    public delegate void HeartbeatReceivedEventHandler(
        DeviceMonitor sender);

    /// <summary>
    /// Class that provides the relevant functionality of the Windows Device Portal.
    /// </summary>
    public partial class DeviceMonitor : IDisposable
    {
        /// <summary>
        /// The default address used when connecting to a device. This address assumes
        /// a USB connection.
        /// </summary>
        public static readonly string DefaultConnectionAddress = "localhost:10080";
        public static readonly string DefaultConnectionAddressAsIp = "127.0.0.1:10080";

        /// <summary>
        /// The min, max and default heartbeat interval values, in seconds.
        /// </summary>
        public static readonly float MinimumHeartbeatIntervalSeconds = 1.0f;
        public static readonly float MaximumHeartbeatIntervalSeconds = 60.0f;
        public static readonly float DefaultHeartbeatIntervalSeconds = 5.0f;
        
        /// <summary>
        /// The executable name of the HoloLens shell application.
        /// </summary>
        private static readonly string ShellApp = "HoloShellApp.exe";

        /// <summary>
        /// Options used to connect to this device.
        /// </summary>
        /// <remarks>
        /// This data is displayed until an initial connection is made to the device.
        /// </remarks>
        private ConnectOptions connectOptions;

        /// <summary>
        /// Dispatcher that allows heartbeats to be marshaled appropriately.
        /// </summary>
        private CoreDispatcher dispatcher;

        /// <summary>
        /// Instance of the IDevicePortalConnection used to connect to this device.
        /// </summary>
        private IDevicePortalConnection devicePortalConnection;

        /// <summary>
        /// Instance of the DevicePortal used to communicate with this device.
        /// </summary>
        private DevicePortal devicePortal;

        /// <summary>
        /// Has a successful heartbeat been received from the device?
        /// </summary>
        private bool firstContact;

        /// <summary>
        /// The current heartbeat interval, in seconds.
        /// </summary>
        private float heartbeatInterval;

        /// <summary>
        /// The timer used to initiate a heartbeat check.
        /// </summary>
        private Timer heartbeatTimer;

        /// <summary>
        /// Event that is sent when the application install status has changed.
        /// </summary>
        public event DeviceMonitorAppInstallStatusEventHandler AppInstallStatus;
        
        /// <summary>
        /// Event that is sent when the heartbeat has been lost.
        /// </summary>
        public event HeartbeatLostEventHandler HeartbeatLost;

        /// <summary>
        /// Event that is sent when the heartbeat has been received.
        /// </summary>
        public event HeartbeatReceivedEventHandler HeartbeatReceived;

        /// <summary>
        /// Initializes a new instance of the <see cref="DeviceMonitor" /> class.
        /// </summary>
        public DeviceMonitor(CoreDispatcher dispatcher) : this(dispatcher, DefaultHeartbeatIntervalSeconds)
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="DeviceMonitor" /> class.
        /// </summary>
        /// <param name="heartbeatInterval">The time, in seconds, between heartbeat checks.</param>
        public DeviceMonitor(CoreDispatcher dispatcher, float heartbeatInterval)
        {
            if (dispatcher == null)
            {
                throw new NullReferenceException("The argument dispatcher cannot be null.");
            }

            this.dispatcher = dispatcher;

            if ((heartbeatInterval < MinimumHeartbeatIntervalSeconds) ||
                (heartbeatInterval > MaximumHeartbeatIntervalSeconds))
            {
                throw new ArgumentException("The heartbeatInterval value is out of bounds");
            }

            this.heartbeatInterval = heartbeatInterval;
            this.firstContact = false;

            // Create the timer, but do not start it.
            this.heartbeatTimer = new Timer(
                HeartbeatCallback,
                null,
                Timeout.Infinite,
                Timeout.Infinite);
        }

        /// <summary>
        /// Finalizer so that we are assured we clean up all encapsulated resources.
        /// </summary>
        /// <remarks>Call Dispose on this object to avoid running the finalizer.</remarks>
        ~DeviceMonitor()
        {
            this.Dispose();
        }

        /// <summary>
        /// Cleans up encapsulated resources.
        /// </summary>
        public void Dispose()
        {
            if (this.devicePortal != null)
            {
                this.devicePortal.AppInstallStatus += DevicePortal_AppInstallStatus;
                this.devicePortal = null;
            }

            // Release the resources we manage.
            this.heartbeatTimer?.Dispose();
            this.heartbeatTimer = null;

            // Suppress finalization to avoid attempting to clean up twice.
            GC.SuppressFinalize(this);
        }

        /// <summary>
        /// Checks for the heartbeat and updates cached information.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        private async Task CheckHeartbeatAsync()
        {
            try
            {
                this.SuspendHeartbeat();

                try
                {
                    // Have connected to the device the first time?
                    if (!this.firstContact)
                    {
                        // Try to connect now.
                        await this.EstablishConnection();
                    }

                    // TODO: this.MachineName = await this.GetMachineNameAsync();
                    await this.UpdateBatteryStatus();
                    await this.UpdateIpd();
                    await this.UpdateThermalStage();

                    this.NotifyHeartbeatReceived();
                }
                catch
                {
                    this.NotifyHeartbeatLost();
                }

                // Resume the timer.
                this.StartHeartbeat();
            }
            catch
            {
                // TODO: - investigate whether or not we ever hit this scenario
            }
        }

        /// <summary>
        /// Called by the heartbeat timer.
        /// </summary>
        /// <param name="data">Not used.</param>
        private void HeartbeatCallback(Object data)
        {
            // Assigning the return value of CheckHeartbeatAsync to a Task object to avoid 
            // warning 4014 (call is not awaited).
            Task t = CheckHeartbeatAsync();
        }

        /// <summary>
        /// Notifies registered listeners that the device heartbeat has been lost.
        /// </summary>
        private void NotifyHeartbeatLost()
        {
            if (!this.dispatcher.HasThreadAccess)
            {
                // Assigning the return value of RunAsync a Task object to avoid 
                // warning 4014 (call is not awaited).
                Task t = this.dispatcher.RunAsync(
                    CoreDispatcherPriority.Normal,
                    () =>
                    {
                        this.NotifyHeartbeatLost();
                    }).AsTask();
                return;
            }

            this.HeartbeatLost?.Invoke(this);
        }

        /// <summary>
        /// Notifies registered listeners that the device heartbeat has been received.
        /// </summary>
        private void NotifyHeartbeatReceived()
        {
            if (!this.dispatcher.HasThreadAccess)
            {
                // Assigning the return value of RunAsync to a Task object to avoid 
                // warning 4014 (call is not awaited).
                Task t = this.dispatcher.RunAsync(
                    CoreDispatcherPriority.Normal,
                    () =>
                    {
                        this.NotifyHeartbeatReceived();
                    }).AsTask();
                return;
            }

            this.HeartbeatReceived?.Invoke(this);
        }

        /// <summary>
        /// Starts/restarts the heartbeat timer by setting a non-infinite interval
        /// </summary>
        internal void StartHeartbeat()
        {
            this.UpdateHeartbeat(
                (int)(this.HeartbeatIntervalSeconds * 1000.0f));
        }

        /// <summary>
        /// Suspends the heartbeat timer by setting an infinite interval
        /// </summary>
        internal void SuspendHeartbeat()
        {
            this.UpdateHeartbeat(Timeout.Infinite);
        }

        /// <summary>
        /// Updates the cached battery data.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        private async Task UpdateBatteryStatus()
        {
            this.BatteryState = await this.devicePortal.GetBatteryStateAsync();
        }

        /// <summary>
        /// Update the heartbeat timer's due time.
        /// </summary>
        /// <param name="dueTimeMs">Milliseconds for the timer to wait until the next tick.</param>
        private void UpdateHeartbeat(int dueTimeMs)
        {
            this.heartbeatTimer.Change(
                dueTimeMs,
                Timeout.Infinite);
        }

        /// <summary>
        /// Updates the cached interpupilary distance data.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        private async Task UpdateIpd()
        {
            try
            {
                this.Ipd = await this.devicePortal.GetInterPupilaryDistanceAsync();
            }
            catch(NotSupportedException)
            {
                // Not supported on this type of device.
            }
        }

        /// <summary>
        /// Updates the cached thermal data.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        private async Task UpdateThermalStage()
        {
            try
            {
                this.ThermalStage = await this.devicePortal.GetThermalStageAsync();
            }
            catch(NotSupportedException)
            {
                // Not supported on this type of device.
            }
        }
    }
}
