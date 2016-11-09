#include "ImageMatcher.h"

const double ransac_thresh = 2.5f; // RANSAC inlier threshold
const double nn_match_ratio = 0.8f; // Nearest-neighbour matching ratio

std::vector<Point2f> ImageMatcher::KeyPointsToPoints(std::vector<KeyPoint> keypoints)
{
	std::vector<Point2f> res;
	for (unsigned i = 0; i < keypoints.size(); i++)
	{
		res.push_back(keypoints[i].pt);
	}
	return res;
}

void ImageMatcher::SetObjectImage(const Mat objectImage, vector<Point2f> bb, string title, Stats& stats)
{
	if (!m_objectImage.empty())
		m_objectImage.release();
	m_objectKeyPoints.clear();
	m_objectImage = objectImage.clone();
	flip(m_objectImage, m_objectImage, 0);
	m_detector->detectAndCompute(m_objectImage, noArray(), m_objectKeyPoints, m_objectDesc);
	stats.keypoints = (int)m_objectKeyPoints.size();
	m_objectBoundingBox = bb;
}


vector<Point2f> ImageMatcher::ProcessFrame(const Mat frame, Stats& stats)
{
	vector<Point2f> foundObjectBoundingBox;
	vector<KeyPoint> kp;
	Mat desc;
	m_detector->detectAndCompute(frame, noArray(), kp, desc);
	stats.keypoints = (int)kp.size();

	drawKeypoints(frame, kp, frame, Scalar(255, 0, 255), DrawMatchesFlags::DRAW_OVER_OUTIMG);
	

	vector< vector<DMatch> > matches;
	vector<KeyPoint> matched1, matched2;
	if (m_objectKeyPoints.size() >= 2 && kp.size() >= 2)
		m_matcher->knnMatch(m_objectDesc, desc, matches, 2);
	else
		return foundObjectBoundingBox;

	for (unsigned i = 0; i < matches.size(); i++)
	{
		if (matches[i][0].distance < nn_match_ratio * matches[i][1].distance)
		{
			matched1.push_back(m_objectKeyPoints[matches[i][0].queryIdx]);
			matched2.push_back(kp[matches[i][0].trainIdx]);
		}
	}

	Mat inlier_mask, homography;
	vector<KeyPoint> inliers1, inliers2;
	vector<DMatch> inlier_matches;
	if (matched1.size() >= 4)
	{
		homography = findHomography(KeyPointsToPoints(matched1), KeyPointsToPoints(matched2),
			RANSAC, ransac_thresh, inlier_mask);
	}
	
	if (matched1.size() < 4 || homography.empty())
	{
		stats.inliers = 0;
		stats.ratio = 0;
		return foundObjectBoundingBox;
	}
	for (unsigned i = 0; i < matched1.size(); i++)
	{
		if (inlier_mask.at<uchar>(i))
		{
			int new_i = static_cast<int>(inliers1.size());
			inliers1.push_back(matched1[i]);
			inliers2.push_back(matched2[i]);
			inlier_matches.push_back(DMatch(new_i, new_i, 0));
		}
	}
	stats.inliers = (int)inliers1.size();
	drawKeypoints(frame, inliers1, frame, Scalar(0, 255, 0), DrawMatchesFlags::DRAW_OVER_OUTIMG | DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	perspectiveTransform(m_objectBoundingBox, foundObjectBoundingBox, homography);	

	return foundObjectBoundingBox;
}