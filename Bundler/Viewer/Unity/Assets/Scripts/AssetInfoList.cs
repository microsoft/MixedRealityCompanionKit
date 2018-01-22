// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;

[System.Serializable]
public class AssetInfoList
{
    public AssetInfo[] items;

    public static AssetInfoList CreateFromJSON(string jsonString)
    {
        var newList = JsonUtility.FromJson<AssetInfoList>(jsonString);
        for (var i = 0; i < newList.items.Length; ++i)
        {
            var assetInfo = newList.items[i];

            // Bundles are not guaranteed to be backwards or forwards compatible
            // Uncomment out this line to have the viewer only show the bundles for a specific build of Unity
            //assetInfo.Bundles = assetInfo.Bundles.FindAll((LODBundle bundle) => { return bundle.UnityVersion == "2017.2"; });
          
            newList.items[i] = assetInfo;
        }
        return newList;
    }
}
