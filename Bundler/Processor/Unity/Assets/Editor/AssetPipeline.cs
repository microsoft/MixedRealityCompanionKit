// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using UnityEngine;
using UnityEditor;
using System.IO;
using System.Collections.Generic;
using UnityEngine.SceneManagement;
using UnityEditor.SceneManagement;

public class BuildAssetBundles : MonoBehaviour
{
    [MenuItem("Bundle/CreateAssetBundles")]
    static void BuildBundles()
    {
        var importDir = @"Assets\toBundle\";
        var exportDir = @"AssetBundles\";

        //build all assetBundles
        if (AssetDatabase.IsValidFolder(exportDir) == false)
        {
            Directory.CreateDirectory(exportDir);
        }

        AssetDatabase.RemoveUnusedAssetBundleNames();

        var subDirectories = Directory.GetDirectories(importDir);

        foreach (string m in subDirectories)
        {
            var meshes = Directory.GetFiles(m);

            //loop through each file in dir and find .fbx files
            foreach (string a in meshes)
            {
                if (Path.GetExtension(a) == ".fbx")
                {
                    Scene newScene = EditorSceneManager.NewScene(NewSceneSetup.EmptyScene, NewSceneMode.Single);
                    
                    var obj = AssetDatabase.LoadAssetAtPath<GameObject>(a);
                    var newObj = GameObject.Instantiate(obj);
                    newObj.name = obj.name;

                    var sceneName = m + "\\" + obj.name + ".unity";
                    EditorSceneManager.SaveScene(newScene, sceneName);
                    AssetImporter asset = AssetImporter.GetAtPath(sceneName);
                    asset.assetBundleName = obj.name + ".unity3d";
                    asset.SaveAndReimport();

                    BuildPipeline.BuildAssetBundles(exportDir, BuildAssetBundleOptions.None, BuildTarget.WSAPlayer);
                }
            }
        }
    }
}