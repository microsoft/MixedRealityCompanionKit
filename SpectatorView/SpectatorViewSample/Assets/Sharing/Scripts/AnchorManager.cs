// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using UnityEngine;
using UnityEngine.XR.WSA;
using UnityEngine.XR.WSA.Sharing;
#if WINDOWS_UWP
using Windows.Networking;
using Windows.Networking.Connectivity;
#endif

namespace SimpleSharing
{
    public class AnchorManager : SimpleSharing.Singleton<AnchorManager>
    {
        /// <summary>
        /// Keeps track of the name of the world anchor to use.
        /// </summary>
        string AnchorName = "";

        /// <summary>
        /// Sometimes we'll see a really small anchor blob get generated.
        /// These tend to not work, so we have a minimum trustable size.
        /// </summary>
        private const uint minTrustworthySerializedAnchorDataSize = 500000;

        List<string> localIPs;
#pragma warning disable 0414
        bool? anchorOwner = null;
#pragma warning restore 0414

        /// <summary>
        /// List of bytes that represent the anchor data to export.
        /// </summary>
        private List<byte> exportingAnchorBytes = new List<byte>();

        /// <summary>
        /// The UNetNetworkTransmitter in the scene which can send an anchor to another device.
        /// </summary>
        private AnchorNetworkTransmitter networkTransmitter;

        /// <summary>
        /// The anchorData to import.
        /// </summary>
        private byte[] anchorData = null;

        /// <summary>
        /// Tracks if we have updated data to import.
        /// </summary>
#pragma warning disable 0414
        private bool gotOne = false;
#pragma warning restore 0414

        /// <summary>
        /// Tracks if we have a shared anchor established
        /// </summary>
        public bool AnchorEstablished { get; set; }

        /// <summary>
        /// Tracks if an import is in flight.
        /// </summary>
        public bool ImportInProgress { get; private set; }

        /// <summary>
        /// Tracks if a download is in flight.
        /// </summary>
        public bool DownloadingAnchor { get; private set; }



        private void Start()
        {
#if WINDOWS_UWP
            localIPs = new List<string>();

            // Get the ip of the Hololens to see if this device should create and share the anchor.
            IReadOnlyList<HostName> hosts = NetworkInformation.GetHostNames();
            foreach (HostName aName in hosts)
            {
                if (aName.Type == HostNameType.Ipv4)
                {
                    localIPs.Add(aName.ToString().Trim());
                }
            }
#endif

            WorldAnchor anchor = GetComponent<WorldAnchor>();
            if (anchor == null)
            {
                gameObject.AddComponent<WorldAnchor>();
            }

            networkTransmitter = AnchorNetworkTransmitter.Instance;
            networkTransmitter.dataReadyEvent += NetworkTransmitter_dataReadyEvent;
        }

        private void Update()
        {
#if WINDOWS_UWP
            // Check if we should create an anchor.
            if (!anchorOwner.HasValue && 
                AnchorNetworkTransmitter.Instance != null)
            {
                if (localIPs.Contains(AnchorNetworkTransmitter.Instance.AnchorOwnerIP))
                {
                    anchorOwner = true;
                    CreateAnchor();
                }
                else
                {
                    anchorOwner = false;
                    WaitForAnchor();
                }
            }

            if (gotOne)
            {
                Debug.Log("importing");
                gotOne = false;
                ImportInProgress = true;
                WorldAnchorTransferBatch.ImportAsync(anchorData, ImportComplete);
            }
#endif
        }

        /// <summary>
        /// If we are supposed to create the anchor for export, this is the function to call.
        /// </summary>
        public void CreateAnchor()
        {
            exportingAnchorBytes.Clear();
            AnchorNetworkTransmitter.Instance.SetData(null);
            AnchorName = Guid.NewGuid().ToString();

            WorldAnchorTransferBatch watb = new WorldAnchorTransferBatch();
            WorldAnchor worldAnchor = gameObject.GetComponent<WorldAnchor>();
            if (worldAnchor == null)
            {
                worldAnchor = gameObject.AddComponent<WorldAnchor>();
            }

            Debug.Log("exporting " + AnchorName);
            watb.AddWorldAnchor(AnchorName, worldAnchor);
            WorldAnchorTransferBatch.ExportAsync(watb, WriteBuffer, ExportComplete);
        }

        /// <summary>
        /// If we don't have the anchor already, call this to download the anchor.
        /// </summary>
        public void WaitForAnchor()
        {
            DownloadingAnchor = networkTransmitter.RequestAndGetData();
            if (!DownloadingAnchor)
            {
                Invoke("WaitForAnchor", 0.5f);
            }
        }

        /// <summary>
        /// Called when anchor data is ready.
        /// </summary>
        /// <param name="data">The data blob to import.</param>
        private void NetworkTransmitter_dataReadyEvent(byte[] data)
        {
            Debug.Log("Anchor data arrived.");
            anchorData = data;
            Debug.Log(data.Length);
            DownloadingAnchor = false;
            gotOne = true;
        }

        /// <summary>
        /// Called when a remote anchor has been deserialized
        /// </summary>
        /// <param name="status">Tracks if the import worked</param>
        /// <param name="wat">The WorldAnchorTransferBatch that has the anchor information.</param>
        private void ImportComplete(SerializationCompletionReason status, WorldAnchorTransferBatch wat)
        {
            if (status == SerializationCompletionReason.Succeeded && wat.GetAllIds().Length > 0)
            {
                Debug.Log("Import complete");

                string first = wat.GetAllIds()[0];
                Debug.Log("Anchor name: " + first);

                WorldAnchor existingAnchor = gameObject.GetComponent<WorldAnchor>();
                if (existingAnchor != null)
                {
                    DestroyImmediate(existingAnchor);
                }

                wat.LockObject(first, gameObject);

                ImportInProgress = false;
            }
            else
            {
                // if we failed, we can simply try again.
                gotOne = true;
                Debug.Log("Import fail");
            }
        }

        /// <summary>
        /// Called as anchor data becomes available to export
        /// </summary>
        /// <param name="data">The next chunk of data.</param>
        private void WriteBuffer(byte[] data)
        {
            exportingAnchorBytes.AddRange(data);
        }

        /// <summary>
        /// Called when serializing an anchor is complete.
        /// </summary>
        /// <param name="status">If the serialization succeeded.</param>
        private void ExportComplete(SerializationCompletionReason status)
        {
            if (status == SerializationCompletionReason.Succeeded && exportingAnchorBytes.Count > minTrustworthySerializedAnchorDataSize)
            {
                anchorData = exportingAnchorBytes.ToArray();
                AnchorNetworkTransmitter.Instance.SetData(anchorData);
                Debug.Log("Anchor ready " + exportingAnchorBytes.Count);
                AnchorNetworkTransmitter.Instance.ConfigureAsServer();
                AnchorEstablished = true;
            }
            else
            {
                Debug.Log("Create anchor failed " + status + " " + exportingAnchorBytes.Count);
                exportingAnchorBytes.Clear();
                DestroyImmediate(gameObject.GetComponent<WorldAnchor>());
                CreateAnchor();
            }
        }

    }
}
