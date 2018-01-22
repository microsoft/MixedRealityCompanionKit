// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using System;
using System.Collections.Generic;
using UnityEngine;

namespace Showcases.Input
{
    public class VoiceStateData : InteractionSourceData
    {
        public override SourceKind kind { get { return SourceKind.Voice; } }
    }
}
