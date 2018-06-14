// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;

namespace SpectatorView
{
    public static class TransformValidation
    {
        // Unity requires quaternions to be normalized.
        public static Quaternion GetNormalizedQuaternion(Quaternion q)
        {
            float f = 1.0f / Mathf.Sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
            return new Quaternion(q.x * f, q.y * f, q.z * f, q.w * f);
        }

        public static void ValidateVector(ref Vector3 pos)
        {
            // Unity will fail if position values are too small.
            float epsilon = 0.00001f;
            if (Mathf.Abs(pos.x) < epsilon) { pos.x = 0; }
            if (Mathf.Abs(pos.y) < epsilon) { pos.y = 0; }
            if (Mathf.Abs(pos.z) < epsilon) { pos.z = 0; }
        }

        public static void ValidatePose(ref Vector3 pos, ref Quaternion rot)
        {
            ValidateVector(ref pos);

            // Unity will fail if a transform's rotation is not normalized.
            rot = GetNormalizedQuaternion(rot);
        }
    }
}
