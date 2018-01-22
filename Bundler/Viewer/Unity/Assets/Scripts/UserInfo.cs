// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;

[System.Serializable]
public class UserInfo
{
    public string DisplayableId;
    public string FamilyName;
    public string GivenName;
    public string UniqueId;
    public string ExpiresDateTimeStr;

    public override string ToString()
    {
        return
            "DisplayableId: " + DisplayableId + "\n" +
            "FamilyName: " + FamilyName + "\n" +
            "GivenName: " + GivenName + "\n" +
            "UniqueId: " + UniqueId + "\n" +
            "ExpiresDateTimeStr: " + ExpiresDateTimeStr + "\n";
    }

    public static UserInfo CreateFromJSON(string jsonString)
    {
        return JsonUtility.FromJson<UserInfo>(jsonString);
    }
}
