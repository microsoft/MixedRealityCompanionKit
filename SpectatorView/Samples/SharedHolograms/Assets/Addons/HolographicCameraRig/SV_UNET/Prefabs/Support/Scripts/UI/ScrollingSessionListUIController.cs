// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.Linq;
using HoloToolkit.Unity;

namespace SpectatorView
{
    public class ScrollingSessionListUIController : SingleInstance<ScrollingSessionListUIController>
    {
        public bool CanEstablishAnchor()
        {
#if UNITY_EDITOR
            return false;
#endif

            //TODO: differentiate between immersive devices and HoloLens devices.
            return true;
        }

        NetworkDiscoveryWithAnchors networkDiscovery;
        //CurrentSessionManager 
        Dictionary<string, NetworkDiscoveryWithAnchors.SessionInfo> sessionList;
        int SessionIndex = 0;

        public SessionListButton[] SessionControls;
        public NetworkDiscoveryWithAnchors.SessionInfo SelectedSession { get; private set; }

        // Use this for initialization
        void Start()
        {
#if UNITY_EDITOR
            gameObject.GetComponent<SimpleTagalong>().TagalongDistance = 1;
#endif

            networkDiscovery = NetworkDiscoveryWithAnchors.Instance;
            networkDiscovery.SessionListChanged += NetworkDiscovery_SessionListChanged;
            networkDiscovery.ConnectionStatusChanged += NetworkDiscovery_ConnectionStatusChanged;
            ScrollSessions(0);
        }

        private void NetworkDiscovery_ConnectionStatusChanged(object sender, EventArgs e)
        {
            SetChildren(networkDiscovery.running && !networkDiscovery.isServer);
        }

        private void NetworkDiscovery_SessionListChanged(object sender, EventArgs e)
        {
            sessionList = networkDiscovery.remoteSessions;
            SessionIndex = Mathf.Min(SessionIndex, sessionList.Count);

            ScrollSessions(0);
        }

        public void Show()
        {
            gameObject.transform.position = Camera.main.transform.position + Camera.main.transform.forward * 2.0f;

            SetChildren(true);
            networkDiscovery.SessionListChanged += NetworkDiscovery_SessionListChanged;
            networkDiscovery.ConnectionStatusChanged += NetworkDiscovery_ConnectionStatusChanged;
            ScrollSessions(0);
        }

        void SetChildren(bool Enabled)
        {
            foreach (Renderer mr in GetComponentsInChildren<Renderer>())
            {
                mr.enabled = Enabled;
            }

            foreach (BoxCollider bc in GetComponentsInChildren<BoxCollider>())
            {
                bc.enabled = Enabled;
            }
        }


        public void ScrollSessions(int Direction)
        {
            int sessionCount = sessionList == null ? 0 : sessionList.Count;
            SessionIndex = Mathf.Clamp(SessionIndex + Direction, 0, Mathf.Max(0, sessionCount - SessionControls.Length));

            for (int index = 0; index < SessionControls.Length; index++)
            {
                if (SessionIndex + index < sessionCount)
                {
                    SessionControls[index].gameObject.SetActive(true);
                    NetworkDiscoveryWithAnchors.SessionInfo sessionInfo = sessionList.Values.ElementAt(SessionIndex + index);
                    SessionControls[index].SetSessionInfo(sessionInfo);
                }
                else
                {
                    SessionControls[index].gameObject.SetActive(false);
                }
            }
        }

        public void SetSelectedSession(NetworkDiscoveryWithAnchors.SessionInfo sessionInfo)
        {
            SelectedSession = sessionInfo;
            ScrollSessions(0);
        }

        public void JoinSelectedSession()
        {
            if (SelectedSession != null && networkDiscovery.running)
            {
                networkDiscovery.JoinSession(SelectedSession);
            }
        }
    }
}
