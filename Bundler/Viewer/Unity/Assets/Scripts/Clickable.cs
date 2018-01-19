// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using UnityEngine.EventSystems;

[RequireComponent(typeof(Collider))]
public class Clickable : MonoBehaviour, IPointerClickHandler, IPointerEnterHandler, IPointerExitHandler
{
    public delegate void ButtonClickedDelegate(Clickable source);
    public event ButtonClickedDelegate ButtonClicked;
    public ButtonCommands Command;
    public string CommandParam;
        
    protected bool isOver;
    
    private void Start()
    {
        Initialize();
    }

    protected virtual void Initialize()
    {
        updateButton();
    }

    private void OnEnable()
    {
        updateButton();
    }

    private void OnDisable()
    {
        isOver = false;
        updateButton();
    }

    protected virtual void updateButton()
    {        
    }

    private void OnButtonClicked(PointerEventData eventData)
    {
        if (ButtonClicked != null)
        {
            ButtonClicked(this);
        }        
    }

    public void OnPointerClick(PointerEventData eventData)
    {
        this.OnButtonClicked(null);
    }

    public void OnPointerEnter(PointerEventData eventData)
    {
        isOver = true;
        updateButton();
    }

    public void OnPointerExit(PointerEventData eventData)
    {
        isOver = false;
        updateButton();
    }
}
