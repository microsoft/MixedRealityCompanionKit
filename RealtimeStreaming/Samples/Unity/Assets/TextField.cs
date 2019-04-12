using System.Collections;
using System.Collections.Generic;
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
