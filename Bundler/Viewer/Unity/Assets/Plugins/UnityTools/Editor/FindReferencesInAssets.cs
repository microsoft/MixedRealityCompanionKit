// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEditor;
using UnityEngine;

public class FindReferencesInAssets
{
    [MenuItem("Assets/Find References")]
    public static void FindReferencesMenu()
    {
        var guids = Selection.assetGUIDs;
        if (guids == null)
        {
            Debug.LogError("No selection!");
            return;
        }

        var guid = guids[0];

        var asset = AssetDatabaseUtils.LoadAssetByGUID<Object>(guid);
        if (asset == null)
        {
            Debug.LogError("Couldn't load asset!");
            return;
        }

        int countFound = 0;

        var gameObjects = AssetDatabaseUtils.FindAndLoadAssets<GameObject>();
        Debug.LogFormat(asset, "Searching <b>{0}</b> AssetDatabase GameObjects for references to: <b>{1}</b>", gameObjects.Count, asset.name);
        foreach (var go in gameObjects)
        {
            countFound += FindReferences(asset, go);
        }

        var sceneGameObjects = Resources.FindObjectsOfTypeAll<GameObject>();
        Debug.LogFormat(asset, "Searching <b>{0}</b> Scene GameObjects for references to: <b>{1}</b>", sceneGameObjects.Length, asset.name);
        foreach (var go in sceneGameObjects)
        {
            countFound += FindReferences(asset, go);
        }

        Debug.LogFormat(asset, "<b>Completed search, {0} references found</b>", countFound);
    }


    private static int FindReferences(Object asset, GameObject go)
    {
        int countFound = 0;
        if (PrefabUtility.GetPrefabType(go) == PrefabType.PrefabInstance)
        {
            if (PrefabUtility.GetPrefabParent(go) == asset)
            {
                Debug.LogFormat("Reference at: <b>{0}</b>", go.GetFullPath());
                ++countFound;
            }
        }

        var components = go.GetComponents<Component>();
        foreach (var component in components)
        {
            if (!component)
            {
                continue;
            }

            var so = new SerializedObject(component);

            var sp = so.GetIterator();
            while (sp.NextVisible(true))
            {
                if ((sp.propertyType == SerializedPropertyType.ObjectReference) && (sp.objectReferenceValue == asset))
                {
                    Debug.LogFormat(go, "Reference at: <b>{0}</b>, Component {1}, Property {2}", go.GetFullPath(), ObjectNames.GetClassName(component), ObjectNames.NicifyVariableName(sp.name));
                    ++countFound;
                }
            }
        }

        return countFound;
    }
}
