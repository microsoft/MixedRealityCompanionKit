// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Threading.Tasks;
using System.Windows.Input;
using System.Xml.Serialization;
using Windows.Storage;
using Windows.UI.Popups;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Microsoft.Tools.WindowsDevicePortal;

namespace HoloLensCommander
{
    /// <summary>
    /// The view model for the MainPage.
    /// </summary>
    partial class MainWindowViewModel
    {
        /// <summary>
        /// The identifiers of the connect context menu commands.
        /// </summary>
        private enum ConnectContextMenuCommandIds
        {
            ReconnectPreviousSession = 0,
            SetDefaultCredentials,
            UseDefaultCredentials
        }

        /// <summary>
        /// Command used to close all applications on the selected devices.
        /// </summary>
        public ICommand CloseAllAppsCommand
        { get; private set; }

        /// <summary>
        /// Implementation of the close all applications command.
        /// </summary>
        private void CloseAllApps()
        {
            foreach (DeviceMonitorControl monitor in this.GetCopyOfRegisteredDevices())
            {
                // Assigning the return value of CloseAllAppsAsync to a Task object to avoid 
                // warning 4014 (call is not awaited).
                Task t = monitor.CloseAllAppsAsync();
            }

            this.StatusMessage = "";
        }

        /// <summary>
        /// Command used to connect to a device.
        /// </summary>
        public ICommand ConnectToDeviceCommand
        { get; private set; }

        /// <summary>
        /// Implementation of the connect to device command.
        /// </summary>
        /// <param name="connectOptions">Options used when connecting to the device.</param>
        /// <param name="name">Descriptive name to assign to the device.</param>
        /// <param name="suppressDialog">True causes the connection dialog to not be shown./param>
        /// <returns>Task object used for tracking method completion.</returns>
        private async Task ConnectToDeviceAsync(
            ConnectOptions connectOptions,
            string name,
            bool suppressDialog = false)
        {
            this.StatusMessage = string.Empty;

            if (!suppressDialog)
            {
                ContentDialog dialog = new ConnectToDeviceDialog(connectOptions);
                ContentDialogResult result = await dialog.ShowAsync().AsTask<ContentDialogResult>();

                // Primary button == "Ok"
                if (result != ContentDialogResult.Primary)
                {
                    // The user decided not to connect.
                    return;
                }
            }

            if (string.IsNullOrWhiteSpace(connectOptions.Address))
            {
                connectOptions.Address = DeviceMonitor.DefaultConnectionAddress;
            }

            DeviceMonitor monitor = new DeviceMonitor(this.dispatcher);
            try
            {
                this.StatusMessage = string.Format(
                    "Connecting to the device at {0}",
                    connectOptions.Address);

                await monitor.ConnectAsync(connectOptions); 

                await this.RegisterDeviceAsync(
                    monitor, 
                    name);

                await this.RefreshCommonAppsAsync();

                this.StatusMessage = "";
            }
            catch
            {
                string addr = connectOptions.Address;
                if (connectOptions.Address == DeviceMonitor.DefaultConnectionAddress)
                {
                    addr = "the attached device";
                }

                this.StatusMessage = string.Format(
                    "Failed to connect to {0}. Is it powered on? Paired with the Windows Device Portal?",
                    addr);
            }
        }

        /// <summary>
        /// Command used to deselect all of the registered devices.
        /// </summary>
        public ICommand DeselectAllDevicesCommand
        { get; private set; }

        /// <summary>
        /// Implementation of the deselect all devices command.
        /// </summary>
        private void DeselectAllDevices()
        {
            // Suppress the refresh call that occurs in the SelectionChanged event handler.
            this.suppressRefreshCommonApps = true;

            foreach (DeviceMonitorControl monitor in this.GetCopyOfRegisteredDevices())
            {
                DeviceMonitorControlViewModel monitorViewModel = monitor.ViewModel;
                if ((monitorViewModel.Filter & this.SelectionFilter) == monitorViewModel.Filter)
                {
                    monitorViewModel.IsSelected = false;
                }
            }

            // Assigning the return value of RefreshCommonAppsAsync to a Task object to avoid 
            // warning 4014 (call is not awaited).
            Task t = this.RefreshCommonAppsAsync();

            // Restore the nominal behavior of the SelectionChanged event handler.
            this.suppressRefreshCommonApps = false;
        }

