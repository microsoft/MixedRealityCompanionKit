// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
#if UNITY_UWP
using System.Globalization;
using System.Net.Http;
using System.Threading.Tasks;
using System.IO.Compression;
#endif

public class ServerManager : MonoBehaviour
{
    public string ServerRoot = "http://your.server.here";

    public string ServerBundleRoot { get { return ServerRoot + "/AssetBundles/"; } }

    public string DataUrl { get { return ServerBundleRoot + "bundles.json"; } }

    public UserInfo UserInfo = new UserInfo();

    void Awake()
    {
        var localFolder = Application.persistentDataPath;

        Debug.Log("Looking for local AssetBundles: " + localFolder);
#if UNITY_UWP
       

        if (System.IO.Directory.Exists(localFolder + "/AssetBundles") &&
            System.IO.File.Exists(localFolder + "/AssetBundles.zip"))
        {
            System.IO.Directory.Delete(localFolder + "/AssetBundles", true);
        }

        if (System.IO.File.Exists(localFolder + "/AssetBundles.zip"))
        {
            Debug.Log("Extracting AssetBundles.zip: " + localFolder);
            ZipFile.ExtractToDirectory(localFolder + "/AssetBundles.zip", localFolder);
            System.IO.File.Delete(localFolder + "/AssetBundles.zip");
        }

#endif

        if (System.IO.Directory.Exists(localFolder + "/AssetBundles"))
        {
            Debug.Log("Using local AssetBundles path: " + localFolder);
            localFolder = localFolder.Replace(" ", "%20");
            ServerRoot = "file:///" + localFolder;
        }

        Debug.Log("AssetBundles data path: " + DataUrl);
    }

    void Start()
    {
#if UNITY_UWP
        var json = UnityEngine.WSA.Application.arguments;
        if (json != string.Empty)
        {
            UserInfo = UserInfo.CreateFromJSON(json);
        }
        Debug.Log("User Info: \n" + UserInfo.ToString());
#endif
    }
}
