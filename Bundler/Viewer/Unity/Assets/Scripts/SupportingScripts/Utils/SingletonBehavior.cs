// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[System.Serializable]
[DisallowMultipleComponent]
public abstract class SingletonBehavior<T> : MonoBehaviour where T : SingletonBehavior<T>
{
    public static T Instance
    {
        get
        {
            if (instance == null)
            {
                instance = GameObject.FindObjectOfType(typeof(T)) as T;
                if (instance == null)
                {
                    Debug.LogWarning("Unable to find singleton.  It has probably been destroyed already:  " + typeof(T));
                    var go = new GameObject(typeof(T).ToString() + "_Instance");
                    instance = go.AddComponent<T>();
                }
            }

            return instance;
        }
    }
    [SerializeField] private static T instance;

    private void Awake()
    {
        if (instance != null && instance != this)
        {
            Debug.LogWarningFormat("Already have an istance of {0}, removing GameObject:'{1}' from scene.", typeof(T).ToString(), this.gameObject.name);

            DestroyImmediate(this.gameObject);

            return;
        }

        instance = this as T;

        InternalAwake();
    }

    private void OnDestroy()
    {
        InternalOnDestroy();
    }

    protected abstract void InternalAwake();
    protected abstract void InternalOnDestroy();
}
