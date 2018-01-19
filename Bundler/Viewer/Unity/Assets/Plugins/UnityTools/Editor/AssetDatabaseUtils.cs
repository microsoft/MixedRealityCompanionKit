// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

public static class AssetDatabaseUtils
{
    public static List<T> FindAndLoadAssets<T>() where T : UnityEngine.Object
    {
        var guids = FindAssetGUIDs<T>();

        return LoadAssetsByGUIDs<T>(guids);
    }

    public static string[] FindAssetGUIDs<T>() where T : UnityEngine.Object
    {
        var typeName = typeof(T).ToString();

        var lastIndex = typeName.LastIndexOf('.');

        if (lastIndex > 0 && lastIndex < typeName.Length - 2)
        {
            typeName = typeName.Substring(lastIndex + 1);
        }

        string search = string.Format("t:{0}", typeName);

        return AssetDatabase.FindAssets(search);
    }

    public static List<T> LoadAssetsByGUIDs<T>(string[] guids) where T : UnityEngine.Object
    {
        var assets = new List<T>(guids.Length);

        foreach (var guid in guids)
        {
            T asset = LoadAssetByGUID<T>(guid);

            if (asset != null)
            {
                assets.Add(asset);
            }
        }

        return assets;
    }

    public static T LoadAssetByGUID<T>(string guid) where T : UnityEngine.Object
    {
        var assetPath = AssetDatabase.GUIDToAssetPath(guid);
        return AssetDatabase.LoadAssetAtPath<T>(assetPath);     
    }
}
