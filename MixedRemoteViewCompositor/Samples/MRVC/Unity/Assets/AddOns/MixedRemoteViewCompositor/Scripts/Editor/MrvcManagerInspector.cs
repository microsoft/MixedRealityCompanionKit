// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEditor;
using UnityEngine;

namespace MixedRemoteViewCompositor
{
    [CustomEditor(typeof (MrvcManager))]
    public class MrvcManagerEditor : Editor
    {
        private const string SelectedListenerHelpText =
            "The 'selected listener' controls which device opens the network port and waits for a connection.";

        private MrvcManager manager = null;

        private void OnEnable()
        {
            this.manager = target as MrvcManager;
        }

        public override void OnInspectorGUI()
        {
            this.serializedObject.Update();

            var selectedListenerProperty = this.serializedObject.FindProperty("SelectedListener");
            var connectTo = this.serializedObject.FindProperty("ConnectTo");
            var portProperty = this.serializedObject.FindProperty("Port");
            var enableMRCProperty = this.serializedObject.FindProperty("EnableMRC");
            var sceneCamera = this.serializedObject.FindProperty("SceneCamera");
            var stopOnPaused = this.serializedObject.FindProperty("StopOnPaused");

            EditorGUILayout.HelpBox(SelectedListenerHelpText, UnityEditor.MessageType.Info);
            EditorGUILayout.PropertyField(selectedListenerProperty);
            var labelText = string.Format("{0} address:",
                selectedListenerProperty.enumValueIndex == 0 ? "Playback device" : "Capture device");
            EditorGUILayout.PropertyField(connectTo, new GUIContent(labelText));
            EditorGUILayout.PropertyField(portProperty);
            EditorGUILayout.PropertyField(enableMRCProperty);
            EditorGUILayout.PropertyField(sceneCamera);
            EditorGUILayout.PropertyField(stopOnPaused);

            this.serializedObject.ApplyModifiedProperties();

            if (GUI.changed)
            {
                EditorUtility.SetDirty(target);

                if(this.manager != null)
                {
                    this.manager.SetConnectorAddress(connectTo.stringValue);
                    this.manager.StopOnPaused = stopOnPaused.boolValue;
                    this.manager.SceneCamera = sceneCamera.objectReferenceValue as Camera;
                }
            }
        }
    }
}
