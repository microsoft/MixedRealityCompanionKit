// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Diagnostics;
using System.Threading.Tasks;
using System.Windows.Input;
using Windows.UI.Popups;
using Windows.UI.Xaml;
using static Microsoft.Tools.WindowsDevicePortal.DevicePortal;

namespace HoloLensCommander
{
    /// <summary>
    /// The view model for the DeviceMonitorControl object.
    /// </summary>
    partial class DeviceMonitorControlViewModel
    {
        /// <summary>
        /// The identifiers of the context menu commands.
        /// </summary>
        private enum MonitorContextMenuCommandIds
        {
            DeviceInfo = 0,
            ManageApps,
            MixedRealityView,
            DevicePortal,
            Disconnect,
            Reboot
        }

        /// <summary>
        /// Command used to clear the application status message.
        /// </summary>
        public ICommand ClearStatusMessageCommand
        { get; private set; }

        /// <summary>
        /// Command used to disconnect from the device.
        /// </summary>
        public ICommand DisconnectCommand
        { get; private set; }

        /// <summary>
        /// Implementation of the disconnect command.
        /// </summary>
        internal void Disconnect()
        {
            this.deviceMonitor.Disconnect();
            this.IsConnected = false;
            this.deviceMonitorControl.NotifyDisconnected();
        }

        /// <summary>
        /// Implementation of the reboot menu command
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        internal async Task Reboot()
        {
            await this.deviceMonitor.RebootAsync();
        }

        /// <summary>
        /// Sets the underlying DeviceMonitor's heartbeat interval.
        /// </summary>
        /// <param name="heartbeatInterval">The time, in secondsm between heartbeat checks.</param>
        internal void SetHeartbeatInterval(float heartbeatInterval)
        {
            this.deviceMonitor.HeartbeatInterval = heartbeatInterval;
        }

        /// <summary>
        /// Command used to display the set ipd dialog.
        /// </summary>
        public ICommand SetIpdCommand
        { get; private set; }

        /// <summary>
        /// Command used to display the set tag dialog.
        /// </summary>
        public ICommand SetTagCommand
        { get; private set; }

        /// <summary>
        /// Command used to display the context menu.
        /// </summary>
        public ICommand ShowContextMenuCommand
        { get; private set; }

        /// <summary>
        /// Implementation of the show context menu command
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        private async Task ShowContextMenuAsync(object sender)
        {
            PopupMenu contextMenu = new PopupMenu();

            contextMenu.Commands.Add(new UICommand(
                "Device Information",
                ContextMenuCommandHandler,
                MonitorContextMenuCommandIds.DeviceInfo));
            contextMenu.Commands.Add(new UICommand(
                "Manage apps", 
                ContextMenuCommandHandler,
                MonitorContextMenuCommandIds.ManageApps));
            // Virtual Machines do not support Mixed Reality, base the decision
            // on whether or not to show the Mixed Reality view item on the platform
            // rather than the filter.
            if (this.deviceMonitor.Platform == DevicePortalPlatforms.HoloLens)
            {
                contextMenu.Commands.Add(new UICommand(
                    "Mixed Reality view",
                    ContextMenuCommandHandler,
                    MonitorContextMenuCommandIds.MixedRealityView));
            }
            contextMenu.Commands.Add(new UICommand(
                "Show Device Portal",
                ContextMenuCommandHandler,
                MonitorContextMenuCommandIds.DevicePortal));
            contextMenu.Commands.Add(new UICommand(
                "Unregister",
                ContextMenuCommandHandler,
                MonitorContextMenuCommandIds.Disconnect));

            await contextMenu.ShowForSelectionAsync(
                Utilities.GetFrameworkElementRect((FrameworkElement)sender));
        }

        /// <summary>
        /// Handles context menu command selection.
        /// </summary>
        /// <param name="command">The command which was selected from the context menu</param>
        private void ContextMenuCommandHandler(IUICommand command)
        {
            // Assigning the return value of the async commands to a Task object to avoid 
            // warning 4014 (call is not awaited).
            Task t;

            switch((MonitorContextMenuCommandIds)command.Id)
            {
                case MonitorContextMenuCommandIds.DeviceInfo:
                    try
                    {
                        t = this.ShowDeviceInfoAsync();
                    }
                    catch (Exception e)
                    {
                        this.StatusMessage = string.Format(
                            "Failed to display device information ({0})",
                            e.Message);
                    }
                    break;

                case MonitorContextMenuCommandIds.ManageApps:
                    try
                    {
                        t = this.ManageAppsAsync();
                    }
                    catch (Exception e)
                    {
                        this.StatusMessage = string.Format(
                            "Failed to manage applications ({0})",
                            e.Message);
                    }
                    break;

                case MonitorContextMenuCommandIds.MixedRealityView:
                    try
                    {
                        t = this.MixedRealityViewAsync();
                    }
                    catch (Exception e)
                    {
                        this.StatusMessage = string.Format(
                            "Failed to display Mixed Reality view ({0})",
                            e.Message);
                    }
                    break;

                case MonitorContextMenuCommandIds.DevicePortal:
                    try
                    {
                        t = this.LaunchDevicePortalAsync();
                    }
                    catch (Exception e)
                    {
                        this.StatusMessage = string.Format(
                            "Failed to launch the Windows Device Portal ({0})",
                            e.Message);
                    }
                    break;

                case MonitorContextMenuCommandIds.Disconnect:
                    this.Disconnect();
                    break;

                default:
                    Debug.Assert(false,
                        string.Format(
                        "Unrecognized context menu command id: {0}",
                        (int)command.Id));
                    break;
            }
        }
    }
}