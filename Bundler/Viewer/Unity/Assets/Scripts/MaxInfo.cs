// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Text;
using UnityEngine;

[System.Serializable]
public class MaxInfo
{
    public string DateTimeStarted;
    public string DateTimeEnded;
    public bool CustomSettings;
    public int ModelsInZip;
    public LODTargets TargetLODs;
    public int DontDecimateVertsUnder;
    public LODTargetInfo Source;
    public LODTargetInfo LOD0;
    public LODTargetInfo LOD1;
    public LODTargetInfo LOD2;
    public LODTargetInfo LOD3;

    public static MaxInfo CreateFromJSON(string jsonString)
    {
        return JsonUtility.FromJson<MaxInfo>(jsonString);
    }

    public override string ToString()
    {
        StringBuilder sb = new StringBuilder();
        sb.AppendLine("Started: " + DateTimeStarted);
        sb.AppendLine("Ended: " + DateTimeEnded);
        sb.AppendLine("CustomSettings: " + CustomSettings);
        sb.AppendLine("ModelsInZip: " + ModelsInZip);
        sb.AppendLine("DontDecimateVertsUnder: " + DontDecimateVertsUnder);

        sb.AppendLine(TargetLODs.ToString());
        sb.AppendLine("Generated LODs: ");
        if (Source.HasData())
        {
            sb.AppendLine(Source.ToString());
        }
        if (LOD0.HasData())
        {
            sb.AppendLine(LOD0.ToString());
        }
        if (LOD1.HasData())
        {
            sb.AppendLine(LOD1.ToString());
        }
        if (LOD2.HasData())
        {
            sb.AppendLine(LOD2.ToString());
        }
        if (LOD3.HasData())
        {
            sb.AppendLine(LOD3.ToString());
        }

        return sb.ToString();
    }
}
