// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;

public class AppExit : MonoBehaviour
{
   
    public void Quit()
    {
#if NETFX_CORE
            Windows.ApplicationModel.Core.CoreApplication.Exit();
#else
        UnityEngine.Application.Quit();
#endif
    }
}
