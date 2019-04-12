using UnityEngine;

public class KeyboardController : MonoBehaviour
{
    public TouchScreenKeyboard keyboard;
    public static string keyboardText = "";

    private TextField currentTextField;

#if UNITY_WSA
    private void Update()
    {
        if (keyboard != null)
        {
            keyboardText = keyboard.text;
            if (!TouchScreenKeyboard.visible)
            {
                keyboard = null;
                currentTextField = null;
                return;
            }
            
            if (currentTextField != null)
            {
                currentTextField.TextUI.text = keyboardText;
            }
        }
    }
#endif

    public void SetTextFieldFocus(TextField textField, string defaultText, 
        TouchScreenKeyboardType type = TouchScreenKeyboardType.Default)
    {
        currentTextField = textField;
        keyboard = TouchScreenKeyboard.Open(defaultText, type, false, false, false, false);
    }
}
