// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using TMPro;
using UnityEngine;

public class TextField : MonoBehaviour
{
    public KeyboardController Controller;
    public TextMeshPro TextUI;

    public string DefaultTextValue;
    public TouchScreenKeyboardType InputType = TouchScreenKeyboardType.Default;

    private void Awake()
    {
        Debug.Assert(TextUI != null);

        this.TextUI.text = DefaultTextValue;
    }

    public string Value
    {
        get
        {
            return TextUI == null ? string.Empty : TextUI.text;
        }
    }

    public void OnFocus()
    {
        Controller.SetTextFieldFocus(this, Value, InputType);
    }
}
