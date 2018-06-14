// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

fixed4 AlphaBlend(fixed4 backCol, fixed4 frontCol, float alpha)
{
    return (1 - alpha * frontCol.a) * backCol + alpha * frontCol;
}
