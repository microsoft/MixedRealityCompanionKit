// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using UnityEngine;
using UnityEngine.XR.WSA;
using UnityEngine.XR.WSA.Sharing;
using UnityEngine.XR.WSA.Persistence;
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

        const string SavedAnchorKey = "SavedAnchorName";

        WorldAnchorStore anchorStore = null;

        /// <summary>
        /// Sometimes we'll see a really small anchor blob get generated.
        /// These tend to not work, so we have a minimum trustable size.
        /// </summary>
        private const uint minTrustworthySerializedAnchorDataSize = 500000;

        List<string> localIPs;
        string localIPv4 = string.Empty;
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
        /// The object to attach the anchor to when created or imported.
        /// </summary>
        public GameObject objectToAnchor;

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

                    if (aName.ToString().Split('.').Length == 4)
                    {
                        localIPv4 = aName.ToString().Trim();
                        Debug.Log("Setting local IP to " + localIPv4);
                    }
                }
            }

            WorldAnchorStore.GetAsync(AnchorStoreReady);
#endif

            networkTransmitter = AnchorNetworkTransmitter.Instance;
            networkTransmitter.dataReadyEvent += NetworkTransmitter_dataReadyEvent;
        }

        void AnchorStoreReady(WorldAnchorStore store)
        {
            anchorStore = store;
        }

        private void Update()
        {
            if (anchorStore == null)
            {
                return;
            }

#if WINDOWS_UWP
            // Check if we should create an anchor.
            if (!anchorOwner.HasValue &&
                AnchorNetworkTransmitter.Instance != null &&
                (AnchorNetworkTransmitter.Instance.AnchorOwnerIP != string.Empty
                || AnchorNetworkTransmitter.Instance.ForceCreateAnchor)
            )
            {
                if (localIPs.Contains(AnchorNetworkTransmitter.Instance.AnchorOwnerIP)
                    || AnchorNetworkTransmitter.Instance.ForceCreateAnchor)
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

        public void ResetAnchor()
        {
#if WINDOWS_UWP
            if (anchorStore != null)
            {
                anchorStore.Clear();
            }

            if (anchorOwner.HasValue && anchorOwner.Value == true)
            {
                MakeNewAnchor();
            }
#endif
        }

        /// <summary>
        /// If we are supposed to create the anchor for export, this is the function to call.
        /// </summary>
        public void CreateAnchor()
        {
            if (PlayerPrefs.HasKey(SavedAnchorKey) && AttachToCachedAnchor(PlayerPrefs.GetString(SavedAnchorKey)))
            {
                AnchorName = PlayerPrefs.GetString(SavedAnchorKey);
                Debug.Log("found " + AnchorName + " again");
            }
            else
            {
                Debug.Log("Could not find cached anchor.");
                AnchorName = Guid.NewGuid().ToString();
            }

            exportingAnchorBytes.Clear();
            AnchorNetworkTransmitter.Instance.SetData(null);

            WorldAnchor worldAnchor = objectToAnchor.GetComponent<WorldAnchor>();
            if (worldAnchor == null)
            {
                Debug.Log("Adding a new anchor.");
                worldAnchor = objectToAnchor.AddComponent<WorldAnchor>();
            }

            if (!AnchorEstablished)
            {
                if (worldAnchor.isLocated)
                {
                    Debug.Log("World anchor already located.");

                    Debug.Log("Saving Anchor.");
                    AnchorEstablished = true;
                    SaveAnchor(worldAnchor);
                }
                else
                {
                    worldAnchor.OnTrackingChanged += Anchor_OnTrackingChanged;
                    Anchor_OnTrackingChanged(worldAnchor, worldAnchor.isLocated);
                }
            }

            Debug.Log("exporting " + AnchorName);
            WorldAnchorTransferBatch watb = new WorldAnchorTransferBatch();
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
        /// Attempts to attach to  an anchor by anchorName in the local store..
        /// </summary>
        /// <returns>True if it attached, false if it could not attach</returns>
        private bool AttachToCachedAnchor(string CachedAnchorName)
        {
            if (string.IsNullOrEmpty(CachedAnchorName))
            {
                Debug.Log("Ignoring empty name");
                return false;
            }
#if UNITY_WSA
            if (anchorStore == null)
            {
                Debug.LogWarning("Could not find anchor store.");
                return false;
            }

            Debug.Log("Looking for " + CachedAnchorName);
            string[] ids = anchorStore.GetAllIds();
            for (int index = 0; index < ids.Length; index++)
            {
                if (ids[index] == CachedAnchorName)
                {
                    Debug.Log("Using what we have");
                    WorldAnchor anchor = anchorStore.Load(ids[index], objectToAnchor);

                    if (anchor == null)
                    {
                        // We did not find a valid anchor.
                        return false;
                    }

                    AnchorEstablished = true;
                    return true;
                }
                else
                {
                    Debug.Log(ids[index]);
                }
            }
#endif
            // Didn't find the anchor.
            return false;
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

                if (AttachToCachedAnchor(first))
                {
                    Debug.Log("Imported and attached to an anchor that we already cached.");
                    return;
                }

                WorldAnchor existingAnchor = objectToAnchor.GetComponent<WorldAnchor>();
                if (existingAnchor != null)
                {
                    DestroyImmediate(existingAnchor);
                }

                WorldAnchor anchor = wat.LockObject(first, objectToAnchor);
                anchor.OnTrackingChanged += Anchor_OnTrackingChanged;
                Anchor_OnTrackingChanged(anchor, anchor.isLocated);

                ImportInProgress = false;
            }
            else
            {
                // if we failed, we can simply try again.
                gotOne = true;
                Debug.Log("Import fail");
            }
        }

        private void Anchor_OnTrackingChanged(WorldAnchor self, bool located)
        {
#if UNITY_WSA
            if (located)
            {
                Debug.Log("Saving Anchor.");
                AnchorEstablished = true;
                SaveAnchor(self);
                self.OnTrackingChanged -= Anchor_OnTrackingChanged;
            }
#endif
        }

        private void SaveAnchor(WorldAnchor wa)
        {
            anchorStore.Save(AnchorName, wa);

            PlayerPrefs.SetString(SavedAnchorKey, AnchorName);
            PlayerPrefs.Save();
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
                // Let server know our anchor is ready.
                SharingManager.Instance.SendAnchorName(AnchorName, localIPv4);
                InvokeRepeating("SendAnchorName", 5, 5);
                AnchorEstablished = true;
            }
            else
            {
                Debug.Log("Create anchor failed " + status + " " + exportingAnchorBytes.Count);
                exportingAnchorBytes.Clear();
                DestroyImmediate(objectToAnchor.GetComponent<WorldAnchor>());
                CreateAnchor();
            }
        }

        public void AnchorFoundRemotely()
        {
            if (UnityEngine.XR.WSA.HolographicSettings.IsDisplayOpaque)
            {
                return;
            }
#if UNITY_WSA
            Debug.Log("Setting saved anchor to " + AnchorName);
            SaveAnchor(objectToAnchor.GetComponent<WorldAnchor>());
#endif
        }

        public void MakeNewAnchor()
        {
            Debug.Log("Making a new anchor.");

            if (PlayerPrefs.HasKey(SavedAnchorKey))
            {
                Debug.Log("Deleting existing anchor key.");
                PlayerPrefs.DeleteKey(SavedAnchorKey);
            }

            WorldAnchor currentAnchor = objectToAnchor.GetComponent<WorldAnchor>();
            if (currentAnchor != null)
            {
                DestroyImmediate(currentAnchor);
            }

            AnchorName = "";
            CreateAnchor();
        }

        // Periodically tell the server our anchor name.
        // Only sending the name once may cause our server to miss the name if the server was started after the anchor was created.
        //NOTE: You may not need to do this if you are using UNET's HLAPI syncvars in your networking stack.
        private void SendAnchorName()
        {
            if (AnchorEstablished)
            {
                SharingManager.Instance.SendAnchorName(AnchorName, localIPv4);
            }
        }

        private void OnDestroy()
        {
            CancelInvoke("SendAnchorName");
        }

    }
}
