// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.IO;
using UnityEngine;

#if UNITY_WSA
using UnityEngine.XR.WSA;
using UnityEngine.XR.WSA.Persistence;
using UnityEngine.XR.WSA.Sharing;
#endif

namespace Persistence
{
    public enum PersistenceEventType
    {
        /// <summary>
        /// Atttempting to apply a shared anchor
        /// </summary>
        ApplyingShared,

        /// <summary>
        /// Successfully applied a shared anchor
        /// </summary>
        AppliedShared,

        /// <summary>
        /// An anchor was loaded from cached
        /// </summary>
        Loaded,

        /// <summary>
        /// An anchor was saved to the cache
        /// </summary>
        Saved,
    }

    /// <summary>
    /// Holds data on the presistence event
    /// </summary>
    public class PersistenceEventArgs
    {
        /// <summary>
        /// The type of the persistence event
        /// </summary>
        public PersistenceEventType Type { get; set; }

        /// <summary>
        /// The anchor id
        /// </summary>
        public string AnchorId { get; set; }


        /// <summary>
        /// The game object that owns this anchor
        /// </summary>
        public GameObject AnchorOwner { get; set; }
    }

    [Serializable]
    public class PersistenceSaveLoad: ScriptableObject
    {
        /// <summary>
        /// Invoked when this object finished an action, like save or load.
        /// </summary>
        public delegate void OnPersistenceEvent(PersistenceSaveLoad source, PersistenceEventArgs args);
        public event OnPersistenceEvent PersistenceEvent;

#if UNITY_WSA
        private string GetPathForGameObject(GameObject gameObject)
        {
            var path = Path.Combine(Application.persistentDataPath.Replace("/", "\\"), string.Format("{0}.worldanchor", gameObject.name));
            Debug.Log("WorldAnchor save path:  " + path);
            return path;
        }

        public bool DeleteAllWorldAnchorFiles(WorldAnchorStore store)
        {
            if (store == null)
            {
                Debug.LogWarning("Cannot delete WorldAnchor with null WorldAnchorStore");
                return false;
            }

            DirectoryInfo dirInfo = new DirectoryInfo(Application.persistentDataPath);
            if (!dirInfo.Exists)
            {
                return false;
            }

            var worldAnchorFiles = dirInfo.GetFiles("*.worldanchor");
            foreach (var worldAnchorFile in worldAnchorFiles)
            {
                var loadedGuid = Guid.Empty;
                var loadedScale = Vector3.one;
                if (TryGetSpaceAndScale(worldAnchorFile.FullName, out loadedGuid, out loadedScale))
                {
                    // delete this id from the store
                    store.Delete(loadedGuid.ToString());
                }
                worldAnchorFile.Delete();
            }
            return true;
        }

        public bool DeleteLocation(GameObject gameObject, WorldAnchorStore store)
        {
            if (gameObject == null)
            {
                Debug.LogWarning("Cannot delete WorldAnchor on null gameObject");
                return false;
            }
            if (store == null)
            {
                Debug.LogWarning("Cannot delete WorldAnchor with null WorldAnchorStore");
                return false;
            }

            Debug.Log("Attempting to delete world position\r\n   " + gameObject.transform.localPosition.ToString("0.00"));

            var anchor = gameObject.GetComponent<WorldAnchor>();
            if (anchor != null)
            {
                DestroyImmediate(anchor);
            }

            var path = GetPathForGameObject(gameObject);
            FileInfo file = new FileInfo(path);
            if (file.Exists)
            {
                var loadedGuid = Guid.Empty;
                var loadedScale = Vector3.one;
                if (TryGetSpaceAndScale(path, out loadedGuid, out loadedScale))
                {
                    // delete this id from the store
                    store.Delete(loadedGuid.ToString());
                }
                file.Delete();
                Debug.Log("Deleted anchor file.");
            }
            else
            {
                Debug.Log("No saved location to delete.");
                return false;
            }

            return true;
        }

