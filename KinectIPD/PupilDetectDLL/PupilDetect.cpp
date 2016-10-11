// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "stdafx.h"
#include "PupilDetect.h"

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/photo/photo.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include <iostream>
#include <queue>
#include <stdio.h>
#include <math.h>

#include "constants.h"
#include "findEyeCenter.h"

using namespace cv;

IPupilDetect* CreatePupilDetect() {
	return new PupilDetect::PupilDetect();
}

int FindCenter(IPupilDetect* ii, BYTE * pixels, int width, int height, PupilDetect::PupilInfo** irisInfo, bool writeFiles, const char *fileName)
{
	if (ii == nullptr)
		return 0;
	return ii->FindCenter(pixels, width, height, irisInfo, writeFiles, fileName);
}

void SetSettings(IPupilDetect* ii, PupilDetect::PupilDetectSettings* settings)
{
	if (ii == nullptr)
		return;
	ii->SetSettings(settings);
}

PupilDetect::PupilDetectSettings* GetSettings(IPupilDetect* ii)
{
	if (ii == nullptr)
		return nullptr;
	return ii->GetSettings();
}

void DestroyPupilDetect(IPupilDetect* ii)
{
	delete ii;
}

namespace PupilDetect
{
	void PupilDetect::SetSettings(PupilDetectSettings* settings)
	{
		m_settings = *settings;
	}

	PupilDetectSettings* PupilDetect::GetSettings()
	{
		return &m_settings;
	}

	PupilInfo info;
	int PupilDetect::FindCenter(BYTE * pixels, int width, int height, PupilInfo** irisInfo, bool writeFiles, const char * fileNamePrefix)
	{
		Mat newImg = Mat(height, width, CV_8UC4, pixels);
		//Mat newImg = imread("DebugImages/20160201_072423/left_orig2.png", CV_8UC4);

		std::vector<int> compression_params;
		compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
		compression_params.push_back(9);
		auto prefix = std::string(fileNamePrefix);
		if (writeFiles)
			imwrite(prefix + "_orig.png", newImg, compression_params);

		Mat biggerImg;
		int scale = m_settings.ScaleInput;
		cv::resize(newImg, biggerImg, cvSize(width * scale, height * scale));
		if (writeFiles)
			imwrite(prefix + "_bigger.png", biggerImg, compression_params);


		Mat grayImg;
		cvtColor(biggerImg, grayImg, CV_BGRA2GRAY);
		cv::Rect eyeRegion(0, 0, width * scale, height * scale);
		auto eyeCenter = EyeCenter::EyeCenter();
		eyeCenter.kFastEyeWidth = m_settings.FastEyeWidth;
		eyeCenter.kWeightBlurSize = m_settings.WeightBlurSize;
		eyeCenter.kEnableWeight = m_settings.EnableWeight;
		eyeCenter.kWeightDivisor = m_settings.WeightDivisor;
		eyeCenter.kGradientThreshold = m_settings.GradientThreshold;
		eyeCenter.kEnablePostProcess = m_settings.EnablePostProcess;
		eyeCenter.kPostProcessThreshold = m_settings.PostProcessThreshold;
		eyeCenter.kMaxMag = m_settings.MaxMag;
		cv::Point pupil = eyeCenter.findEyeCenter(grayImg, eyeRegion, "", writeFiles, prefix);
		PupilInfo *info = new PupilInfo{ (float)pupil.x / scale, (float)pupil.y / scale };
		*irisInfo = info;
		return 1;// result;
	}

}