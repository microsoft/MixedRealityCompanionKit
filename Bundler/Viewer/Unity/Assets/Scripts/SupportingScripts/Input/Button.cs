// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using System;
using UnityEngine;

namespace Showcases.Input
{
#if UNITY_EDITOR
    using UnityEditor;

    [CustomPropertyDrawer(typeof(ButtonState))]
    public class ButtonStateDrawer : PropertyDrawer
    {
        public override float GetPropertyHeight(SerializedProperty property, GUIContent label)
        {
            return EditorGUI.GetPropertyHeight(property, label, true);
        }

        public override void OnGUI(Rect position, SerializedProperty property, GUIContent label)
        {
            int oldIndentLevel = EditorGUI.indentLevel;

            var pressedThisFrame = property.FindPropertyRelative("pressedThisFrame");
            var releasedThisFrame = property.FindPropertyRelative("releasedThisFrame");
            var pressed = property.FindPropertyRelative("pressed");


            label = EditorGUI.BeginProperty(position, label, property);

            Rect contentPosition = EditorGUI.PrefixLabel(position, label);
            if (position.height > 16f)
            {
                position.height = 16f;
                EditorGUI.indentLevel += 1;
                contentPosition = EditorGUI.IndentedRect(position);
                contentPosition.y += 18f;
            }

            // set width for each control
            contentPosition.width *= 0.33333f;
            //EditorGUI.indentLevel = 0;

            if (pressed != null)
            {
                EditorGUIUtility.labelWidth = 16.0f;
                EditorGUI.PropertyField(contentPosition, pressed, new GUIContent("P"));
                contentPosition.x += contentPosition.width;
            }

            if (pressedThisFrame != null)
            {
                EditorGUIUtility.labelWidth = 16.0f;
                EditorGUI.PropertyField(contentPosition, pressedThisFrame, new GUIContent("D"));
                contentPosition.x += contentPosition.width;
            }

            if (releasedThisFrame != null)
            {
                EditorGUIUtility.labelWidth = 16.0f;
                EditorGUI.PropertyField(contentPosition, releasedThisFrame, new GUIContent("U"));
            }

            EditorGUI.EndProperty();

            EditorGUI.indentLevel = oldIndentLevel;
        }
    }

    [CustomPropertyDrawer(typeof(Button<>))]
    public class ButtonDrawer : PropertyDrawer
    {
        public override void OnGUI(Rect position, SerializedProperty property, GUIContent label)
        {
            int oldIndentLevel = EditorGUI.indentLevel;

            EditorGUI.BeginProperty(position, label, property);

            Rect contentPosition = EditorGUI.PrefixLabel(position, label);

            EditorGUI.indentLevel -= 2;

            var type = property.FindPropertyRelative("type");
            var typePos = contentPosition;
            typePos.width = 75.0f;
            typePos.width = typePos.width;
            EditorGUI.PropertyField(typePos, type, GUIContent.none);

            contentPosition.x += typePos.width;
            contentPosition.width -= typePos.width;

            var state = property.FindPropertyRelative("state");
            EditorGUI.PropertyField(contentPosition, state, GUIContent.none);

            EditorGUI.EndProperty();
            EditorGUI.indentLevel = oldIndentLevel;
        }
    }
#endif

    [Serializable]
    public class ButtonState
    {
        [ReadOnly] public bool pressedThisFrame;

        [ReadOnly] public bool releasedThisFrame;

        [ReadOnly] public bool pressed;
    }

    [Serializable]
    public class Button<T> where T : struct
    {
        [SerializeField] public T type;
        [SerializeField] public ButtonState state;
    }
}