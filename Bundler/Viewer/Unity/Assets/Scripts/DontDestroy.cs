// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;

public class DontDestroy : MonoBehaviour
{
    public bool dontDestroyOnLoad = true;

    void Start()
    {
        if (dontDestroyOnLoad)
        {
            // unparent
            transform.SetParent(null);

            DontDestroyOnLoad(gameObject);
        }
    }
}