        /// <summary>
        /// Command used to forget all of the registered devices.
        /// </summary>
        public ICommand ForgetConnectionsCommand
        { get; private set; }

        /// <summary>
        /// Implementation of the forget all connections command.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        private async Task ForgetAllConnectionsAsync()
        {
            YesNoMessageDialog messageDialog = new YesNoMessageDialog(
                "Are you sure you want to remove all connected devices?");
            if (MessageDialogButtonId.Yes != await messageDialog.ShowAsync())
            {
                return;
            }

            this.suppressSave = true;

            foreach (DeviceMonitorControl monitor in this.GetCopyOfRegisteredDevices())
            {
                monitor.Disconnect();
            }

            this.suppressSave = false;

            this.ClearRegisteredDevices();

            await this.SaveConnectionsAsync();

            this.StatusMessage = "";
        }

        /// <summary>
        /// Command used to install an application on the selected devices.
        /// </summary>
        public ICommand InstallAppCommand
        { get; private set; }

        /// <summary>
        /// Implementation of the install application command.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        private async Task InstallAppAsync()
        {
            // Prompt the user for the required file.
            AppInstallFiles installFiles = new AppInstallFiles();
            ContentDialog dialog = new GetAppInstallFilesDialog(installFiles);
            await dialog.ShowAsync();
            
            if (installFiles.AppPackageFile == null)
            {
                return;
            }

            foreach (DeviceMonitorControl monitor in this.GetCopyOfRegisteredDevices())
            {
                DeviceMonitorControlViewModel viewModel = monitor.ViewModel;
                if (!viewModel.IsSelected) { continue; }

                // Assigning the return value of InstallAppAsync to a Task object to avoid 
                // warning 4014 (call is not awaited).
                Task t = monitor.InstallAppAsync(installFiles);
            }
        }

        /// <summary>
        /// Command used to launch an application on the selected devices.
        /// </summary>
        public ICommand LaunchAppCommand
        { get; private set; }

        /// <summary>
        /// Implementation of the launch application command.
        /// </summary>
        private void LaunchApp()
        {
            string appName = this.SelectedApp as string;

            foreach (DeviceMonitorControl monitor in this.GetCopyOfRegisteredDevices())
            {
                // Assigning the return value of LaunchAppAsync to a Task object to avoid 
                // warning 4014 (call is not awaited).
                Task t = monitor.LaunchAppAsync(appName);
            }
        }

        /// <summary>
        /// Command used to reboot the selected devices.
        /// </summary>
        public ICommand RebootDevicesCommand
        { get; private set; }

        /// <summary>
        /// Implementation of the reboot devices command.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        private async Task RebootDevicesAsync()
        {
            YesNoMessageDialog messageDialog = new YesNoMessageDialog(
                "Are you sure you want to reboot the selected devices?");
            if (MessageDialogButtonId.Yes != await messageDialog.ShowAsync())
            {
                return;
            }

            foreach (DeviceMonitorControl monitor in this.GetCopyOfRegisteredDevices())
            {
                // Assigning the return value of RebootAsync to a Task object to avoid 
                // warning 4014 (call is not awaited).
                Task t = monitor.RebootAsync();
            }
        }

        /// <summary>
        /// Command used to refresh the list of applications that are installed on all of on the selected devices.
        /// </summary>
        public ICommand RefreshCommonAppsCommand
        { get; private set; }

        /// <summary>
        /// Implementation of the refresh common applications command.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        private async Task RefreshCommonAppsAsync()
        {
            this.StatusMessage = "Refreshing common applications";

            List<string> commonAppNames = new List<string>();

            foreach (DeviceMonitorControl monitor in this.GetCopyOfRegisteredDevices())
            {
                DeviceMonitorControlViewModel viewModel = monitor.ViewModel;
                if (!viewModel.IsSelected) { continue; }

                List<string> deviceAppNames = await monitor.GetInstalledAppNamesAsync();

                // If this is the first device queried...
                if (commonAppNames.Count == 0)
                {
                    // ... Add all apps.
                    commonAppNames.AddRange(deviceAppNames);
                }
                else
                {
                    List<string> appNamesToRemove = new List<string>();

                    // Remove apps that do not exist on this device.
                    foreach (string name in commonAppNames)
                    {
                        if (!deviceAppNames.Contains(name))
                        {
                            appNamesToRemove.Add(name);
                        }
                    }
                    
                    foreach (string name in appNamesToRemove)
                    {
                        commonAppNames.Remove(name);
                    }
                }
            }

            this.UpdateCommonAppsCollection(commonAppNames);

            this.StatusMessage = "";
        }

