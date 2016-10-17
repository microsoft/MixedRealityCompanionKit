// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#include "PupilInfo.h"

class __declspec(dllexport) IPupilDetect
{
public:
	virtual int FindCenter(BYTE * pixels, int width, int height, PupilDetect::PupilInfo**, bool writeFiles, const char * fileName) = 0;
	virtual void SetSettings(PupilDetect::PupilDetectSettings*) = 0;
	virtual PupilDetect::PupilDetectSettings* GetSettings() = 0;
};

extern "C" __declspec(dllexport) IPupilDetect* CreatePupilDetect();
extern "C" __declspec(dllexport) int FindCenter(IPupilDetect*, BYTE * pixels, int width, int height, PupilDetect::PupilInfo**, bool writeFiles, const char * fileName);
extern "C" __declspec(dllexport) void SetSettings(IPupilDetect*, PupilDetect::PupilDetectSettings*);
extern "C" __declspec(dllexport) PupilDetect::PupilDetectSettings* GetSettings(IPupilDetect*);
extern "C" __declspec(dllexport) void DestroyPupilDetect(IPupilDetect*);