        /// <summary>
        /// Add an anchor to a game object and attempt to save this anchor to the app's local state. Note that this 
        /// will delete the previously saved anchor before attempting to save the new anchor. Therefore if the save 
        /// fails, the old saved anchor will be lost.
        /// </summary>
        /// <returns>True if game object is anchored</returns>
        public bool SaveLocation(GameObject gameObject, WorldAnchorStore store)
        {
            if (gameObject == null)
            {
                Debug.LogWarning("Cannot create WorldAnchor on null gameObject");
                return false;
            }
            if (store == null)
            {
                Debug.LogWarning("Cannot create WorldAnchor with null WorldAnchorStore");
                return false;
            }

            Debug.Log("Attempting save world position\r\n   " + gameObject.transform.localPosition.ToString("0.00"));

            // delete any previous WorldAnchors on this object
            DeleteLocation(gameObject, store);

            // add a new anchor to gameObject
            gameObject.AddComponent<WorldAnchor>();

            // now save the new anchor
            return SaveExistingLocation(Guid.NewGuid(), gameObject, store);
        }

        /// <summary>
        /// Given a transfer batch, apply the batch's first anchor to the given game object. Once applied, attempt to 
        /// save the new anchor to the app's local state. Note that this will delete the previously saved anchor before 
        /// attempting to apply the new anchor. Therefore if the application fails, the old saved anchor will be lost.
        /// </summary>
        /// <returns>True if game object is anchored</returns>
        public bool ApplySharedLocation(GameObject gameObject, WorldAnchorTransferBatch batch, WorldAnchorStore store)
        {
            if (gameObject == null)
            {
                Debug.LogWarning("[PersistenceSaveLoad] Cannot apply a shared WorldAnchor on null gameObject");
                return false;
            }

            if (batch == null)
            {
                Debug.LogWarning("[PersistenceSaveLoad] Cannot apply a shared WorldAnchor with null WorldAnchorTransferBatch");
                return false;
            }

            var batchIds = batch.GetAllIds();
            if (batchIds == null)
            {
                Debug.LogWarning("[PersistenceSaveLoad] Cannot apply a shared WorldAnchor with an null id list on WorldAnchorTransferBatch");
                return false;
            }

            if (batchIds.Length == 0)
            {
                Debug.LogWarning("[PersistenceSaveLoad] Cannot apply a shared WorldAnchor with an empty WorldAnchorTransferBatch");
                return false;
            }

            string storageIdString = batchIds[0];
            if (String.IsNullOrEmpty(storageIdString))
            {

                Debug.LogWarning("[PersistenceSaveLoad] Cannot apply a shared WorldAnchor with an empty storage id");
                return false;
            }

            Guid storageId;
            try
            {
                 storageId = new Guid(storageIdString);
            }
            catch
            {
                Debug.LogWarning("[PersistenceSaveLoad] Cannot apply a shared WorldAnchor with an invalid GUID");
                return false;
            }

            if (store == null)
            {
                Debug.LogWarning("[PersistenceSaveLoad] Cannot apply a shared WorldAnchor with null WorldAnchorStore");
                return false;
            }

            Debug.Log("[PersistenceSaveLoad] Attempting to apply a shared world position: " + storageId);
            bool successful = false;

            // delete any previous WorldAnchors on this object
            DeleteLocation(gameObject, store);

            RaisePersistenceEvent(PersistenceEventType.ApplyingShared, gameObject, storageIdString);
            WorldAnchor.OnTrackingChangedDelegate trackingChanged = null;
            trackingChanged = (WorldAnchor self, bool located) =>
            {
                if (!located)
                {
                    Debug.Log("[PersistenceSaveLoad] Haven't located anchor yet: " + storageId);
                    return;
                }


                Debug.Log("[PersistenceSaveLoad] Located anchor: " + storageId);
                self.OnTrackingChanged -= trackingChanged;
                SaveExistingLocation(storageId, gameObject, store);
                RaisePersistenceEvent(PersistenceEventType.AppliedShared, gameObject, storageIdString);
            };

            // actually lock the game object to the batch's first anchor
            WorldAnchor anchor = batch.LockObject(storageIdString, gameObject);
            if (anchor != null)
            {
                anchor.OnTrackingChanged += trackingChanged;
                trackingChanged(anchor, anchor.isLocated);

                // assume locating is successful. If a caller wants to trully know when anchor was located, they should
                // listen for the "AppliedShared" and "Saved" events
                successful = true;
            }
            else
            {
                Debug.LogError("[PersistenceSaveLoad] Couldn't lock object with the given storage id: " + storageId);
            }

            return successful;
        }

