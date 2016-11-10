// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using System;
using System.IO;
using UnityEngine;
using UnityEngine.Assertions;
using UnityEngine.VR.WSA;
using UnityEngine.VR.WSA.Persistence;

[Serializable]
public class PersistenceSaveLoad
    : ScriptableObject
{
    private string GetPathForGameObject(GameObject gameObject)
    {
        var path = Path.Combine(Application.persistentDataPath.Replace("/", "\\"), string.Format("{0}.worldanchor",gameObject.name));
        Debug.Log("WorldAnchor save path:  " + path);
        return path;
    }

    public void DeleteAllWorldAnchorFiles()
    {
        DirectoryInfo dirInfo = new DirectoryInfo(Application.persistentDataPath);
        if (!dirInfo.Exists)
        {
            return;
        }

        var worldAnchorFiles = dirInfo.GetFiles("*.worldanchor");
        foreach (var worldAnchorFile in worldAnchorFiles)
        {
            worldAnchorFile.Delete();
        }
    }

    public bool DeleteLocation(GameObject gameObject, ref WorldAnchorStore store)
    {
        if (gameObject == null)
        {
            Debug.Log("Cannot delete WorldAnchor on null gameObject");
            return false;
        }

        Debug.Log("Attempting delete world position\r\n   " + gameObject.transform.localPosition.ToString("0.00"));
                
        var anchor = gameObject.GetComponent<WorldAnchor>(); 
        if (anchor != null)
        {
            DestroyImmediate(anchor);
        }

        var path = GetPathForGameObject(gameObject);
        FileInfo file = new FileInfo(path);
        if (file.Exists)
        {
            file.Delete();
            Debug.Log("Deleted anchor file.");
        }
        else
        {
            Debug.Log("Failed to get world locked position.");
            return false;
        }
        
        return true;
    }

    public bool SaveLocation(GameObject gameObject, ref WorldAnchorStore store)
    {
        if(gameObject == null)
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

        bool success = false;

        var storageId = Guid.NewGuid();
        var storageIdString = storageId.ToString();

        // delete any preivous WorldAnchors
        var anchor = gameObject.GetComponent<WorldAnchor>(); // can we reuse WorldAnchor
        if (anchor != null)
        {
            Debug.Log("Found existing anchor");
            DestroyImmediate(anchor);
        }

        // add a new anchor to gameObject
        gameObject.AddComponent<WorldAnchor>();

        // get instance of anchor to save in worldAnchor store
        anchor = gameObject.GetComponent<WorldAnchor>();
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

        Debug.Log("Saved location:  " + storageId);
        Debug.Log("      position:  " + gameObject.transform.localPosition.ToString("0.00"));
        return true;
    }

    public bool LoadLocation(GameObject gameObject, ref WorldAnchorStore store)
    {
        var path = GetPathForGameObject(gameObject);

        Debug.Log("Attempting load from path:  " + path);

        Assert.IsNotNull(gameObject);
        var loadedGuid = Guid.Empty;
        var loadedScale = Vector3.one;
        if (TryGetSpaceAndScale(path, out loadedGuid, out loadedScale))
        {
            if (store.Load(loadedGuid.ToString(), gameObject))
            {
                gameObject.transform.localScale = loadedScale;
                Debug.Log("Found saved location, target position: " + gameObject.transform.localPosition + "\n  Scale: " + loadedScale.ToString("0.00"));
                return true;
            }

            // delete this id from the store
            store.Delete(loadedGuid.ToString());

            // destroy any anchors on the object
            var worldAnchor = gameObject.GetComponent<WorldAnchor>();
            if (worldAnchor != null)
            {
                Debug.Log("Deleting existing world anchor object");

                DestroyObject(worldAnchor);
            }

            Debug.Log("Guid not found in savedlocations.");
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
}
