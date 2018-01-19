// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
#ifndef HOLOTOOLKIT_COMMON
#define HOLOTOOLKIT_COMMON

// Helper function for focal plane fading
float4 _NearPlaneFadeDistance;

float ComputeNearPlaneFadeLinear(float4 vertex)
{
	float distToCamera = -(mul(UNITY_MATRIX_MV, vertex).z);
	return saturate(mad(distToCamera, _NearPlaneFadeDistance.y, _NearPlaneFadeDistance.x));
}

#endif //HOLOTOOLKIT_COMMON