// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using System.Collections.Generic;
using UnityEngine.Networking;
using HoloToolkit.Unity;
#if UNITY_2017_2_OR_NEWER
using UnityEngine.XR.WSA;
using UnityEngine.XR.WSA.Sharing;
using UnityEngine.XR.WSA.Persistence;
#else
using UnityEngine.VR.WSA;
using UnityEngine.VR.WSA.Sharing;
using UnityEngine.VR.WSA.Persistence;
#endif
using System;

namespace SpectatorView
{
    /// <summary>
    /// Creates, exports, and imports anchors as required.
    /// </summary>
    public class UNetAnchorManager : NetworkBehaviour
    {
        const string SavedAnchorKey = "SavedAnchorName";

        /// <summary>
        ///  Since we aren't a MonoBehavior we can't just use the singleton class
        ///  so we'll reroll it as a one off here.
        /// </summary>
        private static UNetAnchorManager _Instance;

        public static UNetAnchorManager Instance
        {
            get
            {
                if (_Instance == null)
                {
                    _Instance = FindObjectOfType<UNetAnchorManager>();
                }
                return _Instance;
            }
        }

        /// <summary>
        /// Sometimes we'll see a really small anchor blob get generated.
        /// These tend to not work, so we have a minimum trustable size.
        /// </summary>
        private const uint minTrustworthySerializedAnchorDataSize = 500000;

        /// <summary>
        /// Keeps track of the name of the world anchor to use.
        /// </summary>
        [SyncVar]
        public string AnchorName = "";

        [SyncVar(hook="CreateAnchorHook")]
        public string AnchorOwnerIP = "";

        private void CreateAnchorHook(string value)
        {
            AnchorOwnerIP = value;
            if (value != string.Empty)
            {
                Debug.Log("Setting server IP: " + value);
                GenericNetworkTransmitter.Instance.SetServerIP(value);
                Debug.Log("Creating Anchor");
                if (PlayerController.Instance != null)
                {
                    PlayerController.Instance.CreateAnchor(value);
                }
                else
                {
                    Debug.LogError("Player controller is null, cannot create anchor.");
                }
            }
        }

        public void FindAnchorOwner()
        {
            createdAnchor = false;

#if !UNITY_EDITOR
            AnchorEstablished = false;
#endif

            if (PlayerController.Instance != null)
            {
                PlayerController.Instance.CmdFindNewAnchorOwner();
            }
        }

        /// <summary>
        /// List of bytes that represent the anchor data to export.
        /// </summary>
        private List<byte> exportingAnchorBytes = new List<byte>();

        /// <summary>
        /// The UNet network manager in the scene.
        /// </summary>
        private NetworkManager networkManager;

        /// <summary>
        /// The UNetNetworkTransmitter in the scene which can send an anchor to another device.
        /// </summary>
        private GenericNetworkTransmitter networkTransmitter;

        /// <summary>
        /// Keeps track of if we created the anchor.
        /// </summary>
#pragma warning disable 0414
        private bool createdAnchor = false;
#pragma warning restore 0414

        /// <summary>
        /// The object to attach the anchor to when created or imported.
        /// </summary>
        private GameObject objectToAnchor;

        /// <summary>
        /// Previous anchor name.
        /// </summary>
#pragma warning disable 0414
        private string oldAnchorName = "";
#pragma warning restore 0414

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
        /// Keeps track of the name of the anchor we are exporting.
        /// </summary>
        private string exportingAnchorName;

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

        /// <summary>
        /// Ensures that the scene has what we need to continue.
        /// </summary>
        /// <returns>True if we can proceed, false otherwise.</returns>
        private bool CheckConfiguration()
        {
            networkTransmitter = GenericNetworkTransmitter.Instance;
            if (networkTransmitter == null)
            {
                Debug.Log("No UNetNetworkTransmitter found in scene");
                return false;
            }

            networkManager = NetworkManager.singleton;
            if (networkManager == null)
            {
                Debug.Log("No NetworkManager found in scene");
                return false;
            }

            if (UNetAnchorManager.Instance == null)
            {
                Debug.Log("No UNetAnchorManager found in scene");
                return false;
            }
            else
            {
                objectToAnchor = UNetAnchorManager.Instance.gameObject;
            }

            return true;
        }

        private void Start()
        {
            if (!CheckConfiguration())
            {
                Debug.Log("Missing required component for UNetAnchorManager");
                Destroy(this);
                return;
            }

#if UNITY_EDITOR
            AnchorEstablished = true;
#else
            networkTransmitter.dataReadyEvent += NetworkTransmitter_dataReadyEvent;
#endif
        }

