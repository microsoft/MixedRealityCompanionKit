#ifndef EYE_CENTER_H
#define EYE_CENTER_H
#pragma once
#include <opencv2/imgproc/imgproc.hpp>

class EyeCenter
{
public:
	EyeCenter() {}
	int kFastEyeWidth = 75;
	int kWeightBlurSize = 9;
	bool kEnableWeight = true;
	float kWeightDivisor = 80.0;
	double kGradientThreshold = 150.0;
	bool kEnablePostProcess = true;
	float kPostProcessThreshold = 0.95;
	float kMaxMag = 50;

cv::Point findEyeCenter(cv::Mat face, cv::Rect eye, std::string debugWindow, bool writeFiles, const std::string fileNamePrefix);
};
#endif