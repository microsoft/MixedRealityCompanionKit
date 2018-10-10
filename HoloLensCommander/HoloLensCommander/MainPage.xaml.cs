// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using Windows.UI.Xaml.Controls;

namespace HoloLensCommander
{
    /// <summary>
    /// The main window of the application.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        /// <summary>
        /// The name of the stored connections file.
        /// </summary>
        internal static readonly string ConnectionsFileName = "HoloLensCommanderConnections.xml";

        public MainWindowViewModelReactive ReactiveViewModel { get; } = new MainWindowViewModelReactive();
        /// <summary>
        /// Initializes a new instance of the <see cref="MainPage" /> class.
        /// </summary>
        public MainPage()
        {
            this.DataContext = new MainWindowViewModel(this.Dispatcher);
            this.InitializeComponent();
        }
    }
}
