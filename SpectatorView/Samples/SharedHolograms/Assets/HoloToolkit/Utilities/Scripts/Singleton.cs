// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;

namespace HoloToolkit.Unity
{
    /// <summary>
    /// Singleton behaviour class, used for components that should only have one instance
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public class Singleton<T> : MonoBehaviour where T : Singleton<T>
    {
        private static T _Instance;

        public static  bool IsInitialized
        {
            get
            {
                return _Instance != null;
            }
        }

        protected virtual void Awake()
        {
        }

        protected virtual void OnDestroy()
        {
        }

        public static T Instance
        {
            get
            {
                if (_Instance == null)
                {
                    _Instance = FindObjectOfType<T>();
                }
                return _Instance;
            }
        }
    }
}