        /// <summary>
        /// Command used to reconnect to devices from the previous session.
        /// </summary>
        public ICommand ReconnectToDevicesCommand
        { get; private set; }

        /// <summary>
        /// Implementation of the reconnect to devices command.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        private async Task ReconnectToDevicesAsync()
        {
            // TODO: allow the user to cancel this if it is taking too long

            this.reconnected = true;
            this.suppressSave = true;
            
            List<ConnectionInformation> connections = await this.LoadConnectionsAsync();

            foreach (ConnectionInformation connectionInfo in connections)
            {
                ConnectOptions connectOptions = new ConnectOptions(
                    connectionInfo.Address,
                    this.UserName,
                    this.Password,
                    false);

                await this.ConnectToDeviceAsync(
                    connectOptions,
                    connectionInfo.Name,
                    true); // Do not show the connect dialog on re-connect.
            }

            this.suppressSave = false;
            this.UpdateCanReconnect();
        }

        /// <summary>
        /// Command used to save mixed reality files.
        /// </summary>
        public ICommand SaveMixedRealityFilesCommand
        { get; private set; }

        /// <summary>
        /// Implementation of the save mixed reality files command.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        private async Task SaveMixedRealityFiles()
        {
            // We save images and videos in a subfolder of the pictures library.
            StorageFolder picturesLibrary = KnownFolders.PicturesLibrary; 
            StorageFolder folder = await picturesLibrary.CreateFolderAsync(
                MixedRealityFilesFolderName,
                CreationCollisionOption.OpenIfExists);

            foreach (DeviceMonitorControl monitor in this.GetCopyOfRegisteredDevices())
            {
                DeviceMonitorControlViewModel monitorViewModel = monitor.ViewModel;

                if ((monitorViewModel.Filter & DeviceFilters.HoloLens) == DeviceFilters.HoloLens)
                {
                    await monitor.GetMixedRealityFilesAsync(
                        folder,
                        this.DeleteMixedRealityFilesAfterSave);
                }
            }
        }

        /// <summary>
        /// Command used to select all of the the registered devices.
        /// </summary>
        public ICommand SelectAllDevicesCommand
        { get; private set; }

        /// <summary>
        /// Implementation of the select all devices command.
        /// </summary>
        private void SelectAllDevices()
        {
            // Suppress the refresh call that occurs in the SelectionChanged event handler.
            this.suppressRefreshCommonApps = true;

            foreach (DeviceMonitorControl monitor in this.GetCopyOfRegisteredDevices())
            {
                DeviceMonitorControlViewModel monitorViewModel = monitor.ViewModel;
                if ((monitorViewModel.Filter & this.SelectionFilter) == monitorViewModel.Filter)
                {
                    monitorViewModel.IsSelected = true;
                }
            }

            // Assigning the return value of RefreshCommonAppsAsync to a Task object to avoid 
            // warning 4014 (call is not awaited).
            Task t = this.RefreshCommonAppsAsync();

            // Restore the nominal behavior of the SelectionChanged event handler.
            this.suppressRefreshCommonApps = false;
        }

        /// <summary>
        /// Command used to send a message to the admiral account.
        /// </summary>
        public ICommand SendMessageToAdmiralCommand
        { get; private set; }

        /// <summary>
        /// Command used to display the connect context menu.
        /// </summary>
        public ICommand ShowConnectContextMenuCommand
        { get; private set; }

