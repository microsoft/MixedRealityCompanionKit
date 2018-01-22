// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine.Networking;

[System.Serializable]
public struct LODBundle
{
    public string Name;
    public int LODNumber;
    public string Bundle;
    public string UnityVersion;

    public void Serialize(NetworkWriter writer)
    {
        writer.Write(Name);
        writer.Write(LODNumber);
        writer.Write(Bundle);
        writer.Write(UnityVersion);
    }

    public static LODBundle Deserialize(NetworkReader reader)
    {

        var ret = new LODBundle();
        ret.Name = reader.ReadString();
        ret.LODNumber = reader.ReadInt32();
        ret.Bundle = reader.ReadString();
        ret.UnityVersion = reader.ReadString();
        return ret;
    }
}
