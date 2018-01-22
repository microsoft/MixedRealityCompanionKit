// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using System.Collections.Generic;
using UnityEngine;

#if UNITY_EDITOR
using UnityEditor;
using UnityEditorInternal;

[CustomPropertyDrawer(typeof(DataList<>))]
public class DataListDrawer : PropertyDrawer
{
    protected ReorderableList reorderablelist;

    protected virtual void RefreshList(SerializedProperty property)
    {
        RefreshList(property, true, true, true, true);
    }

    protected void RefreshList(SerializedProperty property, bool draggable, bool displayHeader, bool displayAdd, bool displayDelete)
    {
        if (property == null)
            return;

        if (this.reorderablelist == null && property.isArray)
        {
            this.reorderablelist = new ReorderableList(property.serializedObject, property, draggable, displayHeader, displayAdd, displayDelete);

            this.reorderablelist.drawHeaderCallback = (Rect rect) =>
            {
                EditorGUI.LabelField(rect, "List <" + property.propertyPath + ">");
            };

            this.reorderablelist.drawElementCallback = (Rect rect, int index, bool isActive, bool isFocused) =>
            {
                var element = this.reorderablelist.serializedProperty.GetArrayElementAtIndex(index);
                rect.y += 2;
                EditorGUI.PropertyField(
                    new Rect(rect.x, rect.y, rect.width, EditorGUIUtility.singleLineHeight), element, GUIContent.none);
            };
        }
    }

    public override float GetPropertyHeight(SerializedProperty property, GUIContent label)
    {
        float height = EditorGUIUtility.singleLineHeight;

        RefreshList(property.FindPropertyRelative("items"));
        if (this.reorderablelist != null)
        {
            height = this.reorderablelist.GetHeight();
        }

        return height;
    }

    public override void OnGUI(Rect position, SerializedProperty property, GUIContent label)
    {
        RefreshList(property.FindPropertyRelative("items"));
        if (this.reorderablelist != null)
        {
            this.reorderablelist.DoList(position);
        }
    }
}
#endif

[System.Serializable]
public class DataList<T>
{
    public List<T> items = new List<T>();
    public int Length { get { return items.Count; } }
}