        /// <summary>
        /// Implementation of the show connect context menu command
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        private async Task ShowConnectContextMenuAsync(object sender)
        {
            PopupMenu contextMenu = new PopupMenu();

            // The reconnect command is only shown when CanReconnectDevices is true.
            if (this.CanReconnectDevices)
            {
                contextMenu.Commands.Add(new UICommand(
                    "Reconnect to previous session",
                    ConnectContextMenuCommandHandler,
                    ConnectContextMenuCommandIds.ReconnectPreviousSession));
            }
            contextMenu.Commands.Add(new UICommand(
                "Set credentials as new default",
                ConnectContextMenuCommandHandler,
                ConnectContextMenuCommandIds.SetDefaultCredentials));
            contextMenu.Commands.Add(new UICommand(
                "Use default credentials",
                ConnectContextMenuCommandHandler,
                ConnectContextMenuCommandIds.UseDefaultCredentials));

            await contextMenu.ShowForSelectionAsync(
                Utilities.GetFrameworkElementRect((FrameworkElement)sender));
        }

        /// <summary>
        /// Command used to shutdown the selected devices.
        /// </summary>
        public ICommand ShutdownDevicesCommand
        { get; private set; }

        /// <summary>
        /// Implementation of the shutdown devices command.
        /// </summary>
        private async Task ShutdownDevicesAsync()
        {
            YesNoMessageDialog messageDialog = new YesNoMessageDialog(
                "Are you sure you want to shutdown the selected devices?");
            if (MessageDialogButtonId.Yes != await messageDialog.ShowAsync())
            {
                return;
            }

            foreach (DeviceMonitorControl monitor in this.GetCopyOfRegisteredDevices())
            {
                // Assigning the return value of ShutdownAsync to a Task object to avoid 
                // warning 4014 (call is not awaited).
                Task t = monitor.ShutdownAsync();
            }
        }

        /// <summary>
        /// Command used to start recording a mixed reality video on the selected devices.
        /// </summary>
        public ICommand StartMixedRealityRecordingCommand
        { get; private set; }

        /// <summary>
        /// Implementation of the start mixed reality recording command.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        private void StartMixedRealityRecording()
        {
            foreach (DeviceMonitorControl monitor in this.GetCopyOfRegisteredDevices())
            {
                DeviceMonitorControlViewModel monitorViewModel = monitor.ViewModel;

                if ((monitorViewModel.Filter & DeviceFilters.HoloLens) == DeviceFilters.HoloLens)
                {
                    // Assigning the return value of StartMixedRealityRecordingAsync 
                    // to a Task object to avoid warning 4014 (call is not awaited).
                    Task t = monitor.StartMixedRealityRecordingAsync();
                }
            }
        }

        /// <summary>
        /// Command used to stop the mixed reality recording on the selected devices.
        /// </summary>
        public ICommand StopMixedRealityRecordingCommand
        { get; private set; }

        /// <summary>
        /// Implementation of the stop mixed reality recording command.
        /// </summary>
        private void StopMixedRealityRecording()
        {
            foreach (DeviceMonitorControl monitor in this.GetCopyOfRegisteredDevices())
            {
                DeviceMonitorControlViewModel monitorViewModel = monitor.ViewModel;

                if ((monitorViewModel.Filter & DeviceFilters.HoloLens) == DeviceFilters.HoloLens)
                {
                    // Assigning the return value of StopMixedRealityRecordingAsync 
                    // to a Task object to avoid warning 4014 (call is not awaited).
                    Task t = monitor.StopMixedRealityRecordingAsync();
                }                
            }
        }       
        /// <summary>
        /// Command used to uninstall an application on the selected devices.
        /// </summary>
        public ICommand UninstallAppCommand
        { get; private set; }

        /// <summary>
        /// Implementation of the uninstall application command.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        private void UninstallApp()
        {
            string appName = this.SelectedApp as string;

            foreach (DeviceMonitorControl monitor in this.GetCopyOfRegisteredDevices())
            {
                // Assigning the return value of UninstallAppAsync to a Task object to avoid 
                // warning 4014 (call is not awaited).
                Task t = monitor.UninstallAppAsync(appName);
            }
        }

        /// <summary>
        /// Command indicating that the select all/none buttons should use the all devices filter.
        /// </summary>
        public ICommand UseAllDevicesFilterCommand
        { get; private set; }

        /// <summary>
        /// Command indicating that the select all/none buttons should use the desktop filter.
        /// </summary>
        public ICommand UseDesktopFilterCommand
        { get; private set; }

