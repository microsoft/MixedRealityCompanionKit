// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Diagnostics;
using System.Threading.Tasks;
using System.Windows.Input;
using Windows.UI.Popups;
using Windows.UI.Xaml;

namespace HoloLensCommander
{
    /// <summary>
    /// The view model for the HoloLensMonitorControl object.
    /// </summary>
    partial class HoloLensMonitorControlViewModel
    {
        /// <summary>
        /// The identifiers of the context menu commands.
        /// </summary>
        private enum MonitorContextMenuCommandIds
        {
            Tag = 0,
            ManageApps,
            MixedRealityView,
            DevicePortal,
            Disconnect,
            Reboot
        }

        /// <summary>
        /// Command used to disconnect from the HoloLens.
        /// </summary>
        public ICommand DisconnectCommand
        { get; private set; }

        /// <summary>
        /// Implementation of the disconnect command.
        /// </summary>
        internal void Disconnect()
        {
            this.holoLensMonitor.Disconnect();
            this.IsConnected = false;
            this.holoLensMonitorControl.NotifyDisconnected();
        }

        /// <summary>
        /// Implementation of the reboot menu command
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        internal async Task Reboot()
        {
            await this.holoLensMonitor.RebootAsync();
        }

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
                "Tag / IPD",
                ContextMenuCommandHandler,
                MonitorContextMenuCommandIds.Tag));
            contextMenu.Commands.Add(new UICommand(
                "Manage apps", 
                ContextMenuCommandHandler,
                MonitorContextMenuCommandIds.ManageApps));
            contextMenu.Commands.Add(new UICommand(
                "Mixed Reality view",
                ContextMenuCommandHandler,
                MonitorContextMenuCommandIds.MixedRealityView));
            contextMenu.Commands.Add(new UICommand(
                "Show Device Portal",
                ContextMenuCommandHandler,
                MonitorContextMenuCommandIds.DevicePortal));
            contextMenu.Commands.Add(new UICommand(
                "Disconnect",
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
                case MonitorContextMenuCommandIds.Tag:
                    t = this.TagHoloLensAsync();
                    break;

                case MonitorContextMenuCommandIds.ManageApps:
                    t = this.ManageAppsAsync();
                    break;

                case MonitorContextMenuCommandIds.MixedRealityView:
                    t = this.MixedRealityViewAsync();
                    break;

                case MonitorContextMenuCommandIds.DevicePortal:
                    t = this.LaunchDevicePortalAsync();
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