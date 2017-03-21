// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using Windows.UI.Xaml.Controls;
    
namespace HoloLensCommander
{
    public sealed partial class DeviceInformationDialog : ContentDialog
    {
        public DeviceInformationDialog(DeviceMonitor monitor)
        {
            this.DataContext = new DeviceInformationDialogViewModel(monitor);
            this.InitializeComponent();
        }
    }
}