        /// <summary>
        /// Command indicating that the select all/none buttons should use the HoloLens filter.
        /// </summary>
        public ICommand UseHoloLensFilterCommand
        { get; private set; }

        /// <summary>
        /// Command that wipes all camera roll content on selected devices
        /// </summary>
        public ICommand WipeCameraRollCommand
        { get; private set; }

        /// <summary>
        /// Implementation of wipe camera roll command
        /// </summary>
        private async void WipeCameraRoll()
        {
            foreach (DeviceMonitorControl monitor in this.GetCopyOfRegisteredDevices())
            {
                DeviceMonitorControlViewModel monitorViewModel = monitor.ViewModel;

                if ((monitorViewModel.Filter & DeviceFilters.HoloLens) == DeviceFilters.HoloLens)
                {
                    await monitor.WipeCameraRollAsync();
                }
            }
        }

        /// <summary>
        /// Handles connect context menu command selection.
        /// </summary>
        /// <param name="command">The command which was selected from the context menu</param>
        private void ConnectContextMenuCommandHandler(IUICommand command)
        {
            // Assigning the return value of the async commands to a Task object to avoid 
            // warning 4014 (call is not awaited).
            Task t;

            switch((ConnectContextMenuCommandIds)command.Id)
            {
                case ConnectContextMenuCommandIds.ReconnectPreviousSession:
                    t = this.ReconnectToDevicesAsync();
                    break;

                case ConnectContextMenuCommandIds.SetDefaultCredentials:
                    this.SetDefaultCredentials();
                    break;

                case ConnectContextMenuCommandIds.UseDefaultCredentials:
                    this.UseDefaultCredentials();
                    break;

                default:
                    Debug.Assert(false,
                        string.Format(
                        "Unrecognized context menu command id: {0}",
                        (int)command.Id));
                    break;
            }
        }

        /// <summary>
        /// Handles the AppInstalled event.
        /// </summary>
        /// <param name="sender">The object which sent this event.</param>
        private void DeviceMonitorControl_AppInstalled(DeviceMonitorControl sender)
        {
            // Assigning the return value of RefreshCommonAppsAsync to a Task object to avoid 
            // warning 4014 (call is not awaited).
            Task t = this.RefreshCommonAppsAsync();
        }

        /// <summary>
        /// Handles the AppUninstalled event.
        /// </summary>
        /// <param name="sender">The object which sent this event.</param>
        private void DeviceMonitorControl_AppUninstalled(DeviceMonitorControl sender)
        {
            // Assigning the return value of RefreshCommonAppsAsync to a Task object to avoid 
            // warning 4014 (call is not awaited).
            Task t = this.RefreshCommonAppsAsync();
        }

        /// <summary>
        /// Handles the Disconnected event.
        /// </summary>
        /// <param name="sender">The object which sent this event.</param>
        private void DeviceMonitorControl_Disconnected(DeviceMonitorControl sender)
        {
            // Assigning the return value of UnregisterDeviceAsync to a Task object to avoid 
            // warning 4014 (call is not awaited).
            Task t = this.UnregisterDeviceAsync(sender);
        }

        /// <summary>
        /// Handles the SelectedChanged event.
        /// </summary>
        /// <param name="sender">The object which sent this event.</param>
        private void DeviceMonitorControl_SelectedChanged(DeviceMonitorControl sender)
        {
            if (this.suppressRefreshCommonApps) { return; }

            // Assigning the return value of UnregisterDeviceAsync to a Task object to avoid 
            // warning 4014 (call is not awaited).
            Task t = this.RefreshCommonAppsAsync();
        }

        /// <summary>
        /// Handles the TagChanged event.
        /// </summary>
        /// <param name="sender">The object which sent this event.</param>
        private void DeviceMonitorControl_TagChanged(DeviceMonitorControl sender)
        {
            // Assigning the return value of SaveConnectionsAsync to a Task object to avoid 
            // warning 4014 (call is not awaited).
            Task t = this.SaveConnectionsAsync();
        }

