// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using UnityEngine;
using UnityEngine.VR.WSA.Persistence;
using System;

/// <summary>
/// Loads or saves an anchor with this gameobject name.
/// </summary>
public class AnchorPersistence : MonoBehaviour
{
    public event Action AnchorLoaded;
    public bool AutoLoadOnEnable = true;
    private WorldAnchorStore worldAnchorStore;
    private PersistenceSaveLoad saveLoad;

    private bool _isAnchored;
    public bool IsAnchored
    {
        get
        {
            return _isAnchored;
        }
    }
    
    void OnEnable()
    {
        if (saveLoad == null)
        {
            saveLoad = ScriptableObject.CreateInstance("PersistenceSaveLoad") as PersistenceSaveLoad;
        }

        Debug.Log("AnchorLoader:OnEnable - " + gameObject.name);
        
        if (worldAnchorStore == null)
        {
            WorldAnchorStore.GetAsync(delegate (WorldAnchorStore store)
            {
                this.worldAnchorStore = store;
                if(AutoLoadOnEnable)
                    LoadAnchor();
            });
        }
        else
        {
            if (AutoLoadOnEnable)
                LoadAnchor();
        }
    }

    public bool LoadAnchor()
    {
        _isAnchored = saveLoad.LoadLocation(gameObject, ref worldAnchorStore);
        if (!_isAnchored)
        {
            Debug.Log("No saved anchors found for zone " + gameObject.name + ", align and save anchors first");
        }
        if (_isAnchored && AnchorLoaded != null)
            AnchorLoaded();
        return _isAnchored;
    }

    public bool ClearAnchor()
    {
        return saveLoad.DeleteLocation(gameObject, ref worldAnchorStore);
    }

    public bool AnchorAndSave()
    {
        _isAnchored = saveLoad.SaveLocation(gameObject, ref worldAnchorStore);
        return _isAnchored;
    }
}
