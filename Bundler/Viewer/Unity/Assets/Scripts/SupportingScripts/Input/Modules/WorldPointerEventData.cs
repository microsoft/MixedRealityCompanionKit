// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using UnityEngine;
using UnityEngine.EventSystems;

namespace Showcases.Input.Modules
{
    public class WorldPointerEventData : PointerEventData
    {
        public new Vector3 worldPosition { get; set; }

        public Vector3 worldDelta { get; set; }

        public WorldPointerEventData(EventSystem eventSystem)
            : base(eventSystem)
        {
        }

        public override string ToString()
        {
            var sb = new System.Text.StringBuilder();
            sb.AppendLine("<b>Position</b>: " + position);
            sb.AppendLine("<b>worldPosition</b>: " + this.worldPosition);
            sb.AppendLine("<b>delta</b>: " + delta);
            sb.AppendLine("<b>worldDelta</b>: " + this.worldDelta);
            sb.AppendLine("<b>eligibleForClick</b>: " + eligibleForClick);
            sb.AppendLine("<b>pointerEnter</b>: " + pointerEnter);
            sb.AppendLine("<b>pointerPress</b>: " + pointerPress);
            sb.AppendLine("<b>lastPointerPress</b>: " + lastPress);
            sb.AppendLine("<b>pointerDrag</b>: " + pointerDrag);
            sb.AppendLine("<b>Use Drag Threshold</b>: " + useDragThreshold);
            sb.AppendLine("<b>Enter Event Camera</b>: " + this.enterEventCamera);
            sb.AppendLine("<b>Current Rayast:</b>");
            sb.AppendLine(pointerCurrentRaycast.ToString());
            sb.AppendLine("<b>Enter Event Camera</b>: " + this.pressEventCamera);
            sb.AppendLine("<b>Press Rayast:</b>");
            sb.AppendLine(pointerPressRaycast.ToString());

            return sb.ToString();
        }
    }
}