        /// <summary>
        /// Read saved device connections from disk.
        /// </summary>
        /// <returns>List of connection information (address, name, etc).</returns>
        private async Task<List<ConnectionInformation>> LoadConnectionsAsync()
        {
            // NOTE: When we load a connection, we assume that we are to use the
            // default credentials. If our connection fails, we will skip it.

            List<ConnectionInformation> connections = null;

            try
            {
                StorageFile connectionsFile = await this.localFolder.GetFileAsync(
                    MainPage.ConnectionsFileName);

                using (Stream stream = await connectionsFile.OpenStreamForReadAsync())
                {
                    XmlSerializer serializer = new XmlSerializer(typeof(List<ConnectionInformation>));
                    connections = serializer.Deserialize(stream) as List<ConnectionInformation>;
                }
            }
            catch 
            {
                connections = new List<ConnectionInformation>();
            }

            return connections;
        }

        /// <summary>
        /// Registers the DeviceMonitor with the application.
        /// </summary>
        /// <param name="monitor">Device to register.</param>
        /// <param name="name">Descriptive name of the device.</param>
        /// <returns>Task object used for tracking method completion.</returns>
        private async Task RegisterDeviceAsync(
            DeviceMonitor monitor,
            string name)
        {
            DeviceMonitorControl deviceMonitorControl = new DeviceMonitorControl(monitor);
            deviceMonitorControl.AppInstalled += DeviceMonitorControl_AppInstalled;
            deviceMonitorControl.AppUninstalled += DeviceMonitorControl_AppUninstalled;
            deviceMonitorControl.DeviceDisconnected += DeviceMonitorControl_Disconnected;
            deviceMonitorControl.SelectedChanged += DeviceMonitorControl_SelectedChanged;
            deviceMonitorControl.TagChanged += DeviceMonitorControl_TagChanged;

            DeviceMonitorControlViewModel viewModel = deviceMonitorControl.DataContext as DeviceMonitorControlViewModel;
            viewModel.Name = name;

            this.RegisteredDevices.Add(deviceMonitorControl);
            if (this.RegisteredDevices.Count > 0)
            {
                this.HaveRegisteredDevices = true;
            }

            await this.SaveConnectionsAsync();
        }

        /// <summary>
        /// Saves device connections to disk.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        private async Task SaveConnectionsAsync()
        {
            if (this.suppressSave)
            {
                return;
            }

            List<ConnectionInformation> connections = new List<ConnectionInformation>();

            foreach (DeviceMonitorControl monitor in this.GetCopyOfRegisteredDevices())
            {
                DeviceMonitorControlViewModel monitorViewModel = monitor.ViewModel;

                connections.Add(new ConnectionInformation(
                    monitorViewModel.Address,
                    monitorViewModel.Name));
            }

            try 
            {
                StorageFile connectionsFile = await this.localFolder.CreateFileAsync(
                    MainPage.ConnectionsFileName,
                    CreationCollisionOption.ReplaceExisting);

                using (Stream stream = await connectionsFile.OpenStreamForWriteAsync())
                {
                    XmlSerializer serializer = new XmlSerializer(typeof(List<ConnectionInformation>));
                    serializer.Serialize(stream, connections);
                    await stream.FlushAsync();
                }
            }
            catch
            {
                this.StatusMessage = "Failed to save connection information";
            }
        }

        /// <summary>
        /// Stores a set of credentials to be used as the default when connecting to a device.
        /// </summary>
        private void SetDefaultCredentials()
        {
            this.appSettings.Values[DefaultUserNameKey] = this.UserName;
            this.appSettings.Values[DefaultPasswordKey] = this.Password;
        }

        /// <summary>
        /// Removes the specified device from monitoring.
        /// </summary>
        /// <param name="monitorControl">The DeviceMonitorControl tracking the device to be removed.</param>
        /// <returns>Task object used for tracking method completion.</returns>
        private async Task UnregisterDeviceAsync(DeviceMonitorControl monitorControl)
        {
            this.RegisteredDevices.Remove(monitorControl);
            if (this.RegisteredDevices.Count == 0)
            {
                this.HaveRegisteredDevices = false;
            }

            await this.SaveConnectionsAsync();
        }

        /// <summary>
        /// Updates the current credentials uses when connecting to a device to the previously stored default.
        /// </summary>
        private void UseDefaultCredentials()
        {
            this.UserName = this.appSettings.Values[DefaultUserNameKey] as string;
            this.Password = this.appSettings.Values[DefaultPasswordKey] as string;
        }
    }
}
