// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace SimpleSharing
{
    public class Singleton<T> : MonoBehaviour where T : Singleton<T>
    {
        private static T _Instance;

        public static bool IsInitialized
        {
            get
            {
                return _Instance != null;
            }
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
