// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections.Generic;
using System.Runtime.Serialization.Json;
using System.Runtime.Serialization;
using System.IO;

namespace AssetCreator
{
    // Note:  These data contracts are written to JSON files and are consumed by the Unity client.  
    //        Ensure that any changes here are reflected in the Unity client.
    class AssetCatalog
    {
        [DataContract]
        public class AssetInfoList
        {
            [DataMember]
            internal string UpdatedDateTime;
            [DataMember]
            internal List<AssetInfo> items;
        }

        [DataContract]
        public class LODBundle
        {
            [DataMember]
            internal string Name;

            [DataMember]
            internal int LODNumber;

            [DataMember]
            internal string Bundle;

            [DataMember]
            internal string UnityVersion;
        }

        [DataContract]
        public class AssetInfo
        {
            [DataMember]
            internal string Id;
            [DataMember]
            internal string OwnerId;
            [DataMember]
            internal string Name;
            [DataMember]
            internal string Created;
            [DataMember]
            internal string Updated;
            [DataMember]
            internal string Thumbnail;
            [DataMember]
            internal List<LODBundle> Bundles;
            [DataMember]
            internal string Status;
            [DataMember]
            internal int Version;
        }


        private static volatile bool updatingAssetInfos = false;
        public static void UpdateCatalog(string baseDataPath)
        {
            if (updatingAssetInfos)
                return;
            updatingAssetInfos = true;
            try
            {
                if (!Directory.Exists(baseDataPath + "\\"))
                    Directory.CreateDirectory(baseDataPath + "\\");                

                foreach (var subFolderPath in Directory.GetDirectories(baseDataPath))
                {

                    var bundlesPath = subFolderPath + "\\Bundles";
                    if (!Directory.Exists(bundlesPath + "\\"))
                        Directory.CreateDirectory(bundlesPath + "\\");

                    var assetInfos = new AssetInfoList()
                    {
                        UpdatedDateTime = DateTime.Now.ToString(),
                        items = new List<AssetInfo>()
                    };
                    foreach (var bundlePath in Directory.GetDirectories(bundlesPath))
                    {
                        if (File.Exists(bundlePath + "\\bundleInfo.json"))
                        {
                            using (FileStream stream = new FileStream(bundlePath + "\\bundleInfo.json", FileMode.Open, FileAccess.Read))
                            {
                                DataContractJsonSerializer ser = new DataContractJsonSerializer(typeof(AssetInfo));
                                assetInfos.items.Add((AssetInfo)ser.ReadObject(stream));
                            }
                        }
                    }
                    using (FileStream stream = new FileStream(bundlesPath + "\\" + "bundles.json", FileMode.OpenOrCreate, FileAccess.Write))
                    {
                        stream.SetLength(0);
                        DataContractJsonSerializer ser = new DataContractJsonSerializer(typeof(AssetInfoList));
                        ser.WriteObject(stream, assetInfos);
                    }
                }

            }
            catch (Exception ex)
            {
                Console.ForegroundColor = ConsoleColor.Red;
                Console.WriteLine("*** Exception updating infos {0}", ex.Message);
                Console.ResetColor();
            }
            finally { updatingAssetInfos = false; }
        }


        public static AssetInfo LoadAssetInfo(string assetInfoPath)
        {
            if (!File.Exists(assetInfoPath))
                return null;

            AssetInfo assetInfo;
            using (FileStream stream = new FileStream(assetInfoPath, FileMode.Open, FileAccess.Read))
            {
                DataContractJsonSerializer ser = new DataContractJsonSerializer(typeof(AssetInfo));
                assetInfo = (AssetInfo)ser.ReadObject(stream);
            }
            return assetInfo;
        }

        public static void UpdateAssetInfo(AssetInfo assetInfo, string assetInfoPath)
        {
            using (FileStream stream = new FileStream(assetInfoPath, FileMode.OpenOrCreate, FileAccess.Write))
            {
                stream.SetLength(0);
                DataContractJsonSerializer ser = new DataContractJsonSerializer(typeof(AssetInfo));
                ser.WriteObject(stream, assetInfo);
            }
        }
    }
}
