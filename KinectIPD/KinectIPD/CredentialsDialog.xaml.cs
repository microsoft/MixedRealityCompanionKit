// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Net;
using System.Windows;
using System.Windows.Controls;

namespace KinectIPD
{
    /// <summary>
    /// Interaction logic for Credentials.xaml
    /// </summary>
    public partial class CredentialsDialog : Window
    {
        public string DeviceName = "";
        public string IPAddress = "";
        public string UserName = "";
        public string Password = "";
        public bool AutoIP = false;

        public CredentialsDialog()
        {
            InitializeComponent();            
        }
        
        private void OkClick(object sender, RoutedEventArgs e)
        {
            DeviceName = DeviceNameBox.Text;
            IPAddress = AutoIP ? "http://127.0.0.1:10080" : IPBox.Text;
            UserName = UsernameBox.Text;
            Password = PWBox.Password;

            this.DialogResult = true;
        }

        private void Update()
        {
            AutoIP = (bool)(AutoIPCheckBox).IsChecked;
            IPBox.IsEnabled = !AutoIP;
            if (AutoIP)
            {                
                IPBox.Text = "http://127.0.0.1:10080";
            }
        }

        private void CheckBox_Checked(object sender, RoutedEventArgs e)
        {
            Update();
        }
    }
}
