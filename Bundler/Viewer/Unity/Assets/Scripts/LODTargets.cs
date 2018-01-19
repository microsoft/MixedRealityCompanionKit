// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Text;

[System.Serializable]
public class LODTargets
{
    public LODTarget LOD1;
    public LODTarget LOD2;
    public LODTarget LOD3;

    public override string ToString()
    {
        StringBuilder sb = new StringBuilder();
        sb.AppendLine("    LODTargets: ");
        sb.AppendLine(LOD1.ToString());
        sb.AppendLine(LOD2.ToString());
        sb.AppendLine(LOD3.ToString());
        return sb.ToString();
    }
}
