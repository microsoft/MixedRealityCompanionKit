// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using UnityEngine;
using UnityEngine.EventSystems;

namespace PosterAlignment.InputUtilities
{
    public class WorldPointerEventData : PointerEventData
    {
        public StateManager.InputType inputType { get; set; }

        public new Vector3 worldPosition { get; set; }

        public Vector3 worldDelta { get; set; }

        public WorldPointerEventData(EventSystem eventSystem) 
            : base(eventSystem)
        {
        }

        public override string ToString()
        {
            var sb = new System.Text.StringBuilder();

            sb.AppendLine("<b>type</b>: " + this.inputType);
            sb.AppendLine("<b>worldPosition</b>: " + this.worldPosition);
            sb.AppendLine("<b>worldDelta</b>: " + this.worldDelta);
            sb.AppendLine(base.ToString());

            return sb.ToString();
        }

    }
}
