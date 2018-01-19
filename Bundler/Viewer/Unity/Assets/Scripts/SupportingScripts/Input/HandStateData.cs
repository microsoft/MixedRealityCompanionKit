// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using System;
using System.Collections.Generic;
using UnityEngine;

namespace Showcases.Input
{
#if UNITY_EDITOR
using UnityEditor;

[CustomPropertyDrawer(typeof(HandButton))]
public class HandButtonDrawer : ButtonDrawer { }
#endif

    [Serializable]
    public class HandButton : Button<InteractionSourceData.Button> { }

    public class HandStateData : InteractionSourceData
    {
        public override SourceKind kind { get { return SourceKind.Hand; } }

        public HandButton button;

        public override bool UpdateState()
        {
            bool updated = base.UpdateState();

            SetButtonStateData(this.button);

            return updated;
        }
    }
}
