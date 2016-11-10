// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using System;
using System.Diagnostics.CodeAnalysis;
using UnityEngine;

[DisallowMultipleComponent]
public abstract class ImprovedSingletonBehavior<T> : MonoBehaviour where T : ImprovedSingletonBehavior<T>
{
    [SuppressMessage("Microsoft.Design", "CA1000:DoNotDeclareStaticMembersOnGenericTypes", Justification = "Fix later")]
    private static T instance;
    public static T Instance
    {
        get
        {
            if (instance != null)
            {
                return instance;
            }
                       
            // first time init?
            var sceneInstance = GameObject.FindObjectOfType(typeof(T)) as T;
            if (!sceneInstance)
            {
                Debug.LogWarning("Unable to find singleton.  It has probably been destroyed already:  " + typeof(T));
                return null;
            }

            sceneInstance.Initialize();
            instance = sceneInstance;

            return instance;
        }
    }

    private bool initialized;
    private object initilizedLock = new object();

    public void Initialize()
    {
        lock ( initilizedLock )
        {
            if (!initialized)
            {
                initialized = true;
                InitializeInternal();
            }
        }
    }

    public virtual void InitializeInternal() { }
}