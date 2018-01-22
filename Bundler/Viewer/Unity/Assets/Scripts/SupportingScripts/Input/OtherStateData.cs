// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using System;
using System.Collections.Generic;
using UnityEngine;

namespace Showcases.Input
{
#if UNITY_EDITOR
    using UnityEditor;

    [CustomPropertyDrawer(typeof(OtherButton))]
    public class OtherButtonDrawer : ButtonDrawer { }
#endif

    [Serializable]
    public class OtherButton : Button<InteractionSourceData.Button> { }

    public class OtherStateData : InteractionSourceData
    {
        public override SourceKind kind { get { return SourceKind.Other; } }

        public OtherButton button;
    }
}