        private bool SaveExistingLocation(Guid storageId, GameObject gameObject, WorldAnchorStore store)
        {
            bool success = false;
            var storageIdString = storageId.ToString();

            // get instance of anchor to save in worldAnchor store
            var anchor = gameObject.GetComponent<WorldAnchor>();
            if (anchor != null)
            {
                success = store.Save(storageIdString, anchor);
            }

            if (!success)
            {
                Debug.Log("Failed to get world locked position.");
                return false;
            }

            Debug.Log("Saved world locked position, writing to disk.");
            var path = GetPathForGameObject(gameObject);

            using (var memStream = new MemoryStream())
            {
                using (var writer = new BinaryWriter(memStream))
                {
                    writer.Write(storageId.ToByteArray());
                    writer.Write(gameObject.transform.localScale.x);
                    writer.Write(gameObject.transform.localScale.y);
                    writer.Write(gameObject.transform.localScale.z);
                    File.WriteAllBytes(path, memStream.ToArray());
                }
            }

            RaisePersistenceEvent(PersistenceEventType.Saved, gameObject, storageIdString);
            Debug.Log("Saved location:  " + storageId);
            Debug.Log("      position:  " + gameObject.transform.localPosition.ToString("0.00"));
            return true;
        }

        public bool LoadLocation(GameObject gameObject, WorldAnchorStore store)
        {
            if (gameObject == null)
            {
                Debug.LogWarning("Cannot load WorldAnchor on null gameObject");
                return false;
            }
            if (store == null)
            {
                Debug.LogWarning("Cannot load WorldAnchor with null WorldAnchorStore");
                return false;
            }

            var path = GetPathForGameObject(gameObject);

            Debug.Log("Attempting load from path:  " + path);
            
            var loadedGuid = Guid.Empty;
            var loadedScale = Vector3.one;
            if (TryGetSpaceAndScale(path, out loadedGuid, out loadedScale))
            {
                var loadedGuidString = loadedGuid.ToString();
                if (store.Load(loadedGuidString, gameObject))
                {
                    gameObject.transform.localScale = loadedScale;
                    Debug.Log("Found saved location, target position: " + gameObject.transform.localPosition + "\n  Scale: " + loadedScale.ToString("0.00"));
                    RaisePersistenceEvent(PersistenceEventType.Loaded, gameObject, loadedGuidString);
                    return true;
                }

                Debug.Log("Guid not found in savedlocations.  Cleaning up.");

                // failed to load the anchor info, so delete this id from the store
                DeleteLocation(gameObject, store);
            }

            Debug.Log("Failed to get spaceID");
            return false;
        }

        private bool TryGetSpaceAndScale(string path, out Guid guid, out Vector3 scale)
        {
            Debug.Log("Space id file:  " + path);
            if (File.Exists(path))
            {
                using (var memStream = new MemoryStream(File.ReadAllBytes(path)))
                {
                    using (var reader = new BinaryReader(memStream))
                    {
                        byte[] guidBytes = reader.ReadBytes(16);
                        guid = new Guid(guidBytes);
                        // Anchors don't save scale, so save it out along with the id
                        scale = new Vector3(reader.ReadSingle(), reader.ReadSingle(), reader.ReadSingle());
                        return true;
                    }
                }
            }
            else
            {
                Debug.Log("   **  No SpaceID file found **");
            }

            guid = Guid.Empty;
            scale = Vector3.one;
            return false;
        }

        private void RaisePersistenceEvent(PersistenceEventType type, GameObject owner, string anchorId)
        {
            Debug.LogFormat("[PersistenceSaveLoad] RaisePersistenceEvent (type: {0}) (anchor id: {1})", type, anchorId);
            if (PersistenceEvent != null)
            {
                PersistenceEventArgs args = new PersistenceEventArgs();
                args.AnchorId = anchorId;
                args.AnchorOwner = owner;
                args.Type = type;

                PersistenceEvent(this, args);
            }
        }
#endif
    }
}