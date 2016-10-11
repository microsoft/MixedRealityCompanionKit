// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

namespace PupilDetect
{
	struct PupilInfo
	{
		float CenterX;
		float CenterY;
	};

	struct PupilDetectSettings
	{
		int ScaleInput;
		int FastEyeWidth;
		int WeightBlurSize;
		bool EnableWeight;
		float WeightDivisor;
		double GradientThreshold;
		bool EnablePostProcess;
		float PostProcessThreshold;
		float MaxMag;
	};
}