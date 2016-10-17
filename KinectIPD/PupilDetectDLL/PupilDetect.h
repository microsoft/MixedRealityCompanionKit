// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#include "PupilDetectExports.h"

namespace PupilDetect
{
	class PupilDetect :public IPupilDetect
	{
		PupilDetectSettings m_settings =
		{
			10, 		//	ScaleInput
			75, 		//	FastEyeWidth
			9, 			//	WeightBlurSize
			false, 		//	EnableWeight
			80.0, 		//	WeightDivisor
			150.0f,		//	GradientThreshold
			false, 		//	EnablePostProcess
			0.95f,		//	PostProcessThreshold
			50,			//	MaxMag
		};

	public:
		PupilDetect() {}
		int FindCenter(BYTE * pixels, int width, int height, PupilInfo** irisInfo, bool writeFiles, const char *fileNamePrefix);
		void SetSettings(PupilDetectSettings*);
		PupilDetectSettings* GetSettings();
	};
}