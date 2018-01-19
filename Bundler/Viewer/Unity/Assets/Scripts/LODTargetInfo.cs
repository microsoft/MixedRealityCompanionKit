// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Text;

[System.Serializable]
public class LODTargetInfo
{
    public string targetplatform;
    public int polycount;
    public int vertexcount;
    public int partcount;
    public int surfacearea;
    public int decimatedPartsCount;

    public bool HasData()
    {
        return targetplatform != string.Empty;
    }

    public override string ToString()
    {
        StringBuilder sb = new StringBuilder();
        sb.AppendLine("    Target Platform: " + targetplatform);
        sb.AppendLine("        Poly Count: " + polycount);
        sb.AppendLine("        Vertex Count: " + vertexcount);
        sb.AppendLine("        Part Count: " + partcount);
        sb.AppendLine("        Parts Decimated: " + decimatedPartsCount);
        sb.AppendLine("        Surface Area: " + surfacearea);
        return sb.ToString();
    }
}