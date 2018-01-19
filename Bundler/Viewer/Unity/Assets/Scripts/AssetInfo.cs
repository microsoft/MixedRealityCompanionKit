// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;

[System.Serializable]
public struct AssetInfo : IEquatable<AssetInfo>
{
    public string Id;
    public string OwnerId;
    public string Name;
    public string Created;
    public string Updated;
    public string Thumbnail;
    public List<LODBundle> Bundles;
    public string Status;
    public int Version;

    public bool IsEmpty() { return string.IsNullOrEmpty(Id); }

    public static AssetInfo CreateFromJSON(string jsonString)
    {

        var assetInfo = JsonUtility.FromJson<AssetInfo>(jsonString);

        // Uncomment out this line to have the viewer only show the bundles for a specific build of Unity
        //assetInfo.Bundles = assetInfo.Bundles.FindAll((LODBundle bundle) => { return bundle.UnityVersion == "2017.2"; });
        
        return assetInfo;
    }

    public bool Equals(AssetInfo other)
    {
        return Name == other.Name && OwnerId == other.OwnerId && Id == other.Id && Version == other.Version && Status == other.Status;
    }

    public void Serialize(NetworkWriter writer)
    {
        writer.Write(Id);
        writer.Write(OwnerId);
        writer.Write(Name);
        writer.Write(Created);
        writer.Write(Updated);
        writer.Write(Thumbnail);
        writer.Write(Status);
        writer.Write(Version);
        if (Bundles == null)
        {
            writer.Write(0);
        }
        else
        {
            writer.Write(Bundles.Count);
            foreach (var bundle in Bundles)
            {
                bundle.Serialize(writer);
            }
        }
    }

    public static AssetInfo Deserialize(NetworkReader reader)
    {

        var ret = new AssetInfo();
        ret.Id = reader.ReadString();
        ret.OwnerId = reader.ReadString();
        ret.Name = reader.ReadString();
        ret.Created = reader.ReadString();
        ret.Updated = reader.ReadString();
        ret.Thumbnail = reader.ReadString();
        ret.Status = reader.ReadString();
        ret.Version = reader.ReadInt32();
        var bundleCount = reader.ReadInt32();
        ret.Bundles = new List<LODBundle>(bundleCount);
        for (var i = 0; i < bundleCount; ++i)
        {
            ret.Bundles.Add(LODBundle.Deserialize(reader));
        }
        return ret;
    }
}
