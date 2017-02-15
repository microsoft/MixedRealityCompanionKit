// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using Windows.UI.Xaml.Controls;
    
namespace HoloLensCommander
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class HoloLensInformationDialog : ContentDialog
    {
        public HoloLensInformationDialog(HoloLensMonitor monitor)
        {
            this.DataContext = new HoloLensInformationDialogViewModel(monitor);
            this.InitializeComponent();
        }
    }
}
