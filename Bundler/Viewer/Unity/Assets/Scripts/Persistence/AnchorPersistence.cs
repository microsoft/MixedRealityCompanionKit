// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;

#if UNITY_WSA
using UnityEngine.XR.WSA.Persistence;
using UnityEngine.XR.WSA;
#endif
using System;


namespace Persistence
{
    /// <summary>
    /// Loads or saves an anchor with this gameobject name.
    /// </summary>
    public class AnchorPersistence : MonoBehaviour
    {
        public event Action AnchorLoaded;

        [HideInInspector]
        public GameObject TargetGameObject = null;

        public bool AutoLoadOnEnable = true;
        public bool IsReady { get; private set; }

        private bool _isAnchored;
        public bool IsAnchored
        {
            get
            {
                return _isAnchored;
            }
        }
#if UNITY_WSA

        private WorldAnchorStore worldAnchorStore;
#endif
        private PersistenceSaveLoad saveLoad;

        void OnEnable()
        {
            if (saveLoad == null)
            {
                saveLoad = ScriptableObject.CreateInstance("PersistenceSaveLoad") as PersistenceSaveLoad;
            }
#if UNITY_WSA
            if (worldAnchorStore == null)
            {
                IsReady = false;
                WorldAnchorStore.GetAsync(delegate (WorldAnchorStore store)
                {
                    this.worldAnchorStore = store;
                    IsReady = true;
                    if (AutoLoadOnEnable)
                        LoadAnchor();
                });
            }
            else
            {
                if (AutoLoadOnEnable)
                {
                    LoadAnchor();
                }
            }
#endif
        }

        public bool LoadAnchor()
        {
            TargetGameObject = TargetGameObject == null ? gameObject : TargetGameObject;

#if UNITY_WSA
            _isAnchored = saveLoad.LoadLocation(TargetGameObject, ref worldAnchorStore);
#endif
            if (!_isAnchored)
            {
                Debug.Log("No saved anchors found for zone " + TargetGameObject.name + ", align and save anchors first");
                
            }
            if (_isAnchored && AnchorLoaded != null)
            {
                AnchorLoaded();
            }
            return _isAnchored;
        }

        public bool ClearAnchor(bool removeSavedLocation)
        {
            _isAnchored = false;
            TargetGameObject = TargetGameObject == null ? gameObject : TargetGameObject;

#if UNITY_WSA
            if (removeSavedLocation)
            {
                return saveLoad.DeleteLocation(TargetGameObject, ref worldAnchorStore);
            }

            if (TargetGameObject.GetComponent<WorldAnchor>() != null)
            {
                DestroyImmediate(TargetGameObject.GetComponent<WorldAnchor>());
            }
#endif
            return true;
        }

        public bool PlaceAnchor(bool saveAchor)
        {
            TargetGameObject = TargetGameObject == null ? gameObject : TargetGameObject;
#if UNITY_WSA
            if (saveAchor)
            {
                _isAnchored = saveLoad.SaveLocation(TargetGameObject, ref worldAnchorStore);

            }
            else
            {
                ClearAnchor(false);
                TargetGameObject.AddComponent<WorldAnchor>();
                _isAnchored = true;
            }
#endif
            return _isAnchored;
        }
    }
}