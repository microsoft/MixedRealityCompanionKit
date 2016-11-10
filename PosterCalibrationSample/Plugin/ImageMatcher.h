// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"

using namespace std;
using namespace cv;

struct Stats
{
	int matches;
	int inliers;
	double ratio;
	int keypoints;

	Stats() : matches(0),
		inliers(0),
		ratio(0),
		keypoints(0)
	{}
};

class ImageMatcher
{
public:
	ImageMatcher() {}
	ImageMatcher(Ptr<Feature2D> detector, Ptr<DescriptorMatcher> matcher) :
		m_detector(detector),
		m_matcher(matcher)
	{}
	bool IsValid;
	void SetObjectImage(const Mat objectImage, std::vector<Point2f> bb, std::string title, Stats& stats);
	vector<Point2f> ProcessFrame(const Mat frame, Stats& stats);
	Ptr<Feature2D> GetDetector() {
		return m_detector;
	}
protected:

	vector<Point2f> KeyPointsToPoints(vector<KeyPoint> keypoints);
	void DrawBoundingBox(Mat image, vector<Point2f> bb);

	Ptr<Feature2D> m_detector;
	Ptr<DescriptorMatcher> m_matcher;
	Mat m_objectImage, m_objectDesc;
	std::vector<KeyPoint> m_objectKeyPoints;
	std::vector<Point2f> m_objectBoundingBox;
};
