// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class LoadBundle : MonoBehaviour {

    string bundleName = ".unity3d";
    string assetPath;

    private void OnGUI()
    {
        var style = new GUIStyle();
        style.normal.textColor = Color.black;
        GUI.Label(new Rect(10, 5, 200, 20), "Bundle Name", style);
        bundleName = GUI.TextField(new Rect(10, 20, 200, 20), bundleName, 75);
        if(GUI.Button(new Rect(220, 20, 50, 20),"Load"))
        {
            StartCoroutine(LoadTheBundle());
        }
    }

    IEnumerator LoadTheBundle()
    {
        if (Application.isEditor)
        {
            assetPath = "file://" + Application.dataPath + "/../AssetBundles/" + bundleName;
        }
        else
        {
            assetPath = Application.dataPath + "/" + bundleName;
        }


        Debug.Log("Downloading...");
        
        var www = new WWW(assetPath);
        while (www.isDone == false)
            yield return null;

        if (transform.childCount > 0)
        {
            Destroy(transform.GetChild(0).gameObject);
        }
        
        Debug.Log("LoadSceneAsync...");
        var scenePath = www.assetBundle.GetAllScenePaths();
        var sceneName = System.IO.Path.GetFileNameWithoutExtension(scenePath[0]);
        var loadOperation =  SceneManager.LoadSceneAsync(sceneName, LoadSceneMode.Additive);
        while (loadOperation.isDone == false)
            yield return null;

        Debug.Log("Merging...");
        var newScene = SceneManager.GetSceneByName(sceneName);
        var root = newScene.GetRootGameObjects()[0];
        var activeScene = SceneManager.GetActiveScene();
        
        SceneManager.MergeScenes(newScene, activeScene);
        root.transform.SetParent(null);

        www.assetBundle.Unload(false);
    }
}
