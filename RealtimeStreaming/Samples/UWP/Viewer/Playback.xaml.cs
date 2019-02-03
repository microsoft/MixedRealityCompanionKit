// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections.Generic;
using Windows.Foundation.Collections;
using Windows.Media;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using MixedRemoteViewCompositor.Network;

namespace Viewer
{
    public sealed partial class Playback : Page
    {
        private MediaExtensionManager mediaExtensionManager = null;

        private Connector connector = null;
        private Connection connection = null;

        public Playback()
        {
            this.InitializeComponent();
        }

        private async void bnConnect_Click(object sender, RoutedEventArgs e)
        {
            ushort port = 0;
            if (UInt16.TryParse(this.txPort.Text, out port))
            {
                if (string.IsNullOrEmpty(this.txAddress.Text))
                {
                    this.txAddress.Text = this.txAddress.PlaceholderText;
                }

                this.connector = new Connector(this.txAddress.Text, port);
                if (this.connector != null)
                {
                    this.connection = await this.connector.ConnectAsync();
                    if(this.connection != null)
                    {
                        this.bnConnect.IsEnabled = false;
                        this.bnClose.IsEnabled = true;
                        this.bnStart.IsEnabled = true;
                        this.bnStop.IsEnabled = false;

                        this.connection.Disconnected += Connection_Disconnected;

                        var propertySet = new PropertySet();
                        var propertySetDictionary = propertySet as IDictionary<string, object>;
                        propertySet["Connection"] = this.connection;

                        RegisterSchemeHandler(propertySet);
                    }
                }
            }
        }

        private void bnClose_Click(object sender, RoutedEventArgs e)
        {
            CloseConnection();
        }

        private void bnStart_Click(object sender, RoutedEventArgs e)
        {
            StartPlayback();
        }

        private void bnStop_Click(object sender, RoutedEventArgs e)
        {
            StopPlayback();
        }

        private void Connection_Disconnected(Connection sender)
        {
            CloseConnection();
        }

        private void CloseConnection()
        {
            StopPlayback();

            this.bnStart.IsEnabled = false;

            this.bnConnect.IsEnabled = true;
            this.bnClose.IsEnabled = false;

            if (this.connection != null)
            {
                this.connection.Disconnected -= Connection_Disconnected;
                this.connection.Uninitialize();
                this.connection = null;
            }

            if (this.connector != null)
            {
                this.connector.Uninitialize();
                this.connector = null;
            }
        }

        private void StartPlayback()
        {
            this.videoPlayer.Source = new Uri(string.Format("mrvc://{0}:{1}", this.txAddress.Text, this.txPort.Text));

            this.bnStart.IsEnabled = false;
            this.bnStop.IsEnabled = true;
        }

        private void StopPlayback()
        {
            this.videoPlayer.Stop();
            this.videoPlayer.Source = null;

            this.bnStart.IsEnabled = true;
            this.bnStop.IsEnabled = false;
        }

        public void RegisterSchemeHandler(PropertySet propertySet)
        {
            if (this.mediaExtensionManager == null)
            {
                this.mediaExtensionManager = new MediaExtensionManager();
            }

           this.mediaExtensionManager.RegisterSchemeHandler("MixedRemoteViewCompositor.Media.MrvcSchemeHandler", "mrvc:", propertySet);
        }
    }
}
