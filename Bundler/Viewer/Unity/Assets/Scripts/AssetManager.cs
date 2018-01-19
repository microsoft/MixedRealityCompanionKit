// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class AssetManager : MonoBehaviour
{

    public delegate void AssetLoadedDelegate(AssetInfo assetInfo, int lodIndex, GameObject gameObject);
    public delegate void AssetLoadFailedDelegate(AssetInfo assetInfo, int lodIndex, string message);
    public delegate void AssetLoadUpdatedDelegate(AssetInfo assetInfo, int lodIndex, string message);
    public event AssetLoadedDelegate AssetLoaded;
    public event AssetLoadFailedDelegate AssetLoadFailed;
    public event AssetLoadUpdatedDelegate AssetLoadUpdated;

    public Renderer ShaderOverride;

    public AssetInfo CurrentAssetInfo { get; private set; }
    public string CurrentAssetInfoID { get { return CurrentAssetInfo.IsEmpty() ? null : CurrentAssetInfo.Id; } }

    public MaxInfo CurrentMaxInfo { get; private set; }

    public Text InfoTextArea;

    IEnumerator LoadMaxInfo()
    {
        var sourceUrl = (MasterController.Instance.ServerManager.ServerBundleRoot + CurrentAssetInfo.Id + "/maxinfo.json").Replace(" ", "%20");
        WWW www = new WWW(sourceUrl);

        yield return www;

        if (www.error == null)
        {
            Debug.Log("data: " + www.text);

            CurrentMaxInfo = MaxInfo.CreateFromJSON(www.text);

            Debug.Log(CurrentMaxInfo.ToString());
            InfoTextArea.text = CurrentMaxInfo.ToString();
        }
        else
        {
            Debug.LogError("WWW error: " + www.error);
        }
    }

    public void CleanCache()
    {
        Caching.ClearCache();
    }

    public void LoadBundle(AssetInfo assetInfo)
    {
        var lodIndex = -1;
        assetInfo.Bundles.Sort((bundle1, bundle2) => bundle1.LODNumber.CompareTo(bundle2.LODNumber));
        var targetLod = (HololensHelper.IsDeviceHololens() ? 2 : 1);
        for (var i = 0; i < assetInfo.Bundles.Count; ++i)
        {
            lodIndex = i;
            if (assetInfo.Bundles[i].LODNumber >= targetLod)
            {
                break;
            }
        }

        lodIndex = Math.Max(0, Math.Min(lodIndex, assetInfo.Bundles.Count - 1));
        if (lodIndex > assetInfo.Bundles.Count - 1)
        {
            return;
        }

        CurrentAssetInfo = assetInfo;
        StartCoroutine(LoadBundleAsync(assetInfo, lodIndex));
        StartCoroutine(LoadMaxInfo());
    }

    private IEnumerator LoadBundleAsync(AssetInfo assetInfo, int lodIndex, System.Action<GameObject> callback = null)
    {
        Application.backgroundLoadingPriority = ThreadPriority.Normal;

        while (!Caching.ready)
        {
            yield return null;
        }

        if (lodIndex == -1 || lodIndex > assetInfo.Bundles.Count - 1)
        {
            if (AssetLoadFailed != null)
            {
                AssetLoadFailed(assetInfo, lodIndex, "Loading failed: \nBad LOD index");
            }
        }

        //Make any spaces in URL friendly
        var bundleURL = (MasterController.Instance.ServerManager.ServerBundleRoot + assetInfo.Id + "/" + assetInfo.Bundles[lodIndex].Bundle).Replace(" ", "%20");

        
        AssetLoadUpdated(assetInfo, lodIndex, "Loading Model: \nDownloading...");
        using (WWW www = new WWW(bundleURL))
        {
            while (!www.isDone)
            {
                AssetLoadUpdated(assetInfo, lodIndex, "Loading Model: \nDownloading... " + (www.progress * 100).ToString("0") + "%");
                yield return null;
            }

            
            if (www.error != null || www.assetBundle == null)
            {
                //throw new Exception("WWW download had an error:" + www.error);
                var message = www.error;
                if (AssetLoadFailed != null)
                {
                    AssetLoadFailed(assetInfo, lodIndex, "Loading failed: \n" + message);
                }
                yield break;
            }

            var bundle = www.assetBundle;

            GameObject newObject;

            var scenePath = www.assetBundle.GetAllScenePaths();
            if (scenePath.Length == 0)
            {
                Debug.LogWarning("LoadAssetAsync...*** Using old asset bundle.");

                var request = bundle.LoadAssetAsync(assetInfo.Bundles[lodIndex].Name, typeof(GameObject));

                while (!request.isDone)
                {
                    yield return null;
                }

                var gameObject = request.asset as GameObject;
                newObject = Instantiate<GameObject>(gameObject);
            }
            else
            {
                Debug.Log("LoadSceneAsync...");
                AssetLoadUpdated(assetInfo, lodIndex, "Loading Model: \nLoading...");
                var sceneName = System.IO.Path.GetFileNameWithoutExtension(scenePath[0]);
                var loadOperation = SceneManager.LoadSceneAsync(sceneName, LoadSceneMode.Additive);
                while (loadOperation.isDone == false)
                {
                    yield return null;
                }

                AssetLoadUpdated(assetInfo, lodIndex, "Loading Model: \nMerging...");
                Debug.Log("Merging...");
                var newScene = SceneManager.GetSceneByName(sceneName);
                newObject = newScene.GetRootGameObjects()[0];
                newObject.SetActive(false);
                var activeScene = SceneManager.GetActiveScene();
                SceneManager.MergeScenes(newScene, activeScene);
                newObject.transform.SetParent(null);
            }

            yield return null;
            var lastYieldTime = DateTime.Now;
            var renderers = newObject.GetComponentsInChildren<MeshRenderer>();
            int count = 0;
            foreach (var renderer in renderers)
            {
                ++count;
                AssetLoadUpdated(assetInfo, lodIndex, "Loading Model: \nMaterials... (" + count + "/" + renderers.Length + ")");
                renderer.enabled = false;
                renderer.gameObject.AddComponent<MeshCollider>();

                if (ShaderOverride != null)
                {
                    if (renderer.sharedMaterial != null)
                    {
                        renderer.sharedMaterial.shader = ShaderOverride.sharedMaterial.shader;
                        renderer.sharedMaterial.EnableKeyword("_USEMAINTEX_ON");
                        renderer.sharedMaterial.EnableKeyword("_USEMAINCOLOR_ON");
                        renderer.sharedMaterial.EnableKeyword("_USEAMBIENT_ON");
                        renderer.sharedMaterial.EnableKeyword("_USEDIFFUSE_ON");
                        renderer.sharedMaterial.EnableKeyword("_USESPECULAR_ON");
                    }
                    if (renderer.sharedMaterials != null)
                    {
                        for (var i = 0; i < renderer.sharedMaterials.Length; ++i)
                        {
                            var material = renderer.sharedMaterials[i];
                            if (material == null)
                            {
                                renderer.sharedMaterials[i] = new Material(ShaderOverride.sharedMaterial);
                                material = renderer.sharedMaterials[i];
                            }

                            material.shader = ShaderOverride.sharedMaterial.shader;
                            material.EnableKeyword("_USEMAINTEX_ON");
                            material.EnableKeyword("_USEMAINCOLOR_ON");
                            material.EnableKeyword("_USEAMBIENT_ON");
                            material.EnableKeyword("_USEDIFFUSE_ON");
                            material.EnableKeyword("_USESPECULAR_ON");
                        }
                    }
                }
                if ((DateTime.Now - lastYieldTime).TotalMilliseconds > 30)
                {
                    lastYieldTime = DateTime.Now;
                    yield return new WaitForEndOfFrame();
                }
            }
            AssetLoadUpdated(assetInfo, lodIndex, "Loading Model: \nMaking active...");
            yield return new WaitForSeconds(.1f);
            newObject.SetActive(true);

            if (AssetLoaded != null)
            {
                AssetLoaded(assetInfo, lodIndex, newObject);
            }

            if (callback != null)
            {
                callback(newObject);
            }
            bundle.Unload(false);
        }


    }
}
