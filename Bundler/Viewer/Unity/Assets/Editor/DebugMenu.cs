// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using UnityEditor;
 
 public static class DebugMenu
 {
     [MenuItem("Debug/Print Global Position")]
     public static void PrintGlobalPosition()
     {
         if (Selection.activeGameObject != null)
         {
             Debug.Log(Selection.activeGameObject.name + " is at " + Selection.activeGameObject.transform.position);
         }
     }
 }