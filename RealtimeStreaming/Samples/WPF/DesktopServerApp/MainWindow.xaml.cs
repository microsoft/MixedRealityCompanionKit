// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Text.RegularExpressions;
using System.Windows;
using System.Windows.Input;

namespace DesktopServerApp
{
    public partial class MainWindow : Window, INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        public SampleManager Manager
        {
            get => _Manager;
            set => this.SetProperty(ref this._Manager, value);
        }
        private SampleManager _Manager;

        public MainWindow()
        {
            InitializeComponent();

            this.Loaded += this.OnLoaded;
        }

        public void OnLoaded(object sender, RoutedEventArgs e)
        {
            this.DataContext = this;

            this._Manager = new SampleManager(this.Dispatcher);
        }

        public void OnStartButtonClick(object sender, RoutedEventArgs e)
        {
            _Manager.StartServer();
        }

        public void OnStopButtonClick(object sender, RoutedEventArgs e)
        {
            _Manager.StopServer();
        }

        public void OnCloseStreamButtonClick(object sender, RoutedEventArgs e)
        {
            _Manager.StopStreaming();
        }

        public void FirePropertyChanged([CallerMemberName] string propertyName = null)
        {
            this.PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        bool SetProperty<T>(ref T storage, T value, [CallerMemberName] string propertyName = null)
        {
            if (object.Equals(storage, value)) return false;

            storage = value;
            this.FirePropertyChanged(propertyName);
            return true;
        }

        private void NumberValidationTextBox(object sender, TextCompositionEventArgs e)
        {
            Regex regex = new Regex("[^0-9]+");
            e.Handled = regex.IsMatch(e.Text);
        }
    }
}