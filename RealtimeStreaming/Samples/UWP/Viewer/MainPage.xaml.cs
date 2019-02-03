// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

namespace Viewer
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();
        }

        private void bnCapture_Click(object sender, RoutedEventArgs e)
        {
            ScenarioFrame.Navigate(typeof(Capture));
        }

        private void bnPlayback_Click(object sender, RoutedEventArgs e)
        {
            ScenarioFrame.Navigate(typeof(Playback));
        }
    }
}
