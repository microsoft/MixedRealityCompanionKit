// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Text;

[System.Serializable]
public class LODTarget
{
    public string TargetPlatform;
    public int TargetPlatformVertexCount;

    public override string ToString()
    {
        StringBuilder sb = new StringBuilder();
        sb.AppendLine("    TargetPlatform: " + TargetPlatform);
        sb.AppendLine("        TargetPlatformVertexCount: " + TargetPlatformVertexCount);
        return sb.ToString();
    }
}