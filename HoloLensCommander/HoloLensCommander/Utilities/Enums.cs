// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;

namespace HoloLensCommander
{
    /// <summary>
    /// Flags used to specify the types of devices to select/deselect.
    /// </summary>
    [Flags]
    internal enum DeviceFilters
    {
        None = 0,

        HoloLens = 0x01,
        Desktop = 0x02,

        All = 0xFF
    }

    /// <summary>
    /// Application assigned identifiers for message dialog buttons.
    /// </summary>
    internal enum MessageDialogButtonId
    {
        Yes = 0,
        No,
        Ok,
        Cancel
    }
}