        private void Update()
        {
#if WINDOWS_UWP
            if (gotOne)
            {
                Debug.Log("importing");
                gotOne = false;
                ImportInProgress = true;
                WorldAnchorTransferBatch.ImportAsync(anchorData, ImportComplete);
            }

            if (oldAnchorName != AnchorName && 
                !createdAnchor && 
                !String.IsNullOrEmpty(GenericNetworkTransmitter.Instance.serverIP))
            {
                Debug.LogFormat("New anchor name {0} => {1}", oldAnchorName, AnchorName);
                Debug.Log("Server IP: " + GenericNetworkTransmitter.Instance.serverIP);

                oldAnchorName = AnchorName;
                if (string.IsNullOrEmpty(AnchorName))
                {
                    Debug.Log("anchor is empty");
                    AnchorEstablished = false;
                }
                else if (PlayerPrefs.HasKey(SavedAnchorKey) && AttachToCachedAnchor(PlayerPrefs.GetString(SavedAnchorKey)))
                {
                    Debug.Log("__found " + AnchorName + " again");
                }
                else if (!AttachToCachedAnchor(AnchorName))
                {
                    Debug.Log("Need to import anchor.");
                    WaitForAnchor();
                }
            }
#else
            return;
#endif
        }

        /// <summary>
        /// If we are supposed to create the anchor for export, this is the function to call.
        /// </summary>
        public void CreateAnchor()
        {
#if UNITY_EDITOR
            Debug.LogError("Anchors cannot be created from the Unity editor.");
#endif
    
            exportingAnchorBytes.Clear();
            GenericNetworkTransmitter.Instance.SetData(null);
            objectToAnchor = UNetAnchorManager.Instance.gameObject;

            WorldAnchor worldAnchor = objectToAnchor.GetComponent<WorldAnchor>();
            if (worldAnchor == null)
            {
                worldAnchor = objectToAnchor.AddComponent<WorldAnchor>();
            }

            Debug.Log("Checking for saved anchor: " + AnchorName);
            if (PlayerPrefs.HasKey(SavedAnchorKey) && AttachToCachedAnchor(PlayerPrefs.GetString(SavedAnchorKey)))
            {
                exportingAnchorName = PlayerPrefs.GetString(SavedAnchorKey);
                Debug.Log("found " + AnchorName + " again");
            }
            else if (PlayerPrefs.HasKey(AnchorName) && AttachToCachedAnchor(AnchorName))
            {
                exportingAnchorName = AnchorName;
                Debug.Log("_found " + AnchorName + " again");
            }
            else
            {
                exportingAnchorName = Guid.NewGuid().ToString();
            }
            
            WorldAnchorTransferBatch watb = new WorldAnchorTransferBatch();

            Debug.Log("exporting " + exportingAnchorName);
            watb.AddWorldAnchor(exportingAnchorName, worldAnchor);
            WorldAnchorTransferBatch.ExportAsync(watb, WriteBuffer, ExportComplete);

            SaveAnchor(exportingAnchorName);
        }

        /// <summary>
        /// If we don't have the anchor already, call this to download the anchor.
        /// </summary>
        public void WaitForAnchor()
        {
            DownloadingAnchor = true;
            networkTransmitter.RequestAndGetData();
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

            Debug.Log("Looking for " + CachedAnchorName);
            if (SV_WorldAnchorManager.Instance == null)
            {
                Debug.LogError("WorldAnchorManager is null.");
            }
            if (SV_WorldAnchorManager.Instance.AnchorStore == null)
            {
                Debug.LogError("AnchorStore is null.");
            }

            WorldAnchorStore anchorStore = SV_WorldAnchorManager.Instance.AnchorStore;
            string[] ids = anchorStore.GetAllIds();
            Debug.Log(ids.Length + " stored anchors.");
            for (int index = 0; index < ids.Length; index++)
            {
                if (ids[index] == CachedAnchorName)
                {
                    Debug.Log("Using what we have");
                    anchorStore.Load(ids[index], objectToAnchor);
                    AnchorEstablished = true;
                    return true;
                }
                else
                {
                    Debug.Log(ids[index]);
                }
            }

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
            if (located)
            {
                AnchorEstablished = true;
                SV_WorldAnchorManager.Instance.AnchorStore.Save(AnchorName, self);
                self.OnTrackingChanged -= Anchor_OnTrackingChanged;
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
                AnchorName = exportingAnchorName;
                anchorData = exportingAnchorBytes.ToArray();
                GenericNetworkTransmitter.Instance.SetData(anchorData);
                createdAnchor = true;
                Debug.Log("Anchor ready "+exportingAnchorBytes.Count);
                GenericNetworkTransmitter.Instance.ConfigureAsServer();

                AnchorEstablished = true;
                PlayerController.Instance.CmdSetAnchorName(exportingAnchorName);
            }
            else
            {
                Debug.Log("Create anchor failed "+status+" "+exportingAnchorBytes.Count);
                exportingAnchorBytes.Clear();
                objectToAnchor = UNetAnchorManager.Instance.gameObject;
                DestroyImmediate(objectToAnchor.GetComponent<WorldAnchor>());
                CreateAnchor();
            }
        }

        public void AnchorFoundRemotely()
        {
            Debug.Log("Setting saved anchor to " + AnchorName);
#if !UNITY_EDITOR
            SaveAnchor(AnchorName);
#endif
        }

        private void SaveAnchor(string anchorName)
        {
            SV_WorldAnchorManager.Instance.AnchorStore.Save(anchorName, objectToAnchor.GetComponent<WorldAnchor>());
            PlayerPrefs.SetString(SavedAnchorKey, anchorName);
            PlayerPrefs.Save();
        }

        public void MakeNewAnchor()
        {
            if (PlayerPrefs.HasKey(SavedAnchorKey))
            {
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
    }
}